####################################################
# PLATFORM build
####################################################
PLATFORM_C_SOURCES += $(addprefix cmsis/core/, \
    core_cm3.c \
    )

PLATFORM_C_SOURCES += $(addprefix cmsis/device/, \
    system_stm32f10x.c \
    )

PLATFORM_C_SOURCES += $(addprefix drivers/src/, \
    misc.c \
    stm32f10x_adc.c \
    stm32f10x_bkp.c \
    stm32f10x_can.c \
    stm32f10x_cec.c \
    stm32f10x_crc.c \
    stm32f10x_dac.c \
    stm32f10x_dbgmcu.c \
    stm32f10x_dma.c \
    stm32f10x_exti.c \
    stm32f10x_flash.c \
    stm32f10x_fsmc.c \
    stm32f10x_gpio.c \
    stm32f10x_i2c.c \
    stm32f10x_iwdg.c \
    stm32f10x_pwr.c \
    stm32f10x_rcc.c \
    stm32f10x_rtc.c \
    stm32f10x_sdio.c \
    stm32f10x_spi.c \
    stm32f10x_tim.c \
    stm32f10x_usart.c \
    stm32f10x_wwdg.c \
    )

PLATFORM_C_SOURCES += \
    alarm.c \
    logging.c \
    misc.c \
    system.c \
    uart-stm32f10x.c

PLATFORM_C_SOURCES += $(addprefix ../utils/, \
    debug_uart.c \
    )

PLATFORM_STARTUP_FILES = stm32f10x.s

####################################################
OBJS_PLATFORM += $(addprefix $(BUILD)/, $(PLATFORM_C_SOURCES:.c=.o))
OBJS_PLATFORM += $(addprefix $(BUILD)/, $(PLATFORM_STARTUP_FILES:.s=.o))

OBJS_DIR += $(sort $(dir $(OBJS_PLATFORM)))

LIB_PLATFORM_ARCHIVE = $(BUILD)/$(LIB_PLATFORM)
LIB_PLATFORM_OBJS   += $(OBJS_PLATFORM)

ARCHIVE_FILES += $(LIB_PLATFORM_ARCHIVE)

all: | $(OBJS_DIR) $(ARCHIVE_FILES)
$(OBJS_DIR):
	$(MKDIR) -p $@

$(BUILD)/%.o: %.c
	$(ECHO) "CC $<"
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/%.o: %.s
	$(ECHO) "AS $<"
	$(CC) $(ASFLAGS) -c -o $@ $<

$(BUILD)/%.a:
	$(ECHO) "AR $@"
	$(AR) -cr $@ $^

$(LIB_PLATFORM_ARCHIVE): $(LIB_PLATFORM_OBJS)

-include $(OBJS_PLATFORM:%.o=%.d)
