#if !defined(WRAP_STDINT_H)
#define WRAP_STDINT_H

#include "gobo-core-config.h"

/* generally all compilers support this */
/* Visual Studio only after VS2015 (aka: 19.00) */

#if defined(_MSC_VER) && (_MSC_VER < 1900)
/* types from this page: https://msdn.microsoft.com/en-us/library/29dh1w7z.aspx */

typedef unsigned __int8  uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef __int8  int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

#else
/* use the compiler supplied solution */
#include <stdint.h>
#endif

#endif // WRAP_STDINT_H
