//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Base\Core\error.h"
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include "MeshConstructor.h"

namespace Caustic
{
    //
    //        5--------5--------6
    //       /|                /|
    //      9 4              10 |
    //     /  |              /  6
    //    1----------1------2   |
    //    |   |             |   |
    //    |   4-------7-----|---7
    //    0  /              2  /
    //    | 8               | 11
    //    |/                |/
    //    0---------3-------3
    //
    // The following table is from GPU Gems III: Generating Complex Procedural Terrains Using the GPU
    // by Ryan Geiss of NVidia
    static int edgeTable[256][16] = {
        /* 00000000 */ { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000001 */ { 1, 0,  8,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000010 */ { 1, 0,  1,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000011 */ { 2, 1,  8,  3,  9,  8,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000100 */ { 1, 1,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000101 */ { 2, 0,  8,  3,  1,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000110 */ { 2, 9,  2, 10,  0,  2,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00000111 */ { 3, 2,  8,  3,  2, 10,  8, 10,  9,  8, -1, -1, -1, -1, -1, -1 },
        /* 00001000 */ { 1, 3, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00001001 */ { 2, 0, 11,  2,  8, 11,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00001010 */ { 2, 1,  9,  0,  2,  3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00001011 */ { 3, 1, 11,  2,  1,  9, 11,  9,  8, 11, -1, -1, -1, -1, -1, -1 },
        /* 00001100 */ { 2, 3, 10,  1, 11, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00001101 */ { 3, 0, 10,  1,  0,  8, 10,  8, 11, 10, -1, -1, -1, -1, -1, -1 },
        /* 00001110 */ { 3, 3,  9,  0,  3, 11,  9, 11, 10,  9, -1, -1, -1, -1, -1, -1 },
        /* 00001111 */ { 2, 9,  8, 10, 10,  8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00010000 */ { 1, 4,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00010001 */ { 2, 4,  3,  0,  7,  3,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00010010 */ { 2, 0,  1,  9,  8,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00010011 */ { 3, 4,  1,  9,  4,  7,  1,  7,  3,  1, -1, -1, -1, -1, -1, -1 },
        /* 00010100 */ { 2, 1,  2, 10,  8,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00010101 */ { 3, 3,  4,  7,  3,  0,  4,  1,  2, 10, -1, -1, -1, -1, -1, -1 },
        /* 00010110 */ { 3, 9,  2, 10,  9,  0,  2,  8,  4,  7, -1, -1, -1, -1, -1, -1 },
        /* 00010111 */ { 4, 2, 10,  9,  2,  9,  7,  2,  7,  3,  7,  9,  4, -1, -1, -1 },
        /* 00011000 */ { 2, 8,  4,  7,  3, 11,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00011001 */ { 3, 11,  4,  7, 11,  2,  4,  2,  0,  4, -1, -1, -1, -1, -1, -1 },
        /* 00011010 */ { 3, 9,  0,  1,  8,  4,  7,  2,  3, 11, -1, -1, -1, -1, -1, -1 },
        /* 00011011 */ { 4, 4,  7, 11,  9,  4, 11,  9, 11,  2,  9,  2,  1, -1, -1, -1 },
        /* 00011100 */ { 3, 3, 10,  1,  3, 11, 10,  7,  8,  4, -1, -1, -1, -1, -1, -1 },
        /* 00011101 */ { 4, 1, 11, 10,  1,  4, 11,  1,  0,  4,  7, 11,  4, -1, -1, -1 },
        /* 00011110 */ { 4, 4,  7,  8,  9,  0, 11,  9, 11, 10, 11,  0,  3, -1, -1, -1 },
        /* 00011111 */ { 3, 4,  7, 11,  4, 11,  9,  9, 11, 10, -1, -1, -1, -1, -1, -1 },
        /* 00100000 */ { 1, 9,  5,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00100001 */ { 2, 9,  5,  4,  0,  8,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00100010 */ { 2, 0,  5,  4,  1,  5,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00100011 */ { 3, 8,  5,  4,  8,  3,  5,  3,  1,  5, -1, -1, -1, -1, -1, -1 },
        /* 00100100 */ { 2, 1,  2, 10,  9,  5,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00100101 */ { 3, 3,  0,  8,  1,  2, 10,  4,  9,  5, -1, -1, -1, -1, -1, -1 },
        /* 00100110 */ { 3, 5,  2, 10,  5,  4,  2,  4,  0,  2, -1, -1, -1, -1, -1, -1 },
        /* 00100111 */ { 4, 2, 10,  5,  3,  2,  5,  3,  5,  4,  3,  4,  8, -1, -1, -1 },
        /* 00101000 */ { 2, 9,  5,  4,  2,  3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00101001 */ { 3, 0, 11,  2,  0,  8, 11,  4,  9,  5, -1, -1, -1, -1, -1, -1 },
        /* 00101010 */ { 3, 0,  5,  4,  0,  1,  5,  2,  3, 11, -1, -1, -1, -1, -1, -1 },
        /* 00101011 */ { 4, 2,  1,  5,  2,  5,  8,  2,  8, 11,  4,  8,  5, -1, -1, -1 },
        /* 00101100 */ { 3, 10,  3, 11, 10,  1,  3,  9,  5,  4, -1, -1, -1, -1, -1, -1 },
        /* 00101101 */ { 4, 4,  9,  5,  0,  8,  1,  8, 10,  1,  8, 11, 10, -1, -1, -1 },
        /* 00101110 */ { 4, 5,  4,  0,  5,  0, 11,  5, 11, 10, 11,  0,  3, -1, -1, -1 },
        /* 00101111 */ { 3, 5,  4,  8,  5,  8, 10, 10,  8, 11, -1, -1, -1, -1, -1, -1 },
        /* 00110000 */ { 2, 9,  7,  8,  5,  7,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00110001 */ { 3, 9,  3,  0,  9,  5,  3,  5,  7,  3, -1, -1, -1, -1, -1, -1 },
        /* 00110010 */ { 3, 0,  7,  8,  0,  1,  7,  1,  5,  7, -1, -1, -1, -1, -1, -1 },
        /* 00110011 */ { 2, 1,  5,  3,  3,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 00110100 */ { 3, 9,  7,  8,  9,  5,  7, 10,  1,  2, -1, -1, -1, -1, -1, -1 },
        /* 00110101 */ { 4, 10,  1,  2,  9,  5,  0,  5,  3,  0,  5,  7,  3, -1, -1, -1 },
        /* 00110110 */ { 4, 8,  0,  2,  8,  2,  5,  8,  5,  7, 10,  5,  2, -1, -1, -1 },
        /* 00110111 */ { 3, 2, 10,  5,  2,  5,  3,  3,  5,  7, -1, -1, -1, -1, -1, -1 },
        /* 00111000 */ { 3, 7,  9,  5,  7,  8,  9,  3, 11,  2, -1, -1, -1, -1, -1, -1 },
        /* 00111001 */ { 4, 9,  5,  7,  9,  7,  2,  9,  2,  0,  2,  7, 11, -1, -1, -1 },
        /* 00111010 */ { 4, 2,  3, 11,  0,  1,  8,  1,  7,  8,  1,  5,  7, -1, -1, -1 },
        /* 00111011 */ { 3, 11,  2,  1, 11,  1,  7,  7,  1,  5, -1, -1, -1, -1, -1, -1 },
        /* 00111100 */ { 4, 9,  5,  8,  8,  5,  7, 10,  1,  3, 10,  3, 11, -1, -1, -1 },
        /* 00111101 */ { 5, 5,  7,  0,  5,  0,  9,  7, 11,  0,  1,  0, 10, 11, 10,  0 },
        /* 00111110 */ { 5, 11, 10,  0, 11,  0,  3, 10,  5,  0,  8,  0,  7,  5,  7,  0 },
        /* 00111111 */ { 2, 11, 10,  5,  7, 11,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01000000 */ { 1, 10,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01000001 */ { 2, 0,  8,  3,  5, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01000010 */ { 2, 9,  0,  1,  5, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01000011 */ { 3, 1,  8,  3,  1,  9,  8,  5, 10,  6, -1, -1, -1, -1, -1, -1 },
        /* 01000100 */ { 2, 1,  6,  5,  2,  6,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01000101 */ { 3, 1,  6,  5,  1,  2,  6,  3,  0,  8, -1, -1, -1, -1, -1, -1 },
        /* 01000110 */ { 3, 9,  6,  5,  9,  0,  6,  0,  2,  6, -1, -1, -1, -1, -1, -1 },
        /* 01000111 */ { 4, 5,  9,  8,  5,  8,  2,  5,  2,  6,  3,  2,  8, -1, -1, -1 },
        /* 01001000 */ { 2, 2,  3, 11, 10,  6,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01001001 */ { 3,11,  0,  8, 11,  2,  0, 10,  6,  5, -1, -1, -1, -1, -1, -1 },
        /* 01001010 */ { 3, 0,  1,  9,  2,  3, 11,  5, 10,  6, -1, -1, -1, -1, -1, -1 },
        /* 01001011 */ { 4, 5, 10,  6,  1,  9,  2,  9, 11,  2,  9,  8, 11, -1, -1, -1 },
        /* 01001100 */ { 3, 6,  3, 11,  6,  5,  3,  5,  1,  3, -1, -1, -1, -1, -1, -1 },
        /* 01001101 */ { 4, 0,  8, 11,  0, 11,  5,  0,  5,  1,  5, 11,  6, -1, -1, -1 },
        /* 01001110 */ { 4, 3, 11,  6,  0,  3,  6,  0,  6,  5,  0,  5,  9, -1, -1, -1 },
        /* 01001111 */ { 3, 6,  5,  9,  6,  9, 11, 11,  9,  8, -1, -1, -1, -1, -1, -1 },
        /* 01010000 */ { 2, 5, 10,  6,  4,  7,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01010001 */ { 3, 4,  3,  0,  4,  7,  3,  6,  5, 10, -1, -1, -1, -1, -1, -1 },
        /* 01010010 */ { 3, 1,  9,  0,  5, 10,  6,  8,  4,  7, -1, -1, -1, -1, -1, -1 },
        /* 01010011 */ { 4,10,  6,  5,  1,  9,  7,  1,  7,  3,  7,  9,  4, -1, -1, -1 },
        /* 01010100 */ { 3, 6,  1,  2,  6,  5,  1,  4,  7,  8, -1, -1, -1, -1, -1, -1 },
        /* 01010101 */ { 4, 1,  2,  5,  5,  2,  6,  3,  0,  4,  3,  4,  7, -1, -1, -1 },
        /* 01010110 */ { 4, 8,  4,  7,  9,  0,  5,  0,  6,  5,  0,  2,  6, -1, -1, -1 },
        /* 01010111 */ { 5, 7,  3,  9,  7,  9,  4,  3,  2,  9,  5,  9,  6,  2,  6,  9 },
        /* 01011000 */ { 3, 3, 11,  2,  7,  8,  4, 10,  6,  5, -1, -1, -1, -1, -1, -1 },
        /* 01011001 */ { 4, 5, 10,  6,  4,  7,  2,  4,  2,  0,  2,  7, 11, -1, -1, -1 },
        /* 01011010 */ { 4, 0,  1,  9,  4,  7,  8,  2,  3, 11,  5, 10,  6, -1, -1, -1 },
        /* 01011011 */ { 5, 9,  2,  1,  9, 11,  2,  9,  4, 11,  7, 11,  4,  5, 10,  6 },
        /* 01011100 */ { 4, 8,  4,  7,  3, 11,  5,  3,  5,  1,  5, 11,  6, -1, -1, -1 },
        /* 01011101 */ { 5, 5,  1, 11,  5, 11,  6,  1,  0, 11,  7, 11,  4,  0,  4, 11 },
        /* 01011110 */ { 5, 0,  5,  9,  0,  6,  5,  0,  3,  6, 11,  6,  3,  8,  4,  7 },
        /* 01011111 */ { 4, 6,  5,  9,  6,  9, 11,  4,  7,  9,  7, 11,  9, -1, -1, -1 },
        /* 01100000 */ { 2,10,  4,  9,  6,  4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01100001 */ { 3, 4, 10,  6,  4,  9, 10,  0,  8,  3, -1, -1, -1, -1, -1, -1 },
        /* 01100010 */ { 3,10,  0,  1, 10,  6,  0,  6,  4,  0, -1, -1, -1, -1, -1, -1 },
        /* 01100011 */ { 4, 8,  3,  1,  8,  1,  6,  8,  6,  4,  6,  1, 10, -1, -1, -1 },
        /* 01100100 */ { 3, 1,  4,  9,  1,  2,  4,  2,  6,  4, -1, -1, -1, -1, -1, -1 },
        /* 01100101 */ { 4, 3,  0,  8,  1,  2,  9,  2,  4,  9,  2,  6,  4, -1, -1, -1 },
        /* 01100110 */ { 2, 0,  2,  4,  4,  2,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01100111 */ { 3, 8,  3,  2,  8,  2,  4,  4,  2,  6, -1, -1, -1, -1, -1, -1 },
        /* 01101000 */ { 3,10,  4,  9, 10,  6,  4, 11,  2,  3, -1, -1, -1, -1, -1, -1 },
        /* 01101001 */ { 4, 0,  8,  2,  2,  8, 11,  4,  9, 10,  4, 10,  6, -1, -1, -1 },
        /* 01101010 */ { 4, 3, 11,  2,  0,  1,  6,  0,  6,  4,  6,  1, 10, -1, -1, -1 },
        /* 01101011 */ { 5, 6,  4,  1,  6,  1, 10,  4,  8,  1,  2,  1, 11,  8, 11,  1 },
        /* 01101100 */ { 4, 9,  6,  4,  9,  3,  6,  9,  1,  3, 11,  6,  3, -1, -1, -1 },
        /* 01101101 */ { 5, 8, 11,  1,  8,  1,  0, 11,  6,  1,  9,  1,  4,  6,  4,  1 },
        /* 01101110 */ { 3, 3, 11,  6,  3,  6,  0,  0,  6,  4, -1, -1, -1, -1, -1, -1 },
        /* 01101111 */ { 2, 6,  4,  8, 11,  6,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01110000 */ { 3, 7, 10,  6,  7,  8, 10,  8,  9, 10, -1, -1, -1, -1, -1, -1 },
        /* 01110001 */ { 4, 0,  7,  3,  0, 10,  7,  0,  9, 10,  6,  7, 10, -1, -1, -1 },
        /* 01110010 */ { 4,10,  6,  7,  1, 10,  7,  1,  7,  8,  1,  8,  0, -1, -1, -1 },
        /* 01110011 */ { 3,10,  6,  7, 10,  7,  1,  1,  7,  3, -1, -1, -1, -1, -1, -1 },
        /* 01110100 */ { 4, 1,  2,  6,  1,  6,  8,  1,  8,  9,  8,  6,  7, -1, -1, -1 },
        /* 01110101 */ { 5, 2,  6,  9,  2,  9,  1,  6,  7,  9,  0,  9,  3,  7,  3,  9 },
        /* 01110110 */ { 3, 7,  8,  0,  7,  0,  6,  6,  0,  2, -1, -1, -1, -1, -1, -1 },
        /* 01110111 */ { 2, 7,  3,  2,  6,  7,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01111000 */ { 4, 2,  3, 11, 10,  6,  8, 10,  8,  9,  8,  6,  7, -1, -1, -1 },
        /* 01111001 */ { 5, 2,  0,  7,  2,  7, 11,  0,  9,  7,  6,  7, 10,  9, 10,  7 },
        /* 01111010 */ { 5, 1,  8,  0,  1,  7,  8,  1, 10,  7,  6,  7, 10,  2,  3, 11 },
        /* 01111011 */ { 4,11,  2,  1, 11,  1,  7, 10,  6,  1,  6,  7,  1, -1, -1, -1 },
        /* 01111100 */ { 5, 8,  9,  6,  8,  6,  7,  9,  1,  6, 11,  6,  3,  1,  3,  6 },
        /* 01111101 */ { 2, 0,  9,  1, 11,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 01111110 */ { 4, 7,  8,  0,  7,  0,  6,  3, 11,  0, 11,  6,  0, -1, -1, -1 },
        /* 01111111 */ { 1, 7, 11,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10000000 */ { 1, 7,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10000001 */ { 2, 3,  0,  8, 11,  7,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10000010 */ { 2, 0,  1,  9, 11,  7,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10000011 */ { 3, 8,  1,  9,  8,  3,  1, 11,  7,  6, -1, -1, -1, -1, -1, -1 },
        /* 10000100 */ { 2,10,  1,  2,  6, 11,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10000101 */ { 3, 1,  2, 10,  3,  0,  8,  6, 11,  7, -1, -1, -1, -1, -1, -1 },
        /* 10000110 */ { 3, 2,  9,  0,  2, 10,  9,  6, 11,  7, -1, -1, -1, -1, -1, -1 },
        /* 10000111 */ { 4, 6, 11,  7,  2, 10,  3, 10,  8,  3, 10,  9,  8, -1, -1, -1 },
        /* 10001000 */ { 2, 7,  2,  3,  6,  2,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10001001 */ { 3, 7,  0,  8,  7,  6,  0,  6,  2,  0, -1, -1, -1, -1, -1, -1 },
        /* 10001010 */ { 3, 2,  7,  6,  2,  3,  7,  0,  1,  9, -1, -1, -1, -1, -1, -1 },
        /* 10001011 */ { 4, 1,  6,  2,  1,  8,  6,  1,  9,  8,  8,  7,  6, -1, -1, -1 },
        /* 10001100 */ { 3,10,  7,  6, 10,  1,  7,  1,  3,  7, -1, -1, -1, -1, -1, -1 },
        /* 10001101 */ { 4,10,  7,  6,  1,  7, 10,  1,  8,  7,  1,  0,  8, -1, -1, -1 },
        /* 10001110 */ { 4, 0,  3,  7,  0,  7, 10,  0, 10,  9,  6, 10,  7, -1, -1, -1 },
        /* 10001111 */ { 3, 7,  6, 10,  7, 10,  8,  8, 10,  9, -1, -1, -1, -1, -1, -1 },
        /* 10010000 */ { 2, 6,  8,  4, 11,  8,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10010001 */ { 3, 3,  6, 11,  3,  0,  6,  0,  4,  6, -1, -1, -1, -1, -1, -1 },
        /* 10010010 */ { 3, 8,  6, 11,  8,  4,  6,  9,  0,  1, -1, -1, -1, -1, -1, -1 },
        /* 10010011 */ { 4, 9,  4,  6,  9,  6,  3,  9,  3,  1, 11,  3,  6, -1, -1, -1 },
        /* 10010100 */ { 3, 6,  8,  4,  6, 11,  8,  2, 10,  1, -1, -1, -1, -1, -1, -1 },
        /* 10010101 */ { 4, 1,  2, 10,  3,  0, 11,  0,  6, 11,  0,  4,  6, -1, -1, -1 },
        /* 10010110 */ { 4, 4, 11,  8,  4,  6, 11,  0,  2,  9,  2, 10,  9, -1, -1, -1 },
        /* 10010111 */ { 5,10,  9,  3, 10,  3,  2,  9,  4,  3, 11,  3,  6,  4,  6,  3 },
        /* 10011000 */ { 3, 8,  2,  3,  8,  4,  2,  4,  6,  2, -1, -1, -1, -1, -1, -1 },
        /* 10011001 */ { 2, 0,  4,  2,  4,  6,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10011010 */ { 4, 1,  9,  0,  2,  3,  4,  2,  4,  6,  4,  3,  8, -1, -1, -1 },
        /* 10011011 */ { 3, 1,  9,  4,  1,  4,  2,  2,  4,  6, -1, -1, -1, -1, -1, -1 },
        /* 10011100 */ { 4, 8,  1,  3,  8,  6,  1,  8,  4,  6,  6, 10,  1, -1, -1, -1 },
        /* 10011101 */ { 3,10,  1,  0, 10,  0,  6,  6,  0,  4, -1, -1, -1, -1, -1, -1 },
        /* 10011110 */ { 5, 4,  6,  3,  4,  3,  8,  6, 10,  3,  0,  3,  9, 10,  9,  3 },
        /* 10011111 */ { 2,10,  9,  4,  6, 10,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10100000 */ { 2, 4,  9,  5,  7,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10100001 */ { 3, 0,  8,  3,  4,  9,  5, 11,  7,  6, -1, -1, -1, -1, -1, -1 },
        /* 10100010 */ { 3, 5,  0,  1,  5,  4,  0,  7,  6, 11, -1, -1, -1, -1, -1, -1 },
        /* 10100011 */ { 4,11,  7,  6,  8,  3,  4,  3,  5,  4,  3,  1,  5, -1, -1, -1 },
        /* 10100100 */ { 3, 9,  5,  4, 10,  1,  2,  7,  6, 11, -1, -1, -1, -1, -1, -1 },
        /* 10100101 */ { 4, 6, 11,  7,  1,  2, 10,  0,  8,  3,  4,  9,  5, -1, -1, -1 },
        /* 10100110 */ { 4, 7,  6, 11,  5,  4, 10,  4,  2, 10,  4,  0,  2, -1, -1, -1 },
        /* 10100111 */ { 5, 3,  4,  8,  3,  5,  4,  3,  2,  5, 10,  5,  2, 11,  7,  6 },
        /* 10101000 */ { 3, 7,  2,  3,  7,  6,  2,  5,  4,  9, -1, -1, -1, -1, -1, -1 },
        /* 10101001 */ { 4, 9,  5,  4,  0,  8,  6,  0,  6,  2,  6,  8,  7, -1, -1, -1 },
        /* 10101010 */ { 4, 3,  6,  2,  3,  7,  6,  1,  5,  0,  5,  4,  0, -1, -1, -1 },
        /* 10101011 */ { 5, 6,  2,  8,  6,  8,  7,  2,  1,  8,  4,  8,  5,  1,  5,  8 },
        /* 10101100 */ { 4, 9,  5,  4, 10,  1,  6,  1,  7,  6,  1,  3,  7, -1, -1, -1 },
        /* 10101101 */ { 5, 1,  6, 10,  1,  7,  6,  1,  0,  7,  8,  7,  0,  9,  5,  4 },
        /* 10101110 */ { 5, 4,  0, 10,  4, 10,  5,  0,  3, 10,  6, 10,  7,  3,  7, 10 },
        /* 10101111 */ { 4, 7,  6, 10,  7, 10,  8,  5,  4, 10,  4,  8, 10, -1, -1, -1 },
        /* 10110000 */ { 3, 6,  9,  5,  6, 11,  9, 11,  8,  9, -1, -1, -1, -1, -1, -1 },
        /* 10110001 */ { 4, 3,  6, 11,  0,  6,  3,  0,  5,  6,  0,  9,  5, -1, -1, -1 },
        /* 10110010 */ { 4, 0, 11,  8,  0,  5, 11,  0,  1,  5,  5,  6, 11, -1, -1, -1 },
        /* 10110011 */ { 3, 6, 11,  3,  6,  3,  5,  5,  3,  1, -1, -1, -1, -1, -1, -1 },
        /* 10110100 */ { 4, 1,  2, 10,  9,  5, 11,  9, 11,  8, 11,  5,  6, -1, -1, -1 },
        /* 10110101 */ { 5, 0, 11,  3,  0,  6, 11,  0,  9,  6,  5,  6,  9,  1,  2, 10 },
        /* 10110110 */ { 5,11,  8,  5, 11,  5,  6,  8,  0,  5, 10,  5,  2,  0,  2,  5 },
        /* 10110111 */ { 4, 6, 11,  3,  6,  3,  5,  2, 10,  3, 10,  5,  3, -1, -1, -1 },
        /* 10111000 */ { 4, 5,  8,  9,  5,  2,  8,  5,  6,  2,  3,  8,  2, -1, -1, -1 },
        /* 10111001 */ { 3, 9,  5,  6,  9,  6,  0,  0,  6,  2, -1, -1, -1, -1, -1, -1 },
        /* 10111010 */ { 5, 1,  5,  8,  1,  8,  0,  5,  6,  8,  3,  8,  2,  6,  2,  8 },
        /* 10111011 */ { 2, 1,  5,  6,  2,  1,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10111100 */ { 5, 1,  3,  6,  1,  6, 10,  3,  8,  6,  5,  6,  9,  8,  9,  6 },
        /* 10111101 */ { 4,10,  1,  0, 10,  0,  6,  9,  5,  0,  5,  6,  0, -1, -1, -1 },
        /* 10111110 */ { 2, 0,  3,  8,  5,  6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 10111111 */ { 1,10,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11000000 */ { 2,11,  5, 10,  7,  5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11000001 */ { 3,11,  5, 10, 11,  7,  5,  8,  3,  0, -1, -1, -1, -1, -1, -1 },
        /* 11000010 */ { 3, 5, 11,  7,  5, 10, 11,  1,  9,  0, -1, -1, -1, -1, -1, -1 },
        /* 11000011 */ { 4,10,  7,  5, 10, 11,  7,  9,  8,  1,  8,  3,  1, -1, -1, -1 },
        /* 11000100 */ { 3,11,  1,  2, 11,  7,  1,  7,  5,  1, -1, -1, -1, -1, -1, -1 },
        /* 11000101 */ { 4, 0,  8,  3,  1,  2,  7,  1,  7,  5,  7,  2, 11, -1, -1, -1 },
        /* 11000110 */ { 4, 9,  7,  5,  9,  2,  7,  9,  0,  2,  2, 11,  7, -1, -1, -1 },
        /* 11000111 */ { 5, 7,  5,  2,  7,  2, 11,  5,  9,  2,  3,  2,  8,  9,  8,  2 },
        /* 11001000 */ { 3, 2,  5, 10,  2,  3,  5,  3,  7,  5, -1, -1, -1, -1, -1, -1 },
        /* 11001001 */ { 4, 8,  2,  0,  8,  5,  2,  8,  7,  5, 10,  2,  5, -1, -1, -1 },
        /* 11001010 */ { 4, 9,  0,  1,  5, 10,  3,  5,  3,  7,  3, 10,  2, -1, -1, -1 },
        /* 11001011 */ { 5, 9,  8,  2,  9,  2,  1,  8,  7,  2, 10,  2,  5,  7,  5,  2 },
        /* 11001100 */ { 2, 1,  3,  5,  3,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11001101 */ { 3, 0,  8,  7,  0,  7,  1,  1,  7,  5, -1, -1, -1, -1, -1, -1 },
        /* 11001110 */ { 3, 9,  0,  3,  9,  3,  5,  5,  3,  7, -1, -1, -1, -1, -1, -1 },
        /* 11001111 */ { 2, 9,  8,  7,  5,  9,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11010000 */ { 3, 5,  8,  4,  5, 10,  8, 10, 11,  8, -1, -1, -1, -1, -1, -1 },
        /* 11010001 */ { 4, 5,  0,  4,  5, 11,  0,  5, 10, 11, 11,  3,  0, -1, -1, -1 },
        /* 11010010 */ { 4, 0,  1,  9,  8,  4, 10,  8, 10, 11, 10,  4,  5, -1, -1, -1 },
        /* 11010011 */ { 5,10, 11,  4, 10,  4,  5, 11,  3,  4,  9,  4,  1,  3,  1,  4 },
        /* 11010100 */ { 4, 2,  5,  1,  2,  8,  5,  2, 11,  8,  4,  5,  8, -1, -1, -1 },
        /* 11010101 */ { 5, 0,  4, 11,  0, 11,  3,  4,  5, 11,  2, 11,  1,  5,  1, 11 },
        /* 11010110 */ { 5, 0,  2,  5,  0,  5,  9,  2, 11,  5,  4,  5,  8, 11,  8,  5 },
        /* 11010111 */ { 2, 9,  4,  5,  2, 11,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11011000 */ { 4, 2,  5, 10,  3,  5,  2,  3,  4,  5,  3,  8,  4, -1, -1, -1 },
        /* 11011001 */ { 3, 5, 10,  2,  5,  2,  4,  4,  2,  0, -1, -1, -1, -1, -1, -1 },
        /* 11011010 */ { 5, 3, 10,  2,  3,  5, 10,  3,  8,  5,  4,  5,  8,  0,  1,  9 },
        /* 11011011 */ { 4, 5, 10,  2,  5,  2,  4,  1,  9,  2,  9,  4,  2, -1, -1, -1 },
        /* 11011100 */ { 3, 8,  4,  5,  8,  5,  3,  3,  5,  1, -1, -1, -1, -1, -1, -1 },
        /* 11011101 */ { 2, 0,  4,  5,  1,  0,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11011110 */ { 4, 8,  4,  5,  8,  5,  3,  9,  0,  5,  0,  3,  5, -1, -1, -1 },
        /* 11011111 */ { 1, 9,  4,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11100000 */ { 3, 4, 11,  7,  4,  9, 11,  9, 10, 11, -1, -1, -1, -1, -1, -1 },
        /* 11100001 */ { 4, 0,  8,  3,  4,  9,  7,  9, 11,  7,  9, 10, 11, -1, -1, -1 },
        /* 11100010 */ { 4, 1, 10, 11,  1, 11,  4,  1,  4,  0,  7,  4, 11, -1, -1, -1 },
        /* 11100011 */ { 5, 3,  1,  4,  3,  4,  8,  1, 10,  4,  7,  4, 11, 10, 11,  4 },
        /* 11100100 */ { 4, 4, 11,  7,  9, 11,  4,  9,  2, 11,  9,  1,  2, -1, -1, -1 },
        /* 11100101 */ { 5, 9,  7,  4,  9, 11,  7,  9,  1, 11,  2, 11,  1,  0,  8,  3 },
        /* 11100110 */ { 3,11,  7,  4, 11,  4,  2,  2,  4,  0, -1, -1, -1, -1, -1, -1 },
        /* 11100111 */ { 4,11,  7,  4, 11,  4,  2,  8,  3,  4,  3,  2,  4, -1, -1, -1 },
        /* 11101000 */ { 4, 2,  9, 10,  2,  7,  9,  2,  3,  7,  7,  4,  9, -1, -1, -1 },
        /* 11101001 */ { 5, 9, 10,  7,  9,  7,  4, 10,  2,  7,  8,  7,  0,  2,  0,  7 },
        /* 11101010 */ { 5, 3,  7, 10,  3, 10,  2,  7,  4, 10,  1, 10,  0,  4,  0, 10 },
        /* 11101011 */ { 2, 1, 10,  2,  8,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11101100 */ { 3, 4,  9,  1,  4,  1,  7,  7,  1,  3, -1, -1, -1, -1, -1, -1 },
        /* 11101101 */ { 4, 4,  9,  1,  4,  1,  7,  0,  8,  1,  8,  7,  1, -1, -1, -1 },
        /* 11101110 */ { 2, 4,  0,  3,  7,  4,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11101111 */ { 1, 4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11110000 */ { 2, 9, 10,  8, 10, 11,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11110001 */ { 3, 3,  0,  9,  3,  9, 11, 11,  9, 10, -1, -1, -1, -1, -1, -1 },
        /* 11110010 */ { 3, 0,  1, 10,  0, 10,  8,  8, 10, 11, -1, -1, -1, -1, -1, -1 },
        /* 11110011 */ { 2, 3,  1, 10, 11,  3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11110100 */ { 3, 1,  2, 11,  1, 11,  9,  9, 11,  8, -1, -1, -1, -1, -1, -1 },
        /* 11110101 */ { 4, 3,  0,  9,  3,  9, 11,  1,  2,  9,  2, 11,  9, -1, -1, -1 },
        /* 11110110 */ { 2, 0,  2, 11,  8,  0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11110111 */ { 1, 3,  2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11111000 */ { 3, 2,  3,  8,  2,  8, 10, 10,  8,  9, -1, -1, -1, -1, -1, -1 },
        /* 11111001 */ { 2, 9, 10,  2,  0,  9,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11111010 */ { 4, 2,  3,  8,  2,  8, 10,  0,  1,  8,  1, 10,  8, -1, -1, -1 },
        /* 11111011 */ { 1, 1, 10,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11111100 */ { 2, 1,  3,  8,  9,  1,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11111101 */ { 1, 0,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11111110 */ { 1, 0,  3,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
        /* 11111111 */ { 0,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
    };

#ifdef CHECK_MARCHING_CUBES
    CRefObj<IMesh> CMeshConstructor::BuildMarchingCubeCases()
    {
        static Vector3 verts[12] =
        {
            Vector3(0.0f, 0.5f, 0.0f),
            Vector3(0.5f, 1.0f, 0.0f),
            Vector3(1.0f, 0.5f, 0.0f),
            Vector3(0.5f, 0.0f, 0.0f),
            Vector3(0.0f, 0.5f, 1.0f),
            Vector3(0.5f, 1.0f, 1.0f),
            Vector3(1.0f, 0.5f, 1.0f),
            Vector3(0.5f, 0.0f, 1.0f),
            Vector3(0.0f, 0.5f, 0.5f),
            Vector3(0.5f, 1.0f, 0.5f),
            Vector3(1.0f, 0.5f, 0.5f),
            Vector3(0.5f, 0.0f, 0.5f),
        };

        MeshOpen();
        for (int i = 0; i < 256; i++)
        {
            int numPolys = edgeTable[i][0];
            if (numPolys > 0)
            {
                SubMeshOpen();
                float xoffset = float((i % 16) * 2);
                float yoffset = float((i / 16) * 2);
                for (int l = 0; l < numPolys; l++)
                {
                    FaceOpen();
                    Vector3 pos0 = verts[edgeTable[i][3 * l + 1]] + Vector3(xoffset, yoffset, 0.0f);
                    Vector3 pos1 = verts[edgeTable[i][3 * l + 2]] + Vector3(xoffset, yoffset, 0.0f);
                    Vector3 pos2 = verts[edgeTable[i][3 * l + 3]] + Vector3(xoffset, yoffset, 0.0f);
                    Vector3 p0 = pos1 - pos0;
                    Vector3 p1 = pos2 - pos0;
                    Vector3 norm = p0.Cross(p1);
                    norm.Normalize();
                    Vector2 uv(0.0f, 0.0f);
                    VertexAdd(pos0, norm, uv);
                    VertexAdd(pos1, norm, uv);
                    VertexAdd(pos2, norm, uv);
                    FaceClose();
                }
                SubMeshClose();
            }
        }
        MeshClose();
        return m_spMesh;
    }
#endif

    CRefObj<IMesh> CMeshConstructor::MeshFromDensityFunction(int numBlocks, std::function<float(Vector3&)> fn)
    {
#ifdef CHECK_MARCHING_CUBES
        return BuildMarchingCubeCases();
#else
        float *data = new float[(numBlocks + 1) * (numBlocks + 1) * (numBlocks + 1)];
        float delta = 1.0f / float(numBlocks);
        float z = 0.0f;
        int i = 0;
        while (z <= 1.0f)
        {
            int j = 0;
            float y = 0.0f;
            while (y <= 1.0f)
            {
                int k = 0; 
                float x = 0.0f;
                while (x <= 1.0f)
                {
                    Vector3 v(x, y, z);
                    float val = fn(v);
                    data[i * numBlocks * numBlocks + j * numBlocks + k] = val;
                    x += delta;
                    k++;
                }
                y += delta;
                j++;
            }
            z += delta;
            i++;
        }
        MeshOpen();
        SubMeshOpen();

        z = 0.0f;
        for (int bz = 0; bz < numBlocks; bz++)
        {
            float y = 0.0f;
            for (int by = 0; by < numBlocks; by++)
            {
                float x = 0.0f;
                for (int bx = 0; bx < numBlocks; bx++)
                {
                    float d0 = data[bz * numBlocks * numBlocks + by * numBlocks + bx];
                    float d1 = data[bz * numBlocks * numBlocks + (by + 1) * numBlocks + bx];
                    float d2 = data[bz * numBlocks * numBlocks + (by + 1) * numBlocks + (bx + 1)];
                    float d3 = data[bz * numBlocks * numBlocks + by * numBlocks + (bx + 1)];
                    float d4 = data[(bz + 1) * numBlocks * numBlocks + by * numBlocks + bx];
                    float d5 = data[(bz + 1) * numBlocks * numBlocks + (by + 1) * numBlocks + bx];
                    float d6 = data[(bz + 1) * numBlocks * numBlocks + (by + 1) * numBlocks + (bx + 1)];
                    float d7 = data[(bz + 1) * numBlocks * numBlocks + by * numBlocks + (bx + 1)];
                    Vector3 edgePos[12];
                    float t0 = -d0 / (d1 - d0);
                    float t1 = -d1 / (d2 - d1);
                    float t2 = -d2 / (d3 - d2);
                    float t3 = -d3 / (d0 - d3);
                    float t4 = -d4 / (d5 - d4);
                    float t5 = -d5 / (d6 - d5);
                    float t6 = -d6 / (d7 - d6);
                    float t7 = -d7 / (d4 - d7);
                    float t8 = -d0 / (d4 - d0);
                    float t9 = -d1 / (d5 - d1);
                    float t10 = -d2 / (d6 - d2);
                    float t11 = -d3 / (d7 - d3);
                    Vector3 e0(x, y, z);
                    Vector3 e1(x, y + delta, z);
                    Vector3 e2(x + delta, y + delta, z);
                    Vector3 e3(x + delta, y, z);
                    Vector3 e4(x, y, z + delta);
                    Vector3 e5(x, y + delta, z + delta);
                    Vector3 e6(x + delta, y + delta, z + delta);
                    Vector3 e7(x + delta, y, z + delta);
                    edgePos[0] = e0 + (e1 - e0) * t0;
                    edgePos[1] = e1 + (e2 - e1) * t1;
                    edgePos[2] = e2 + (e3 - e2) * t2;
                    edgePos[3] = e3 + (e0 - e3) * t3;
                    edgePos[4] = e4 + (e5 - e4) * t4;
                    edgePos[5] = e5 + (e6 - e5) * t5;
                    edgePos[6] = e6 + (e7 - e6) * t6;
                    edgePos[7] = e7 + (e4 - e7) * t7;
                    edgePos[8] = e0 + (e4 - e0) * t8;
                    edgePos[9] = e1 + (e5 - e1) * t9;
                    edgePos[10] = e2 + (e6 - e2) * t10;
                    edgePos[11] = e3 + (e7 - e3) * t11;
                    uint8 code = 0;
                    code = (code << 1) | ((d7 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d6 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d5 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d4 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d3 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d2 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d1 >= 0.0f) ? 1 : 0);
                    code = (code << 1) | ((d0 >= 0.0f) ? 1 : 0);
                    int numPolys = edgeTable[code][0];
                    for (int l = 0; l < numPolys; l++)
                    {
                        FaceOpen();
                        Vector3 pos0 = edgePos[edgeTable[code][3 * l + 1]];
                        Vector3 pos1 = edgePos[edgeTable[code][3 * l + 2]];
                        Vector3 pos2 = edgePos[edgeTable[code][3 * l + 3]];
                        Vector3 p0 = pos1 - pos0;
                        Vector3 p1 = pos2 - pos0;
                        Vector3 norm = p0.Cross(p1);
                        norm.Normalize();
                        Vector2 uv(0.0f, 0.0f);
                        VertexAdd(pos0, norm, uv);
                        VertexAdd(pos1, norm, uv);
                        VertexAdd(pos2, norm, uv);
                        FaceClose();
                    }
                    x += delta;
                }
                y += delta;
            }
            z += delta;
        }

        SubMeshClose();
        MeshClose();
        return m_spMesh;
#endif
    }

    CAUSTICAPI CRefObj<IMeshConstructor> CreateMeshConstructor()
    {
        return CRefObj<IMeshConstructor>(new CMeshConstructor());
    }

    CMeshConstructor::CMeshConstructor() :
        m_pCurFace(nullptr)
    {
    }

    void CMeshConstructor::MeshOpen()
    {
        m_spMesh = CreateEmptyMesh();
    }

    CRefObj<IMesh> CMeshConstructor::MeshClose()
    {
        return m_spMesh;
    }

    void CMeshConstructor::SubMeshOpen()
    {
        CT((m_spSubMesh == nullptr) ? S_OK : E_FAIL);
        m_spSubMesh = new CSubMesh();
        m_spSubMesh->AddRef();
    }

    CRefObj<ISubMesh> CMeshConstructor::SubMeshClose()
    {
        if (m_spMesh)
        {
#ifdef CHECK_CONSISTENCY
            m_spSubMesh->CheckConsistency();
#endif // CHECK_CONSISTENCY
            m_spMesh->AddSubMesh(m_spSubMesh);
        }
        CRefObj<ISubMesh> spSubMesh = m_spSubMesh;
        m_spSubMesh = nullptr;
        return spSubMesh;
    }

    void CMeshConstructor::FaceOpen()
    {
        // Allocate new face
        m_pCurFace = m_spSubMesh->AllocateFace();
        m_pPrevEdge = nullptr;
        m_pPrevVertex = nullptr;
    }

    void CMeshConstructor::AddEdge(CGeomVertex *pTail, CGeomVertex *pHead)
    {
        // First see if the edge already exists
        CHalfEdge *pNewEdge = m_spSubMesh->FindEdge(pHead, pTail);
        if (pNewEdge == nullptr)
        {
            // Edge doesn't exist yet. Create the edge.
            pNewEdge = m_spSubMesh->AllocateEdge(pHead, pTail);
        }
        pNewEdge->m_pFace = m_pCurFace;
        m_spSubMesh->LinkEdges(m_pPrevEdge, pNewEdge);
        m_pPrevEdge = pNewEdge;
        if (m_pCurFace->m_pEdge == nullptr)
            m_pCurFace->m_pEdge = pNewEdge;
    }

    void CMeshConstructor::FaceClose()
    {
        // Add edge between first and last vertex
        AddEdge(m_pPrevVertex, m_pCurFace->m_vertices[0]);

        // Compute current face's center point
        m_pCurFace->m_center.x /= (float)m_pCurFace->m_vertices.size();
        m_pCurFace->m_center.y /= (float)m_pCurFace->m_vertices.size();
        m_pCurFace->m_center.z /= (float)m_pCurFace->m_vertices.size();

        // Link up first and last edge
        if (m_pCurFace->m_pEdge && m_pPrevEdge)
            m_spSubMesh->LinkEdges(m_pPrevEdge, m_pCurFace->m_pEdge);

        // Compute face normal vector.
        m_pCurFace->ComputeFaceNormal();

//#define DEBUG_LINKS
#ifdef DEBUG_LINKS
        wchar_t buf[1024];
        swprintf_s(buf, L"Face Verts: ");
        OutputDebugString(buf);
        for (int i = 0; i < 3; i++)
        {
            swprintf_s(buf, L"%d ", m_pCurFace->GetVertex(i)->index);
            OutputDebugString(buf);
        }
        swprintf_s(buf, L"\n");
        OutputDebugString(buf);
        swprintf_s(buf, L"Face Verts Pos: ");
        OutputDebugString(buf);
        for (int i = 0; i < 3; i++)
        {
            swprintf_s(buf, L"%f %f %f, ", m_pCurFace->GetVertex(i)->pos.x, m_pCurFace->GetVertex(i)->pos.y, m_pCurFace->GetVertex(i)->pos.z);
            OutputDebugString(buf);
        }
        swprintf_s(buf, L"\n");
        OutputDebugString(buf);
        swprintf_s(buf,L"Face Edges: \n");
        OutputDebugString(buf);
        CHalfEdge *pEdge = m_pCurFace->GetEdge();
        for (int i = 0; i < 3; i++)
        {
            swprintf_s(buf,L"  %d: Prev:%d  Next:%d  Opposite:%d  OppPrev:%d  OppNext:%d  Face:%d\n",
                pEdge->index, pEdge->m_pPrev->index, pEdge->m_pNext->index, 
                pEdge->m_pOpposite->index,
                pEdge->m_pOpposite->m_pPrev->index,
                pEdge->m_pOpposite->m_pNext->index,
                pEdge->m_pFace->index);
            OutputDebugString(buf);
            pEdge = pEdge->m_pNext;
        }
        swprintf_s(buf,L"\n");
        OutputDebugString(buf);
#endif
        m_pCurFace = nullptr;
        return;
    }

    void CMeshConstructor::VertexAdd(Vector3 &pos, Vector3 &normal, Vector2 &uv)
    {
        if (m_pCurFace == nullptr)
            CT(E_FAIL); // Face isn't open

        //CGeomVertex *pVertex = m_spSubMesh->FindVertex(pos, nullptr, &uv);
        //if (pVertex == nullptr)
            CGeomVertex* pVertex = m_spSubMesh->AllocateGeomVertex(pos, normal, uv);
        m_pCurFace->m_vertices.push_back(pVertex);

        // Update running average of where current face's center point is
        m_pCurFace->m_center.x += pVertex->pos.x;
        m_pCurFace->m_center.y += pVertex->pos.y;
        m_pCurFace->m_center.z += pVertex->pos.z;
        
        // If this is NOT the first vertex being added to the face
        // then create an edge between the last vertex and the new vertex.
        if (m_pPrevVertex)
            AddEdge(m_pPrevVertex, pVertex);
        m_pPrevVertex = pVertex;
        return;
    }
}
