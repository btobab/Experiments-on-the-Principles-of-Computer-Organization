#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/number.h"
#include "lib/calculate.h"
#include "lib/tools.h"

#define N 32

// specific value
#define INF 1e9
#define NaN -1e9
#define DENORMS -1

// switch
#define UNSIGNED 0
#define SIGNED 0
#define FLOAT 0
#define NUMBER 0
#define COMPLEMENT 0
#define CALCULATE 0
#define FLOAT_ADD 1
#define MUL 0

int main() {
#if NUMBER
    char binary[N + 1];
    unsigned int uvalue = 0;
    int svalue = 0;
    float fvalue;
    memset(binary, '0', N + 1);
#if UNSIGNED
    fgets(binary, N+1, stdin);
    uvalue = unsignedToValue(binary, N);
    printf("unsigned value is %u\n", uvalue);
#endif

#if SIGNED
    fgets(binary, N + 1, stdin);
    svalue = intToValue(binary, N);
    printf("signed value is %d\n", svalue);
    return 0;
#endif

#if FLOAT
    fgets(binary, N + 1, stdin);
    fvalue = floatToValue(binary, N);
    if (fvalue == INF) {
        printf("float value is INF\n");
    } else if (fvalue == NaN) {
        printf("float value is NaN\n");
    } else if (fvalue == DENORMS) {
        printf("float value is denorms\n");
    }
    else {
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
    // printf("int1 is %s\n flag is %d\n int2 is %s\n", int1, flag, int2);
    sign = addsub(int1, int2, flag, N);
    printf("%s %c %s equals %s\n", int1, op, int3, int2);
    printf("OF: %c, CF: %c, SF: %c, ZF: %c\n", sign[0], sign[1], sign[2], sign[3]);
    free(sign);
    return 0;
#endif

#if FLOAT_ADD
    float value;
    char float1[N + 1], float2[N + 1], *result;
    fgets(float1, N + 1, stdin);
    getchar();
    fgets(float2, N + 1, stdin);
    result = addfloat(float1, float2);
    if (result) {
        value = floatToValue(result, N);
        if (value == INF | value == NaN) {
            printf("float is overflow\n");
        } else {
            printf("float value is %f\n", value);
            free(result);
        }
    } else {
        printf("float is overflow\n");
    }
    return 0;
#endif

#if MUL
    char int1[N + 1], int2[N + 1], _int1[N + 1], _int2[N + 1];
    fgets(int1, N + 1, stdin);
    getchar();
    fgets(int2, N + 1, stdin);
    strcpy(_int1, int1);
    strcpy(_int2, int2);
    mul(int1, int2, N);
    printf("%s mul %s equals %s%s\n", _int1, _int2, int1, int2);
    return 0;
#endif
}
