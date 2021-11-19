#include "calculate.h"
#include "number.h"
#include "tools.h"
#include <string.h>
#include <stdlib.h>

#define i(x) (int)(x - '0')
#define c(x) (int8_t)(x) + '0'

#define SIGN_LEN 5
#define EXP_LEN 8
#define FRAC_LEN 23
#define _FRAC_LEN 27
#define N 33

// end denote '\0'
int start = 0, end = 0;

void update(int _start, int _end) {
    start = _start;
    end = _end;
}

char *addsub(const char int1[], char int2[], int flag, int n) {
    int origin = 0, carry = 0, i = 0, tmp = 0, int1_sign = -1, int2_sign = -1, _int2_sign = -1;
    char *sign = (char *) malloc(sizeof(char) * SIGN_LEN);
    // sign[0]:OF, sign[1]:CF, sign[2]:SF, sign[3]:ZF
    memset(sign, '\0', SIGN_LEN);
    _int2_sign = i(int2[0]);
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
    sign[0] = c(((flag ^ 1) & (int1_sign ^ 1) & (_int2_sign ^ 1) & int2_sign) |
                ((flag ^ 1) & int1_sign & _int2_sign & (int2_sign ^ 1)) |
                (flag & (int1_sign ^ 1) & int2_sign) |
                (flag & int1_sign & int2_sign));
    // CF: carry flag
    // plus | minus
    sign[1] = c(((flag ^ 1) & carry) | (flag & (int1_sign ^ 1) & int2_sign));
    // SF: sign flag
    sign[2] = c(int2_sign);
    // ZF: zero flag
    sign[3] = c(tmp ^ 1);
    return sign;
}

char *addfloat(char float1[], char float2[]) {
    char *result, exp1[EXP_LEN + 1], exp2[EXP_LEN + 1], *exp, frac1[_FRAC_LEN], frac2[_FRAC_LEN], exp_one[EXP_LEN + 1];
    int delta_e = 0, e1 = 0, e2 = 0, flag, *p_start = &start, *p_end = &end, exp_end = EXP_LEN, offset = 1;

    memset(exp_one, '0', sizeof(char) * (EXP_LEN - 1));
    exp_one[EXP_LEN - 1] = '1';
    exp_one[EXP_LEN] = '\0';
    result = (char *) malloc(sizeof(char) * N);

    memset(frac1, '0', _FRAC_LEN);
    memset(frac2, '0', _FRAC_LEN);
    copy_arr(float1, exp1, 1, EXP_LEN);
    copy_arr(float2, exp2, 1, EXP_LEN);
    copy_arr(float1, frac1, 1 + EXP_LEN, EXP_LEN + FRAC_LEN);
    copy_arr(float2, frac2, 1 + EXP_LEN, EXP_LEN + FRAC_LEN);

    // add 1 bit to frac
    update(0, FRAC_LEN);
    mov_arr(frac1, p_start, p_end, 1, _FRAC_LEN, 0);
    update(0, FRAC_LEN);
    mov_arr(frac2, p_start, p_end, 1, _FRAC_LEN, 0);
    frac1[0] = '1';
    frac2[0] = '1';

    // compare exp
    e1 = (int) unsignedToValue(exp1, EXP_LEN);
    e2 = (int) unsignedToValue(exp2, EXP_LEN);
    delta_e = (e1 - e2 > 0) ? (e1 - e2) : (e2 - e1);
    exp = (e1 - e2 > 0) ? exp1 : exp2;
    update(0, FRAC_LEN);
    if (e1 > e2) {
        mov_arr(frac2, p_start, p_end, delta_e, _FRAC_LEN, 1);
    } else {
        mov_arr(frac1, p_start, p_end, delta_e, _FRAC_LEN, 1);
    }

    // calculate frac
    update(0, end);
    flag = add_arr(frac1, frac2, p_start, p_end);

    // format frac2
    if (flag) {
        mov_arr(frac2, p_start, p_end, 1, _FRAC_LEN, 1);
        offset++;
        if (add_arr(exp_one, exp, p_start, &exp_end)) {
            return NULL;
        }
    }

    // rounding
    start = 0;
    int round_end = end - delta_e;
    if (delta_e) {
        if (i(frac2[round_end])) {
            char one[round_end];
            memset(one, '0', sizeof(char) * (round_end - 2));
            one[round_end - 2] = '1';
            one[round_end - 1] = '\0';
            add_arr(one, frac2, p_start, &round_end);
        }
    }

    // assemble
    result[0] = '0';
    strcpy(result + 1, exp);
    strncpy(result + EXP_LEN + 1, frac2 + offset, FRAC_LEN);
    result[N - 1] = '\0';
    return result;
}

void mul(char int1[], char int2[], int n) {
    char *matrix[n];
    char result[2 * n + 1];
    int i, j, tmp, over;
    int carry[2 * n];
    for (i = 0; i < n; ++i) {
        char *arr = (char *) malloc(sizeof(char) * 2 * n);
        memset(arr, '0', sizeof(char) * 2 * n);
        arr[2 * n - 1] = '\0';
        matrix[i] = arr;
    }

    memset(result, '0', sizeof(char) * (2 * n));
    result[2 * n] = '\0';
    memset(carry, 0, sizeof(int) * 2 * n);
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            matrix[i][j + n - 1 - i] = c(i(int1[j]) & i(int2[n - 1 - i]));
        }
    }
    for (i = 2 * n - 2; i >= 0; --i) {
        tmp = 0;
        for (j = 0; j < n; ++j) {
            tmp += i(matrix[j][i]);
        }
        result[i + 1] = c((tmp + carry[i + 1]) % 2);
        if (tmp + carry[i + 1] > 1) {
            over = (tmp + carry[i + 1]) / 2;
            carry[i] += over;
        }
    }
    result[0] = c(carry[0]);
    for (i = 0; i < n; ++i) {
        int1[i] = result[i];
        int2[i] = result[i + n];
    }
}