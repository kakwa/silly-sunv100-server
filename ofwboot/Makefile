#	$OpenBSD: Makefile,v 1.29 2025/01/30 21:46:25 kurt Exp $
#	$NetBSD: Makefile,v 1.2 2001/03/04 14:50:05 mrg Exp $

CURDIR=	${.CURDIR}
S=	${CURDIR}/deps

#
# Override normal settings
#

WARNS=		0

PROG?=		ofwboot
NOMAN=		ja, man!

.if ${PROG} == "ofwboot"
SOFTRAID?=	no
.else
SOFTRAID?=	no
.endif

.PATH:		${S}/arch/sparc64/sparc64
.PATH:		${S}/lib/libsa
SRCS=		srt0.s Locore.c alloc.c boot.c elf64_exec.c arc4.c \
		net.c netif_of.c ofdev.c vers.c
.if ${SOFTRAID:L} == "yes"
SRCS+=		diskprobe.c softraid_sparc64.c
.endif

.PATH:		${S}/lib/libkern/arch/sparc64 ${S}/lib/libkern
SRCS+=		strlcpy.c strcmp.c strlcat.c strlen.c ffs.S

.if ${SOFTRAID:L} == "yes"
SRCS+=		aes_xts.c bcrypt_pbkdf.c blowfish.c explicit_bzero.c \
		hmac_sha1.c pkcs5_pbkdf2.c rijndael.c sha1.c sha2.c softraid.c
.endif

CWARNFLAGS+=	-Wno-main
AFLAGS+=	-x assembler-with-cpp -D_LOCORE -D__ELF__ -fno-pie 
CFLAGS+=	${COPTS} -fno-pie -fno-stack-protector
CPPFLAGS+=	-D_STANDALONE -DSUN4U -nostdinc
CPPFLAGS+=  -DNETBOOT
CPPFLAGS+=	-DNETIF_DEBUG

BINMODE=	444

NEWVERSWHAT=	"OpenFirmware Boot"

#
# ELF64 defaults to 1MB
#
RELOC=		100000

ENTRY=		_start

CLEANFILES+=	sparc machine

# Local libsa settings and build rule
SADST?= ${.CURDIR}/deps/lib/libsa
SALIB?= ${SADST}/libsa.a

${SALIB}:
	@echo Building local libsa
	@rm -f ${SALIB}
	@mkdir -p ${SADST}
	@objs=""; \
	for f in ${.CURDIR}/deps/lib/libsa/*.c ${.CURDIR}/deps/lib/libkern/divdi3.c ${.CURDIR}/deps/lib/libkern/qdivrem.c; do \
		b=$${f##*/}; n=$${b%.c}; \
		case "$${b}" in \
			cd9660.c) \
				continue ;; \
			nfs.c|bootparam.c) \
				continue ;; \
			cons.c|putchar.c) \
				continue ;; \
			cread.c) \
				continue ;; \
			loadfile.c|loadfile_elf.c) \
				continue ;; \
			rpc.c) \
				continue ;; \
			softraid.c) \
				continue ;; \
			ufs.c|ufs2.c) \
				continue ;; \
		esac; \
		${CC} ${CFLAGS} -ffreestanding -fno-builtin -nostdinc \
			-I${.CURDIR} -I${.CURDIR}/deps -I${.CURDIR}/deps/include -I${.CURDIR}/deps/lib/libsa \
			-D_STANDALONE -DCOMPAT_UFS -c $$f -o ${SADST}/$$n.o || exit 1; \
		objs="$$objs ${SADST}/$$n.o"; \
	done; \
	${AR} rcs ${SALIB} $$objs; \
	${RANLIB} ${SALIB}

CPPFLAGS+=	-I${.CURDIR}/deps/include -I${CURDIR} -I.
CPPFLAGS+=	-DRELOC=0x${RELOC}

#
# XXXXX FIXME
#
## no local UFS in minimal TFTP build
#CPPFLAGS+=	-DSPARC_BOOT_UFS
#CPPFLAGS+=	-DSPARC_BOOT_HSFS
#.if ${SOFTRAID:L} == "yes"
#CPPFLAGS+=	-DSOFTRAID
#.endif

.if !make(clean) && !make(cleandir) && !make(includes) && !make(libdep) && \
    !make(sadep) && !make(salibdir) && !make(obj)
.BEGIN:
	@([ -h machine ] || ln -s ${.CURDIR}/deps/include machine)
.endif

${PROG}: ${OBJS} ${SALIB}
	${LD} -N -Ttext ${RELOC} -e ${ENTRY} -o ${PROG} --no-pie -znorelro \
	    ${OBJS} -L${SADST} ${SALIB}

NORMAL_S=	${CC} ${AFLAGS} ${CPPFLAGS} -c $<
srt0.o: srt0.s
	${NORMAL_S}

.include <bsd.prog.mk>
