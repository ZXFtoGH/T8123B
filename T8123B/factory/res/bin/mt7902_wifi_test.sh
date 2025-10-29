#!/vendor/bin/sh

module_path_sdio_reset=/vendor/lib/modules/wlan_sdio_reset.ko
module_path_mt7902_sdio=/vendor/lib/modules/wlan_mt7902_sdio_mt6789.ko

result_file=/data/vendor/tmp/scan_result.txt
#result_file2=/data/scan_result
wlan0_is_up=false

if  ifconfig wlan0; then
  wlan0_is_up=true
  echo "wlan0_is_up=true"
fi

if [ $wlan0_is_up = "false" ]; then
echo "insmod wifi mt7902 driver"
  if [ -e $module_path_sdio_reset ]; then
    echo "insmod $module_path_sdio_reset"
    insmod "$module_path_sdio_reset"
    sleep 0.2
    echo "insmod $module_path_mt7902_sdio"
    insmod "$module_path_mt7902_sdio"
    if [ $? -ne 0 ]; then
      echo "insmod driver failed"
      exit 0
    fi
    echo "sleep 1s"
    sleep 1
  else
    echo "can not found $module_path_sdio_reset"
	exit 0
  fi
fi

if  ifconfig wlan0; then
  ifconfig wlan0 up
  if [ ! -d "/data/vendor/tmp" ]; then
    mkdir /data/vendor/tmp
    echo "mkdir /data/vendor/tmp"
  fi
  if [ ! -e $result_file ]; then
    touch $result_file
    echo "touch $result_file"
  fi
  sleep 0.2
  iw dev wlan0 scan | grep "SSID:" | cut -d ": "  -f 3  > $result_file

#  echo "get scan results:"
  cat $result_file

  ssid_array=($(awk '!/^\s*$/ && !/^\s*\\/ {print}' "$result_file"))

  num=0
  for item in "${ssid_array[@]}"; do
    if [[ -z $item ]]; then
      echo "SSID: is empty."
    else
      echo "ssid: $item"
	  num=$((num+1))
    fi
  done

  rm $result_file

  if [ $num -gt 3 ]; then
    echo "num: $num: success"
#	echo 1 > $result_file2
    exit 1
  else
    echo "num: $num: fail"
#	echo 0 > $result_file2
	exit 0
  fi
fi

echo "wlan test failed"
exit 0
