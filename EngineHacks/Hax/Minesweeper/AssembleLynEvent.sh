#!/bin/bash

start_dir="/opt/devkitpro/devkitARM/bin"
as=$"$start_dir/arm-none-eabi-as"
lyn="/opt/devkitpro/lyn"
in_name=$(basename $1 .s)

$as -g -mcpu=arm7tdmi -mthumb-interwork $1 -o $in_name.elf

$lyn $in_name.elf > $in_name.lyn.event

rm $in_name.elf

