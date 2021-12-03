#!/bin/sh

day=$1
dayf=day$(printf "%02d\n" $day)

mkdir $dayf

echo "#include <stdio.h>" > $dayf/main.c
echo "#include <stdlib.h>" >> $dayf/main.c
echo "#include <sys/types.h>" >> $dayf/main.c
echo "#define _GNU_SOURCE 1" >> $dayf/main.c
echo "" >> $dayf/main.c
echo "int main() {" >> $dayf/main.c
echo "    FILE *f = fopen("input.txt", "r");" >> $dayf/main.c
echo "}" >> $dayf/main.c
echo "" >> $dayf/main.c

./grabinput.sh $day

