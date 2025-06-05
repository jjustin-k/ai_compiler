#include <stdio.h>

float w1[] = {0.1,  -0.2, 0.3, 0.4,  0.5, 0.6, -0.7, 0.8,
              -0.9, 1,    0.1, -0.2, 0.3, 0.4, -0.5, 0.6};

float b1[] = {0.1, 0, -0.1, 0.2};

void matmul2d(float *out, float *a, float *b, int size) {

  for (int i = 0; i < m; i++) {
    out[i] = 0;

    for (int j = 0; j < n; j++) {
      out[i] += a[j] * b[j * m + i];
    }
  }
}

void add(float *out, float *a, float *b, int size) {

  for (int i = 0; i < size; i++) {
    out[i] = a[i] + b[i];
  }
}
