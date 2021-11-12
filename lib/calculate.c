#include "calculate.h"
#include <string.h>
#include <stdlib.h>

#define i(x) (int)(x - '0')
#define c(x) (int8_t)(x) + '0'

#define SIGN_LEN 5

void toComplement(char binary[], int n) {
    int i = 0, carry = 1, tmp = 0;
    for (i = 0; i < n; ++i) {
        binary[i] = c(i(binary[i]) ^ 1);
    }
    for (i = n - 1; i >= 0; --i) {
        tmp = i(binary[i]) & carry;
        binary[i] = c(i(binary[i]) ^ carry);
        carry = tmp;
    }
}

char *addsub(const char int1[], char int2[], int flag, int n) {
    int origin = 0, carry = 0, i = 0, tmp = 0, int1_sign = -1, int2_sign = -1;
    char *sign = (char *) malloc(sizeof(char) * SIGN_LEN);
    // sign[0]:OF, sign[1]:CF, sign[2]:SF, sign[3]:ZF
    memset(sign, '\0', SIGN_LEN);
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
    int1_sign = i(int1[0]);
    int2_sign = i(int2[0]);
    // OF: overflow flag
    // positive plus positive | negative plus negative | positive minus negative | negative minus positive
    sign[0] = c(((flag ^ 1) & (int1_sign ^ 1) & int2_sign) |
                ((flag ^ 1) & int1_sign & (int2_sign ^ 1)) |
                (flag & (int1_sign ^ 1) & int2_sign) |
                (flag & int1_sign & (int2_sign ^ 1)));
    // CF: carry flag
    // plus | minus
    sign[1] = c(((flag ^ 1) & carry) | (flag & (int1_sign ^ 1) & int2_sign));
    // SF: sign flag
    sign[2] = c(flag & int2_sign);
    // ZF: zero flag
    sign[3] = c(tmp ^ 1);
    return sign;
}