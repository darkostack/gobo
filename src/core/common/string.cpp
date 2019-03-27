#include "string.hpp"

namespace go {

goError StringBase::Write(char *aBuffer, uint16_t aSize, uint16_t &aLength, const char *aFormat, va_list aArgs)
{
    goError error = GO_ERROR_NONE;
    int     len;

    len = vsnprintf(aBuffer + aLength, aSize - aLength, aFormat, aArgs);

    if (len < 0)
    {
        aLength    = 0;
        aBuffer[0] = 0;
        error      = GO_ERROR_INVALID_ARGS;
    }
    else if (len >= aSize - aLength)
    {
        aLength = aSize - 1;
        error   = GO_ERROR_NO_BUFS;
    }
    else
    {
        aLength += static_cast<uint16_t>(len);
    }

    return error;
};

} // namespace go
