#include "utils/wrap_string.h"

size_t missing_strnlen(const char *s, size_t maxlen)
{
    size_t ret;

    for (ret = 0; (ret < maxlen) && (s[ret] != 0); ret++)
    {
        // Empty loop.
    }

    return ret;
}
