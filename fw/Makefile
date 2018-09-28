.PHONY: clean flash test main
default: main

ARM_PREFIX = arm-none-eabi-
OPENCM3_DIR = libopencm3

CC = gcc
LD = gcc
RM = rm
OBJCOPY = objcopy
MKDIR = mkdir -p

CFLAGS = -g -std=c99 -I$(OPENCM3_DIR)/include -DSTM32F0 

ARM_ARCH_FLAGS = -mthumb -mcpu=cortex-m0
ARM_CFLAGS = $(ARM_ARCH_FLAGS) -DNDEBUG $(CFLAGS)
#ARM_CFLAGS += -fno-common -ffunction-sections -fdata-sections
ARM_LDLIBS = -lopencm3_stm32f0
ARM_LDSCRIPT = $(OPENCM3_DIR)/lib/stm32/f0/stm32f04xz6.ld
ARM_LDFLAGS = -L$(OPENCM3_DIR)/lib --static -nostartfiles -T$(ARM_LDSCRIPT)
ARM_LDFLAGS += $(ARM_ARCH_FLAGS)

SRCDIR = src/
TESTDIR = test/
DRIVERDIR = driver/
FAKEDIR = fake/
BUILDDIR = build/
UNITYDIR = Unity/
TARGETDIR = $(BUILDDIR)target/
TARGETDRIVERDIR = $(TARGETDIR)driver/
NATIVEDIR = $(BUILDDIR)native/
NATIVETESTDIR = $(NATIVEDIR)test/
NATIVEFAKEDIR = $(NATIVEDIR)fake/
RESULTDIR = $(BUILDDIR)results/

# I don't like it, but until something better works out
_dummy := $(shell $(MKDIR) $(TARGETDIR) $(TARGETDIR)main/ $(NATIVEDIR) $(NATIVETESTDIR) $(TARGETDRIVERDIR) $(NATIVEFAKEDIR) $(RESULTDIR))

SRCFILES = $(wildcard $(SRCDIR)*.c)
TESTFILES = $(wildcard $(TESTDIR)*.c)
FAKEFILES = $(wildcard $(FAKEDIR)*.c)
DRIVERFILES = $(wildcard $(DRIVERDIR)*.c)

RESULTS = $(patsubst $(TESTDIR)test_%.c,$(RESULTDIR)%.txt,$(TESTFILES))

targetsrc:  $(patsubst $(SRCDIR)%.c,$(TARGETDIR)%.o,$(SRCFILES))
nativesrc:  $(patsubst $(SRCDIR)%.c,$(NATIVEDIR)%.o,$(SRCFILES))
fakesrc:    $(patsubst $(FAKEDIR)%.c,$(NATIVEFAKEDIR)%.o,$(FAKEFILES))
driversrc:  $(patsubst $(DRIVERDIR)%.c,$(TARGETDRIVERDIR)%.o,$(DRIVERFILES))

main: $(TARGETDIR)main.elf

$(TARGETDIR)%.elf: %.c targetsrc driversrc
	$(ARM_PREFIX)$(CC) $(ARM_CFLAGS) -c $*.c -o $(TARGETDIR)main/$*.o
	$(ARM_PREFIX)$(LD) $(ARM_LDFLAGS) $(TARGETDIR)*.o $(TARGETDIR)main/$*.o\
			$(TARGETDRIVERDIR)*.o $(ARM_LDLIBS) -o $(TARGETDIR)$*.elf
	$(ARM_PREFIX)$(OBJCOPY) -Obinary $(TARGETDIR)$*.elf $(TARGETDIR)$*.bin
	$(ARM_PREFIX)$(OBJCOPY) -Oihex $(TARGETDIR)$*.elf $(TARGETDIR)$*.hex

test: $(RESULTS)
	cat $^

$(RESULTDIR)%.txt: $(NATIVETESTDIR)%.o
	-./$< > $@ 2>&1

$(NATIVETESTDIR)%.o:: $(TESTDIR)test_%.c $(SRCDIR)%.c nativesrc fakesrc
	$(CC) $(CFLAGS) -I$(UNITYDIR)src $(TESTDIR)test_$*.c $(NATIVEDIR)*.o $(NATIVEFAKEDIR)*.o $(UNITYDIR)src/unity.c -o $@

$(TARGETDIR)%.o:: $(SRCDIR)%.c
	$(ARM_PREFIX)$(CC) $(ARM_CFLAGS) -c $< -o $@

$(NATIVEDIR)%.o:: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETDRIVERDIR)%.o:: $(DRIVERDIR)%.c
	$(ARM_PREFIX)$(CC) $(ARM_CFLAGS) -c $< -o $@

$(NATIVEFAKEDIR)%.o:: $(FAKEDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find $(BUILDDIR) -type f -delete

flash: main
	$(ARM_PREFIX)gdb $(TARGETDIR)latest.elf\
			-ex "target extended-remote /dev/ttyBmpGdb"\
			-ex "monitor swdp_scan"\
			-ex "attach 1"\
			-ex "load"\
			-ex "run"
