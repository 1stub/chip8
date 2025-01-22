#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

typedef unsigned char byte;
typedef unsigned short word;

#define GET_BIT(src, bit_no) src & (1 << bit_no)

#endif