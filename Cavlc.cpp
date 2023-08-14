
#include <cassert>

#include "Cavlc.h"

int coeff_token_table(int nC,
    int coeff_token,
    int& coeff_token_bit_length,
    int& TrailingOnes,
    int& TotalCoeff)
{
    if (nC >= 0 && nC < 2) {
        if ((coeff_token >> 15) == 0x0001) //(1)b
        {
            coeff_token_bit_length = 1;
            TrailingOnes = 0;
            TotalCoeff = 0;
        } else if ((coeff_token >> 10) == 0x0005) //(0001 01)b ==> (00 0101)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 1;
        } else if ((coeff_token >> 14) == 0x0001) //(01)b
        {
            coeff_token_bit_length = 2;
            TrailingOnes = 1;
            TotalCoeff = 1;
        } else if ((coeff_token >> 8) == 0x0007) //(0000 0111)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 0;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0004) //(0001 00)b ==> (00 0100)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 2;
        } else if ((coeff_token >> 13) == 0x0001) //(001)b
        {
            coeff_token_bit_length = 3;
            TrailingOnes = 2;
            TotalCoeff = 2;
        } else if ((coeff_token >> 7) == 0x0007) //(0000 0011 1)b ==> (0 0000 0111)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 3;
        } else if ((coeff_token >> 8) == 0x0006) //(0000 0110)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 1;
            TotalCoeff = 3;
        } else if ((coeff_token >> 9) == 0x0005) //(0000 101)b ==> (000 0101)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 3;
        } else if ((coeff_token >> 11) == 0x0003) //(0001 1)b ==> (0 0011)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 3;
            TotalCoeff = 3;
        } else if ((coeff_token >> 6) == 0x0007) //(0000 0001 11)b ==> (00 0000 0111)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 0;
            TotalCoeff = 4;
        } else if ((coeff_token >> 7) == 0x0006) //(0000 0011 0)b ==> (0 0000 0110)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 1;
            TotalCoeff = 4;
        } else if ((coeff_token >> 8) == 0x0005) //(0000 0101)b ==> (0000 0101)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 2;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x0003) //(0000 11)b ==> (00 0011)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 4;
        } else if ((coeff_token >> 5) == 0x0007) //(0000 0000 111)b ==> (000 0000 0111)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 0;
            TotalCoeff = 5;
        } else if ((coeff_token >> 6) == 0x0006) //(0000 0001 10)b ==> (00 0000 0110)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 1;
            TotalCoeff = 5;
        } else if ((coeff_token >> 7) == 0x0005) //(0000 0010 1)b ==> (0 0000 0101)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 2;
            TotalCoeff = 5;
        } else if ((coeff_token >> 9) == 0x0004) //(0000 100)b ==> (000 0100)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 3;
            TotalCoeff = 5;
        } else if ((coeff_token >> 3) == 0x000F) //(0000 0000 0111 1)b ==> (0 0000 0000 1111)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 6;
        } else if ((coeff_token >> 5) == 0x0006) //(0000 0000 110)b ==> (000 0000 0110)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 1;
            TotalCoeff = 6;
        } else if ((coeff_token >> 6) == 0x0005) //(0000 0001 01)b ==> (00 0000 0101)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 2;
            TotalCoeff = 6;
        } else if ((coeff_token >> 8) == 0x0004) //(0000 0100)b ==> (0000 0100)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 3;
            TotalCoeff = 6;
        } else if ((coeff_token >> 3) == 0x000B) //(0000 0000 0101 1)b ==> (0 0000 0000 1011)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 7;
        } else if ((coeff_token >> 3) == 0x000E) //(0000 0000 0111 0)b ==> (0 0000 0000 1110)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 1;
            TotalCoeff = 7;
        } else if ((coeff_token >> 5) == 0x0005) //(0000 0000 101)b ==> (000 0000 0101)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 2;
            TotalCoeff = 7;
        } else if ((coeff_token >> 7) == 0x0004) //(0000 0010 0)b ==> (0 0000 0100)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 3;
            TotalCoeff = 7;
        } else if ((coeff_token >> 3) == 0x0008) //(0000 0000 0100 0)b ==> (0 0000 0000 1000)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 8;
        } else if ((coeff_token >> 3) == 0x000A) //(0000 0000 0101 0)b ==> (0 0000 0000 1010)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 1;
            TotalCoeff = 8;
        } else if ((coeff_token >> 3) == 0x000D) //(0000 0000 0110 1)b ==> (0 0000 0000 1101)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 2;
            TotalCoeff = 8;
        } else if ((coeff_token >> 6) == 0x0004) //(0000 0001 00)b ==> (00 0000 0100)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 3;
            TotalCoeff = 8;
        } else if ((coeff_token >> 2) == 0x000F) //(0000 0000 0011 11)b ==> (00 0000 0000 1111)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 0;
            TotalCoeff = 9;
        } else if ((coeff_token >> 2) == 0x000E) //(0000 0000 0011 10)b ==> (00 0000 0000 1110)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 1;
            TotalCoeff = 9;
        } else if ((coeff_token >> 3) == 0x0009) //(0000 0000 0100 1)b ==> (0 0000 0000 1001)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 2;
            TotalCoeff = 9;
        } else if ((coeff_token >> 5) == 0x0004) //(0000 0000 100)b ==> (000 0000 0100)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 3;
            TotalCoeff = 9;
        } else if ((coeff_token >> 2) == 0x000B) //(0000 0000 0010 11)b ==> (00 0000 0000 1011)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 0;
            TotalCoeff = 10;
        } else if ((coeff_token >> 2) == 0x000A) //(0000 0000 0010 10)b ==> (00 0000 0000 1010)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 1;
            TotalCoeff = 10;
        } else if ((coeff_token >> 2) == 0x000D) //(0000 0000 0011 01)b ==> (00 0000 0000 1101)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 2;
            TotalCoeff = 10;
        } else if ((coeff_token >> 3) == 0x000C) //(0000 0000 0110 0)b ==> (0 0000 0000 1100)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 3;
            TotalCoeff = 10;
        } else if ((coeff_token >> 1) == 0x000F) //(0000 0000 0001 111)b ==> (000 0000 0000 1111)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 0;
            TotalCoeff = 11;
        } else if ((coeff_token >> 1) == 0x000E) //(0000 0000 0001 110)b ==> (000 0000 0000 1110)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 1;
            TotalCoeff = 11;
        } else if ((coeff_token >> 2) == 0x0009) //(0000 0000 0010 01)b ==> (00 0000 0000 1001)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 2;
            TotalCoeff = 11;
        } else if ((coeff_token >> 2) == 0x000C) //(0000 0000 0011 00)b ==> (00 0000 0000 1100)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 3;
            TotalCoeff = 11;
        } else if ((coeff_token >> 1) == 0x000B) //(0000 0000 0001 011)b ==> (000 0000 0000 1011)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 0;
            TotalCoeff = 12;
        } else if ((coeff_token >> 1) == 0x000A) //(0000 0000 0001 010)b ==> (000 0000 0000 1010)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 1;
            TotalCoeff = 12;
        } else if ((coeff_token >> 1) == 0x000D) //(0000 0000 0001 101)b ==> (000 0000 0000 1101)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 2;
            TotalCoeff = 12;
        } else if ((coeff_token >> 2) == 0x0008) //(0000 0000 0010 00)b ==> (00 0000 0000 1000)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 3;
            TotalCoeff = 12;
        } else if ((coeff_token >> 0) == 0x000F) //(0000 0000 0000 1111)b ==> (0000 0000 0000 1111)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 0;
            TotalCoeff = 13;
        } else if ((coeff_token >> 1) == 0x0001) //(0000 0000 0000 001)b ==> (000 0000 0000 0001)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 1;
            TotalCoeff = 13;
        } else if ((coeff_token >> 1) == 0x0009) //(0000 0000 0001 001)b ==> (000 0000 0000 1001)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 2;
            TotalCoeff = 13;
        } else if ((coeff_token >> 1) == 0x000C) //(0000 0000 0001 100)b ==> (000 0000 0000 1100)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 3;
            TotalCoeff = 13;
        } else if ((coeff_token >> 0) == 0x000B) //(0000 0000 0000 1011)b ==> (0000 0000 0000 1011)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 0;
            TotalCoeff = 14;
        } else if ((coeff_token >> 0) == 0x000E) //(0000 0000 0000 1110)b ==> (0000 0000 0000 1110)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 1;
            TotalCoeff = 14;
        } else if ((coeff_token >> 0) == 0x000D) //(0000 0000 0000 1101)b ==> (0000 0000 0000 1101)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 2;
            TotalCoeff = 14;
        } else if ((coeff_token >> 1) == 0x0008) //(0000 0000 0001 000)b ==> (000 0000 0000 1000)b
        {
            coeff_token_bit_length = 15;
            TrailingOnes = 3;
            TotalCoeff = 14;
        } else if ((coeff_token >> 0) == 0x0007) //(0000 0000 0000 0111)b ==> (0000 0000 0000 0111)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 0;
            TotalCoeff = 15;
        } else if ((coeff_token >> 0) == 0x000A) //(0000 0000 0000 1010)b ==> (0000 0000 0000 1010)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 1;
            TotalCoeff = 15;
        } else if ((coeff_token >> 0) == 0x0009) //(0000 0000 0000 1001)b ==> (0000 0000 0000 1001)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 2;
            TotalCoeff = 15;
        } else if ((coeff_token >> 0) == 0x000C) //(0000 0000 0000 1100)b ==> (0000 0000 0000 1100)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 3;
            TotalCoeff = 15;
        } else if ((coeff_token >> 0) == 0x0004) //(0000 0000 0000 0100)b ==> (0000 0000 0000 0100)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 0;
            TotalCoeff = 16;
        } else if ((coeff_token >> 0) == 0x0006) //(0000 0000 0000 0110)b ==> (0000 0000 0000 0110)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 1;
            TotalCoeff = 16;
        } else if ((coeff_token >> 0) == 0x0005) //(0000 0000 0000 0101)b ==> (0000 0000 0000 0101)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 2;
            TotalCoeff = 16;
        } else if ((coeff_token >> 0) == 0x0008) //(0000 0000 0000 1000)b ==> (0000 0000 0000 1000)b
        {
            coeff_token_bit_length = 16;
            TrailingOnes = 3;
            TotalCoeff = 16;
        } else {
            assert(false);
        }
    } else if (nC >= 2 && nC < 4) {
        if ((coeff_token >> 14) == 0x0003) // (11)b
        {
            coeff_token_bit_length = 2;
            TrailingOnes = 0;
            TotalCoeff = 0;
        } else if ((coeff_token >> 10) == 0x000B) // (0010 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 1;
        } else if ((coeff_token >> 14) == 0x0002) // (10)b
        {
            coeff_token_bit_length = 2;
            TrailingOnes = 1;
            TotalCoeff = 1;
        } else if ((coeff_token >> 10) == 0x0007) // (0001 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 2;
        } else if ((coeff_token >> 11) == 0x0007) // (0011 1)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 1;
            TotalCoeff = 2;
        } else if ((coeff_token >> 13) == 0x0003) // (011)b
        {
            coeff_token_bit_length = 3;
            TrailingOnes = 2;
            TotalCoeff = 2;
        } else if ((coeff_token >> 9) == 0x0007) // (0000 111)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x000A) // (0010 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x0009) // (0010 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 3;
        } else if ((coeff_token >> 12) == 0x0005) // (0101)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 3;
        } else if ((coeff_token >> 8) == 0x0007) // (0000 0111)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 0;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x0006) // (0001 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x0005) // (0001 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 4;
        } else if ((coeff_token >> 12) == 0x0004) // (0100)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 4;
        } else if ((coeff_token >> 8) == 0x0004) // (0000 0100)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 0;
            TotalCoeff = 5;
        } else if ((coeff_token >> 9) == 0x0006) // (0000 110)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 1;
            TotalCoeff = 5;
        } else if ((coeff_token >> 9) == 0x0005) // (0000 101)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 5;
        } else if ((coeff_token >> 11) == 0x0006) // (0011 0)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 3;
            TotalCoeff = 5;
        } else if ((coeff_token >> 7) == 0x0007) // (0000 0011 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 6;
        } else if ((coeff_token >> 8) == 0x0006) // (0000 0110)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 1;
            TotalCoeff = 6;
        } else if ((coeff_token >> 8) == 0x0005) // (0000 0101)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 2;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x0008) // (0010 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 6;
        } else if ((coeff_token >> 5) == 0x000F) // (0000 0001 111)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 0;
            TotalCoeff = 7;
        } else if ((coeff_token >> 7) == 0x0006) // (0000 0011 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 1;
            TotalCoeff = 7;
        } else if ((coeff_token >> 7) == 0x0005) // (0000 0010 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 2;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x0004) // (0001 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 7;
        } else if ((coeff_token >> 5) == 0x000B) // (0000 0001 011)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 0;
            TotalCoeff = 8;
        } else if ((coeff_token >> 5) == 0x000E) // (0000 0001 110)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 1;
            TotalCoeff = 8;
        } else if ((coeff_token >> 5) == 0x000D) // (0000 0001 101)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 2;
            TotalCoeff = 8;
        } else if ((coeff_token >> 9) == 0x0004) // (0000 100)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 3;
            TotalCoeff = 8;
        } else if ((coeff_token >> 4) == 0x000F) // (0000 0000 1111)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 0;
            TotalCoeff = 9;
        } else if ((coeff_token >> 5) == 0x000A) // (0000 0001 010)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 1;
            TotalCoeff = 9;
        } else if ((coeff_token >> 5) == 0x0009) // (0000 0001 001)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 2;
            TotalCoeff = 9;
        } else if ((coeff_token >> 7) == 0x0004) // (0000 0010 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 3;
            TotalCoeff = 9;
        } else if ((coeff_token >> 4) == 0x000B) // (0000 0000 1011)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 0;
            TotalCoeff = 10;
        } else if ((coeff_token >> 4) == 0x000E) // (0000 0000 1110)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 1;
            TotalCoeff = 10;
        } else if ((coeff_token >> 4) == 0x000D) // (0000 0000 1101)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 2;
            TotalCoeff = 10;
        } else if ((coeff_token >> 5) == 0x000C) // (0000 0001 100)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 3;
            TotalCoeff = 10;
        } else if ((coeff_token >> 4) == 0x0008) // (0000 0000 1000)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 0;
            TotalCoeff = 11;
        } else if ((coeff_token >> 4) == 0x000A) // (0000 0000 1010)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 1;
            TotalCoeff = 11;
        } else if ((coeff_token >> 4) == 0x0009) // (0000 0000 1001)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 2;
            TotalCoeff = 11;
        } else if ((coeff_token >> 5) == 0x0008) // (0000 0001 000)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 3;
            TotalCoeff = 11;
        } else if ((coeff_token >> 3) == 0x000F) // (0000 0000 0111 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 12;
        } else if ((coeff_token >> 3) == 0x000E) // (0000 0000 0111 0)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 1;
            TotalCoeff = 12;
        } else if ((coeff_token >> 3) == 0x000D) // (0000 0000 0110 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 2;
            TotalCoeff = 12;
        } else if ((coeff_token >> 4) == 0x000C) // (0000 0000 1100)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 3;
            TotalCoeff = 12;
        } else if ((coeff_token >> 3) == 0x000B) // (0000 0000 0101 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 13;
        } else if ((coeff_token >> 3) == 0x000A) // (0000 0000 0101 0)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 1;
            TotalCoeff = 13;
        } else if ((coeff_token >> 3) == 0x0009) // (0000 0000 0100 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 2;
            TotalCoeff = 13;
        } else if ((coeff_token >> 3) == 0x000C) // (0000 0000 0110 0)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 3;
            TotalCoeff = 13;
        } else if ((coeff_token >> 3) == 0x0007) // (0000 0000 0011 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 14;
        } else if ((coeff_token >> 2) == 0x000B) // (0000 0000 0010 11)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 1;
            TotalCoeff = 14;
        } else if ((coeff_token >> 3) == 0x0006) // (0000 0000 0011 0)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 2;
            TotalCoeff = 14;
        } else if ((coeff_token >> 3) == 0x0008) // (0000 0000 0100 0)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 3;
            TotalCoeff = 14;
        } else if ((coeff_token >> 2) == 0x0009) // (0000 0000 0010 01)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 0;
            TotalCoeff = 15;
        } else if ((coeff_token >> 2) == 0x0008) // (0000 0000 0010 00)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 1;
            TotalCoeff = 15;
        } else if ((coeff_token >> 2) == 0x000A) // (0000 0000 0010 10)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 2;
            TotalCoeff = 15;
        } else if ((coeff_token >> 3) == 0x0001) // (0000 0000 0000 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 3;
            TotalCoeff = 15;
        } else if ((coeff_token >> 2) == 0x0007) // (0000 0000 0001 11)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 0;
            TotalCoeff = 16;
        } else if ((coeff_token >> 2) == 0x0006) // (0000 0000 0001 10)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 1;
            TotalCoeff = 16;
        } else if ((coeff_token >> 2) == 0x0005) // (0000 0000 0001 01)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 2;
            TotalCoeff = 16;
        } else if ((coeff_token >> 2) == 0x0004) // (0000 0000 0001 00)b
        {
            coeff_token_bit_length = 14;
            TrailingOnes = 3;
            TotalCoeff = 16;
        } else {
            assert(false);
        }
    } else if (nC >= 4 && nC < 8) {
        if ((coeff_token >> 12) == 0x000F) // (1111)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 0;
            TotalCoeff = 0;
        } else if ((coeff_token >> 10) == 0x000F) // (0011 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 1;
        } else if ((coeff_token >> 12) == 0x000E) // (1110)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 1;
            TotalCoeff = 1;
        } else if ((coeff_token >> 10) == 0x000B) // (0010 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 2;
        } else if ((coeff_token >> 11) == 0x000F) // (0111 1)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 1;
            TotalCoeff = 2;
        } else if ((coeff_token >> 12) == 0x000D) // (1101)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 2;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0008) // (0010 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 3;
        } else if ((coeff_token >> 11) == 0x000C) // (0110 0)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 1;
            TotalCoeff = 3;
        } else if ((coeff_token >> 11) == 0x000E) // (0111 0)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 2;
            TotalCoeff = 3;
        } else if ((coeff_token >> 12) == 0x000C) // (1100)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 3;
        } else if ((coeff_token >> 9) == 0x000F) // (0001 111)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 4;
        } else if ((coeff_token >> 11) == 0x000A) // (0101 0)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 1;
            TotalCoeff = 4;
        } else if ((coeff_token >> 11) == 0x000B) // (0101 1)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 2;
            TotalCoeff = 4;
        } else if ((coeff_token >> 12) == 0x000B) // (1011)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 4;
        } else if ((coeff_token >> 9) == 0x000B) // (0001 011)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 5;
        } else if ((coeff_token >> 11) == 0x0008) // (0100 0)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 1;
            TotalCoeff = 5;
        } else if ((coeff_token >> 11) == 0x0009) // (0100 1)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 2;
            TotalCoeff = 5;
        } else if ((coeff_token >> 12) == 0x000A) // (1010)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 5;
        } else if ((coeff_token >> 9) == 0x0009) // (0001 001)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x000E) // (0011 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x000D) // (0011 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 6;
        } else if ((coeff_token >> 12) == 0x0009) // (1001)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 6;
        } else if ((coeff_token >> 9) == 0x0008) // (0001 000)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x000A) // (0010 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x0009) // (0010 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 7;
        } else if ((coeff_token >> 12) == 0x0008) // (1000)b
        {
            coeff_token_bit_length = 4;
            TrailingOnes = 3;
            TotalCoeff = 7;
        } else if ((coeff_token >> 8) == 0x000F) // (0000 1111)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 0;
            TotalCoeff = 8;
        } else if ((coeff_token >> 9) == 0x000E) // (0001 110)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 1;
            TotalCoeff = 8;
        } else if ((coeff_token >> 9) == 0x000D) // (0001 101)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 8;
        } else if ((coeff_token >> 11) == 0x000D) // (0110 1)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 3;
            TotalCoeff = 8;
        } else if ((coeff_token >> 8) == 0x000B) // (0000 1011)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 0;
            TotalCoeff = 9;
        } else if ((coeff_token >> 8) == 0x000E) // (0000 1110)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 1;
            TotalCoeff = 9;
        } else if ((coeff_token >> 9) == 0x000A) // (0001 010)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 9;
        } else if ((coeff_token >> 10) == 0x000C) // (0011 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 9;
        } else if ((coeff_token >> 7) == 0x000F) // (0000 0111 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 10;
        } else if ((coeff_token >> 8) == 0x000A) // (0000 1010)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 1;
            TotalCoeff = 10;
        } else if ((coeff_token >> 8) == 0x000D) // (0000 1101)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 2;
            TotalCoeff = 10;
        } else if ((coeff_token >> 9) == 0x000C) // (0001 100)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 3;
            TotalCoeff = 10;
        } else if ((coeff_token >> 7) == 0x000B) // (0000 0101 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 11;
        } else if ((coeff_token >> 7) == 0x000E) // (0000 0111 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 1;
            TotalCoeff = 11;
        } else if ((coeff_token >> 8) == 0x0009) // (0000 1001)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 2;
            TotalCoeff = 11;
        } else if ((coeff_token >> 8) == 0x000C) // (0000 1100)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 3;
            TotalCoeff = 11;
        } else if ((coeff_token >> 7) == 0x0008) // (0000 0100 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 12;
        } else if ((coeff_token >> 7) == 0x000A) // (0000 0101 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 1;
            TotalCoeff = 12;
        } else if ((coeff_token >> 7) == 0x000D) // (0000 0110 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 2;
            TotalCoeff = 12;
        } else if ((coeff_token >> 8) == 0x0008) // (0000 1000)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 3;
            TotalCoeff = 12;
        } else if ((coeff_token >> 6) == 0x000D) // (0000 0011 01)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 0;
            TotalCoeff = 13;
        } else if ((coeff_token >> 7) == 0x0007) // (0000 0011 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 1;
            TotalCoeff = 13;
        } else if ((coeff_token >> 7) == 0x0009) // (0000 0100 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 2;
            TotalCoeff = 13;
        } else if ((coeff_token >> 7) == 0x000C) // (0000 0110 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 3;
            TotalCoeff = 13;
        } else if ((coeff_token >> 6) == 0x0009) // (0000 0010 01)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 0;
            TotalCoeff = 14;
        } else if ((coeff_token >> 6) == 0x000C) // (0000 0011 00)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 1;
            TotalCoeff = 14;
        } else if ((coeff_token >> 6) == 0x000B) // (0000 0010 11)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 2;
            TotalCoeff = 14;
        } else if ((coeff_token >> 6) == 0x000A) // (0000 0010 10)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 3;
            TotalCoeff = 14;
        } else if ((coeff_token >> 6) == 0x0005) // (0000 0001 01)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 0;
            TotalCoeff = 15;
        } else if ((coeff_token >> 6) == 0x0008) // (0000 0010 00)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 1;
            TotalCoeff = 15;
        } else if ((coeff_token >> 6) == 0x0007) // (0000 0001 11)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 2;
            TotalCoeff = 15;
        } else if ((coeff_token >> 6) == 0x0006) // (0000 0001 10)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 3;
            TotalCoeff = 15;
        } else if ((coeff_token >> 6) == 0x0001) // (0000 0000 01)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 0;
            TotalCoeff = 16;
        } else if ((coeff_token >> 6) == 0x0004) // (0000 0001 00)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 1;
            TotalCoeff = 16;
        } else if ((coeff_token >> 6) == 0x0003) // (0000 0000 11)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 2;
            TotalCoeff = 16;
        } else if ((coeff_token >> 6) == 0x0002) // (0000 0000 10)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 3;
            TotalCoeff = 16;
        } else {
            assert(false);
        }
    } else if (nC >= 8) {
        if ((coeff_token >> 10) == 0x0003) // (0000 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 0;
        } else if ((coeff_token >> 10) == 0x0000) // (0000 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 1;
        } else if ((coeff_token >> 10) == 0x0001) // (0000 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 1;
        } else if ((coeff_token >> 10) == 0x0004) // (0001 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0005) // (0001 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0006) // (0001 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0008) // (0010 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x0009) // (0010 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x000A) // (0010 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x000B) // (0010 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x000C) // (0011 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x000D) // (0011 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x000E) // (0011 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x000F) // (0011 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x0010) // (0100 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 5;
        } else if ((coeff_token >> 10) == 0x0011) // (0100 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 5;
        } else if ((coeff_token >> 10) == 0x0012) // (0100 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 5;
        } else if ((coeff_token >> 10) == 0x0013) // (0100 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 5;
        } else if ((coeff_token >> 10) == 0x0014) // (0101 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x0015) // (0101 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x0016) // (0101 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x0017) // (0101 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 6;
        } else if ((coeff_token >> 10) == 0x0018) // (0110 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x0019) // (0110 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x001A) // (0110 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x001B) // (0110 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 7;
        } else if ((coeff_token >> 10) == 0x001C) // (0111 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 8;
        } else if ((coeff_token >> 10) == 0x001D) // (0111 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 8;
        } else if ((coeff_token >> 10) == 0x001E) // (0111 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 8;
        } else if ((coeff_token >> 10) == 0x001F) // (0111 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 8;
        } else if ((coeff_token >> 10) == 0x0020) // (1000 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 9;
        } else if ((coeff_token >> 10) == 0x0021) // (1000 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 9;
        } else if ((coeff_token >> 10) == 0x0022) // (1000 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 9;
        } else if ((coeff_token >> 10) == 0x0023) // (1000 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 9;
        } else if ((coeff_token >> 10) == 0x0024) // (1001 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 10;
        } else if ((coeff_token >> 10) == 0x0025) // (1001 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 10;
        } else if ((coeff_token >> 10) == 0x0026) // (1001 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 10;
        } else if ((coeff_token >> 10) == 0x0027) // (1001 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 10;
        } else if ((coeff_token >> 10) == 0x0028) // (1010 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 11;
        } else if ((coeff_token >> 10) == 0x0029) // (1010 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 11;
        } else if ((coeff_token >> 10) == 0x002A) // (1010 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 11;
        } else if ((coeff_token >> 10) == 0x002B) // (1010 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 11;
        } else if ((coeff_token >> 10) == 0x002C) // (1011 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 12;
        } else if ((coeff_token >> 10) == 0x002D) // (1011 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 12;
        } else if ((coeff_token >> 10) == 0x002E) // (1011 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 12;
        } else if ((coeff_token >> 10) == 0x002F) // (1011 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 12;
        } else if ((coeff_token >> 10) == 0x0030) // (1100 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 13;
        } else if ((coeff_token >> 10) == 0x0031) // (1100 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 13;
        } else if ((coeff_token >> 10) == 0x0032) // (1100 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 13;
        } else if ((coeff_token >> 10) == 0x0033) // (1100 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 13;
        } else if ((coeff_token >> 10) == 0x0034) // (1101 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 14;
        } else if ((coeff_token >> 10) == 0x0035) // (1101 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 14;
        } else if ((coeff_token >> 10) == 0x0036) // (1101 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 14;
        } else if ((coeff_token >> 10) == 0x0037) // (1101 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 14;
        } else if ((coeff_token >> 10) == 0x0038) // (1110 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 15;
        } else if ((coeff_token >> 10) == 0x0039) // (1110 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 15;
        } else if ((coeff_token >> 10) == 0x003A) // (1110 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 15;
        } else if ((coeff_token >> 10) == 0x003B) // (1110 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 15;
        } else if ((coeff_token >> 10) == 0x003C) // (1111 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 16;
        } else if ((coeff_token >> 10) == 0x003D) // (1111 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 16;
        } else if ((coeff_token >> 10) == 0x003E) // (1111 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 2;
            TotalCoeff = 16;
        } else if ((coeff_token >> 10) == 0x003F) // (1111 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 16;
        } else {
            assert(false);
        }
    } else if (nC == -1) {
        if ((coeff_token >> 14) == 0x0001) // (01)b
        {
            coeff_token_bit_length = 2;
            TrailingOnes = 0;
            TotalCoeff = 0;
        } else if ((coeff_token >> 10) == 0x0007) // (0001 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 1;
        } else if ((coeff_token >> 15) == 0x0001) // (1)b
        {
            coeff_token_bit_length = 1;
            TrailingOnes = 1;
            TotalCoeff = 1;
        } else if ((coeff_token >> 10) == 0x0004) // (0001 00)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0006) // (0001 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 1;
            TotalCoeff = 2;
        } else if ((coeff_token >> 13) == 0x0001) // (001)b
        {
            coeff_token_bit_length = 3;
            TrailingOnes = 2;
            TotalCoeff = 2;
        } else if ((coeff_token >> 10) == 0x0003) // (0000 11)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 3;
        } else if ((coeff_token >> 9) == 0x0003) // (0000 011)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 1;
            TotalCoeff = 3;
        } else if ((coeff_token >> 9) == 0x0002) // (0000 010)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x0005) // (0001 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 3;
        } else if ((coeff_token >> 10) == 0x0002) // (0000 10)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 0;
            TotalCoeff = 4;
        } else if ((coeff_token >> 8) == 0x0003) // (0000 0011)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 1;
            TotalCoeff = 4;
        } else if ((coeff_token >> 8) == 0x0002) // (0000 0010)b
        {
            coeff_token_bit_length = 8;
            TrailingOnes = 2;
            TotalCoeff = 4;
        } else if ((coeff_token >> 9) == 0x0000) // (0000 000)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 3;
            TotalCoeff = 4;
        } else {
            assert(false);
        }
    } else if (nC == -2) {
        if ((coeff_token >> 15) == 0x0001) // (1)b
        {
            coeff_token_bit_length = 1;
            TrailingOnes = 0;
            TotalCoeff = 0;
        } else if ((coeff_token >> 9) == 0x000F) // (0001 111)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 1;
        } else if ((coeff_token >> 14) == 0x0001) // (01)b
        {
            coeff_token_bit_length = 2;
            TrailingOnes = 1;
            TotalCoeff = 1;
        } else if ((coeff_token >> 9) == 0x000E) // (0001 110)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 0;
            TotalCoeff = 2;
        } else if ((coeff_token >> 9) == 0x000D) // (0001 101)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 1;
            TotalCoeff = 2;
        } else if ((coeff_token >> 13) == 0x0001) // (001)b
        {
            coeff_token_bit_length = 3;
            TrailingOnes = 2;
            TotalCoeff = 2;
        } else if ((coeff_token >> 7) == 0x0007) // (0000 0011 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 3;
        } else if ((coeff_token >> 9) == 0x000C) // (0001 100)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 1;
            TotalCoeff = 3;
        } else if ((coeff_token >> 9) == 0x000B) // (0001 011)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 3;
        } else if ((coeff_token >> 11) == 0x0001) // (0000 1)b
        {
            coeff_token_bit_length = 5;
            TrailingOnes = 3;
            TotalCoeff = 3;
        } else if ((coeff_token >> 7) == 0x0006) // (0000 0011 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 0;
            TotalCoeff = 4;
        } else if ((coeff_token >> 7) == 0x0005) // (0000 0010 1)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 1;
            TotalCoeff = 4;
        } else if ((coeff_token >> 9) == 0x000A) // (0001 010)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 2;
            TotalCoeff = 4;
        } else if ((coeff_token >> 10) == 0x0001) // (0000 01)b
        {
            coeff_token_bit_length = 6;
            TrailingOnes = 3;
            TotalCoeff = 4;
        } else if ((coeff_token >> 6) == 0x0007) // (0000 0001 11)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 0;
            TotalCoeff = 5;
        } else if ((coeff_token >> 6) == 0x0006) // (0000 0001 10)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 1;
            TotalCoeff = 5;
        } else if ((coeff_token >> 7) == 0x0004) // (0000 0010 0)b
        {
            coeff_token_bit_length = 9;
            TrailingOnes = 2;
            TotalCoeff = 5;
        } else if ((coeff_token >> 9) == 0x0009) // (0001 001)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 3;
            TotalCoeff = 5;
        } else if ((coeff_token >> 5) == 0x0007) // (0000 0000 111)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 0;
            TotalCoeff = 6;
        } else if ((coeff_token >> 5) == 0x0006) // (0000 0000 110)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 1;
            TotalCoeff = 6;
        } else if ((coeff_token >> 6) == 0x0005) // (0000 0001 01)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 2;
            TotalCoeff = 6;
        } else if ((coeff_token >> 9) == 0x0008) // (0001 000)b
        {
            coeff_token_bit_length = 7;
            TrailingOnes = 3;
            TotalCoeff = 6;
        } else if ((coeff_token >> 4) == 0x0007) // (0000 0000 0111)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 0;
            TotalCoeff = 7;
        } else if ((coeff_token >> 4) == 0x0006) // (0000 0000 0110)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 1;
            TotalCoeff = 7;
        } else if ((coeff_token >> 5) == 0x0005) // (0000 0000 101)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 2;
            TotalCoeff = 7;
        } else if ((coeff_token >> 6) == 0x0004) // (0000 0001 00)b
        {
            coeff_token_bit_length = 10;
            TrailingOnes = 3;
            TotalCoeff = 7;
        } else if ((coeff_token >> 3) == 0x0007) // (0000 0000 0011 1)b
        {
            coeff_token_bit_length = 13;
            TrailingOnes = 0;
            TotalCoeff = 8;
        } else if ((coeff_token >> 4) == 0x0005) // (0000 0000 0101)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 1;
            TotalCoeff = 8;
        } else if ((coeff_token >> 4) == 0x0004) // (0000 0000 0100)b
        {
            coeff_token_bit_length = 12;
            TrailingOnes = 2;
            TotalCoeff = 8;
        } else if ((coeff_token >> 5) == 0x0004) // (0000 0000 100)b
        {
            coeff_token_bit_length = 11;
            TrailingOnes = 3;
            TotalCoeff = 8;
        } else {
            assert(false);
        }
    } else {
        assert(false);
    }

    return 0;
}

