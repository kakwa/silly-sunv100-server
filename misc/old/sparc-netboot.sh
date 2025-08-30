#!/bin/sh


# listening interface for RARPD
IF=eth0
# Base Network (this script is limited to D class networks)
NET=192.168.100.
# TFTP/RARPD server IP
SRV_IP=192.168.100.100
# ethers file
ETHER_FILE=/etc/ethers
# hosts file
HOSTS_FILE=/etc/hosts
# hostname prefix (install)
PREFIX=sparc
# boot image
IMG_PATH=/usr/lib/debian-installer/images/7.0/sparc/boot.img
# tftp root directory
TFTP_DIR=/srv/tftp/

get_oneip(){
    last=`dd if=/dev/urandom bs=1 count=1 2>/dev/null \
        | od -An -tu1 |sed 's/  *//g'`
    echo ${NET}${last}
}

get_ip(){
    ret=1
    while [ $ret -ne 0 ]
    do
        ip=`get_oneip`
        if ! grep "$ip" "${HOSTS_FILE}" && ! [ "${ip}" = "${SRV_IP}" ]
        then
            ret=0
        fi
    done
    echo $ip
}

help(){
  echo "usage: `basename $0` [-f <configuration file>]"
  echo ""
  echo "rarpd wrapper for easier sparc installation"
  echo ""
  echo "arguments:"
  echo "  -f <configuration file>: the configuration file (optional)"
  echo ""
  echo "Sample config file:"
  echo ""
  echo "# listening interface for RARPD"
  echo "IF=eth0"
  echo "# Base Network (this script is limited to D class networks)"
  echo "NET=192.168.100."
  echo "# TFTP/RARPD server IP"
  echo "SRV_IP=192.168.100.100"
  echo "# ethers file"
  echo "ETHER_FILE=/etc/ethers"
  echo "# hosts file"
  echo "HOSTS_FILE=/etc/hosts"
  echo "# hostname prefix (install)"
  echo "PREFIX=sparc"
  echo "# boot image"
  echo "IMG_PATH=/usr/lib/debian-installer/images/7.0/sparc/boot.img"
  echo "# tftp root directory"
  echo "TFTP_DIR=/srv/tftp/"
  echo ""
  echo "To boot on network from LOM:"
  echo "#."
  echo "lom> break -f"
  echo "ok boot net"
  exit 1
}

while getopts ":hf:" opt; do
  case $opt in

    h) help
        ;;
    f)
        CONF="$OPTARG"
        ;;
    \?)
        echo "Invalid option: -$OPTARG" >&2
        help
        exit 1
        ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
        help
        exit 1
        ;;
  esac
done

if ! [ -z "${CONF}" ] && ! [ -f "${CONF}" ]
then
   echo "[ERROR] configuration '${CONF}' doesn't exist" 
   exit 1
fi

if ! [ -z "${CONF}" ]
then
    . ${CONF}
fi

rarpd -e -dv ${IF} 2>&1 |while read line;
do
    # ugly sed to recover the mac address from rarpd output 
    # lower case hexa is converted to upper case hexa
    MAC=`echo "${line}" | sed  "s/.*\(\([0-9a-f]\{2\}:\)\{5\}[0-9a-f]\{2\}\).*/\1/;tx;d;:x"| tr 'a-z' 'A-Z'`

    # if we have a mac address
    if ! [ -z "${MAC}" ]
    then
        # if the mac address is not in the ethers file
        if ! grep -q "${MAC}" "${ETHER_FILE}" 2>/dev/null
        then
            # get a random, not already used IP
            IP=`get_ip`
            # set the hostname to ${PREFIX}-${IP without dot}
            HOSTNAME="${PREFIX}-`echo "${IP}"| sed 's/.*\.//g'`"
            # fill ethers and hosts files
            echo "${MAC} ${HOSTNAME} #sparc-install" >>${ETHER_FILE}
            echo "${IP} ${HOSTNAME} #sparc-install" >>${HOSTS_FILE}

            # calculate the name of the boot image (IP address in hexadecimal)
            arg="`echo ${IP} | sed 's/\./ /g'`"
            img_name=`printf "%.2X%.2X%.2X%.2X\n" $arg`
            img=`basename ${IMG_PATH}`

            # copy the boot image inside the tftp dir if needed
            if ! [ -f "${TFTP_DIR}/${img}" ]
            then
                cp ${IMG_PATH} ${TFTP_DIR}
            fi
            # symlink it to the needed name
            ln -sf ${img} ${TFTP_DIR}/${img_name}
        fi
    fi
done
