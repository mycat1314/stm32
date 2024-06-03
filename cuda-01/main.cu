#include <iostream>
#include <stdio.h>

__global__ void hello()
{
    printf("hello gpu\n");
}

int main() {
    hello<<<100, 100>>>();

    cudaDeviceSynchronize();
    return 0;
}
