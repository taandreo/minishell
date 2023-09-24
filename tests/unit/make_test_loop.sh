#!/bin/bash

YELLOW='\033[33m'
RED='\033[31m'
NC='\033[0m'

for i in {1..270}; do
    echo -e "${YELLOW}Running make with MALLOC_VALUE=${RED}$i${NC}"
    output=$(make --no-print-directory MALLOC_VALUE=$i clean_test test | grep -e "Error: Memory allocation failed")
    echo "$output"
done

echo "All iterations completed successfully."