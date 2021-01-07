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

for value in "${pioProjects[@]}"; do
    echo $value
    git status $value/ --short
    if [[ $? -eq 0 ]]; then
        pio run -d $value
    fi
done

# buildDirs=()
# # now determine which pio projects have git modified changes in them
# for i in `git diff --name-only`; do
#     for j in "${pioProjects[@]}"; do
#         echo $j
#         # [[$i =~ ${j}/*]] && echo $j
#     done
# done

# Remove duplicates via sort command with -u for unique

# finalList=($(printf "%s\n" "${pioProjects[@]}" | sort -u))


# for value in "${finalList[@]}"
# do
#     echo "Running for Project value $value"
#     pio run -d $value
# done