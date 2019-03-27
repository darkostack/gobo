####################################################
# SRC build
####################################################
SRC_CORE_CPP_SOURCES += $(addprefix core/common/, \
    instance.cpp \
    locator.cpp \
    logging.cpp \
    string.cpp \
    tasklet.cpp \
    timer.cpp \
    )

SRC_CORE_CPP_SOURCES += $(addprefix core/api/, \
    instance_api.cpp \
    tasklet_api.cpp \
    )

SRC_CORE_CPP_SOURCES += $(addprefix core/utils/, \
    heap.cpp \
    )

SRC_CORE_C_SOURCES += $(addprefix core/utils/, \
    missing_strlcat.c \
    missing_strlcpy.c \
    missing_strnlen.c \
    )

ifeq ($(ENABLE_CLI),1)
SRC_CLI_CPP_SOURCES += $(addprefix cli/, \
    cli.cpp \
    cli_uart.cpp \
    )
endif

####################################################
OBJS_SRC_CORE += $(addprefix $(BUILD)/, $(SRC_CORE_CPP_SOURCES:.cpp=.o))
OBJS_SRC_CORE += $(addprefix $(BUILD)/, $(SRC_CORE_C_SOURCES:.c=.o))

ifeq ($(ENABLE_CLI),1)
OBJS_SRC_CLI  += $(addprefix $(BUILD)/, $(SRC_CLI_CPP_SOURCES:.cpp=.o))
endif

OBJS_DIR += $(sort $(dir $(OBJS_SRC_CORE)))

ifeq ($(ENABLE_CLI),1)
OBJS_DIR += $(sort $(dir $(OBJS_SRC_CLI)))
endif

LIB_SRC_CORE_ARCHIVE = $(BUILD)/$(LIB_CORE)
LIB_SRC_CORE_OBJS += $(OBJS_SRC_CORE)

ifeq ($(ENABLE_CLI),1)
LIB_SRC_CLI_ARCHIVE = $(BUILD)/$(LIB_CLI)
LIB_SRC_CLI_OBJS += $(OBJS_SRC_CLI)
endif

ARCHIVE_FILES += $(LIB_SRC_CORE_ARCHIVE)

ifeq ($(ENABLE_CLI),1)
ARCHIVE_FILES += $(LIB_SRC_CLI_ARCHIVE)
endif

all: | $(OBJS_DIR) $(ARCHIVE_FILES)
$(OBJS_DIR):
	$(MKDIR) -p $@

$(BUILD)/%.o: %.c
	$(ECHO) "CC $<"
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/%.o: %.cpp
	$(ECHO) "CPP $<"
	$(CPP) $(CPPFLAGS) -c -o $@ $<

$(BUILD)/%.o: %.s
	$(ECHO) "AS $<"
	$(CC) $(ASFLAGS) -c -o $@ $<

$(BUILD)/%.a:
	$(ECHO) "AR $@"
	$(AR) -cr $@ $^

$(LIB_SRC_CORE_ARCHIVE): $(LIB_SRC_CORE_OBJS)

ifeq ($(ENABLE_CLI),1)
$(LIB_SRC_CLI_ARCHIVE): $(LIB_SRC_CLI_OBJS)
endif

-include $(OBJS_SRC_CORE:%.o=%.d)

ifeq ($(ENABLE_CLI),1)
-include $(OBJS_SRC_CLI:%.o=%.d)
endif
