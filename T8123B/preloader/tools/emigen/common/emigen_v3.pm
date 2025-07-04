#!/usr/local/bin/perl
#
#*****************************************************************************
#  Copyright Statement:
#  --------------------
#  This software is protected by Copyright and the information contained
#  herein is confidential. The software may not be copied and the information
#  contained herein may not be used or disclosed except with the written
#  permission of MediaTek Inc. (C) 2008
#
#  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
#  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
#  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
#  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
#  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
#  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
#  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
#  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
#  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
#  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
#
#  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
#  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
#  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
#  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
#  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
#  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
#  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
#  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
#  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
#  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
#
#****************************************************************************/
#*
#* Filename:
#* ---------
#*   emigen_sp.pl
#*
#* Project:
#* --------
#*
#*
#* Description:
#* ------------
#*   This script will
#*       1. parse custom_MemoryDevice.h to get memory device type and part number
#*       2. read a excel file to get appropriate emi setting based on the part number
#*       3. based on the emi settings, generate custom_EMI.c if not exist
#*       4. based on the emi settings, generate custom_EMI.h if not exist
#*
#* Author:
#* -------
#*
#*============================================================================
#*             HISTORY
#* Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
#*------------------------------------------------------------------------------
#* $Revision$
#* $Modtime$
#* $Log$
#*
#*
#*------------------------------------------------------------------------------
#* Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
#*============================================================================
#****************************************************************************/

#****************************************************************************
# Included Modules
#****************************************************************************

package emigen_v3;

use strict;
use File::Basename;
use File::Path qw(make_path);
use Class::Struct;

my $LOCAL_PATH;

BEGIN {
	$LOCAL_PATH = dirname($0);
}

use lib "$LOCAL_PATH/../";
use pack_dep_gen;

use lib "$LOCAL_PATH";
use xls_v1;

my $start_num = 0;
my $LPDDR_START = 0;

our @ISA= qw( Exporter );
our @EXPORT_OK = qw( emigen_set_id emigen_support_ddr emigen_support_hw_trap emigen_check_bldr emigen_opt_set emigen_run emigen_set_lpddr_id);
our @EXPORT    = qw( emigen_set_id emigen_support_ddr emigen_support_hw_trap emigen_check_bldr emigen_opt_set emigen_run emigen_set_lpddr_id);

#****************************************************************************
# Field define, add new field into SCAN_COL for scan
#****************************************************************************

struct( Column => {
		index => '$',
		name => '$',		# index used in $MDL[$id]->data()
		field_name => '$',	# field name used in xls
		});

my $COL_VENDOR         = Column->new(field_name => 'Vendor', name => 'VENDER');
my $COL_PART_NUMBER    = Column->new(field_name => 'Part Number', name => 'PARTNUM');
my $COL_TYPE           = Column->new(field_name => 'Type', name => 'DEV_TYPE');
my $COL_DENSITY        = Column->new(field_name => 'Density (Mb)', name => 'DENSITY');
my $COL_BOARD_ID       = Column->new(field_name => 'Board ID');
my $COL_NAND_EMMC_ID   = Column->new(field_name => 'NAND/eMMC ID', name => 'NAND_EMMC_ID');
my $COL_CHIP_ID        = Column->new(field_name => 'CHIP_ID', name => 'CHIP_ID');
my $COL_MODE_REG5      = Column->new(field_name => 'MODE_REG5', name => 'MODE_REG5');

my @SCAN_COL = ( $COL_VENDOR, $COL_PART_NUMBER, $COL_TYPE, $COL_DENSITY, $COL_BOARD_ID,
		$COL_NAND_EMMC_ID, $COL_CHIP_ID, $COL_MODE_REG5 );

#****************************************************************************
# DDR type define, add new type into SCAN_COL for scan
#****************************************************************************




struct( DdrType => {
		name => '$',
		type_id => '$',
		support_multi_dis => '$',	# support multiple discrete dram in one load (ie. different rank size)
		supported => '$',
		hw_trap_defined => '$',
		found_mcp => '$',
		found_discrete => '$',
		});

my $DDR_TYPE_DDR1    = DdrType->new(name => 'DDR1', type_id => '01');
my $DDR_TYPE_DDR2    = DdrType->new(name => 'DDR2', type_id => '02');
my $DDR_TYPE_DDR3    = DdrType->new(name => 'DDR3', type_id => '03');
my $DDR_TYPE_DDR4    = DdrType->new(name => 'DDR4', type_id => '04');
my $DDR_TYPE_LPDDR2  = DdrType->new(name => 'LPDDR2', type_id => '05', support_multi_dis => 1);
my $DDR_TYPE_LPDDR3  = DdrType->new(name => 'LPDDR3', type_id => '06', support_multi_dis => 1);
my $DDR_TYPE_PCDDR3  = DdrType->new(name => 'PCDDR3', type_id => '07');
my $DDR_TYPE_LPDDR4  = DdrType->new(name => 'LPDDR4', type_id => '08', support_multi_dis => 1);
my $DDR_TYPE_LPDR4X  = DdrType->new(name => 'LPDR4X', type_id => '09', support_multi_dis => 1);
my $DDR_TYPE_LPDR4P  = DdrType->new(name => 'LPDR4P', type_id => '10', support_multi_dis => 1);
my $DDR_TYPE_LPDDR5  = DdrType->new(name => 'LPDDR5', type_id => '11', support_multi_dis => 1);
my $DDR_TYPE_LPDDR5X = DdrType->new(name => 'LPDDR5X', type_id => '12', support_multi_dis => 1);

my @ALL_DDR_TYPE = ( $DDR_TYPE_DDR1, $DDR_TYPE_LPDDR2, $DDR_TYPE_LPDDR3, $DDR_TYPE_PCDDR3, $DDR_TYPE_LPDDR4, $DDR_TYPE_LPDR4X, $DDR_TYPE_LPDR4P, $DDR_TYPE_LPDDR5, $DDR_TYPE_LPDDR5X);

#****************************************************************************
# data structure for parsing data
#****************************************************************************

struct( MemDevice => {
		data => '%',
		mcp_type => '$',
		ddr => '$',
		mcp_id_str => '$',
		mcp_id_len => '$',
		dram_rank_size => '@',
		emi_setting_str => '$',
		});

#****************************************************************************
# Constants
#****************************************************************************
my $EMIGEN_VERNO  = " V0.01";
# V0.01, Zhen Jiang, Porting emigen to DUMA project
#
my $DebugPrint    = 1; # 1 for debug; 0 for non-debug

my $TotalCustemChips = 0 ;

my $CustBoard_ID ;

my $EMIGEN_CHIP_ID;
my $EMIGEN_TAG_ID;

######################################################################################
my @MDL;
######################################################################################

my $USE_EMMC_ID_LEN=9;
my $MAX_NAND_EMMC_ID_LEN=16;

my $Discrete_DDR = 0;

my @DISCRETE_MDLS;

my $BLDR_CHECK = 0;


