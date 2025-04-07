#!/bin/bash

for file_path in original/*; do
    file_name=$(basename "$file_path")
    file_name="${file_name%.*}"
    
    convert "${file_path}" "${file_name}.bmp"
done
