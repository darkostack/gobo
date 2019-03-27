###################################################
# Build info
###################################################
include .gitinfo.mk

BUILD_START_TIME := $(shell date +%b\ %0d\ %Y\ %0H:%0M:%0S)

# Set verbosity
ifeq ($(v), 1)
Q =
else
Q = @
endif

# Optimization level
OPT ?= -Os

HOST_MACHINE    = $(shell uname -s)
HOST_MACHINE_UC = $(shell echo $(HOST_MACHINE) | tr a-z A-Z)

###################################################
# Commands
###################################################
export CC       = $(Q)arm-none-eabi-gcc
export AS       = $(Q)arm-none-eabi-gcc
export LD       = $(Q)arm-none-eabi-ld
export AR       = $(Q)arm-none-eabi-ar
export CPP      = $(Q)arm-none-eabi-g++
export SIZE     = $(Q)arm-none-eabi-size
export STRIP    = $(Q)arm-none-eabi-strip -s
export OBJCOPY  = $(Q)arm-none-eabi-objcopy
export OBJDUMP  = $(Q)arm-none-eabi-objdump
export RM       = $(Q)rm
export MKDIR    = $(Q)mkdir
export ECHO     = $(Q)@echo
ifeq ($(v),1)
export MAKE     = $(Q)make
else
export MAKE     = $(Q)make -s
endif
export TOP      = $(shell pwd)

###################################################
# Targets
###################################################
GOBO      = gobo
PLATFORM ?= stm32f10x
APPS     ?= cli
BUILD     = $(TOP)/build/$(PLATFORM)

###################################################
# Image info
###################################################
IMAGE     = gobo-$(APPS)
IMAGE_OUT = $(BUILD)/$(IMAGE)

###################################################
# Modules directories
###################################################
PLATFORM_DIR = platform/$(PLATFORM)
SRC_DIR      = src
APPS_DIR     = apps/$(APPS)

###################################################
# BUILD config
###################################################
ifeq ($(APPS),cli)
export ENABLE_CLI = 1
else
export ENABLE_CLI = 0
endif

###################################################
# Include variables used in inc.mk
###################################################
INC += -I$(TOP)
INC += -I$(TOP)/include
INC += -I$(TOP)/platform
INC += -I$(TOP)/src

DEF += -DGOBO_PLATFORM_$(shell echo $(PLATFORM) | tr a-z A-Z)
DEF += -DGOBO_ENABLE_CLI=$(ENABLE_CLI)

DEF += -DPACKAGE_NAME='\"GOBO\"'
DEF += -DPACKAGE_VERSION='\"$(GIT_REPO_COMMIT)\"'

include $(TOP)/$(PLATFORM_DIR)/inc.mk
include $(TOP)/$(SRC_DIR)/inc.mk
include $(TOP)/$(APPS_DIR)/inc.mk

CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += $(OPT)
CFLAGS += -Wno-implicit-fallthrough -Wno-unused-parameter
CFLAGS += $(PLATFORM_FLAGS)
CFLAGS += -Wall -Werror
CFLAGS += -std=c99
CFLAGS += -D_BSD_SOURCE=1 -D_DEFAULT_SOURCE=1
CFLAGS += $(DEF)
CFLAGS += $(INC)

CPPFLAGS += -fdata-sections -ffunction-sections
CPPFLAGS += $(OPT)
CPPFLAGS += -Wno-implicit-fallthrough -Wno-unused-parameter
CPPFLAGS += $(PLATFORM_FLAGS)
CPPFLAGS += -fno-exceptions -fno-rtti
CPPFLAGS += -Wall -Werror
CPPFLAGS += -std=gnu++98 -Wno-c++14-compat
CPPFLAGS += -D_BSD_SOURCE=1 -D_DEFAULT_SOURCE=1
CPPFLAGS += $(DEF)
CPPFLAGS += $(INC)

ASFLAGS += -fdata-sections -ffunction-sections
ASFLAGS += $(OPT)
ASFLAGS += -Wno-implicit-fallthrough -Wno-unused-parameter
ASFLAGS += $(PLATFORM_FLAGS)

LDFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += $(OPT) 
LDFLAGS += -Wno-implicit-fallthrough -Wno-unused-parameter
LDFLAGS += $(PLATFORM_FLAGS)
LDFLAGS += -specs=nano.specs
LDFLAGS += -specs=nosys.specs
LDFLAGS += -Wl,--gc-sections

