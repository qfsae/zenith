#!/bin/bash

echo "Starting PIO builds"

pioProjects=()

# iterate through all the changed files between PR branch and master
# if a directory that has been modified contains a platformio ini file,
# then run a build and ensure that it still compiles.
# for i in `git diff --name-only`
# do
#     directory=`dirname $i`
#     file="$directory/platformio.ini"
#     # Check if the directory has a platformio.ini file
#     if test -e $file; then pioProjects+=(`dirname $file`); fi

# done

# find all directories with a platformIO file
for d in `find . -type d`; do
    file="$d/platformio.ini"
    if test -e $file; then pioProjects+=($d); fi
    
done

BR=`git branch --show-current`
for value in "${pioProjects[@]}"; do
    diff=`git --no-pager diff --name-only master $BR $value/`
    # if diff is not an empty string run the build
    if [[ $diff ]]; then
        echo $value
        pio run -d $value
    fi
done