#include "segment.c"
#include "stdio.h"

int main() {
    Digit digits[2];
    SegmentUpdateDigits(digits, 290, 2, 2);

    while (1);
}

// typedef struct bits_t bits;
// struct bits_t {
//     uint8_t    b0 : 1[8];
//     uint8_t    b1 : 1;
//     uint8_t    b2 : 1;
//     uint8_t    b3 : 1;
//     uint8_t    b4 : 1;
//     uint8_t    b5 : 1;
//     uint8_t    b6 : 1;
//     uint8_t    b7 : 1;
// };

// typedef union Bytee_t Bytee;
// union Bytee_t {
//     uint8_t byte;
//     bits bits;
// };

// int main() {
//     Bytee number = { .byte = 0 };

//     number.bits.b0 = 1;
//     printf("%d\n", number.byte);

//     number.bits.b1 = 1;
//     printf("%d\n", number.byte);

//     number.bits.b2 = 1;
//     printf("%d\n", number.byte);

//     number.bits.b3 = 1;
//     printf("%d\n", number.byte);

//     number.bits.b4 = 1;
//     printf("%d\n", number.byte);
// }


