#!/bin/bash

start_dir="/opt/devkitpro/devkitARM/bin"
gcc=$"$start_dir/arm-none-eabi-gcc"
as=$"$start_dir/arm-none-eabi-as"
ref="/home/ethlyn/Desktop/fireemblem8u/reference/fe8"
incl="/home/ethlyn/Desktop/fireemblem8u/include"
lyn="/opt/devkitpro/lyn"
in_name=$(basename $1 .c)

$gcc -mcpu=arm7tdmi -mabi=aapcs -w -mthumb -mthumb-interwork -fomit-frame-pointer -ffast-math -fno-toplevel-reorder -mlong-calls -I $incl -O2 -c $1 -o $in_name.o

$as -g -mcpu=arm7tdmi -mthumb-interwork $ref.s -o $ref.o

$lyn $in_name.o $ref.o > $in_name.lyn.event

rm $in_name.o
