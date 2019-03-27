#include "cli.hpp"

#include <stdio.h>
#include <stdlib.h>
#include "utils/wrap_string.h"

#include "cli_uart.hpp"

#include "common/encoding.hpp"

using go::Encoding::BigEndian::HostSwap16;
using go::Encoding::BigEndian::HostSwap32;

namespace go {
namespace Cli {

const struct Command Interpreter::sCommands[] = {
    {"help", &Interpreter::ProcessHelp},
    {"version", &Interpreter::ProcessVersion},
    {"reboot", &Interpreter::ProcessReboot},
};

void goFreeMemory(const void *)
{
}

template <class T> class goPtr
{
    T *ptr;

public:
    goPtr(T *_ptr)
        : ptr(_ptr)
    {
    }
    ~goPtr()
    {
        if (ptr)
        {
            goFreeMemory(ptr);
        }
    }
    T *get() const { return ptr; }
       operator T *() const { return ptr; }
    T *operator->() const { return ptr; }
};

typedef goPtr<const char> goStringPtr;

Interpreter::Interpreter(Instance *aInstance)
    : mUserCommands(NULL)
    , mUserCommandsLength(0)
    , mServer(NULL)
    , mInstance(aInstance)
{
    GO_UNUSED_VARIABLE(mInstance);
}

int Interpreter::Hex2Bin(const char *aHex, uint8_t *aBin, uint16_t aBinLength)
{
    size_t      hexLength = strlen(aHex);
    const char *hexEnd    = aHex + hexLength;
    uint8_t *   cur       = aBin;
    uint8_t     numChars  = hexLength & 1;
    uint8_t     byte      = 0;
    int         rval;

    VerifyOrExit((hexLength + 1) / 2 <= aBinLength, rval = -1);

    while (aHex < hexEnd)
    {
        if ('A' <= *aHex && *aHex <= 'F')
        {
            byte |= 10 + (*aHex - 'A');
        }
        else if ('a' <= *aHex && *aHex <= 'f')
        {
            byte |= 10 + (*aHex - 'a');
        }
        else if ('0' <= *aHex && *aHex <= '9')
        {
            byte |= *aHex - '0';
        }
        else
        {
            ExitNow(rval = -1);
        }

        aHex++;
        numChars++;

        if (numChars >= 2)
        {
            numChars = 0;
            *cur++   = byte;
            byte     = 0;
        }
        else
        {
            byte <<= 4;
        }
    }

    rval = static_cast<int>(cur - aBin);

exit:
    return rval;
}

void Interpreter::AppendResult(goError aError) const
{
    if (aError == GO_ERROR_NONE)
    {
        mServer->OutputFormat("Done\r\n");
    }
    else
    {
        mServer->OutputFormat("Error %d: %s\r\n", aError, goErrorToString(aError));
    }
}

void Interpreter::OutputBytes(const uint8_t *aBytes, uint8_t aLength) const
{
    for (int i = 0; i < aLength; i++)
    {
        mServer->OutputFormat("%02x", aBytes[i]);
    }
}

goError Interpreter::ParseLong(char *aString, long &aLong)
{
    char *endptr;
    aLong = strtol(aString, &endptr, 0);
    return (*endptr == '\0') ? GO_ERROR_NONE : GO_ERROR_PARSE;
}

goError Interpreter::ParseUnsignedLong(char *aString, unsigned long &aUnsignedLong)
{
    char *endptr;
    aUnsignedLong = strtoul(aString, &endptr, 0);
    return (*endptr == '\0') ? GO_ERROR_NONE : GO_ERROR_PARSE;
}

void Interpreter::ProcessHelp(int argc, char *argv[])
{
    GO_UNUSED_VARIABLE(argc);
    GO_UNUSED_VARIABLE(argv);

    for (unsigned int i = 0; i < GO_ARRAY_LENGTH(sCommands); i++)
    {
        mServer->OutputFormat("%s\r\n", sCommands[i].mName);
    }

    for (unsigned int i = 0; i < mUserCommandsLength; i++)
    {
        mServer->OutputFormat("%s\r\n", mUserCommands[i].mName);
    }
}

void Interpreter::ProcessVersion(int argc, char *argv[])
{
    GO_UNUSED_VARIABLE(argc);
    GO_UNUSED_VARIABLE(argv);

    goStringPtr version(goGetVersionString());
    mServer->OutputFormat("%s\r\n", (const char *)version);
    AppendResult(GO_ERROR_NONE);
}

void Interpreter::ProcessReboot(int argc, char *argv[])
{
    GO_UNUSED_VARIABLE(argc);
    GO_UNUSED_VARIABLE(argv);

    mInstance->Reset();
}

void Interpreter::ProcessLine(char *aBuf, uint16_t aBufLength, Server &aServer)
{
    char *  argv[kMaxArgs];
    char *  cmd;
    uint8_t argc = 0, i = 0;

    mServer = &aServer;

    VerifyOrExit(aBuf != NULL);

    for (; *aBuf == ' '; aBuf++, aBufLength--)
        ;

    for (cmd = aBuf + 1; (cmd < aBuf + aBufLength) && (cmd != NULL); ++cmd)
    {
        VerifyOrExit(argc < kMaxArgs, mServer->OutputFormat("Error: too many args (max %d)\r\n", kMaxArgs));

        if (*cmd == ' ' || *cmd == '\r' || *cmd == '\n')
        {
            *cmd = '\0';
        }

        if (*(cmd - 1) == '\0' && *cmd != ' ')
        {
            argv[argc++] = cmd;
        }
    }

    cmd = aBuf;

    for (i = 0; i < GO_ARRAY_LENGTH(sCommands); i++)
    {
        if (strcmp(cmd, sCommands[i].mName) == 0)
        {
            (this->*sCommands[i].mCommand)(argc, argv);
            break;
        }
    }

    // Check user defined commands if built-in command
    // has not been found
    if (i == GO_ARRAY_LENGTH(sCommands))
    {
        for (i = 0; i < mUserCommandsLength; i++)
        {
            if (strcmp(cmd, mUserCommands[i].mName) == 0)
            {
                mUserCommands[i].mCommand(argc, argv);
                break;
            }
        }

        if (i == mUserCommandsLength)
        {
            AppendResult(GO_ERROR_PARSE);
        }
    }

exit:
    return;
}

void Interpreter::SetUserCommands(const goCliCommand *aCommands, uint8_t aLength)
{
    mUserCommands       = aCommands;
    mUserCommandsLength = aLength;
}

Interpreter &Interpreter::GetOwner(OwnerLocator &aOwnerLocator)
{
    Interpreter &interpreter = Uart::sUartServer->GetInterpreter();
    GO_UNUSED_VARIABLE(aOwnerLocator);
    return interpreter;
}

} // namespace Cli
} // namespace go
