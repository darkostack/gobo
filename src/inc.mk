INC += -I$(TOP)/src/core
ifeq ($(ENABLE_CLI),1)
INC += -I$(TOP)/src/cli
endif
