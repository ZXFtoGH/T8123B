MKIMAGE                    := tools/mkimage/mkimage
LOADER_EXT_NAME            := loader_ext
LOADER_EXT_ETC_HEADER_CFG  := $(PRELOADER_OUT)/loader_ext_etc_hdr.cfg
LOADER_EXT_DRAM_HEADER_CFG := $(PRELOADER_OUT)/loader_ext_dram_hdr.cfg

LOADER_EXT_ETC_SECTION  := .loader_ext_etc
LOADER_EXT_DRAM_SECTION := .loader_ext_dram
OBJONCHIP_FLAG          := -R $(LOADER_EXT_ETC_SECTION) -R $(LOADER_EXT_DRAM_SECTION)

preloader_bin: $(D_BIN)/$(LOADER_EXT_NAME).img
$(D_BIN)/$(LOADER_EXT_NAME).img: $(D_BIN)/$(PL_IMAGE_NAME).elf
	$(hide) echo "NAME = loader_ext_etc" > $(LOADER_EXT_ETC_HEADER_CFG)
	$(hide) $(OBJCOPY) $(OBJCFLAGS) -j $(LOADER_EXT_ETC_SECTION) $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $(D_BIN)/$(PL_IMAGE_NAME)_ETC_NO_HDR.bin
	$(hide) $(MKIMAGE) $(D_BIN)/$(PL_IMAGE_NAME)_ETC_NO_HDR.bin $(LOADER_EXT_ETC_HEADER_CFG) > $(D_BIN)/$(PL_IMAGE_NAME)_ETC.bin

	$(hide) echo "NAME = loader_ext_dram" > $(LOADER_EXT_DRAM_HEADER_CFG)
	$(hide) $(OBJCOPY) $(OBJCFLAGS) -j $(LOADER_EXT_DRAM_SECTION) $(D_BIN)/$(PL_IMAGE_NAME).elf -O binary $(D_BIN)/$(PL_IMAGE_NAME)_DRAM_NO_HDR.bin
	$(hide) $(MKIMAGE) $(D_BIN)/$(PL_IMAGE_NAME)_DRAM_NO_HDR.bin $(LOADER_EXT_DRAM_HEADER_CFG) > $(D_BIN)/$(PL_IMAGE_NAME)_DRAM.bin

	# loader_ext.img = loader_ext_dram + loader_ext_etc
	cp $(D_BIN)/$(PL_IMAGE_NAME)_DRAM.bin $(D_BIN)/$(LOADER_EXT_NAME).img
	cat $(D_BIN)/$(PL_IMAGE_NAME)_ETC.bin >> $(D_BIN)/$(LOADER_EXT_NAME).img
