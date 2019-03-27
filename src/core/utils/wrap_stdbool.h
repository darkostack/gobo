#if !defined(WRAP_STDBOOL_H)
#define WRAP_STDBOOL_H

#include "gobo-core-config.h"

#if HAVE_STDBOOL_H
#include <stdbool.h>
#else

/* Supply our own */
#if __cplusplus
/* c++ has a built in bool */
#else

#if !defined(__bool_true_false_are_defined)
#define __bool_true_false_are_defined 1
#define false 0
#define true 1

#if defined(_MSC_VER)
#define bool _Bool
#else
typedef _Bool bool;
#endif // visual studio has a bool

#endif // bool defined

#endif // __cplusplus

#endif // HAVE_STDBOOL_H

#endif // WRAP_STDBOOL_H
