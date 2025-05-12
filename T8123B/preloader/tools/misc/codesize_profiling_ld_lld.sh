# This script is to parse the system.map generated from ld.lld

INPUT_FILE=$1
OUTPUT_FILE=$2

AWK_ARG=""
if [[ $(awk --version) = GNU* ]]; then
  AWK_ARG="--non-decimal-data"
fi

echo "Symbol,Address,Size,Object" > ${OUTPUT_FILE}.tmp
awk ${AWK_ARG} '/\(.bss.*\)$/ {
    # extract filename.o:(section) into array a
    m = split($5, a, "/");

    # split filename.o and (section) into array b
    n = split(a[m], b, ":");

    # add leading zero to Size, so that we could use strtonum to covert Size string to hex value later
    printf("%s,%s,0x%s,%s\n", b[n], $1, $3, b[n - 1]);
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/\(.text.*\)$/ {
    # extract filename.o:(section) into array a
    m = split($5, a, "/");

    # split filename.o and (section) into array b
    n = split(a[m], b, ":");

    # add leading zero to Size, so that we could use strtonum to covert Size string to hex value later
    printf("%s,%s,0x%s,%s\n", b[n], $1, $3, b[n - 1]);
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/\(.data.*\)$/ {
    # extract filename.o:(section) into array a
    m = split($5, a, "/");

    # split filename.o and (section) into array b
    n = split(a[m], b, ":");

    # add leading zero to Size, so that we could use strtonum to covert Size string to hex value later
    printf("%s,%s,0x%s,%s\n", b[n], $1, $3, b[n - 1]);
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk ${AWK_ARG} '/\(.rodata.*\)$/ {
    # extract filename.o:(section) into array a
    m = split($5, a, "/");

    # split filename.o and (section) into array b
    n = split(a[m], b, ":");

    # add leading zero to Size, so that we could use strtonum to covert Size string to hex value later
    printf("%s,%s,0x%s,%s\n", b[n], $1, $3, b[n - 1]);
}' ${INPUT_FILE} >> ${OUTPUT_FILE}.tmp

awk '
    BEGIN {
        FS = ",";
            print "Object,Symbol,Size";
    }

    {
        if (NR > 1) {
            obj_sym = $4 " " $1; # use object + symbol as key
            object[obj_sym] = $4;
            size[obj_sym] = strtonum($3);
            total += strtonum($3);
        }
    }

    END {
        for (obj_sym in object) {
            sum[object[obj_sym]] += size[obj_sym];
        }

        PROCINFO["sorted_in"] = "@val_num_desc"
        for (filename in sum) {
            print filename",," sum[filename];

            for (obj_sym in object) {
                if (object[obj_sym] == filename) {
                    symbol = substr(obj_sym, match(obj_sym, / /) + 1);
                    print "," symbol "," size[obj_sym];
                }
            }
        }
        print "Total,," total;
    }
' ${OUTPUT_FILE}.tmp > ${OUTPUT_FILE}

rm ${OUTPUT_FILE}.tmp
echo "End of Profiling..."