int get_total_zeros(
    std::shared_ptr<NalUnit::RbspData> rbsp_data,
    int maxNumCoeff,
    int tzVlcIndex,
    int& total_zeros)
{
    if (maxNumCoeff == 4) // If maxNumCoeff is equal to 4, one of the VLCs specified in Table 9-9 (a) is used.
    {
        int token = 0;
        int token2 = 0;
        int token_length = 0;

        // Table 9-9 – total_zeros tables for chroma DC 2x2 and 2x4 blocks
        //(a) Chroma DC 2x2 block (4:2:0 chroma sampling)
        if (tzVlcIndex == 1) {
            token = rbsp_data->peek_u(3);
            token_length = 0;
            if ((token >> 2) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 1) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 0) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 0) == 0x00) // (000)b
            {
                token_length = 3;
                total_zeros = 3;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 2) {
            token = rbsp_data->peek_u(2);
            token_length = 0;
            if ((token >> 1) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 0) == 0x00) // (00)b
            {
                token_length = 2;
                total_zeros = 2;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 3) {
            token = rbsp_data->peek_u(1);
            token_length = 0;
            if ((token >> 0) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 0) == 0x00) // (0)b
            {
                token_length = 1;
                total_zeros = 1;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else {
            assert(false);
        }
    } else if (maxNumCoeff == 8) // Otherwise, if maxNumCoeff is equal to 8, one of the VLCs specified in Table 9-9 (b) is used.
    {
        int token = 0;
        int token2 = 0;
        int token_length = 0;

        // Table 9-9 – total_zeros tables for chroma DC 2x2 and 2x4 blocks
        //(b) Chroma DC 2x4 block (4:2:2 chroma sampling)
        if (tzVlcIndex == 1) {
            token = rbsp_data->peek_u(5);
            token_length = 0;
            if ((token >> 4) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 2) == 0x02) // (010)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 2) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 1) == 0x02) // (0010)b
            {
                token_length = 4;
                total_zeros = 3;
            } else if ((token >> 1) == 0x03) // (0011)b
            {
                token_length = 4;
                total_zeros = 4;
            } else if ((token >> 1) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 5;
            } else if ((token >> 0) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 6;
            } else if ((token >> 0) == 0x00) // (0000 0)b
            {
                token_length = 5;
                total_zeros = 7;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 2) {
            token = rbsp_data->peek_u(3);
            token_length = 0;
            if ((token >> 0) == 0x00) // (000)b
            {
                token_length = 3;
                total_zeros = 0;
            } else if ((token >> 1) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 0) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 0) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 0) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 0) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 5;
            } else if ((token >> 0) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 6;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 3) {
            token = rbsp_data->peek_u(3);
            token_length = 0;
            if ((token >> 0) == 0x00) // (000)b
            {
                token_length = 3;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 1) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 2;
            } else if ((token >> 1) == 0x02) // (10)b
            {
                token_length = 2;
                total_zeros = 3;
            } else if ((token >> 0) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 0) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 5;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 4) {
            token = rbsp_data->peek_u(3);
            token_length = 0;
            if ((token >> 0) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 0;
            } else if ((token >> 1) == 0x00) // (00)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 1) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 2;
            } else if ((token >> 1) == 0x02) // (10)b
            {
                token_length = 2;
                total_zeros = 3;
            } else if ((token >> 0) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 4;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 5) {
            token = rbsp_data->peek_u(2);
            token_length = 0;
            if ((token >> 0) == 0x00) // (00)b
            {
                token_length = 2;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 0) == 0x02) // (10)b
            {
                token_length = 2;
                total_zeros = 2;
            } else if ((token >> 0) == 0x03) // (11)b
            {
                token_length = 2;
                total_zeros = 3;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 6) {
            token = rbsp_data->peek_u(2);
            token_length = 0;
            if ((token >> 0) == 0x00) // (00)b
            {
                token_length = 2;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 1) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 2;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 7) {
            token = rbsp_data->peek_u(1);
            token_length = 0;
            if ((token >> 0) == 0x00) // (0)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 1;
            } else {
                assert(false);
            }
            token2 = rbsp_data->read_u(token_length);
        } else {
            assert(false);
        }
    } else // Otherwise (maxNumCoeff is not equal to 4 and not equal to 8), VLCs from Tables 9-7 and 9-8 are used.
    {
        int token = 0;
        int token2 = 0;
        int token_length = 0;

        // Table 9-7 – total_zeros tables for 4x4 blocks with tzVlcIndex 1 to 7
        if (tzVlcIndex == 1) {
            token = rbsp_data->peek_u(9);
            token_length = 0;
            if ((token >> 8) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 6) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 6) == 0x02) // (010)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 5) == 0x03) // (0011)b
            {
                token_length = 4;
                total_zeros = 3;
            } else if ((token >> 5) == 0x02) // (0010)b
            {
                token_length = 4;
                total_zeros = 4;
            } else if ((token >> 4) == 0x03) // (0001 1)b
            {
                token_length = 5;
                total_zeros = 5;
            } else if ((token >> 4) == 0x02) // (0001 0)b
            {
                token_length = 5;
                total_zeros = 6;
            } else if ((token >> 3) == 0x03) // (0000 11)b
            {
                token_length = 6;
                total_zeros = 7;
            } else if ((token >> 3) == 0x02) // (0000 10)b
            {
                token_length = 6;
                total_zeros = 8;
            } else if ((token >> 2) == 0x03) // (0000 011)b
            {
                token_length = 7;
                total_zeros = 9;
            } else if ((token >> 2) == 0x02) // (0000 010)b
            {
                token_length = 7;
                total_zeros = 10;
            } else if ((token >> 1) == 0x03) // (0000 0011)b
            {
                token_length = 8;
                total_zeros = 11;
            } else if ((token >> 1) == 0x02) // (0000 0010)b
            {
                token_length = 8;
                total_zeros = 12;
            } else if ((token >> 0) == 0x03) // (0000 0001 1)b
            {
                token_length = 9;
                total_zeros = 13;
            } else if ((token >> 0) == 0x02) // (0000 0001 0)b
            {
                token_length = 9;
                total_zeros = 14;
            } else if ((token >> 0) == 0x01) // (0000 0000 1)b
            {
                token_length = 9;
                total_zeros = 15;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 2) {
            token = rbsp_data->peek_u(6);
            token_length = 0;
            if ((token >> 3) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 0;
            } else if ((token >> 3) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 3) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 3) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 3) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 2) == 0x05) // (0101)b
            {
                token_length = 4;
                total_zeros = 5;
            } else if ((token >> 2) == 0x04) // (0100)b
            {
                token_length = 4;
                total_zeros = 6;
            } else if ((token >> 2) == 0x03) // (0011)b
            {
                token_length = 4;
                total_zeros = 7;
            } else if ((token >> 2) == 0x02) // (0010)b
            {
                token_length = 4;
                total_zeros = 8;
            } else if ((token >> 1) == 0x03) // (0001 1)b
            {
                token_length = 5;
                total_zeros = 9;
            } else if ((token >> 1) == 0x02) // (0001 0)b
            {
                token_length = 5;
                total_zeros = 10;
            } else if ((token >> 0) == 0x03) // (0000 11)b
            {
                token_length = 6;
                total_zeros = 11;
            } else if ((token >> 0) == 0x02) // (0000 10)b
            {
                token_length = 6;
                total_zeros = 12;
            } else if ((token >> 0) == 0x01) // (0000 01)b
            {
                token_length = 6;
                total_zeros = 13;
            } else if ((token >> 0) == 0x00) // (0000 00)b
            {
                token_length = 6;
                total_zeros = 14;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 3) {
            token = rbsp_data->peek_u(6);
            token_length = 0;
            if ((token >> 2) == 0x05) // (0101)b
            {
                token_length = 4;
                total_zeros = 0;
            } else if ((token >> 3) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 3) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 3) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 2) == 0x04) // (0100)b
            {
                token_length = 4;
                total_zeros = 4;
            } else if ((token >> 2) == 0x03) // (0011)b
            {
                token_length = 4;
                total_zeros = 5;
            } else if ((token >> 3) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 6;
            } else if ((token >> 3) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 7;
            } else if ((token >> 2) == 0x02) // (0010)b
            {
                token_length = 4;
                total_zeros = 8;
            } else if ((token >> 1) == 0x03) // (0001 1)b
            {
                token_length = 5;
                total_zeros = 9;
            } else if ((token >> 1) == 0x02) // (0001 0)b
            {
                token_length = 5;
                total_zeros = 10;
            } else if ((token >> 0) == 0x01) // (0000 01)b
            {
                token_length = 6;
                total_zeros = 11;
            } else if ((token >> 1) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 12;
            } else if ((token >> 0) == 0x00) // (0000 00)b
            {
                token_length = 6;
                total_zeros = 13;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 4) {
            token = rbsp_data->peek_u(5);
            token_length = 0;
            if ((token >> 0) == 0x03) // (0001 1)b
            {
                token_length = 5;
                total_zeros = 0;
            } else if ((token >> 2) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 1) == 0x05) // (0101)b
            {
                token_length = 4;
                total_zeros = 2;
            } else if ((token >> 1) == 0x04) // (0100)b
            {
                token_length = 4;
                total_zeros = 3;
            } else if ((token >> 2) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 2) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 5;
            } else if ((token >> 2) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 6;
            } else if ((token >> 1) == 0x03) // (0011)b
            {
                token_length = 4;
                total_zeros = 7;
            } else if ((token >> 2) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 8;
            } else if ((token >> 1) == 0x02) // (0010)b
            {
                token_length = 4;
                total_zeros = 9;
            } else if ((token >> 0) == 0x02) // (0001 0)b
            {
                token_length = 5;
                total_zeros = 10;
            } else if ((token >> 0) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 11;
            } else if ((token >> 0) == 0x00) // (0000 0)b
            {
                token_length = 5;
                total_zeros = 12;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 5) {
            token = rbsp_data->peek_u(5);
            token_length = 0;
            if ((token >> 1) == 0x05) // (0101)b
            {
                token_length = 4;
                total_zeros = 0;
            } else if ((token >> 1) == 0x04) // (0100)b
            {
                token_length = 4;
                total_zeros = 1;
            } else if ((token >> 1) == 0x03) // (0011)b
            {
                token_length = 4;
                total_zeros = 2;
            } else if ((token >> 2) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 2) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 2) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 5;
            } else if ((token >> 2) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 6;
            } else if ((token >> 2) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 7;
            } else if ((token >> 1) == 0x02) // (0010)b
            {
                token_length = 4;
                total_zeros = 8;
            } else if ((token >> 0) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 9;
            } else if ((token >> 1) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 10;
            } else if ((token >> 0) == 0x00) // (0000 0)b
            {
                token_length = 5;
                total_zeros = 11;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 6) {
            token = rbsp_data->peek_u(6);
            token_length = 0;
            if ((token >> 0) == 0x01) // (0000 01)b
            {
                token_length = 6;
                total_zeros = 0;
            } else if ((token >> 1) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 1;
            } else if ((token >> 3) == 0x07) // (111)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 3) == 0x06) // (110)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 3) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 3) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 5;
            } else if ((token >> 3) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 6;
            } else if ((token >> 3) == 0x02) // (010)b
            {
                token_length = 3;
                total_zeros = 7;
            } else if ((token >> 2) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 8;
            } else if ((token >> 3) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 9;
            } else if ((token >> 0) == 0x00) // (0000 00)b
            {
                token_length = 6;
                total_zeros = 10;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 7) {
            token = rbsp_data->peek_u(6);
            token_length = 0;
            if ((token >> 0) == 0x01) // (0000 01)b
            {
                token_length = 6;
                total_zeros = 0;
            } else if ((token >> 1) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 1;
            } else if ((token >> 3) == 0x05) // (101)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 3) == 0x04) // (100)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 3) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 4;
            } else if ((token >> 4) == 0x03) // (11)b
            {
                token_length = 2;
                total_zeros = 5;
            } else if ((token >> 3) == 0x02) // (010)b
            {
                token_length = 3;
                total_zeros = 6;
            } else if ((token >> 2) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 7;
            } else if ((token >> 3) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 8;
            } else if ((token >> 0) == 0x00) // (0000 00)b
            {
                token_length = 6;
                total_zeros = 9;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 8) {
            token = rbsp_data->peek_u(6);
            token_length = 0;
            if ((token >> 0) == 0x01) // (0000 01)b
            {
                token_length = 6;
                total_zeros = 0;
            } else if ((token >> 2) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 1;
            } else if ((token >> 1) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 2;
            } else if ((token >> 3) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 4) == 0x03) // (11)b
            {
                token_length = 2;
                total_zeros = 4;
            } else if ((token >> 4) == 0x02) // (10)b
            {
                token_length = 2;
                total_zeros = 5;
            } else if ((token >> 3) == 0x02) // (010)b
            {
                token_length = 3;
                total_zeros = 6;
            } else if ((token >> 3) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 7;
            } else if ((token >> 0) == 0x00) // (0000 00)b
            {
                token_length = 6;
                total_zeros = 8;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 9) {
            token = rbsp_data->peek_u(6);
            token_length = 0;
            if ((token >> 0) == 0x01) // (0000 01)b
            {
                token_length = 6;
                total_zeros = 0;
            } else if ((token >> 0) == 0x00) // (0000 00)b
            {
                token_length = 6;
                total_zeros = 1;
            } else if ((token >> 2) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 2;
            } else if ((token >> 4) == 0x03) // (11)b
            {
                token_length = 2;
                total_zeros = 3;
            } else if ((token >> 4) == 0x02) // (10)b
            {
                token_length = 2;
                total_zeros = 4;
            } else if ((token >> 3) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 5;
            } else if ((token >> 4) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 6;
            } else if ((token >> 1) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 7;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 10) {
            token = rbsp_data->peek_u(5);
            token_length = 0;
            if ((token >> 0) == 0x01) // (0000 1)b
            {
                token_length = 5;
                total_zeros = 0;
            } else if ((token >> 0) == 0x00) // (0000 0)b
            {
                token_length = 5;
                total_zeros = 1;
            } else if ((token >> 2) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 3) == 0x03) // (11)b
            {
                token_length = 2;
                total_zeros = 3;
            } else if ((token >> 3) == 0x02) // (10)b
            {
                token_length = 2;
                total_zeros = 4;
            } else if ((token >> 3) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 5;
            } else if ((token >> 1) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 6;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 11) {
            token = rbsp_data->peek_u(4);
            token_length = 0;
            if ((token >> 0) == 0x00) // (0000)b
            {
                token_length = 4;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 1;
            } else if ((token >> 1) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 2;
            } else if ((token >> 1) == 0x02) // (010)b
            {
                token_length = 3;
                total_zeros = 3;
            } else if ((token >> 3) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 4;
            } else if ((token >> 1) == 0x03) // (011)b
            {
                token_length = 3;
                total_zeros = 5;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 12) {
            token = rbsp_data->peek_u(4);
            token_length = 0;
            if ((token >> 0) == 0x00) // (0000)b
            {
                token_length = 4;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (0001)b
            {
                token_length = 4;
                total_zeros = 1;
            } else if ((token >> 2) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 2;
            } else if ((token >> 3) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 3;
            } else if ((token >> 1) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 4;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 13) {
            token = rbsp_data->peek_u(4);
            token_length = 0;
            if ((token >> 1) == 0x00) // (000)b
            {
                token_length = 3;
                total_zeros = 0;
            } else if ((token >> 1) == 0x01) // (001)b
            {
                token_length = 3;
                total_zeros = 1;
            } else if ((token >> 3) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 2;
            } else if ((token >> 2) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 3;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 14) {
            token = rbsp_data->peek_u(2);
            token_length = 0;
            if ((token >> 0) == 0x00) // (00)b
            {
                token_length = 2;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (01)b
            {
                token_length = 2;
                total_zeros = 1;
            } else if ((token >> 1) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 2;
            }
            token2 = rbsp_data->read_u(token_length);
        } else if (tzVlcIndex == 15) {
            token = rbsp_data->peek_u(1);
            token_length = 0;
            if ((token >> 0) == 0x00) // (0)b
            {
                token_length = 1;
                total_zeros = 0;
            } else if ((token >> 0) == 0x01) // (1)b
            {
                token_length = 1;
                total_zeros = 1;
            }
            token2 = rbsp_data->read_u(token_length);
        }
    }

    return 0;
}

int get_run_before(
    std::shared_ptr<NalUnit::RbspData> rbsp_data,
    int zerosLeft,
    int& run_before)
{
    int token = 0;
    int token2 = 0;
    int token_length = 0;

    if (zerosLeft == 1) {
        token = rbsp_data->peek_u(1);
        token_length = 0;
        if ((token >> 0) == 0x01) // (1)b
        {
            token_length = 1;
            run_before = 0;
        } else if ((token >> 0) == 0x00) // (0)b
        {
            token_length = 1;
            run_before = 1;
        }
        token2 = rbsp_data->read_u(token_length);
    } else if (zerosLeft == 2) {
        token = rbsp_data->peek_u(2);
        token_length = 0;
        if ((token >> 1) == 0x01) // (1)b
        {
            token_length = 1;
            run_before = 0;
        } else if ((token >> 0) == 0x01) // (01)b
        {
            token_length = 2;
            run_before = 1;
        } else if ((token >> 0) == 0x00) // (00)b
        {
            token_length = 2;
            run_before = 2;
        }
        token2 = rbsp_data->read_u(token_length);
    } else if (zerosLeft == 3) {
        token = rbsp_data->peek_u(2);
        token_length = 0;
        if ((token >> 0) == 0x03) // (11)b
        {
            token_length = 2;
            run_before = 0;
        } else if ((token >> 0) == 0x02) // (10)b
        {
            token_length = 2;
            run_before = 1;
        } else if ((token >> 0) == 0x01) // (01)b
        {
            token_length = 2;
            run_before = 2;
        } else if ((token >> 0) == 0x00) // (00)b
        {
            token_length = 2;
            run_before = 3;
        }
        token2 = rbsp_data->read_u(token_length);
    } else if (zerosLeft == 4) {
        token = rbsp_data->peek_u(3);
        token_length = 0;
        if ((token >> 1) == 0x03) // (11)b
        {
            token_length = 2;
            run_before = 0;
        } else if ((token >> 1) == 0x02) // (10)b
        {
            token_length = 2;
            run_before = 1;
        } else if ((token >> 1) == 0x01) // (01)b
        {
            token_length = 2;
            run_before = 2;
        } else if ((token >> 0) == 0x01) // (001)b
        {
            token_length = 3;
            run_before = 3;
        } else if ((token >> 0) == 0x00) // (000)b
        {
            token_length = 3;
            run_before = 4;
        }
        token2 = rbsp_data->read_u(token_length);
    } else if (zerosLeft == 5) {
        token = rbsp_data->peek_u(3);
        token_length = 0;
        if ((token >> 1) == 0x03) // (11)b
        {
            token_length = 2;
            run_before = 0;
        } else if ((token >> 1) == 0x02) // (10)b
        {
            token_length = 2;
            run_before = 1;
        } else if ((token >> 0) == 0x03) // (011)b
        {
            token_length = 3;
            run_before = 2;
        } else if ((token >> 0) == 0x02) // (010)b
        {
            token_length = 3;
            run_before = 3;
        } else if ((token >> 0) == 0x01) // (001)b
        {
            token_length = 3;
            run_before = 4;
        } else if ((token >> 0) == 0x00) // (000)b
        {
            token_length = 3;
            run_before = 5;
        }
        token2 = rbsp_data->read_u(token_length);
    } else if (zerosLeft == 6) {
        token = rbsp_data->peek_u(3);
        token_length = 0;
        if ((token >> 1) == 0x03) // (11)b
        {
            token_length = 2;
            run_before = 0;
        } else if ((token >> 0) == 0x00) // (000)b
        {
            token_length = 3;
            run_before = 1;
        } else if ((token >> 0) == 0x01) // (001)b
        {
            token_length = 3;
            run_before = 2;
        } else if ((token >> 0) == 0x03) // (011)b
        {
            token_length = 3;
            run_before = 3;
        } else if ((token >> 0) == 0x02) // (010)b
        {
            token_length = 3;
            run_before = 4;
        } else if ((token >> 0) == 0x05) // (101)b
        {
            token_length = 3;
            run_before = 5;
        } else if ((token >> 0) == 0x04) // (100)b
        {
            token_length = 3;
            run_before = 6;
        }
        token2 = rbsp_data->read_u(token_length);
    } else if (zerosLeft > 6) {
        token = rbsp_data->peek_u(11);
        token_length = 0;
        if ((token >> 8) == 0x07) // (111)b
        {
            token_length = 3;
            run_before = 0;
        } else if ((token >> 8) == 0x06) // (110)b
        {
            token_length = 3;
            run_before = 1;
        } else if ((token >> 8) == 0x05) // (101)b
        {
            token_length = 3;
            run_before = 2;
        } else if ((token >> 8) == 0x04) // (100)b
        {
            token_length = 3;
            run_before = 3;
        } else if ((token >> 8) == 0x03) // (011)b
        {
            token_length = 3;
            run_before = 4;
        } else if ((token >> 8) == 0x02) // (010)b
        {
            token_length = 3;
            run_before = 5;
        } else if ((token >> 8) == 0x01) // (001)b
        {
            token_length = 3;
            run_before = 6;
        } else if ((token >> 7) == 0x01) // (0001)b
        {
            token_length = 4;
            run_before = 7;
        } else if ((token >> 6) == 0x01) // (00001)b
        {
            token_length = 5;
            run_before = 8;
        } else if ((token >> 5) == 0x01) // (000001)b
        {
            token_length = 6;
            run_before = 9;
        } else if ((token >> 4) == 0x01) // (0000001)b
        {
            token_length = 7;
            run_before = 10;
        } else if ((token >> 3) == 0x01) // (00000001)b
        {
            token_length = 8;
            run_before = 11;
        } else if ((token >> 2) == 0x01) // (000000001)b
        {
            token_length = 9;
            run_before = 12;
        } else if ((token >> 1) == 0x01) // (0000000001)b
        {
            token_length = 10;
            run_before = 13;
        } else if ((token >> 0) == 0x01) // (00000000001)b
        {
            token_length = 11;
            run_before = 14;
        }
        token2 = rbsp_data->read_u(token_length);
    }

    return 0;
}
