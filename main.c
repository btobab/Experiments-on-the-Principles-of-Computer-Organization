#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "number.h"
#include "calculate.h"

#define N 4
#define NUMBER 0
#define INF 1e9
#define NaN -1e9
#define CALCULATE 1
#define COMPLEMENT 0

int main() {
#if NUMBER
    char binary[N + 1];
    unsigned int uvalue = 0;
    int svalue = 0;
    float fvalue;
    memset(binary, '0', N + 1);
#ifndef BOOL
    fgets(binary, N+1, stdin);
    uvalue = unsignedToValue(binary, N);
    printf("unsigned value is %u\n", uvalue);
#endif

#ifndef BOOL
    fgets(binary, N+1, stdin);
    svalue = intToValue(binary, N);
    printf("signed value is %d\n", svalue);
    return 0;
#endif

#if BOOL
    fgets(binary, N + 1, stdin);
    fvalue = floatToValue(binary, N);
    if (fvalue == INF) {
        printf("float value is INF\n");
    } else if (fvalue == NaN) {
        printf("float value is NaN\n");
    } else {
        printf("float value is %f\n", fvalue);
    }
    return 0;
#endif
#endif

#if COMPLEMENT
    char binary[N + 1], *complement;
    memset(binary, '\0', N + 1);
    fgets(binary, N + 1, stdin);
    toComplement(binary, N);
    printf("complement is %s", binary);
    return 0;
#endif

#if CALCULATE
    char int1[N + 1], int2[N + 1], int3[N + 1], op = '\0', *sign;
    int flag = -1;
    memset(int1, '\0', N + 1);
    memset(int2, '\0', N + 1);
    fgets(int1, N + 1, stdin);
    op = (char) getchar();
    fgets(int2, N + 1, stdin);
    strcpy(int3, int2);
    flag = (op == '+' ? 0 : 1);
    //printf("int1 is %s\nflag is %d\nint2 is %s\n", int1, flag, int2);
    sign = addsub(int1, int2, flag, N);
    printf("%s %c %s equals %s\n", int1, op, int3, int2);
    printf("OF: %c, CF: %c, SF: %c, ZF: %c\n", sign[0], sign[1], sign[2], sign[3]);
    free(sign);
    return 0;
#endif
}
