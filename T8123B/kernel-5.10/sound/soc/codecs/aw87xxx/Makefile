#for AWINIC AW87XXX Smart K PA
#obj-$(CONFIG_SND_SOC_AW87XXX) += aw87xxx/aw87xxx.o  aw87xxx/aw87xxx_device.o  aw87xxx/aw87xxx_monitor.o  aw87xxx/aw87xxx_bin_parse.o  aw87xxx/aw87xxx_dsp.o  aw87xxx/aw87xxx_acf_bin.o
#obj-$(CONFIG_SND_SOC_AW87XXX) += aw87xxx.o  aw87xxx_device.o  aw87xxx_monitor.o  aw87xxx_bin_parse.o  aw87xxx_dsp.o  aw87xxx_acf_bin.o

# MTK include path
subdir-ccflags-y += -I$(srctree)/sound/soc/mediatek/audio_dsp
subdir-ccflags-y += -I$(srctree)/sound/soc/mediatek/common
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/audio_ipi/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
subdir-ccflags-y += -I$(srctree)/drivers/base

# debugging support (also enables trace_printk)
ccflags-y += -Werror
ccflags-y += $(call cc-option,-Wno-date-time,)

#snd-soc-aw883xx-objs:= aw883xx.o aw_monitor.o aw_bin_parse.o aw_device.o aw_init.o aw_calib.o aw_spin.o
#obj-$(CONFIG_SND_SOC_AW883XX) += snd-soc-aw883xx.o

snd-soc-aw87xxx-objs:= aw87xxx.o  aw87xxx_device.o  aw87xxx_monitor.o  aw87xxx_bin_parse.o  aw87xxx_dsp.o  aw87xxx_acf_bin.o
obj-$(CONFIG_SND_SOC_AW87XXX) += snd-soc-aw87xxx.o



