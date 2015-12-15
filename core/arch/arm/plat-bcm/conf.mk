include core/arch/$(ARCH)/plat-$(PLATFORM)/platform_flags.mk

core-platform-cppflags  = -I$(arch-dir)/include
core-platform-subdirs += \
	$(addprefix $(arch-dir)/, kernel mm tee sta) $(platform-dir)
core-platform-subdirs += $(arch-dir)/sm

$(call force,CFG_ARM32_core,y)
$(call force,CFG_GENERIC_BOOT,y)
$(call force,CFG_PL011,y)
$(call force,CFG_PM_STUBS,y)
$(call force,CFG_WITH_VFP,y)
$(call force,CFG_SECURE_TIME_SOURCE_CNTPCT,y)
#$(call force,CFG_SECURE_TIME_SOURCE_REE,y)
#$(call force,CFG_WITH_LPAE,y)
$(call force,CFG_MMU_V7_TTB,y)
#$(call force,CFG_BOOT_SYNC_CPU,y)
#$(call force,CFG_PL310,y)
#$(call force,CFG_CACHE_API,y)
#$(call force,CFG_GIC,y)
#$(call force,CFG_HWSUPP_MEM_PERM_PXN,y)

$(call force,CFG_TEE_CORE_DEBUG,y)
$(call force,CFG_TEE_DEBUG_PANIC,y)
CFG_TEE_CORE_LOG_LEVEL ?= 4

#CFG_WITH_STACK_CANARIES ?= y
#CFG_TEE_FS_KEY_MANAGER_TEST ?= y
#CFG_TEE_CORE_EMBED_INTERNAL_TESTS ?= y
#CFG_WITH_STATS ?= y

include mk/config.mk

ifeq ($(PLATFORM_FLAVOR),rpi)
CFG_DDR_TEETZ_RESERVED_START ?= 0x1BD00000
CFG_DDR_TEETZ_RESERVED_SIZE ?= 0x00300000
else ifeq ($(PLATFORM_FLAVOR),cygnus)
CFG_DDR_TEETZ_RESERVED_START ?= 0x7F800000
CFG_DDR_TEETZ_RESERVED_SIZE ?= 0x00400000
else
$(error PLATFORM_FLAVOR=$(PLATFORM_FLAVOR) is not supported)
endif