#****************************************************************************
# COMBO_MCP_RULE_V
#  1: same emmc id can not be comboed
#  2: same emmc id and same dram type and same rank size can noe be comboed
#****************************************************************************
my $COMBO_MCP_RULE_V = 1;

#
# all above are arrays, each represents an user defined chip.
#
# this is the ID of the custom board.

#****************************************************************************************
# subroutine:  emigen_set_id
#              saving platform information before parsing
# porting:     no need
# input:       $chip_id - chip name (ex: "MT6763")
# input:       $tag_id  - tag id for flashtool (ex: "v30")
# return:      void
#****************************************************************************************
sub emigen_set_id {
	$EMIGEN_CHIP_ID = shift;
	$EMIGEN_TAG_ID = shift;
}



#****************************************************************************************
# subroutine:  emigen_set_lpddr_id
#              Set the LPDDR2 Start ENUM Number from DRAM_DRAM_TYPE_T in dram_pi_api.h
# porting:     no need
# input:       $begin_num  -> if TYPE_LPDDR2 enum number = 5, then input 5
# return:      void
#****************************************************************************************
sub emigen_set_lpddr_id {
	$LPDDR_START = shift;
}
#****************************************************************************************
# subroutine:  emigen_support_ddr
#              specify ddr type which is supported, can be called multiple times
# porting:     no need
# input:       $type - ddr type name (ex: "LPDR4X")
# return:      void
#****************************************************************************************
sub emigen_support_ddr {
	my $type = shift;

	foreach my $n (@ALL_DDR_TYPE) {
		if ($n->name eq $type) {
			$n->supported(1);
			last;
		}
	}
}

#****************************************************************************************
# subroutine:  emigen_support_hw_trap
#              specify ddr type which could be identify by hw trapping
# porting:     no need
# input:       $type - ddr type name (ex: "LPDR4X")
# return:      void
#****************************************************************************************
sub emigen_support_hw_trap {
	my $type = shift;

	foreach my $n (@ALL_DDR_TYPE) {
		if ($n->name eq $type) {
			die("[Error] Need to set support before setting hw trap\n") if ($n->supported ne 1);
			$n->hw_trap_defined(1);
			last;
		}
	}
}

#****************************************************************************************
# subroutine:  emigen_check_bldr
#              specify ddr type which could be identify by hw trapping
# porting:     no need
# input:       $check - 0: dont check bldr with MDL (default) , 1: check bldr with MDL
# return:      void
#****************************************************************************************
sub emigen_check_bldr {
	my $check = shift;

	if ($check == 1) {
		$BLDR_CHECK = 1;
	} else {
		$BLDR_CHECK = 0;
	}
}

#****************************************************************************************
# subroutine:  emigen_opt_set
#              set config for emigen
# porting:     no need
# input:       $opt - config
# input:       $val - value
# return:      void
#****************************************************************************************
sub emigen_opt_set {
	my $opt = shift;
	my $val = shift;

	if ($opt eq "COMBO_MCP_RULE_V") {
		$COMBO_MCP_RULE_V = $val if ($val >=1 && $val <= 2);
	}
}

#****************************************************************************************
# subroutine:  emigen_run
#              parsing mdl from xls and generate hdr file and tag file
# porting:     TBD
# input:       $custom_memory_device_hdr - custom_MemoryDevice.h
# input:       $memory_device_list_xls   - XLS file
# input:       $platform                 - platform id (ex: "MT6763")
# input:       $project                  - project name
# input:       $mtk_emigen_out_dir       - output dir
# input:       $build_opt                - 0: gen hdr file, 1: gen tag file
# return:      void
#****************************************************************************************
sub emigen_run {
	my $custom_memory_device_hdr = shift;
	my $memory_device_list_xls = shift;
	my $project = shift;
	my $mtk_emigen_out_dir = shift;
	my $build_opt = shift;

	my $custom_emi_h = "$mtk_emigen_out_dir/custom_emi.h";
	my $info_tag     = "$mtk_emigen_out_dir/MTK_Loader_Info.tag";

	PrintDependModule($0);

	if ($DebugPrint == 1) {
		print "$custom_memory_device_hdr\n$memory_device_list_xls\n$EMIGEN_CHIP_ID\n";
		print "$mtk_emigen_out_dir\n";
		print "$custom_emi_h\n$info_tag\n"
	}
	my @CustCS_PART_NUMBER = &parse_custom_memory_device_hdr($custom_memory_device_hdr);
	my $bldr_mcp_type;

	$bldr_mcp_type = &parse_cust_bldr_mak($custom_memory_device_hdr) if ($BLDR_CHECK == 1);

	my $Book = &xls_open($memory_device_list_xls);
	PrintDependency($memory_device_list_xls);

# select worksheet
	my $Sheet = &get_sheet($Book, lc $EMIGEN_CHIP_ID) ;
# find cell address of the specified Nand ID

	die("[Error] Can not find worksheet ".lc($EMIGEN_CHIP_ID)."\n") if ($Sheet eq undef);

# scan column index
	&find_field_position($Sheet);

	print "COMBO_MCP_RULE_V: $COMBO_MCP_RULE_V\n";

	my $iter = 0 ;

	while ($iter<scalar(@CustCS_PART_NUMBER)) {
		&DeviceListParser_LPSDRAM($Sheet, $CustCS_PART_NUMBER[$iter++]);
	}

	$iter = 0 ;
	print "TotalCustemChips:$TotalCustemChips\n";
	while ($iter < $TotalCustemChips) {
		die("[Error] Type(".$MDL[$iter]->data('PARTNUM').") is not in supported list !!!\n") if  (!&IsSupportedDram($MDL[$iter]));

		if (&IsDiscreteDram($MDL[$iter])) {
# if all discrete dram are LPDDR2, LPDDR3, LPDDR4, LPDDR4X and LPDDR4P, it is allowed
			if (&SupportMultipleDiscrete($MDL[$iter])) {
				my $iter_dis_dram;

				$iter_dis_dram = $iter + 1;
				while ($iter_dis_dram < $TotalCustemChips) {
					if (&IsDiscreteDram($MDL[$iter_dis_dram])) {
						&CanBeComboDiscrete($MDL[$iter], $MDL[$iter_dis_dram]);
					}
					$iter_dis_dram++;
				}
				$Discrete_DDR++;
			} else {
# DDR1,DDR3 only support one discrete DRAM in the Combo List
# if more then one discrete DRAM in the list, send build error
				if ($Discrete_DDR == 0) {
					$Discrete_DDR++;
					if ($TotalCustemChips > 1) {
						print "[Error]At most one discrete PCDDR3 DRAM is allowed in the Combo MCP list\n" ;
						die "[Error]Combo discrete DRAM only support LPDDR2, LPDDR3, LPDDR4, LPDDR4X and LPDDR4P\n" ;
					}
				} else {
					die "[Error]more than 1 Discrete DDR used!\n" ;
				}
			}
		} else {
# Nand or eMMC's + FW ID must unique
			my $iter2 ;
			$iter2 = $iter + 1 ;
			while ($iter2 < $TotalCustemChips) {
				if (!&IsDiscreteDram($MDL[$iter2])) {
					&CanBeComboMCP($MDL[$iter], $MDL[$iter2]);
				}
				$iter2++;
			}
		}
		$iter++;
	}

	die "[Error] PCDDR3 not support Combo Discrete DRAM feature.\n" if ($DDR_TYPE_PCDDR3->found_discrete > 1);
	die "[Error] PCDDR3 not support Combo Discrete DRAM feature.\n" if (($DDR_TYPE_PCDDR3->found_discrete == 1) && (scalar(@CustCS_PART_NUMBER)>1) );

	&check_bldr_with_mdl($bldr_mcp_type) if ($BLDR_CHECK == 1);

	if ($build_opt == 0) {
		&write_header_file($custom_emi_h, $memory_device_list_xls);
	} elsif ($build_opt == 1) {
# Build Preloader TAG!
		&write_tag($project, $info_tag, $EMIGEN_TAG_ID);
	}
}

