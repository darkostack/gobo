#ifndef GOBO_PLATFORM_TOOLCHAIN_H_
#define GOBO_PLATFORM_TOOLCHAIN_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// =========== TOOLCHAIN SELECTION : START ===========

#if defined(__GNUC__) || defined(__clang__) || defined(__CC_ARM) || defined(__TI_ARM__)

// https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html
// http://www.keil.com/support/man/docs/armcc/armcc_chr1359124973480.htm

#define GO_TOOL_PACKED_BEGIN
#define GO_TOOL_PACKED_FIELD __attribute__((packed))
#define GO_TOOL_PACKED_END __attribute__((packed))
#define GO_TOOL_WEAK __attribute__((weak))

#define GO_TOOL_ALIGN(X)

#elif defined(__ICCARM__) || defined(__ICC8051__)

// http://supp.iar.com/FilesPublic/UPDINFO/004916/arm/doc/EWARM_DevelopmentGuide.ENU.pdf

#include "intrinsics.h"

#define GO_TOOL_PACKED_BEGIN __packed
#define GO_TOOL_PACKED_FIELD
#define GO_TOOL_PACKED_END
#define GO_TOOL_WEAK __weak

#define GO_TOOL_ALIGN(X)

#elif defined(_MSC_VER)

#define GO_TOOL_PACKED_BEGIN __pragma(pack(push, 1))
#define GO_TOOL_PACKED_FIELD
#define GO_TOOL_PACKED_END __pragma(pack(pop))
#define GO_TOOL_WEAK

#define GO_TOOL_ALIGN(X) __declspec(align(4))

#elif defined(__SDCC)

// Structures are packed by default in sdcc, as it primarily targets 8-bit MCUs.

#define GO_TOOL_PACKED_BEGIN
#define GO_TOOL_PACKED_FIELD
#define GO_TOOL_PACKED_END
#define GO_TOOL_WEAK

#define GO_TOOL_ALIGN(X)

#else

#error "Error: No valid Toolchain specified"

// Symbols for Doxygen

#define GO_TOOL_PACKED_BEGIN
#define GO_TOOL_PACKED_FIELD
#define GO_TOOL_PACKED_END
#define GO_TOOL_WEAK

#define GO_TOOL_ALIGN(X)

#endif

// =========== TOOLCHAIN SELECTION : END ===========

#if defined(__ICCARM__)

#include <stddef.h>

#define GO_UNUSED_VARIABLE(VARIABLE) \
    do                               \
    {                                \
        if (&VARIABLE == NULL)       \
        {                            \
        }                            \
    } while (false)

#define GO_UNREACHABLE_CODE(CODE)                                                                    \
    _Pragma("diag_suppress=Pe111") _Pragma("diag_suppress=Pe128") CODE _Pragma("diag_default=Pe111") \
        _Pragma("diag_default=Pe128")

#elif defined(__CC_ARM)

#include <stddef.h>

#define GO_UNUSED_VARIABLE(VARIABLE) \
    do                               \
    {                                \
        if (&VARIABLE == NULL)       \
        {                            \
        }                            \
    } while (false)

#define GO_UNREACHABLE_CODE(CODE) CODE

#elif defined(__TI_ARM__)

#include <stddef.h>

#define GO_UNUSED_VARIABLE(VARIABLE) \
    do                               \
    {                                \
        if (&VARIABLE == NULL)       \
        {                            \
        }                            \
    } while (false)

/*
 * #112-D statement is unreachable
 * #129-D loop is not reachable
 */
#define GO_UNREACHABLE_CODE(CODE) \
    _Pragma("diag_push") _Pragma("diag_suppress 112") _Pragma("diag_suppress 129") CODE _Pragma("diag_pop")

#else

#define GO_UNUSED_VARIABLE(VARIABLE) \
    do                               \
    {                                \
        (void)(VARIABLE);            \
    } while (false)

#define GO_UNREACHABLE_CODE(CODE) CODE

#endif

#ifdef __cplusplus
}
#endif

#endif // GOBO_PLATFORM_TOOLCHAIN_H_
