#!/bin/bash

# Change this to your target directory or pass it as an argument
DIR="${1:-src}"

# Find all files in the directory and subdirectories
find "$DIR" -type f | sort | while read -r file; do
    echo -e "\t<Unit filename=\"$file\" />"
done
