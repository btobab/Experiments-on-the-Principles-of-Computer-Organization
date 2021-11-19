#include "tools.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define i(x) (int)(x - '0')
#define c(x) (int8_t)(x) + '0'

#define CAPACITY 27

void strToNumber(int number[], const char *binary, int n) {
    int i = 0;
    memset(number, 0, sizeof(int) * n);
    while (binary[i] != '\0') {
        number[i] = binary[i] - '0';
        i++;
    }
}

void copy_arr(const char *src, char *dst, int start, int end) {
    int i, j = 0;
    for (i = start; i <= end; ++i) {
        dst[j++] = src[i];
    }
}

void toComplement(char binary[], int n) {
    int i, carry = 1, tmp;
    for (i = 0; i < n; ++i) {
        binary[i] = c(i(binary[i]) ^ 1);
    }
    for (i = n - 1; i >= 0; --i) {
        tmp = i(binary[i]) & carry;
        binary[i] = c(i(binary[i]) ^ carry);
        carry = tmp;
    }
}

void mov_arr(char *str, int *start, int *end, int offset, int capacity, int bool) {
    if (*end + offset > capacity) {
        perror("str is overflow");
        exit(-1);
    }
    int i;
    for (i = *end; i >= *start; --i) {
        str[i + offset] = str[i];
    }
    if (bool) {
        for (i = 0; i < *start + offset; ++i) {
            str[i] = '0';
        }
    }
    *end += offset;
}

int add_arr(const char *frac1, char *frac2, int *start, int *end) {
    int origin, carry = 0, i;
    for (i = *end - 1; i >= *start; --i) {
        origin = i(frac1[i]) ^ i(frac2[i]) ^ carry;
        carry = (i(frac1[i]) & i(frac2[i])) | (i(frac1[i]) & carry) | (i(frac2[i]) & carry);
        frac2[i] = c(origin);
    }
    if (carry == 1) {
        if (*start == 0) {
            mov_arr(frac2, start, end, 1, CAPACITY, 1);
            // *start -= 1;
        }
        frac2[*start] = c(carry);
    }
    return carry;
}