#include "number.h"
#include <string.h>
#include <stdio.h>

#define EXP_LEN 8
#define FRAC_LEN 23
#define BIAS 127
#define INF 1e9
#define NaN -1e9

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
    dst[j] = '\0';
}

unsigned int unsignedToValue(char *binary, int n) {
    int number[n], i;
    strToNumber(number, binary, n);
    unsigned int uvalue = 0;
    for (i = 0; i < n; ++i) {
        uvalue += number[i] << (n - i - 1);
    }
    return uvalue;
}

int intToValue(char *binary, int n) {
    int number[n], svalue = 0, _svalue;
    strToNumber(number, binary, n);
    _svalue = (int) unsignedToValue(binary, n);
    if (number[0] == 0) {
        svalue = _svalue;
    } else {
        svalue = (1 << n) - _svalue;
    }
    return svalue;
}

float floatToValue(char *binary, int n) {
    int number[n], e, exp, frac_m = 1 << FRAC_LEN, frac_c;
    float frac, value;
    char exp_arr[EXP_LEN + 1], frac_arr[FRAC_LEN + 1];
    copy_arr(binary, exp_arr, 1, EXP_LEN);
    copy_arr(binary, frac_arr, 1 + EXP_LEN, n - 1);
    e = (int) unsignedToValue(exp_arr, EXP_LEN);
    frac_c = (int) unsignedToValue(frac_arr, FRAC_LEN);
    if (e == 0xff) {
        if (frac_c == 0) {
            return INF;
        } else {
            return NaN;
        }
    } else if (e == 0) {
        exp = 1 - BIAS;
        frac = (float) frac_c / (float)frac_m;
    } else {
        exp = e - BIAS;
        frac = (float) frac_c / (float)frac_m + 1;
    }
    if (exp < 0) {
        value = frac / (float)(1 << -exp);
    } else {
        value = frac * (float)(1 << exp);
    }
    if (binary[0] == '1') {
        value *= -1;
    }
    return value;
}