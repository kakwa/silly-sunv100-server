/* Minimal <sys/null.h> for standalone build */
#ifndef _SYS_NULL_H_
#define _SYS_NULL_H_

#ifndef NULL
#if defined(__cplusplus)
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#endif /* _SYS_NULL_H_ */


