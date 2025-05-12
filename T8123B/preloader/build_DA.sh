#!/bin/bash
# WARNING: buildDA.sh is used only for DA fast standalone build, nolonger called from Android.mk
# WARNING: post_process is moved to Makefile for incremental build
# WARNING: parameters in buildDA.sh and Android.mk must be identical
# tells sh to exit with a non-zero status as soon as any executed command fails
# (i.e. exits with a non-zero exit status).
set -e
if [ -z ${MTK_PROJECT} ]; then
  echo "[ERROR] MTK_PROJECT is not defined"
fi

DA_ROOT_DIR=`pwd`

PROJECT_T=$(echo ${MTK_PROJECT} | tr [a-z] [A-Z])
if [[ ${PROJECT_T} == FPGA* ]]
then
    DA_MODE=DA_BR_FPGA
else
    DA_MODE=DA_BR
fi

DA_OUT=${DA_ROOT_DIR}/out/${DA_MODE}_OBJ

PATH=${DA_ROOT_DIR}/../../../../../../prebuilts/perl/linux-x86/bin:${PATH}
export PATH

export DA_USE_PL_HW_DRV=yes
echo Clean DA_OUT
rm -rf ${DA_OUT}

if [ "$1" = "-s" ]; then
  echo make -f Makefile.DA DA_OUT=${DA_OUT} MTK_PROJECT=${MTK_PROJECT} DA_MODE=${DA_MODE}
  make -f Makefile.DA DA_OUT=${DA_OUT} MTK_PROJECT=${MTK_PROJECT} DA_MODE=${DA_MODE}
else
  echo make -j36 -f Makefile.DA DA_OUT=${DA_OUT} MTK_PROJECT=${MTK_PROJECT} DA_MODE=${DA_MODE}
  make -j36 -f Makefile.DA DA_OUT=${DA_OUT} MTK_PROJECT=${MTK_PROJECT} DA_MODE=${DA_MODE}
fi