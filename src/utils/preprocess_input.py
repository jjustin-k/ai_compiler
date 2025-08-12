import numpy as np


input_filename = "mnist_test.csv"
output_filename = "output/input.txt"

skip_start = True
with open(input_filename, 'r') as fin, open(output_filename, 'w') as fout:

    for line in fin:

        if skip_start:
            skip_start = False
            continue 

        parts = line.strip().split(',')
      
        expected = int(parts.pop(0))
        floats = [float(x) / 255.0 for x in parts if x.strip() != '']
        floats.append(expected)

        formatted = [f"{x:.5f}," for x in floats]

        fout.write(''.join(formatted) + '\n')
