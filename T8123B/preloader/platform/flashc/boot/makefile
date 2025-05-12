###################################################################
# flag
###################################################################

MOD_SRC  = 
MOD_OBJ  = $(OBJ_PATH)

###################################################################
# source files
# - add your file here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
###################################################################

ifeq ($(DA_BUILD_STAGE),1)
	MOD_SRC := bootstrap1.c
	MOD_SRC += transfer_simple.c
	MOD_SRC += debug_impl.c
	MOD_SRC += storage_access_boot1.c
	MOD_SRC += checksum/checksum_plain_st.c
	MOD_SRC += sparse/sparse_state_machine_stub.c

else
	MOD_SRC += security_export/sec_wrapper.c
	MOD_SRC += security_export/remote_sec_policy.c
	MOD_SRC += bootstrap2.c
	MOD_SRC += sparse/sparse_state_machine.c
	MOD_SRC += transfer_parallel.c
	MOD_SRC += transfer_serial.c
	MOD_SRC	+= debug_impl_sync.c
	MOD_SRC += storage_access_boot2.c
	MOD_SRC += checksum/checksum_plain_mt.c
endif

ifeq ($(CREATE_SEC_LIB),TRUE)
MOD_SRC += security_export/remote_sec_policy.c
MOD_SRC += security_export/sec_wrapper.c
endif

MOD_SRC += error_message.c
MOD_SRC += commands_xml.c
MOD_SRC += protocol_functions.c
MOD_SRC += hal.c
MOD_SRC += handler.c
MOD_SRC += protocol.c
MOD_SRC += system_objects.c
MOD_SRC += partition/partition_info_repertory.c
MOD_SRC += partition/gpt.c
MOD_SRC += partition/pmt.c
MOD_SRC += partition/gpt_gen.c
MOD_SRC += partition/pmt_gen.c
MOD_SRC += partition/hw_depends.c
MOD_SRC += sparse/bulk_process.c
MOD_SRC += sparse/dynamic_partition.c
MOD_SRC += loader_file/commands_preloader.c
MOD_SRC += loader_file/commands_cert.c
MOD_SRC += checksum/checksum_plain.c
MOD_SRC += checksum/checksum_info_manager.c
MOD_SRC += checksum/storage_checksum.c
MOD_SRC += scatter/scatter_parser.c
MOD_SRC += dl_info/dl_archive_proxy.c
MOD_SRC += misc/meta_boot_mode.c
MOD_SRC += dump_speed.c
MOD_SRC += preloader_dummy_functions.c

###################################################################
# target
###################################################################

include $(COMMON_FILE_MK)

###################################################################
# include sub dir
###################################################################
ifeq ($(CREATE_SEC_LIB),TRUE)
SUBDIRS += security
endif

include $(COMMON_DIR_MK)
