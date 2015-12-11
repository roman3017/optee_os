PLATFORM_FLAVOR ?= rpi
PLATFORM_FLAVOR_$(PLATFORM_FLAVOR) := y

ifeq ($(PLATFORM_FLAVOR),rpi)
platform-cpuarch = arm1176jzf-s
else
platform-cpuarch = cortex-a9
platform-cflags = -mthumb -mthumb-interwork
endif

platform-cflags += -mcpu=$(platform-cpuarch) -mtune=$(platform-cpuarch)
platform-cflags += -pipe -mlong-calls
platform-cflags += -fno-short-enums -mno-apcs-float -fno-common
platform-cflags += -mfloat-abi=soft
platform-cflags += -mno-unaligned-access
platform-cflags += -ffunction-sections -fdata-sections

platform-aflags = -mcpu=$(platform-cpuarch)

ifeq ($(DEBUG),1)
platform-cflags += -O0
else
platform-cflags += -Os
endif

platform-cflags += -g
platform-aflags += -g

platform-cflags += -g3
platform-aflags += -g3

CFG_ARM32_user_ta := y
user_ta-platform-cflags += $(platform-cflags)
user_ta-platform-cflags += -fpie
user_ta-platform-cppflags += $(platform-cppflags)
user_ta-platform-aflags += $(platform-aflags)
