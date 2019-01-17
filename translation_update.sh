#!/bin/bash
# this file is used to auto-generate .ts file

ts_list=(`ls *.pro`)

for ts in "${ts_list[@]}"
do
    printf "\nprocess ${ts}\n"
    lupdate "${ts}"
done