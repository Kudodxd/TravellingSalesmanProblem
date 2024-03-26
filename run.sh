#!/bin/bash

# Compile the C program
gcc $1.c -o a.out -lm

# Directory containing input files
input_dir="in_data"

# Check if the input directory exists
if [ ! -d "$input_dir" ]; then
    echo "Error: $input_dir directory not found."
    exit 1
fi

# Loop through each input file in the directory
for input_file in "$input_dir"/*.tsp; do
    # Check if there are matching files
    if [ -e "$input_file" ]; then
        # Get the file name without the directory
        file_name=$(basename "$input_file")

        # Execute the program with the current input file
        echo -n "$file_name :" 
        ./a.out "$input_file"

        # Print the tour.dat file
        #tail -n 1 tour*.dat

        # Add a separator between runs for better readability
        echo ""
    else
        echo "Error: File $input_file not found."
    fi
done

