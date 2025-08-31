#ifndef _SYS_ANSI_H_
#define _SYS_ANSI_H_

#include <sys/cdefs.h>

/* Provide basic typedef backing macros expected by types.h */
#ifndef __int8_t
#define __int8_t signed char
#endif
#ifndef __uint8_t
#define __uint8_t unsigned char
#endif
#ifndef __int16_t
#define __int16_t short
#endif
#ifndef __uint16_t
#define __uint16_t unsigned short
#endif
#ifndef __int32_t
#define __int32_t int
#endif
#ifndef __uint32_t
#define __uint32_t unsigned int
#endif
#ifndef __int64_t
#define __int64_t long long
#endif
#ifndef __uint64_t
#define __uint64_t unsigned long long
#endif

#ifndef __intptr_t
#define __intptr_t long
#endif
#ifndef __uintptr_t
#define __uintptr_t unsigned long
#endif

/* Additional __types used by sys/types.h in this standalone tree */
#ifndef __gid_t
#define __gid_t unsigned int
#endif
#ifndef __uid_t
#define __uid_t unsigned int
#endif
#ifndef __mode_t
#define __mode_t unsigned int
#endif
#ifndef __accmode_t
#define __accmode_t unsigned int
#endif
#ifndef __off_t
#define __off_t long long
#endif
#ifndef __pid_t
#define __pid_t int
#endif
#ifndef __fsblkcnt_t
#define __fsblkcnt_t unsigned long long
#endif
#ifndef __fsfilcnt_t
#define __fsfilcnt_t unsigned long long
#endif
#ifndef __cpu_simple_lock_nv_t
#define __cpu_simple_lock_nv_t unsigned char
#endif

#endif /* _SYS_ANSI_H_ */
