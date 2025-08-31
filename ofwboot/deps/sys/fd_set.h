#ifndef _SYS_FD_SET_H_
#define _SYS_FD_SET_H_

#include <sys/cdefs.h>

typedef unsigned long __fd_mask;
#ifndef FD_SETSIZE
#define FD_SETSIZE 256
#endif

typedef struct fd_set {
	__fd_mask fds_bits[FD_SETSIZE / (8 * sizeof(__fd_mask))];
} fd_set;

#define __FD_MASK(n) ((n) % (8 * (int)sizeof(__fd_mask)))
#define __FD_SETN(n) ((n) / (8 * (int)sizeof(__fd_mask)))

#define FD_SET(n, p)   ((p)->fds_bits[__FD_SETN(n)] |= (1UL << __FD_MASK(n)))
#define FD_CLR(n, p)   ((p)->fds_bits[__FD_SETN(n)] &= ~(1UL << __FD_MASK(n)))
#define FD_ISSET(n, p) (((p)->fds_bits[__FD_SETN(n)] & (1UL << __FD_MASK(n))) != 0)
#define FD_ZERO(p)     do { \
		for (unsigned int __i = 0; __i < (FD_SETSIZE / (8 * sizeof(__fd_mask))); __i++) \
			(p)->fds_bits[__i] = 0; \
	} while (0)

#endif /* _SYS_FD_SET_H_ */
