#!/bin/bash

# File path
file_path="/tmp/afw_pool_alloc_size.txt"

# Check if the file exists
if [[ ! -f "$file_path" ]]; then
    echo "Error: File does not exist."
    exit 1
fi

# Get total number of sizes (lines in file)
total_sizes=$(wc -l < "$file_path")

# Check for zero-size file to avoid division by zero
if [[ "$total_sizes" -eq 0 ]]; then
    echo "Error: File is empty."
    exit 1
fi

# Process the file and count occurrences
declare -A size_counts

while IFS= read -r size; do
    # Increment the count for this size
    ((size_counts["$size"]++))
done < "$file_path"

# Print the percentage for each size
for size in "${!size_counts[@]}"; do
    count=${size_counts[$size]}
    percentage=$(awk "BEGIN {printf \"%.2f\", ($count / $total_sizes) * 100}")
    echo "$size $count $percentage"
done | sort -k1,1n | awk '{printf "Size %s occurs %d times, which is %.2f%% of the total.\n", $1, $2, $3}'

exit 0