###################################################
# BUILD path for each modules
###################################################
BUILD_PLATFORM = $(BUILD)/$(PLATFORM_DIR)
BUILD_SRC      = $(BUILD)/$(SRC_DIR)
BUILD_APPS     = $(BUILD)/$(APPS_DIR)

###################################################
# Libraries definitions
###################################################
export LIB_PLATFORM = libplatform.a
export LIB_CORE     = libcore.a
ifeq ($(ENABLE_CLI),1)
export LIB_CLI      = libcli.a
endif
export LIB_APPS     = libapps.a

###################################################
# Libraries
###################################################
LIBS += $(BUILD_PLATFORM)/$(LIB_PLATFORM)
LIBS += $(BUILD_SRC)/$(LIB_CORE)
ifeq ($(ENABLE_CLI),1)
LIBS += $(BUILD_SRC)/$(LIB_CLI)
endif
LIBS += $(BUILD_APPS)/$(LIB_APPS)

###################################################
# Linker script
###################################################
LDSCRIPT_S = $(PLATFORM_LDSCRIPT_S)/$(PLATFORM).ld.S
LDSCRIPT   = $(BUILD_PLATFORM)/$(PLATFORM).ld

###################################################
# Modules makefile directory
###################################################
MAKE_PLATFORM = $(PLATFORM_DIR)
MAKE_SRC      = $(SRC_DIR)
MAKE_APPS     = $(APPS_DIR)

####################################################
# Make rules
####################################################
all: $(GOBO) BUILD_FINISHED_INFO

$(BUILD):
	$(MKDIR) -p $@

OBJS:
	$(MAKE) -C $(MAKE_PLATFORM) BUILD=$(BUILD_PLATFORM) CFLAGS="$(CFLAGS)" ASFLAGS="$(ASFLAGS)"
	$(MAKE) -C $(MAKE_SRC)      BUILD=$(BUILD_SRC)      CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)"
	$(MAKE) -C $(MAKE_APPS)     BUILD=$(BUILD_APPS)     CFLAGS="$(CFLAGS)" CPPFLAGS="$(CPPFLAGS)"

$(IMAGE_OUT).elf: $(LIBS) $(LDSCRIPT)
	$(CC) $(LDFLAGS) $(patsubst %,-L%,$(patsubst %/,%,$(sort $(dir $(LIBS)))))  \
	-Wl,--whole-archive $(patsubst %,-l%,$(patsubst lib%,%,$(sort $(basename $(notdir $(LIBS)))))) \
	-Wl,--no-whole-archive -T $(LDSCRIPT) \
	-Wl,-Map,$(BUILD)/gobo-$(APPS).map -o $@

$(IMAGE_OUT).bin: $(IMAGE_OUT).elf
	$(OBJCOPY) -O binary $< $@

$(IMAGE_OUT).lst: $(IMAGE_OUT).elf
	$(OBJDUMP) -h -S $< > $@

$(LDSCRIPT): $(LDSCRIPT_S)
	$(CC) -E -P $< $(CFLAGS) -o $@

$(IMAGE): OBJS $(IMAGE_OUT).bin $(IMAGE_OUT).lst
	$(SIZE) --format=berkeley $(IMAGE_OUT).elf

$(GOBO): $(IMAGE) | $(BUILD)

BUILD_FINISHED_INFO:
	$(ECHO) ""
	$(ECHO) "---------------------------------------------------------------------"
	$(ECHO) "\033[32mFinished Building\033[0m : [$(PLATFORM)] in $(HOST_MACHINE)"
	$(ECHO) "---------------------------------------------------------------------"
	$(ECHO) "Image            : build/$(PLATFORM)/\033[32mgobo-$(APPS).bin\033[0m"
	$(ECHO) "---------------------------------------------------------------------"
	$(ECHO) "Build Start Time : $(BUILD_START_TIME)"
	$(ECHO) "Build End Time   : $(shell date +%b\ %0d\ %Y\ %0H:%0M:%0S)"
	$(ECHO) "---------------------------------------------------------------------"
	$(ECHO) "Repository info: $(GIT_REPO_COMMIT) @ [\033[33m$(GIT_REPO_BRANCH)\033[0m], dirty = $(GIT_REPO_DIRTY)"

.PHONY: all clean size

# Note: `make clean` will remove everything inside `build` folder
clean:
	-rm -rf $(BUILD)

size:
	$(SIZE) --format=SysV $(IMAGE_OUT).elf
