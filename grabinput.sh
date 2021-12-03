#!/bin/sh

day=$1

curl --cookie "session=`cat .session`" https://adventofcode.com/2021/day/$day/input > day$(printf "%02d\n" $day)/input.txt
