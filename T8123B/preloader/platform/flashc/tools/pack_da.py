import struct
import sys
import getopt
import hashlib
import time
import xml
from xml.dom import minidom

DA_IDENTIFIER = "MTK_DOWNLOAD_AGENT"
DA_DESC = "MTK_DA_v6_"
DA_VERSION = 0x4
DA_MAGIC = 0x22668899
DA_ENTRY_MAGIC = 0xDADA
DA_MAX_LOAD_REGION = 10
DA_MAX_ENTRY = 64
DA_IDENTIFIER_LEN = 32
DA_DESCRIPTION_LEN = 64
MAX_NAME_LEN = 64
SHA256_SIG_LEN = 256
DA_FAKE_SIGNATURE = "\0"*256
DA_2ND_HASH_TAIL_OFFSET = -48

if __name__ == '__main__':
    if len(sys.argv) != 5:
        print("usage:\n\n pack_da.py chip.xml /da_1st.bin /da_2nd.bin /DA.bin\n")
        sys.exit(-1)

    opts, args = getopt.getopt(sys.argv[1:], '')
    chip_xml = args[0]
    bin1_path = args[1]
    bin2_path = args[2]
    da_bin_out_path = args[3]

    hdr_fmt = '<32s64s3I'
    entry_fmt = '<6HI2H'
    section_fmt = '<5I'

    try:
        #xmlparser
        tree = xml.dom.minidom.parse(chip_xml)
        root = tree.documentElement

        hw_code = int(root.getElementsByTagName("hw_code")[0].firstChild.nodeValue, 16)
        hw_sub_code = int(root.getElementsByTagName("hw_sub_code")[0].firstChild.nodeValue, 16)
        hw_version = int(root.getElementsByTagName("hw_version")[0].firstChild.nodeValue, 16)
        sw_version = int(root.getElementsByTagName("sw_version")[0].firstChild.nodeValue, 16)
        feature_set = int(root.getElementsByTagName("feature_set")[0].firstChild.nodeValue, 16)
        sram_code_start = int(root.getElementsByTagName("sram_code_start")[0].firstChild.nodeValue, 16)
        dram_code_start = int(root.getElementsByTagName("dram_code_start")[0].firstChild.nodeValue, 16)
        entry_index = 0
        sections_cnt = 3

        da_header = struct.pack(hdr_fmt, DA_IDENTIFIER.encode('utf-8'),
                                (DA_DESC+time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())).encode('utf-8'),
                                DA_VERSION, DA_MAGIC, 1)

        padding_align = 0
        da_entry = struct.pack(entry_fmt, DA_ENTRY_MAGIC, hw_code,
                                hw_sub_code, hw_version, sw_version, padding_align, feature_set, entry_index, sections_cnt)

        da_bin1 = open(bin1_path, "rb")
        da_bin2 = open(bin2_path, "rb")
        da_bin1_buf = bytearray(da_bin1.read())
        da_bin2_buf = da_bin2.read()
        
        if da_bin1_buf[DA_2ND_HASH_TAIL_OFFSET:DA_2ND_HASH_TAIL_OFFSET+2] != bytearray([0xBE, 0xBF]) :
            raise Exception("Hash magic is not 0xBE,0xBF. Contact RD.")

        da_bin2_hash = hashlib.sha256(da_bin2_buf).digest()
        da_bin1_buf[DA_2ND_HASH_TAIL_OFFSET:DA_2ND_HASH_TAIL_OFFSET+len(da_bin2_hash)] = da_bin2_hash

        sec1_offset = struct.calcsize(hdr_fmt) + struct.calcsize(entry_fmt) + struct.calcsize(section_fmt) * sections_cnt
        sec1_length = len(da_bin1_buf) + SHA256_SIG_LEN
        sec1_start_addr = sram_code_start
        sec1_sig_offset = sec1_offset + sec1_length - SHA256_SIG_LEN
        sec1_sig_length = SHA256_SIG_LEN
        da_section_1 = struct.pack(section_fmt, sec1_offset, sec1_length,
                                sec1_start_addr, sec1_sig_offset, sec1_sig_length)

        sec2_offset = sec1_sig_offset + sec1_sig_length
        sec2_length = len(da_bin2_buf) + SHA256_SIG_LEN
        sec2_start_addr = dram_code_start
        sec2_sig_offset = sec2_offset + sec2_length - SHA256_SIG_LEN
        sec2_sig_length = SHA256_SIG_LEN
        da_section_2 = struct.pack(section_fmt, sec2_offset, sec2_length,
                                   sec2_start_addr, sec2_sig_offset, sec2_sig_length)

        da_binary = open(da_bin_out_path, "w+b")
        da_binary.seek(0)
        da_binary.write(da_header)
        da_binary.write(da_entry)
        da_binary.write(da_section_1)  #section 0 EPP use section 1
        da_binary.write(da_section_1)
        da_binary.write(da_section_2)
        da_binary.write(da_bin1_buf)
        da_binary.write(DA_FAKE_SIGNATURE.encode('utf-8'))
        da_binary.write(da_bin2_buf)
        da_binary.write(DA_FAKE_SIGNATURE.encode('utf-8'))

        da_binary.close()
    except Exception as e:
        print("pack_da.py exception:")
        print(str(e))
        sys.exit(-1)

    sys.exit(0)

