#include "utils/wrap_string.h"

size_t missing_strlcat(char *dest, const char *src, size_t size)
{
    size_t len = strlen(dest);

    if (len < size - 1)
    {
        return (len + strlcpy(dest + len, src, size - len));
    }

    return len + strlen(src);
}
