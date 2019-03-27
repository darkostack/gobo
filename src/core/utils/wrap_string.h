#if !defined(WRAP_STRING_H)
#define WRAP_STRING_H

#include "gobo-core-config.h"

/* system provided string.h */
#include <string.h>

/* These are C functions */
#if defined(__cplusplus)
#define WRAP_EXTERN_C extern "C"
#else
#define WRAP_EXTERN_C extern
#endif

/* Prototypes for our missing function replacements */

/* See: https://www.freebsd.org/cgi/man.cgi?query=strlcpy */
WRAP_EXTERN_C size_t missing_strlcpy(char *dst, const char *src, size_t dstsize);
/* See: https://www.freebsd.org/cgi/man.cgi?query=strlcat */
WRAP_EXTERN_C size_t missing_strlcat(char *dst, const char *src, size_t dstsize);
/* See: https://www.freebsd.org/cgi/man.cgi?query=strnlen */
WRAP_EXTERN_C size_t missing_strnlen(const char *s, size_t maxlen);

#undef WRAP_EXTERN_C

#if (!HAVE_STRNLEN)
#define strnlen(S, N) missing_strnlen(S, N)
#endif

#if (!HAVE_STRLCPY)
#define strlcpy(D, S, N) missing_strlcpy(D, S, N)
#endif

#if (!HAVE_STRLCAT)
#define strlcat(D, S, N) missing_strlcat(D, S, N)
#endif

#endif // WRAP_STRING_H