#****************************************************************************************
# subroutine:  parse_cust_bldr_mak
#              parse cust_bldr.mak, and save CFG_BOOT_DEV into variables
# porting:     only when new mcp type are added
# input:       $filename - file path to custom_MemoryDevice.h
# return:      $mcp_type - mcp type defined in cust_bldr.mak
#****************************************************************************************
sub parse_cust_bldr_mak {
	my $custom_memory_device_hdr_filename = shift;
	my $filename;
	my $bldr;
	my $mcp_type;

	$filename = basename(dirname(dirname($custom_memory_device_hdr_filename))."/cust_bldr.mak");
	$filename = dirname(dirname($custom_memory_device_hdr_filename))."/cust_bldr.mak";

	open CUST_BLDR_MAK, "<$filename" or &error_handler("$filename: file error!", __FILE__, __LINE__);
	PrintDependency($filename);

	while (<CUST_BLDR_MAK>) {
		if (/^CFG_BOOT_DEV\s?:=\s?(\w*)/) {
			$bldr = $1;
			if ($bldr eq "BOOTDEV_SDMMC") {
				$mcp_type = "02";
			} elsif ($bldr eq "BOOTDEV_UFS") {
				$mcp_type = "03";
			}
		}

	}

	print "\nBLDR: $bldr, mcp_type: $mcp_type\n" if ($DebugPrint == 1);
	close CUST_BLDR_MAK;

	return $mcp_type;
}

