###################################################################
# flag
###################################################################

MOD_SRC  = 
MOD_OBJ  = $(PRELOADER_OUT)/obj

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

MOD_SRC  := \
	download.c \
	handshake_uart.c \
	handshake_usb.c \
	main.c \
	partition.c \
	print.c \
	dram_buffer.c

ifeq ("$(CFG_FUNCTION_PICACHU_SUPPORT)","1")
MOD_SRC += picachu_misc.c
endif

ifeq ("$(CFG_ENABLE_DCACHE)","1")
MOD_SRC += mmu.c \
	mmu_misc.s
endif

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)
