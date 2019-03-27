#ifndef STRING_HPP_
#define STRING_HPP_

#include "gobo-core-config.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include "utils/wrap_string.h"

#include <gobo/error.h>

#include "common/code_utils.hpp"

namespace go {

class StringBase
{
protected:
    static goError Write(char *aBuffer, uint16_t aSize, uint16_t &aLength, const char *aFormat, va_list aArgs);
};

template <uint16_t SIZE> class String : private StringBase
{
public:
    enum
    {
        kSize = SIZE, ///< Size (number of characters) in the buffer storage for the string.
    };

    String(void)
        : mLength(0)
    {
        mBuffer[0] = 0;
    }

    String(const char *aFormat, ...)
        : mLength(0)
    {
        va_list args;
        va_start(args, aFormat);
        Write(mBuffer, kSize, mLength, aFormat, args);
        va_end(args);
    }

    void Clear(void)
    {
        mBuffer[0] = 0;
        mLength    = 0;
    }

    uint16_t GetLength(void) const { return mLength; }

    uint16_t GetSize(void) const { return kSize; }

    const char *AsCString(void) const { return mBuffer; }

    goError Set(const char *aFormat, ...)
    {
        va_list args;
        goError error;

        va_start(args, aFormat);
        mLength = 0;
        error   = Write(mBuffer, kSize, mLength, aFormat, args);
        va_end(args);

        return error;
    }

    goError Append(const char *aFormat, ...)
    {
        va_list args;
        goError error;

        va_start(args, aFormat);
        error = Write(mBuffer, kSize, mLength, aFormat, args);
        va_end(args);

        return error;
    }

private:
    uint16_t mLength;
    char     mBuffer[kSize];
};

} // namespace go

#endif // STRING_HPP_