#****************************************************************************************
# subroutine:  parse_custom_memory_device_hdr
#              parse custom_MemoryDevice.h, and save part number into variables
# porting:     no need
# input:       $filename - file path to custom_MemoryDevice.h
# return:      $CustCS_PART_NUMBER - array with found part number
# return:      $CustCS_CustemChips - total number of found part number
#****************************************************************************************
sub parse_custom_memory_device_hdr {
	my $filename = shift;
	my @CustCS_PART_NUMBER;
	my $CustCS_CustemChips;

	open CUSTOM_MEMORY_DEVICE_HDR, "<$filename" or &error_handler("$filename: file error!", __FILE__, __LINE__);
	PrintDependency($filename);

	@CustCS_PART_NUMBER = ();

	while (<CUSTOM_MEMORY_DEVICE_HDR>) {
		if (/^#if|^#ifdef|^#ifndef|^#elif|^#else/) {
			&error_handler("$filename: Not allowed to set conditional keywords $_ in custom_MemoryDevice.h!", __FILE__, __LINE__)
				unless (/^#ifndef\s+__CUSTOM_MEMORYDEVICE__/);
		}
		if (/^#define\s+(\w+)\[(\d+)\]\s+\(([-\w]*)\)/ || /^#define\s+(\w+)\[(\d+)\]\s+([-\w]*)/ ||
				/^#define\s+(MEMORY_DEVICE_TYPE)\s+\((\w*)\)/ || /^#define\s+(MEMORY_DEVICE_TYPE)\s+(\w*)/ ||
				/^#define\s+(BOARD_ID)\s+\((\w*)\)/ || /^#define\s+(BOARD_ID)\s+(\w*)/) {

			if ($1 eq "BOARD_ID") {
				print "\nBOARD_ID $2\n";
				$CustBoard_ID = $2 ;
			} elsif ($1 eq "CS_PART_NUMBER") {
				print "\nCS0 $2, $3\n" ;
				push @CustCS_PART_NUMBER, $3;
			}
		}

	}

	$CustCS_CustemChips = scalar(@CustCS_PART_NUMBER);
	print "\n$CustCS_CustemChips\n" if ($DebugPrint == 1);
	close CUSTOM_MEMORY_DEVICE_HDR;

	die "\n[Error]CustCS_CustemChips($CustCS_CustemChips) > 20\n" if ($CustCS_CustemChips > 20);
	die "\n[Error]CustCS_CustemChips($CustCS_CustemChips) = 0?\n" if ($CustCS_CustemChips == 0);

	return @CustCS_PART_NUMBER;
}

#****************************************************************************************
# subroutine:  error_handler
#              print error msg and stop program
# porting:     no need
# input:       $error_msg - error msg
# input:       $file      - file
# input:       $line_no   - error line
# return:      void
#****************************************************************************************
sub error_handler {
	my ($error_msg, $file, $line_no) = @_;

	my $final_error_msg = "[Error]EMIGEN ERROR: $error_msg at $file line $line_no\n";
	print $final_error_msg;
	die $final_error_msg;
}

#****************************************************************************************
# subroutine:  find_field_position
#              find all field's position in xls, make sure all field are found
# porting:     no need
# input:       $Sheet - XLS sheet object
# return:      void
#****************************************************************************************
sub find_field_position {
	my ($Sheet) = shift;

	foreach my $n (@SCAN_COL) {
		die("[Error] Not valid Column in @SCAN_COL, please check !!!!!\n") if (ref($n) ne "Column");
	}

	foreach my $n (@ALL_DDR_TYPE) {
		die("[Error] Not valid DdrType in @ALL_DDR_TYPE, please check !!!!!\n") if (ref($n) ne "DdrType");
		$n->found_mcp(0);
		$n->found_discrete(0);
	}

col_scan:
	for (my $col = 0;$col < 78;$col++) {
		for (my $row = 0;$row < 3;$row++) {
			my $field = &xls_cell_value($Sheet, $row, $col);

			next if (length($field) == 0);

			foreach my $n (@SCAN_COL) {
				next if (defined $n->index);

				if ($n->field_name eq $field) {
					$n->index($col);

					next col_scan;
				}
			}
		}
	}

	foreach my $n (@SCAN_COL) {
		die("Column scan: '".$n->field_name."' not found\n") if (!defined $n->index);
	}
}

#****************************************************************************************
# subroutine:  IsDdrType
#              check if $dev's ddr type is $check_type
# porting:     no need
# input:       $dev        - MDL object
# input:       $check_type - ddr type (ex: "LPDR4X")
# return:      1: if $dev's ddr type is $check_type
#****************************************************************************************
sub IsDdrType {
	my $dev = shift;
	my $check_type = shift;

	return 1 if ($check_type eq $dev->ddr->name);

	return 0;
}

#****************************************************************************************
# subroutine:  IsDiscreteDram
#              check if $dev is discrete dram
# porting:     no need
# input:       $dev - MDL object
# return:      1: if $dev is discrete dram
#****************************************************************************************
sub IsDiscreteDram {
	my $dev = shift;

	return 1 if ($dev->mcp_type eq "00" || $dev->mcp_type eq "0F");

	return 0;
}

#****************************************************************************************
# subroutine:  IsSameTypeDram
#              check if $dev1's ddr type equals $dev2's ddr type
# porting:     no need
# input:       $dev1 - MDL object
# input:       $dev2 - MDL object
# return:      1: if $dev1's ddr type equals $dev2's ddr type
#****************************************************************************************
sub IsSameTypeDram {
	my $dev1 = shift;
	my $dev2 = shift;

	return 1 if ($dev1->ddr eq $dev2->ddr);

	return 0;
}

#****************************************************************************************
# subroutine:  SupportMultipleDiscrete
#              check if $dev could be runtime indentify (ex: by rank size, vendor id)
# porting:     no need
# input:       $dev - MDL object
# return:      1: $dev cound be runtime indentify
#****************************************************************************************
sub SupportMultipleDiscrete {
	my $dev = shift;

	return 1 if ($dev->ddr->support_multi_dis == 1);

	return 0;
}

#****************************************************************************************
# subroutine:  SupportHWTrap
#              check if $dev could be identify by HW trapping
# porting:     no need
# input:       $dev - MDL object
# return:      1: $dev could be identify by HW trapping
#****************************************************************************************
sub SupportHWTrap {
	my $dev = shift;

	return 1 if ($dev->ddr->hw_trap_defined == 1);

	return 0;
}

#****************************************************************************************
# subroutine:  IsSupportedDram
#              check if $dev could be supported by platform
# porting:     no need
# input:       $dev - MDL object
# return:      1: $dev could be support by platform
#****************************************************************************************
sub IsSupportedDram {
	my $dev = shift;

	return 1 if ($dev->ddr->supported == 1);

	return 0;
}

#****************************************************************************************
# subroutine:  DeviceListParser_LPSDRAM
#              Find MDL whose part number equals to $CustCS_id from XLS, and parse data into MDL object
# porting:     always need to check
# input:       $Book         - XLS object
# input:       $PartNum      - the part number want to find
# return:      total number of found MDL
#****************************************************************************************
sub DeviceListParser_LPSDRAM {
	my ($Sheet) ;
	my ($PartNum) ;
	my ($id) ;
	my ($iter);
	my ($num_part_found);
	my ($eos_flag);


	$num_part_found = 0 ;
	$Sheet = shift;
	$PartNum = shift;
	$id = $TotalCustemChips;

	my $row;
	my $col;
	my $rows_part_found;                                 # stores the part numbers found in MDL
		$eos_flag = 5;		# if this flag counts to '0', it means End Of Sheet

	$row        = $start_num - 1 ;                    # scan from row 2

	my $scan_idx;
	my @rows_part_found;

	do {{
		++$row ;
		$scan_idx = &xls_cell_value($Sheet, $row, $COL_PART_NUMBER->index) ;
		print "[Bike Check] scan_idx= $scan_idx\n";
		unless (defined $scan_idx) {
			print "[$row][scan_idx]No Value, $eos_flag\n" if $DebugPrint == 1 ;
			$eos_flag -- ;
			next ;
		}
		if ($scan_idx eq "") {
			print "[$row][scan_idx]EQ null, $eos_flag\n" if $DebugPrint == 1 ;
			$eos_flag -- ;
			next ;
		}
		$eos_flag   = 5 ;

# remove leading and tailing spaces
		$scan_idx =~ s/^\s+// if $DebugPrint == 1 ;
		$scan_idx =~ s/\s+$// if $DebugPrint == 1 ;

		$scan_idx =~ s/^\s+// ;
		$scan_idx =~ s/\s+$// ;

		print "$scan_idx ?= $PartNum\n" ;

		if ($scan_idx eq $PartNum) { # scan column 2 for Part Number
			my $boardid ;
			$boardid = &xls_cell_value($Sheet, $row, $COL_BOARD_ID->index) ;
			if ($CustBoard_ID eq $boardid) {
				$rows_part_found[$num_part_found] = $row;

				print "\nPartNum($PartNum==$scan_idx) found in row $row\n" ;
				$num_part_found ++;
				$TotalCustemChips ++;
			}
		}
	}} while (defined ($scan_idx) && ($eos_flag > 0));

	print("[Bike Check]num_part_found $num_part_found\n");
	if ($num_part_found == 0) {
		print "\n[Error]unsupported part number $PartNum\n" ;
		die "\n[Error]unsupported part number $PartNum\n" ;
	}

	my $dev;

	$iter = 0;
	while ($iter<$num_part_found) {
		$dev = MemDevice->new();
		$MDL[$id] = $dev;

		$_ = $rows_part_found[$iter] ;
		$iter ++;

		foreach my $n (@SCAN_COL) {
			next if (!defined $n->name);
			$MDL[$id]->data($n->name, &xls_cell_value($Sheet, $_, $n->index));
		}

		# This function will overwrite NAND_EMMC_ID for discrete
		&check_support_dram_type_mcp_fw_id($MDL[$id]);

		if (!($MDL[$id]->data('CHIP_ID') eq lc($EMIGEN_CHIP_ID))) {
			die "[Error] Wrong Chip ID:".$MDL[$id]->data('CHIP_ID')."\n";
		}

		&format_rank_size($MDL[$id]);

		&gen_emi_setting($MDL[$id], 0);

		$id ++;
	}

	return $id;
}

#****************************************************************************************
# subroutine:  CanBeComboDiscrete
#              check if two discrete dram $dev1 could be co-load with $dev2
# porting:     only when co-load rule changed
# input:       $dev1            - MDL object
# input:       $dev2            - MDL object
# return:      void
#****************************************************************************************
sub CanBeComboDiscrete {
	my $dev1 = shift;
	my $dev2 = shift;

	if (&IsSameTypeDram($dev1, $dev2)) {
		if ($dev1->data('MODE_REG5') eq $dev2->data('MODE_REG5')) {
			if (&IsDdrType($dev1, "LPDDR2")) {
				die "[Error] MODE_REG5(DRAM vendor_ID) should not be the same in the Combo list, MODE_REG5(".$dev1->data('PARTNUM').")==MODE_REG5(".$dev2->data('PARTNUM').")\n" ;
			} elsif (&IsDdrType($dev1, "LPDDR3")) {
				if ((($dev1->dram_rank_size(0) eq $dev2->dram_rank_size(0)) && ($dev1->dram_rank_size(1) eq $dev2->dram_rank_size(1)))) {
					die "[Error] LPDDR3 MODE_REG5+DRAM_RANKx_SIZE should not be the same in the Combo list, MODE_REG5+DRAM_RANKx_SIZE(".$dev1->data('PARTNUM').")==MODE_REG5+DRAM_RANKx_SIZE(".$dev2->data('PARTNUM').")\n" ;
				}
			} elsif (&IsDdrType($dev1, "LPDDR4") || &IsDdrType($dev1, "LPDR4X") || &IsDdrType($dev1, "LPDR4P")) {
				if ((($dev1->dram_rank_size(0) eq $dev2->dram_rank_size(0)) && ($dev1->dram_rank_size(1) eq $dev2->dram_rank_size(1)))) {
					die "[Error] LPDDR4 MODE_REG5+DRAM_RANKx_SIZE should not be the same in the Combo list, MODE_REG5+DRAM_RANKx_SIZE(".$dev1->data('PARTNUM').")==DRAM_RANKx_SIZE(".$dev2->data('PARTNUM').")\n" ;
				}
			} elsif (&IsDdrType($dev1, "LPDDR5")) {
				if ((($dev1->dram_rank_size(0) eq $dev2->dram_rank_size(0)) && ($dev1->dram_rank_size(1) eq $dev2->dram_rank_size(1)))) {
					die "[Error] LPDDR5 MODE_REG5+DRAM_RANKx_SIZE should not be the same in the Combo list, MODE_REG5+DRAM_RANKx_SIZE(".$dev1->data('PARTNUM').")==DRAM_RANKx_SIZE(".$dev2->data('PARTNUM').")\n" ;
				}
			} elsif (&IsDdrType($dev1, "LPDDR5X")) {
				if ((($dev1->dram_rank_size(0) eq $dev2->dram_rank_size(0)) && ($dev1->dram_rank_size(1) eq $dev2->dram_rank_size(1)))) {
					die "[Error] LPDDR5X MODE_REG5+DRAM_RANKx_SIZE should not be the same in the Combo list, MODE_REG5+DRAM_RANKx_SIZE(".$dev1->data('PARTNUM').")==DRAM_RANKx_SIZE(".$dev2->data('PARTNUM').")\n" ;
				}
			}
		}
	} else {
		if (!&SupportHWTrap($dev1) || !&SupportHWTrap($dev2)) {
			die("[Error] Not support different type of discrete dram Type(".$dev1->data('PARTNUM').")!=Type(".$dev2->data('PARTNUM').")\n");
		}
	}
}

#****************************************************************************************
# subroutine:  CanBeComboMCP
#              check if two MCP $dev1 could be co-load with $dev2
# porting:     only when co-load rule changed
# input:       $dev1            - MDL object
# input:       $dev2            - MDL object
# return:      void
#****************************************************************************************
sub CanBeComboMCP {
	my $dev1 = shift;
	my $dev2 = shift;

		if ((!&SupportHWTrap($dev1) || !&SupportHWTrap($dev2)) && !&IsSameTypeDram($dev1, $dev2)) {
		 die("[Error] Not support different type of dram Type(".$dev1->data('PARTNUM').")!=Type(".$dev2->data('PARTNUM').") without HW TRAP\n");
	}
# NAND can't use different page size.
	if ($dev1->mcp_type == "01" && $dev2->mcp_type == "01") {
		die "[Error]different MCP page size! NAND_PAGE_SIZE(".$dev1->data('PARTNUM').")!=NAND_PAGE_SIZE(".$dev2->data('PARTNUM').")\n" if ($dev1->data('NAND_PAGE_SIZE') != $dev2->data('NAND_PAGE_SIZE'));
	}

	if ($dev1->data('NAND_EMMC_ID') eq $dev2->data('NAND_EMMC_ID')) {
		if ($COMBO_MCP_RULE_V > 1) {
			return if (!&IsSameTypeDram($dev1, $dev2));
			return if ((($dev1->dram_rank_size(0) ne $dev2->dram_rank_size(0)) || ($dev1->dram_rank_size(1) ne $dev2->dram_rank_size(1))));
		}

		if ( $dev1->data('PARTNUM') ne $dev2->data('PARTNUM') ) {
			die "[Error]Different part number:".$dev1->data('PARTNUM').",".$dev2->data('PARTNUM')." with same NAND/eMMC ID:".$dev1->data('NAND_EMMC_ID')."\n";
		}
	}
}

#****************************************************************************************
#****************************************************************************************
# Parsing/Checking function
#****************************************************************************************
#****************************************************************************************

#****************************************************************************************
# subroutine:  format_mcp_fw_id
#              format mcp_id and fw_id from hex to string array
# porting:     only when new id are added
# input:       $dev  - MDL object
# input:       $type - 'NAND_EMMC_ID'
# return:      void
#****************************************************************************************
sub format_mcp_fw_id {
	my $dev = shift;
	my $type = shift;
	my $string;
	my $id_str;
	my $id_len = 0;
	my $max_id_len;

	if ($type eq "NAND_EMMC_ID") {
		$id_str = "{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}";
		$max_id_len = $MAX_NAND_EMMC_ID_LEN;
	} else {
		die("[Error] Wrong type for format_mcp_fw_id!!!!\n");
	}

	$string = $dev->data($type);
	$id_len = length($string);

	die ("[Error] The wrong ".$type.": ".$string." !! It is not byte align\n") if ($id_len & 0x1);

	$id_len = ($id_len - 2) / 2;
	$id_len = 0 if ($id_len < 0);

	if ($id_len > $max_id_len) {
		die("[Error] ".$type." length too long\n");
	}

	if ($id_len > 0) {
		my @VEC = ($string =~ m/([\dA-Fa-f]{2})/gs);

		my $last = pop(@VEC);
		$id_str = "{";
		foreach my $a (@VEC) {
			$id_str .= "0x$a,";
		}
		$id_str .= "0x$last";

		if ($id_len < $max_id_len) {
			for(my $i = 0; $i <($max_id_len - $id_len); $i++) {
				$id_str.= ",0x0"
			}
		}
		$id_str .= "}";
	}

	if ($type eq "NAND_EMMC_ID") {
		$dev->mcp_id_len($id_len);
		$dev->mcp_id_str($id_str);
	}
}

#****************************************************************************************
# subroutine:  check_support_dram_type_mcp_fw_id
#              check $dev's mcp type and ddr type, then format mcp_id and fw_id if needed
# porting:     only when new mcp type or ddr type are added
# input:       $dev     - MDL object
# input:       $PartNum - Part Number
# return:      void
#****************************************************************************************
sub check_support_dram_type_mcp_fw_id {
	my $dev = shift;

	my ($mcp_type, $ddr_str, $ddr_type, $is_discrete);

	if ($dev->data('DEV_TYPE') =~ /^Discrete Tablet (\S+)$/) {
		$dev->mcp_type("0F");
		$dev->data('NAND_EMMC_ID', "");

		$is_discrete = 1;
		$ddr_str = $1;
	} elsif ($dev->data('DEV_TYPE') =~ /^Discrete (\S+)$/) {
		$dev->mcp_type("00");
		$dev->data('NAND_EMMC_ID', "");

		$is_discrete = 1;
		$ddr_str = $1;
	} elsif ($dev->data('DEV_TYPE') =~ /^MCP\(NAND\+(\S+)\)$/) {
		$dev->mcp_type("01");

		$is_discrete = 0;
		$ddr_str = $1;
	} elsif ($dev->data('DEV_TYPE') =~ /^MCP\(eMMC\+(\S+)\)$/) {
		$dev->mcp_type("02");

		$is_discrete = 0;
		$ddr_str = $1;
	} elsif ($dev->data('DEV_TYPE') =~ /^MCP\(UFS\+(\S+)\)$/) {
		$dev->mcp_type("03");

		$is_discrete = 0;
		$ddr_str = $1;
	} else {
		die "[Error] unknown MCP type ".$dev->data('DEV_TYPE')." for ".$dev->data('PARTNUM')."\n" ;
	}

	&format_mcp_fw_id($dev, "NAND_EMMC_ID");

	if ($ddr_str eq "DDR1") {
		$dev->ddr($DDR_TYPE_DDR1);

		$is_discrete = 1;
	} elsif ($ddr_str eq "LPDDR2") {
		$dev->ddr($DDR_TYPE_LPDDR2);
	} elsif ($ddr_str eq "LPDDR3") {
		$dev->ddr($DDR_TYPE_LPDDR3);
	} elsif ($ddr_str eq "PCDDR3") {
		$dev->ddr($DDR_TYPE_PCDDR3);
	} elsif ($ddr_str eq "LPDDR4") {
		$dev->ddr($DDR_TYPE_LPDDR4);
	} elsif ($ddr_str eq "LPDR4X") {
		$dev->ddr($DDR_TYPE_LPDR4X);
	} elsif ($ddr_str eq "LPDR4P") {
		$dev->ddr($DDR_TYPE_LPDR4P);
	} elsif ($ddr_str eq "LPDDR5") {
		$dev->ddr($DDR_TYPE_LPDDR5);
	} elsif ($ddr_str eq "LPDDR5X") {
		$dev->ddr($DDR_TYPE_LPDDR5X);
	} else {
		die "[Error] unknown DDR type ".$dev->data('DEV_TYPE')." for ".$dev->data('PARTNUM')."\n" ;
	}

	if ($ddr_str eq "LPDDR3" || $ddr_str eq "LPDDR4" || $ddr_str eq "LPDR4X" || $ddr_str eq "LPDR4P" || $ddr_str eq "LPDDR5" || $ddr_str eq "LPDDR5X") {
		if (!$dev->data('MODE_REG5')) {
			die "[Error](".$dev->data('PARTNUM').")MDL error, no MODE_REG5 information found, please update the MDL\n";
		}
	}

	$dev->ddr->found_discrete($dev->ddr->found_discrete + 1) if ($is_discrete == 1);
	$dev->ddr->found_mcp($dev->ddr->found_mcp + 1) if ($is_discrete == 0);

	push(@DISCRETE_MDLS, $dev) if ($is_discrete == 1);
}

#****************************************************************************************
# subroutine:  check_bldr_with_mdl
#              check if MDL and cust_bldr.mak are not match
# porting:     only when new mcp type are added
# input:       $mcp_type - MCP type defined in bldr
# return:      void
#****************************************************************************************
sub check_bldr_with_mdl {
	my $mcp_type = shift;
	my $iter = 0 ;

	for $iter (0..$TotalCustemChips-1) {
		return if (&IsDiscreteDram($MDL[$iter]) || $MDL[$iter]->mcp_type eq $mcp_type);
	}

	die("[Error] No discrete dram and none of MCP matched CFG_BOOT_DEV, please check MDL\n");
}

#****************************************************************************************
# subroutine:  format_rank_size
#              get rank size from MDL
# porting:     no need
# input:       $dev - MDL object
# return:      void
#****************************************************************************************
sub format_rank_size {
	my $dev = shift;

	if ($dev->data('DENSITY') =~ /^(\d+)\+(\d+)$/) {
		&check_and_format_density_size($dev, 0, $1);
		&check_and_format_density_size($dev, 1, $2);
	} elsif ($dev->data('DENSITY') =~ /^(\d+)$/) {
		&check_and_format_density_size($dev, 0, $1);
		$dev->dram_rank_size(1, 0);
	} else {
		die "[Error]Wrong Density size!!DENSITY:".$dev->data('DENSITY').", Please check Density in MDL.\n";
	}
}

#****************************************************************************************
# subroutine:  check_and_format_density_size
#              specify ddr type which could be identify by hw trapping
# porting:     no need
# input:       $dev  - MDL object
# input:       $rank - 0: rank0, 1: rank1
# input:       $raw  - raw string parsed from XLS
# return:      void
#****************************************************************************************
sub check_and_format_density_size {
	my $dev = shift;
	my $rank = shift;
	my $raw = shift;

	my $q = $raw / 2048;

	die ("Wrong density size!!!DENSITY:".$raw) if ($q * 2048 != $raw);

	$dev->dram_rank_size($rank, sprintf("0x%X", 0x10000000 * $q));
}

#****************************************************************************************
# subroutine:  clone_default_discrete
#              clone a MDL object from $dev, but make the clone one into discrete dram
# porting:     only when emi structure are modified, or new dram type are added
# input:       $dev   - MDL object
# return:      $clone - MDL object
#****************************************************************************************
sub clone_default_discrete {
	my $dev = shift;

	my $clone = bless { %$dev }, ref $dev;

	# non scalar need to copy again
	$clone->data({ %{$dev->data} });
	$clone->dram_rank_size([ @{$dev->dram_rank_size} ]);

	$clone->dram_rank_size(0, "0x20000000");
	$clone->dram_rank_size(1, 0);

	$clone->mcp_type("00");
	$clone->data('NAND_EMMC_ID', '');

	&format_mcp_fw_id($clone, "NAND_EMMC_ID");

	$clone->data('MODE_REG5', '0x00000000');

	&gen_emi_setting($clone, 1);

	return $clone;
}

#****************************************************************************************
# subroutine:  gen_emi_setting
#              transform MDL into emi_settings string
# porting:     when changing emi settings structure, or new dram type are added
# input:       $dev         - MDL object
# input:       $is_discrete - if empty, print "for discrete" in comment
# return:      void
#****************************************************************************************
sub gen_emi_setting {
	my $dev = shift;
	my $is_discrete = shift;
	my $emi_settings = "";
	my $ddr_type = sprintf("%X",$LPDDR_START + $dev->ddr->type_id); 

	$emi_settings .= "\n\t//".$dev->data('PARTNUM');
	$emi_settings .= " for discrete" if ($is_discrete);
	$emi_settings .= "\n\t{\n\t\t";
	$emi_settings .= "0x" . $dev->mcp_type . $ddr_type. ",\t\t/* TYPE */\n\t\t" ;
	$emi_settings .= $dev->mcp_id_len . ",\t\t/* EMMC ID/FW ID checking length */\n\t\t" ;
	$emi_settings .= $dev->mcp_id_str . ",\t\t/* NAND_EMMC_ID */\n\t\t" ;

	$emi_settings .= "{" . $dev->dram_rank_size(0) . "," .  $dev->dram_rank_size(1) . ",0,0},\t\t/* DRAM RANK SIZE */\n\t\t" ;
	$emi_settings .= "{0,0,0,0,0,0},\t\t/* reserved 6 */\n\t\t" ;

	if (&IsDdrType($dev, "DDR1")) {
	} elsif (&IsDdrType($dev, "LPDDR2")) {
	} elsif (&IsDdrType($dev, "LPDDR3")) {
		$emi_settings .= $dev->data('MODE_REG5') . ",\t\t/* LPDDR3_MODE_REG5 */\n\t}" ;
	} elsif (&IsDdrType($dev, "PCDDR3")) {
	} elsif (&IsDdrType($dev, "LPDDR4")) {
		$emi_settings .= $dev->data('MODE_REG5') . ",\t\t/* LPDDR4_MODE_REG5 */\n\t}" ;
	} elsif (&IsDdrType($dev, "LPDR4X")) {
		$emi_settings .= $dev->data('MODE_REG5') . ",\t\t/* LPDDR4X_MODE_REG5 */\n\t}" ;
	} elsif (&IsDdrType($dev, "LPDR4P")) {
		$emi_settings .= $dev->data('MODE_REG5') . ",\t\t/* LPDDR4P_MODE_REG5 */\n\t}" ;
	} elsif (&IsDdrType($dev, "LPDDR5")) {
		$emi_settings .= $dev->data('MODE_REG5') . ",\t\t/* LPDDR5_MODE_REG5 */\n\t}" ;
	} elsif (&IsDdrType($dev, "LPDDR5X")) {
		$emi_settings .= $dev->data('MODE_REG5') . ",\t\t/* LPDDR5X_MODE_REG5 */\n\t}" ;
	}

	print  $emi_settings ;
	print "\n\n" ;

	$dev->emi_setting_str($emi_settings);
}

#****************************************************************************************
# subroutine:  write_header_file
#              write header file for c code
# porting:     no need
# input:       $custom_emi_h           - file path to c header file
# input:       $memory_device_list_xls - file path to xls
# return:      void
#****************************************************************************************
sub write_header_file {
	my $custom_emi_h = shift;
	my $memory_device_list_xls = shift;

	unlink ($custom_emi_h) if (-e $custom_emi_h);

	my $temp_path = &dirname($custom_emi_h);
	&make_path($temp_path);

	open (CUSTOM_EMI_H, ">$custom_emi_h") or &error_handler("CUSTOM_EMI_H: file error!", __FILE__, __LINE__);

	print CUSTOM_EMI_H &copyright_file_header();
	print CUSTOM_EMI_H &description_file_header("custom_emi.h", $memory_device_list_xls,
			"This Module defines the EMI (external memory interface) related setting.",
			"EMI auto generator". $EMIGEN_VERNO);
	print CUSTOM_EMI_H &custom_EMI_h_file_body();
	close CUSTOM_EMI_H or &error_handler("$custom_emi_h: file error!", __FILE__, __LINE__);

	print "\n$custom_emi_h is generated\n";
}

#****************************************************************************************
# subroutine:  copyright_file_header
#              generate copyright header string
# porting:     no need
# input:       void
# return:      copyright header string
#****************************************************************************************
sub copyright_file_header {
	my $template = <<"__TEMPLATE";
/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
__TEMPLATE

		return $template;
}

#****************************************************************************************
# subroutine:  description_file_header
#              generate header for c header file
# porting:     no need
# input:       $filename:     filename
# input:       $description:  one line description
# input:       $author:       optional
# return:      file header -- description
#****************************************************************************
sub description_file_header {
	my ($filename, $xls, $description, $author) = @_;
	my @stat_ar = stat $xls;
	my ($day, $month, $year) = (localtime($stat_ar[9]))[3,4,5]; $month++; $year+=1900;
	my $template = <<"__TEMPLATE";
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $filename
 *
 * Project:
 * --------
 *   Android
 *
 * Description:
 * ------------
 *   $description
 *
 * Author:
 * -------
 *  $author
 *
 *   Memory Device database last modified on $year/$month/$day
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * \$Revision\$
 * \$Modtime\$
 * \$Log\$
 *
 *------------------------------------------------------------------------------
 * WARNING!!!  WARNING!!!   WARNING!!!  WARNING!!!  WARNING!!!  WARNING!!!
 * This file is generated by EMI Auto-gen Tool.
 * Please do not modify the content directly!
 * It could be overwritten!
 *============================================================================
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

__TEMPLATE

		return $template;
}

#****************************************************************************************
# subroutine:  custom_EMI_h_file_body
#              generate body of c header file
# porting:     no need
# input:       void
# return:      void
#****************************************************************************************
sub custom_EMI_h_file_body {
	my $EMI_SETTINGS_string = "" ;
	my $DEF_MDL;
	my $ddr = -1 ;

	my $iter = 0 ;

	if ($#DISCRETE_MDLS > 0) {
		$DEF_MDL = &clone_default_discrete($DISCRETE_MDLS[0]);
	} else {
		$DEF_MDL = &clone_default_discrete($MDL[0]);
	}

	for $iter (0..$TotalCustemChips-1) {
		if (!&IsDiscreteDram($MDL[$iter])) {
			$EMI_SETTINGS_string .= $MDL[$iter]->emi_setting_str;
			$EMI_SETTINGS_string .= " ," ;
		}
	}
	for $iter (0..$TotalCustemChips-1) {
		if (&IsDiscreteDram($MDL[$iter])) {
			$EMI_SETTINGS_string .= $MDL[$iter]->emi_setting_str;
			$EMI_SETTINGS_string .= " ," ;
		}
	}

	my $template = <<"__TEMPLATE";
#ifndef __CUSTOM_EMI__
#define __CUSTOM_EMI__

#include "dramc_custom.h"
#include "emi.h"

#define COMBO_LPDDR2  (@{[$DDR_TYPE_LPDDR2->found_mcp]}+@{[$DDR_TYPE_LPDDR2->found_discrete]})
#define COMBO_LPDDR3  (@{[$DDR_TYPE_LPDDR3->found_mcp]}+@{[$DDR_TYPE_LPDDR3->found_discrete]})
#define COMBO_LPDDR4  (@{[$DDR_TYPE_LPDDR4->found_mcp]}+@{[$DDR_TYPE_LPDDR4->found_discrete]})
#define COMBO_LPDR4X  (@{[$DDR_TYPE_LPDR4X->found_mcp]}+@{[$DDR_TYPE_LPDR4X->found_discrete]})
#define COMBO_LPDR4P  (@{[$DDR_TYPE_LPDR4P->found_mcp]}+@{[$DDR_TYPE_LPDR4P->found_discrete]})
#define COMBO_LPDDR5  (@{[$DDR_TYPE_LPDDR5->found_mcp]}+@{[$DDR_TYPE_LPDDR5->found_discrete]})
#define COMBO_LPDDR5X (@{[$DDR_TYPE_LPDDR5X->found_mcp]}+@{[$DDR_TYPE_LPDDR5X->found_discrete]})
#define COMBO_PCDDR3 (@{[$DDR_TYPE_PCDDR3->found_mcp]}+@{[$DDR_TYPE_PCDDR3->found_discrete]})
#define NUM_EMI_RECORD ($TotalCustemChips)

int num_of_emi_records = NUM_EMI_RECORD ;

QVL_LIST_T qvl_list[] =
{
	$EMI_SETTINGS_string
};

#endif /* __CUSTOM_EMI__ */
__TEMPLATE
		return $template ;
}


#****************************************************************************************
# subroutine:  write_tag
#              generate tag file
# porting:     only when tag file format are modified
# input:       $project         - project name
# input:       $info_tag        - file path to tag file
# input:       $platform_tag_id - tag id for flashtool (ex: "v30")
# return:      void
#****************************************************************************************
sub write_tag {
	my $project = lc(shift);
	my $info_tag = shift;
	my $platform_tag_id = shift;
	my $filesize = 0x0 ;
	my $ddr = -1 ;
	my $DEF_MDL;

	unlink ($info_tag) if (-e $info_tag);

	my $temp_path = &dirname($info_tag);
	&make_path($temp_path);

	open FILE,">$info_tag";

	print FILE pack("a24", "MTK_BLOADER_INFO_".$platform_tag_id); #MT6763
#    print FILE pack("a24", "MTK_BLOADER_INFO_v00");
		$filesize = $filesize + 24 ;
	seek(FILE, 0x1b, 0);
	my $pre_bin = "preloader_${project}.bin";
	print "PROJECT = $project, pre_bin = $pre_bin\n";
	print FILE pack("a64", $pre_bin);
	$filesize = $filesize + 64 ;
	seek(FILE, 0x58, 0);
	print FILE pack("H8", 56313136);
	$filesize = $filesize + 4 ;
	print FILE pack("H8", 22884433);
	$filesize = $filesize + 4 ;
	print FILE pack("H8", "90007000");
	$filesize = $filesize + 4 ;
	print FILE pack("a8", "MTK_BIN");
	$filesize = $filesize + 8 ;

#    print FILE pack("H8", bc000000);


	seek(FILE,0x6c, 0);
# 1.LPDDR2/LPDDR3/LPDDR4/LPDDR4X/LPDDR4P discrete dram number >= 2
#[DEL] 2.LPDDR2 discrete dram > 0, LPDDR2 MCP > 0
#[DEL] 3.LPDDR3 discrete dram > 0, LPDDR3 MCP > 0
#[DEL] 4.LPDDR4 discrete dram > 0, LPDDR4 MCP > 0
#[DEL] 5.LPDDR4X discrete dram > 0, LPDDR4X MCP > 0
	print FILE pack("L", $TotalCustemChips);     # number of emi settings.
	$filesize = $filesize + 4 ;

	my $iter = 0 ;

# 1.LPDDR2/LPDDR3/LPDDR4/LPDDR4X/LPDDR4P discrete dram number >= 2
#[DEL] 2.LPDDR2 discrete dram > 0, LPDDR2 MCP > 0
#[DEL] 3.LPDDR3 discrete dram > 0, LPDDR3 MCP > 0
#[DEL] 4.LPDDR4 discrete dram > 0, LPDDR4 MCP > 0
#[DEL] 5.LPDDR4X discrete dram > 0, LPDDR4X MCP > 0
#[DEL] 6.LPDDR4P discrete dram > 0, LPDDR4P MCP > 0
	for $iter (0..$TotalCustemChips-1) {
# generate MCP dram
		if (!&IsDiscreteDram($MDL[$iter])) {
			$filesize = $filesize + &write_tag_one_element ($MDL[$iter]) ;
		}
	}
	for $iter (0..$TotalCustemChips-1) {
# generate discrete dram
		if (&IsDiscreteDram($MDL[$iter])) {
			$filesize = $filesize + &write_tag_one_element ($MDL[$iter]) ;
		}
	}
#    $filesize = $filesize + 4 ;

#    print "2.file size is $filesize \n";

	print FILE pack("L", $filesize) ;

	close (FILE) ;
	print "$info_tag is generated!\n";
	return ;
}

#****************************************************************************************
# subroutine:  write_tag_one_element
#              output binary for emi structure
# porting:     only when emi structure are modified
# input:       $dev - MDL object
# return:      void
#****************************************************************************************
sub write_tag_one_element {
	my $dev = shift;
	my $ddr_type = sprintf("%X", $LPDDR_START + $dev->ddr->type_id);
	my $type = "0x".$ddr_type;
	my $fs = 0 ;
	print FILE pack("L", hex($type));                           #type
		$fs = $fs + 4 ;

	print FILE pack ("L", scalar($dev->mcp_id_len)) ;        # EMMC ID checking length
		$fs = $fs + 4 ;

	$_ = $dev->mcp_id_str ;
	if (/(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+),(\w+)/) {
		print FILE pack ("C", hex ($1)) ;            #id
		print FILE pack ("C", hex ($2)) ;
		print FILE pack ("C", hex ($3)) ;
		print FILE pack ("C", hex ($4)) ;
		print FILE pack ("C", hex ($5)) ;
		print FILE pack ("C", hex ($6)) ;
		print FILE pack ("C", hex ($7)) ;
		print FILE pack ("C", hex ($8)) ;
		print FILE pack ("C", hex ($9)) ;
		print FILE pack ("C", hex ($10)) ;
		print FILE pack ("C", hex ($11)) ;
		print FILE pack ("C", hex ($12)) ;
		print FILE pack ("C", hex ($13)) ;
		print FILE pack ("C", hex ($14)) ;
		print FILE pack ("C", hex ($15)) ;
		print FILE pack ("C", hex ($16)) ;
		$fs = $fs + 16 ;
	}

	print FILE pack ("Q", hex (lc($dev->dram_rank_size(0)))) ;                                  #  DRAM_RANK_SIZE[4]
	print FILE pack ("Q", hex (lc($dev->dram_rank_size(1)))) ;
	print FILE pack ("Q", hex ("0")) ;
	print FILE pack ("Q", hex ("0")) ;
	$fs = $fs + 32 ;

	print FILE pack ("L", hex ("0")) ;                                  #reserved[6]
	print FILE pack ("L", hex ("0")) ;
	print FILE pack ("L", hex ("0")) ;
	print FILE pack ("L", hex ("0")) ;
	print FILE pack ("L", hex ("0")) ;
	print FILE pack ("L", hex ("0")) ;
	$fs = $fs + 24 ;

	if (&IsDdrType($dev, "LPDDR3") || &IsDdrType($dev, "LPDDR4") || &IsDdrType($dev, "LPDR4X") || &IsDdrType($dev, "LPDR4P") || &IsDdrType($dev, "LPDDR5") || &IsDdrType($dev, "LPDDR5X")) {
		print FILE pack ("L", hex (lc($dev->data('MODE_REG5')))) ;        # LPDDR3_MODE_REG5
			$fs = $fs + 4 ;
	}

#       print "1.file size is $fs \n";
	return $fs;
}

1;
