#include <stdio.h>

extern int add_numbers(unsigned char* output_buffer, unsigned char* input_buffer, int* kernel, int width, int height);


int main(void)
{
    unsigned char *a;
    int b[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1}; 

    printf("%d\n", add_numbers(a, a, b, 1, 20));
    return 0;
}
