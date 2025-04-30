#
# Makefile for the Goodix gt9xx touchscreen driver.
#
#subdir-ccflags-y += -DDEBUG

ifeq ($(CONFIG_TOUCHSCREEN_GT9XX), m)
obj-m	+= gt9xx_drv.o
gt9xx_drv-y	:= gt9xx.o \
				gt9xx_update.o \
				goodix_tool.o
else
obj-$(CONFIG_TOUCHSCREEN_GT9XX)		+= gt9xx.o
obj-$(CONFIG_TOUCHSCREEN_GT9XX_UPDATE)	+= gt9xx_update.o
obj-$(CONFIG_TOUCHSCREEN_GT9XX_TOOL)	+= goodix_tool.o
endif
