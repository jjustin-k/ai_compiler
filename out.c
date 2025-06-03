#include<stdio.h>

int size = 10;

void matmul(float* out, float* a, float* b, size){
    
for(int i = 0; i < size; i++){
    out[i] = a[i] - b[i];}

}

void add(float* out, float* a, float* b, size){
    
for(int i = 0; i < size; i++){
    out[i] = a[i] + b[i];}

}

void sub(float* out, float* a, float* b, size){
    
for(int i = 0; i < size; i++){
    out[i] = a[i] - b[i];}

}
