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
    if test -e $file; then array+=(`dirname $file`); fi
done

# Remove duplicates via sort command with -u for unique
finalList=($(printf "%s\n" "${array[@]}" | sort -u))

for value in "${finalList[@]}"
do
    echo "Running for Project value $value"
    pio run -d $value
done