####################################################
# APPS build
####################################################
APPS_C_SOURCES += \
   main.c

####################################################
OBJS_APPS += $(addprefix $(BUILD)/, $(APPS_C_SOURCES:.c=.o))

OBJS_DIR += $(sort $(dir $(OBJS_APPS)))

LIB_APPS_ARCHIVE = $(BUILD)/$(LIB_APPS)
LIB_APPS_OBJS += $(OBJS_APPS)

ARCHIVE_FILES += $(LIB_APPS_ARCHIVE)

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

$(LIB_APPS_ARCHIVE): $(LIB_APPS_OBJS)

-include $(OBJS_APPS:%.o=%.d)
