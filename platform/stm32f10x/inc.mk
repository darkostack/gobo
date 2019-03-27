INC += -I$(TOP)/platform/stm32f10x
INC += -I$(TOP)/platform/stm32f10x/cmsis/core
INC += -I$(TOP)/platform/stm32f10x/cmsis/device
INC += -I$(TOP)/platform/stm32f10x/drivers/inc

DEF += -DSTM32F10X_MD
DEF += -DUSE_STDPERIPH_DRIVER
DEF += -DGOBO_PROJECT_CORE_CONFIG_FILE='\"gobo-core-stm32f10x-config.h\"'

PLATFORM_FLAGS += -mcpu=cortex-m3 -mlittle-endian -mthumb -mfloat-abi=soft
PLATFORM_FLAGS += -march=armv7-m -mno-unaligned-access

PLATFORM_LDSCRIPT_S = $(TOP)/platform/stm32f10x
