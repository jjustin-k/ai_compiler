#include <stdio.h>

int x_size = 64;
float x[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
             33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
             49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};

int w1_size = 16;
float w1[] = {0.1,  -0.2, 0.3, 0.4,  0.5, 0.6, -0.7, 0.8,
              -0.9, 1,    0.1, -0.2, 0.3, 0.4, -0.5, 0.6};

int b1_size = 4;
float b1[] = {0.1, 0, -0.1, 0.2};

void maxpool(float *out, float *a, int size) {

  for (int i = 0; i < size; i++) {
    out[i] = (a[i] > 0.0f) ? a[i] : 0.0f;
  }
}

void matmul2d(float *out, float *a, float *b, int m, int n) {

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

void relu(float *a, int size) {

  for (int i = 0; i < size; i++) {
    a[i] = (a[i] > 0.0f) ? a[i] : 0.0f;
  }
}

void relu(float *a, int size) {

  for (int i = 0; i < size; i++) {
    a[i] = (a[i] > 0.0f) ? a[i] : 0.0f;
  }
}

int main() {

  maxpool(maxpool1, x, x_size);

  matmul(matmul1, maxpool1, maxpool1_size);

  float add1[matmul1_size];

  add(add1, matmul1, b1, matmul1_size);

  relu(relu1, add1_size);

  relu(out, relu1_size);

  return 0;
};
