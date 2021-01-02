#!/bin/bash

echo "Starting PIO builds"

BR=`git branch --show-current` 

array=()
# iterate through all the changed files between PR branch and master
# if a directory that has been modified contains a platformio ini file,
# then run a build and ensure that it still compiles.
for i in `git diff --name-only master $BR`
do
    directory=`dirname $i`
    file="$directory/platformio.ini"
    # Check if the directory has a platformio.ini file
    # NOTE: This currently finds the right paths but still has repitions
    # must strip list of presenting same directory twice
    if test -e $file; then array+=($file); fi
done

finalList=($(printf "%s\n" "${array[@]}" | sort -u))

for value in "${finalList[@]}"
do
     echo $value
done