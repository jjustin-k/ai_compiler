#include <stdio.h>

int x_size = 64;
float x[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
             17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
             33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
             49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};

int w1_size = 16;
float w1[] = {0.1,  -0.2, 0.3, 0.4,  0.5, 0.6, -0.7, 0.8,
              -0.9, 1,    0.1, -0.2, 0.3, 0.4, -0.5, 0.6};

int b1_size = 16;
float b1[] = {0.1, 0, -0.1, 0.2, 0.1, 0, -0.1, 0.2,
              0.1, 0, -0.1, 0.2, 0.1, 0, -0.1, 0.2};

void maxpool2d(float *out, float *a, int width, int height, int pool_size,
               int stride) {

  int out_w = (width - pool_size) / stride + 1;
  int out_h = (height - pool_size) / stride + 1;

  for (int oy = 0; oy < out_h; oy++) {

    for (int ox = 0; ox < out_w; ox++) {

      float max_val = -1e9;

      for (int ky = 0; ky < pool_size; ky++) {

        for (int kx = 0; kx < pool_size; kx++) {

          int ix = ox * stride + kx;
          int iy = oy * stride + ky;
          float val = a[iy * width + ix];
          if (val > max_val) {
            max_val = val;
          };
        }
      }

      out[oy * out_w + ox] = max_val;
    }
  }
}

void matmul2d(float *out, float *a, float *b, int m, int n, int p) {

  for (int i = 0; i < m; i++) {

    for (int j = 0; j < p; j++) {
      float sum = 0.0f;

      for (int k = 0; k < n; k++) {
        sum += a[i * n + k] * b[k * p + j];
      }

      out[i * p + j] = sum;
    }
  }
}

void add(float *out, float *a, float *b) {

  for (int i = 0; i < 16; i++) {
    out[i] = a[i] + b[i];
  }
}

void relu(float *a) {

  for (int i = 0; i < 16; i++) {
    a[i] = (a[i] > 0.0f) ? a[i] : 0.0f;
  }
}

int main() {

  float maxpool1[64];

  maxpool2d(maxpool1, x, 8, 8, 2, 2);
  int matmul1_m = 4;
  int matmul1_n = 4;
  int matmul1_p = 4;

  float matmul1[16];

  matmul2d(matmul1, maxpool1, w1, matmul1_m, matmul1_n, matmul1_p);

  float add1[16];

  add(add1, matmul1, b1);

  relu(add1);

  relu(add1);

  for(int i = 0; i < 16; i++){
    printf("%f ", add1[i]);
  }
  printf("\n");

  return 0;
};
