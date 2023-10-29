#!/bin/bash

mandatory_files=$(find mandatory/ -type f)
echo $mandatory_files

for file in $mandatory_files; do
    var=$(echo $file | cut -d "/" -f 2-)
    var2="bonus/$var" 
    var=$(echo $var2 | cut -d '.' -f 1)
    bonus_file=$(echo "$var"_bonus.c)
    echo "### $file ###"
    diff -I '^[(\/\*)(#)]' $file $bonus_file
    echo -e " \n\n\n"
done