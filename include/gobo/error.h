#ifndef GOBO_ERROR_H_
#define GOBO_ERROR_H_

#include <gobo/platform/toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum goError {
    GO_ERROR_NONE                         = 0,
    GO_ERROR_FAILED                       = 1,
    GO_ERROR_DROP                         = 2,
    GO_ERROR_NO_BUFS                      = 3,
    GO_ERROR_BUSY                         = 4,
    GO_ERROR_PARSE                        = 5,
    GO_ERROR_INVALID_ARGS                 = 6,
    GO_ERROR_SECURITY                     = 7,
    GO_ERROR_ABORT                        = 8,
    GO_ERROR_NOT_IMPLEMENTED              = 9,
    GO_ERROR_INVALID_STATE                = 10,
    GO_ERROR_NOT_FOUND                    = 11,
    GO_ERROR_ALREADY                      = 12,
    GO_ERROR_NOT_CAPABLE                  = 13,
    GO_ERROR_RESPONSE_TIMEOUT             = 14,
    GO_ERROR_DISABLED_FEATURE             = 15,
    GO_ERROR_GENERIC                      = 255,
} goError;

const char *goErrorToString(goError aError);

#ifdef __cplusplus
}
#endif

#endif // GOBO_ERROR_H_
