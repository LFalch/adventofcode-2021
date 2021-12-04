#!/bin/sh

day=$1
dayf=day$(printf "%02d\n" $day)


main=$dayf/main.c

if [ ! -f "$main" ]; then
    mkdir -p $dayf
    ./grabinput.sh $day

    echo "#include <stdio.h>" > $main
    echo "#include <stdlib.h>" >> $main
    echo "#include <sys/types.h>" >> $main
    echo "#define _GNU_SOURCE 1" >> $main
    echo "" >> $main
    echo "int main() {" >> $main
    echo "    FILE *f = fopen(\"input.txt\", \"r\");" >> $main
    echo "}" >> $main
else
    echo "main.c already exists. to avoid deleting source code, setup will abort"
    exit 1
fi
