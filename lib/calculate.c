#include "calculate.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define i(x) (int)(x - '0')
#define c(x) (int8_t)(x) + '0'

void *toComplement(char intn[], int n) {
    int i = 0, carry = 1, tmp = 0;
    while (intn[i] != '\0') {
        intn[i] = c(i(intn[i]) ^ 1);
        i++;
    }
    for (i = n - 1; i >= 0; --i) {
        tmp = i(intn[i]) & carry;
        intn[i] = c(i(intn[i]) ^ carry);
        carry = tmp;
    }
    return intn;
}

char *addsub(const char int1[], char int2[], int flag, int n) {
    int origin = 0, carry = 0, i = 0, tmp = 0;
    char *sign = (char *) malloc(sizeof(char) * 5);
    memset(sign, '\0', 5);  //sign[0]:OF, sign[1]:CF, sign[2]:SF, sign[3]:ZF
    if (flag) {
        toComplement(int2, n);
    }
    for (i = n - 1; i >= 0; --i) {
        origin = i(int1[i]) ^ i(int2[i]) ^ carry;
        carry = (i(int1[i]) & i(int2[i])) | (i(int1[i]) & carry) | (i(int2[i]) & carry);
        int2[i] = c(origin);
    }
    for (i = 0; i < n; ++i) {
        tmp = tmp | i(int2[i]);
    }
    // positive plus positive | negative plus negative | positive minus negative
    sign[0] = c(((flag ^ 1) & (i(int1[0]) ^ 1) & i(int2[0])) |
                ((flag ^ 1) & i(int1[0]) & (i(int2[0]) ^ 1)) |
                (flag & (i(int1[0]) ^ 1) & i(int2[0])));
    // plus | minus
    sign[1] = c(((flag ^ 1) & carry) | (flag & (i(int1[0]) ^ 1) & i(int2[0])));
    sign[2] = c(flag & i(int2[0]));
    sign[3] = c(tmp ^ 1);
    return sign;
}