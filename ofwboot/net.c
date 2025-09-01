/*	$OpenBSD: net.c,v 1.9 2023/06/01 17:24:56 krw Exp $	*/
/*	$NetBSD: net.c,v 1.1 2000/08/20 14:58:38 mrg Exp $	*/

/*
 * Copyright (C) 1995 Wolfgang Solfrank.
 * Copyright (C) 1995 TooLs GmbH.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by TooLs GmbH.
 * 4. The name of TooLs GmbH may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY TOOLS GMBH ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TOOLS GMBH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This module implements a "raw device" interface suitable for
 * use by the stand-alone I/O library NFS code.  This interface
 * does not support any "block" access, and exists only for the
 * purpose of initializing the network interface, getting boot
 * parameters, and performing the NFS mount.
 *
 * At open time, this does:
 *
 * find interface	- netif_open()
 *
 * The root file handle from mountd is saved in a global
 * for use by the NFS open code (NFS/lookup).
 *
 * Note: this is based in part on sys/arch/sparc/stand/net.c
 */

#include <sys/param.h>
#include <sys/socket.h>

#include <net/if.h>
#include <netinet/in.h>

#include <lib/libsa/stand.h>
#include <lib/libsa/net.h>
#include <lib/libsa/netif.h>
#include <lib/libsa/tftp.h>

#include "ofdev.h"

extern struct in_addr servip;

int net_mountroot_rarp(void);

extern char	rootpath[FNAME_SIZE];
struct in_addr servip;

static	int netdev_sock = -1;
static	int open_count;

int
net_mountroot(void)
{
	int error;

#ifdef DEBUG
	printf("net_mountroot\n");
#endif

	error = net_mountroot_rarp();
	if (error != 0)
		return (error);
	if (rootip.s_addr != 0)
	    servip = rootip;
	    printf("TFTP IP address: %s\n", inet_ntoa(servip));
	return 0;
}

/*
 * Called by devopen after it sets f->f_dev to our devsw entry.
 * This opens the low-level device and sets f->f_devdata.
 */
int
net_open(struct of_dev *op)
{
	int error = 0;

	/*
	 * On first open, do netif open, mount, etc.
	 */
	if (open_count == 0) {
		/* Find network interface. */
		if ((netdev_sock = netif_open(op)) < 0) {
			error = errno;
			goto bad;
		}
		if ((error = net_mountroot()) != 0)
			goto bad;
	}
	open_count++;
bad:
	if (netdev_sock >= 0 && open_count == 0) {
		netif_close(netdev_sock);
		netdev_sock = -1;
	}
	return error;
}

void
net_close(struct of_dev *op)
{
	/*
	 * On last close, do netif close, etc.
	 */
	if (open_count > 0)
		if (--open_count == 0) {
			netif_close(netdev_sock);
			netdev_sock = -1;
		}
}

int
net_mountroot_rarp(void)
{
	rarp_getipaddress(netdev_sock);

	if (myip.s_addr == 0)
		return(ENOENT);

	printf("Using RARP protocol: ");
	printf("ip address: %s", inet_ntoa(myip));

	if (hostname[0])
		printf(", hostname: %s", hostname);
	if (netmask)
		printf(", netmask: %s", intoa(netmask));
	if (rootip.s_addr)
		printf(", server: %s", inet_ntoa(rootip));
	if (gateip.s_addr)
		printf(", gateway: %s", inet_ntoa(gateip));
	printf("\n");

	return (0);
}


