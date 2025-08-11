#!/bin/bash

echo " Compiling..."
gtime -f "🛠️ Compile Time: %e sec" gcc -o out out.c

echo -e "\nRunning..."
gtime -f  "🧮 Execution Time: %e sec" ./out

echo " Compiling..."
gtime -f "🛠️ Compile Time: %e sec" gcc -o temp temp.c

echo -e "\nRunning..."
gtime -f  "🧮 Execution Time: %e sec" ./temp
