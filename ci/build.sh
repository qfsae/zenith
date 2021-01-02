#!/bin/bash

echo "Starting PIO builds"

# BR=`git branch --show-current` 

# iterate through all the changed files between PR branch and master
# if a directory that has been modified contains a platformio ini file,
# then run a build and ensure that it still compiles.
for i in `git diff --name-only master`
do
    directory=`dirname $i`
    file="$directory/platformio.ini"
    # Check if the directory has a platformio.ini file
    if test -e $file; then echo $file; fi
done