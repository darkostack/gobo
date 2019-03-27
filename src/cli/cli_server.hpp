#ifndef CLI_SERVER_HPP_
#define CLI_SERVER_HPP_

#include "gobo-core-config.h"

namespace go {
namespace Cli {

class Server
{
public:
    virtual int Output(const char *aBuf, uint16_t aBufLength) = 0;

    virtual int OutputFormat(const char *aFmt, ...) = 0;
};

} // namespace Cli
} // namespace go

#endif // CLI_SERVER_HPP_
