#include "utils/wrap_string.h"

size_t missing_strlcpy(char *dest, const char *src, size_t size)
{
    const size_t slen = strlen(src);

    if (size != 0)
    {
        size--;

        if (slen < size)
        {
            size = slen;
        }

        if (size != 0)
        {
            memcpy(dest, src, size);
        }

        dest[size] = 0;
    }

    return slen;
}
