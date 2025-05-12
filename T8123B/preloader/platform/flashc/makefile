###################################################################
# flag
###################################################################

MOD_SRC  =
MOD_OBJ  = $(OBJ_PATH)

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################
ifeq ($(DA_ARCH_AARCH64),1)
ifeq ($(DA_BUILD_STAGE),1)
MOD_SRC 		+= arch/arm64/bl31_entrypoint.S

else
MOD_SRC 		+= arch/arm64/arch.c
MOD_SRC 		+= arch/arm64/start.S
MOD_SRC 		+= arch/arm64/exceptions.S
MOD_SRC 		+= arch/arm64/exceptions_c.c
MOD_SRC += kernel/arm64/event.c
MOD_SRC += kernel/arm64/mp.c
MOD_SRC += arch/arm64/mmu.c
MOD_SRC += kernel/arm64/kmain.c
endif
MOD_SRC 		+= arch/arm64/asm.S
MOD_SRC 		+= arch/arm64/mediatek/asm.S
MOD_SRC 		+= arch/arm64/cache-ops.S
MOD_SRC 		+= arch/arm64/mediatek/cache-ops.S
MOD_SRC 		+= arch/arm64/mp.c
MOD_SRC 		+= arch/arm64/thread.c
MOD_SRC 		+= arch/arm64/synchronic_api.c
MOD_SRC += kernel/arm64/novm/novm.c
MOD_SRC 		+= arch/arm64/fpu.c
MOD_SRC += kernel/arm64/mutex.c
MOD_SRC += kernel/arm64/event.c
MOD_SRC += kernel/arm64/thread.c
MOD_SRC += kernel/arm64/timer.c
MOD_SRC += kernel/arm64/dpc.c
else
ifeq ($(DA_BUILD_STAGE),1)
MOD_SRC 		+= arch/arm/crt-init.S
else
MOD_SRC 		+= arch/arm/arch.c
MOD_SRC 		+= arch/arm/cache.c
MOD_SRC 		+= arch/arm/faults.c
MOD_SRC 		+= arch/arm/mmu.c
MOD_SRC 		+= arch/arm/thread.c
MOD_SRC 		+= arch/arm/asm.S
MOD_SRC 		+= arch/arm/crt-loop.S
MOD_SRC 		+= arch/arm/exceptions.S
MOD_SRC         += kernel/arm/kmain.c
endif

MOD_SRC 		+= arch/arm/mmu_lpae.c
MOD_SRC 		+= arch/arm/ops.S
MOD_SRC 		+= arch/arm/cache-ops.S
MOD_SRC 		+= arch/arm/synchronic_api.c

MOD_SRC 		+= arch/arm/div/crt.S
MOD_SRC 		+= arch/arm/div/idiv.S
MOD_SRC 		+= arch/arm/div/idivmod.S
MOD_SRC 		+= arch/arm/div/ldivmod.S
MOD_SRC 		+= arch/arm/div/umodsi3.S
MOD_SRC 		+= arch/arm/div/umoddi3.S
MOD_SRC 		+= arch/arm/div/modsi3.S

MOD_SRC += kernel/arm/dpc.c
MOD_SRC += kernel/arm/event.c
MOD_SRC += kernel/arm/mutex.c
MOD_SRC += kernel/arm/thread.c
MOD_SRC += kernel/arm/timer.c

endif
###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# include sub dir
###################################################################l

SUBDIRS += boot
SUBDIRS += lib
SUBDIRS += driver

include $(COMMON_DIR_MK)
