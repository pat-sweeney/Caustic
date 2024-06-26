//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <d3d11_4.h>
#include <any>
#include <cinttypes>

module Rendering.SceneGraph.SceneMarchingCubesElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IShaderMgr;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ICausticFactory;
import Rendering.SceneGraph.ISceneMarchingCubesElem;
import Geometry.Mesh.RenderTypes;

namespace Caustic
{
    static int32_t s_edgeTable[256][16] = {
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

    CSceneMarchingCubesElem::CSceneMarchingCubesElem(IRenderer* pRenderer, uint32_t subdivisions, std::function<float(Vector3&)> sdf, bool drawIndexed)
    {
        m_initialized = false;
        m_drawIndexed = drawIndexed;

        m_sdf = sdf;
        CRefObj<IShaderMgr> spShaderMgr = pRenderer->GetShaderMgr();
        if (drawIndexed)
        {
            m_spMCCountVertsShader = spShaderMgr->FindShader(L"MCCountVerts");
            m_spMCAllocVertsShader = spShaderMgr->FindShader(L"MCAllocVerts");
            m_spMCEmitVertsShader = spShaderMgr->FindShader(L"MCEmitVerts");
        }
        else
            m_spMCShader = spShaderMgr->FindShader(L"MCSinglePass");

        // NOTE: Since each voxel represents one corner of the MC algorithm (i.e. the value of the voxel is considered
        // to be the lower left front corner) we need to expand out voxel grid by 1 voxel in each direction. For instance,
        // if the user specified they want '1' subdisivion (i.e. 1 voxel) that voxel would only contain the value of vertex
        // 0 in the following diagram. The values for the other vertices are stored in neighboring voxels (i.e. vertex 1's
        // value is stored in the +Y voxel, 2 in the +X,+Y voxel, 3 in the +X voxel, etc)
        //        5-----------------6
        //       /|                /|
        //      / |               / |
        //     /  |              /  |
        //    1-----------------2   |
        //    |   |             |   |
        //    |   8-------------|---7
        //    |  /              |  /
        //    | /               | /
        //    |/                |/
        //    0-----------------3
        m_subdivisions = subdivisions + 1;

        // Here we need to determine how many threads and groups the dispatch involves. We need to make our structures
        // match these sizes even if the client requested something different. For instance, if the shader sets its
        // numThreads(8,8,8) then we will need the SDF to also be a multiple of 8. To do this we will make the SDF
        // larger but filled with zeroes.
        uint32_t xThreads, yThreads, zThreads;
        if (m_drawIndexed)
            m_spMCCountVertsShader->GetShaderInfo()->GetThreadGroupSize(&xThreads, &yThreads, &zThreads);
        else
            m_spMCShader->GetShaderInfo()->GetThreadGroupSize(&xThreads, &yThreads, &zThreads);
        m_numGroupsX = ((m_subdivisions + xThreads - 1) / xThreads);
        m_numGroupsY = ((m_subdivisions + yThreads - 1) / yThreads);
        m_numGroupsZ = ((m_subdivisions + zThreads - 1) / zThreads);
        m_xRoundedSubdivisions = xThreads * m_numGroupsX;
        m_yRoundedSubdivisions = yThreads * m_numGroupsY;
        m_zRoundedSubdivisions = zThreads * m_numGroupsZ;

        float delta = 1.0f / float(subdivisions);
        m_sdfData = new float[m_xRoundedSubdivisions * m_yRoundedSubdivisions * m_zRoundedSubdivisions];
        for (uint32_t i = 0; i < m_xRoundedSubdivisions * m_yRoundedSubdivisions * m_zRoundedSubdivisions; i++)
            m_sdfData[i] = FLT_MAX;
        float z = 0.0f;
        for (uint32_t i = 0; i < m_subdivisions; i++)
        {
            float y = 0.0f;
            for (uint32_t j = 0; j < m_subdivisions; j++)
            {
                float x = 0.0f;
                for (uint32_t k = 0; k < m_subdivisions; k++)
                {
                    Vector3 v(x, y, z);
                    float val = sdf(v);
                    m_sdfData[i * m_xRoundedSubdivisions * m_yRoundedSubdivisions + j * m_xRoundedSubdivisions + k] = val;
                    x += delta;
                }
                y += delta;
            }
            z += delta;
        }
    }
    
    void CSceneMarchingCubesElem::Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx)
    {
        if (!(m_passes & pRenderCtx->GetCurrentPass()))
            return;
        if (m_prerenderCallback)
            if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                return;
        std::vector<CRefObj<ILight>> lights;
        
        uint32_t totalCells = m_xRoundedSubdivisions * m_yRoundedSubdivisions * m_zRoundedSubdivisions;
        if (!m_initialized)
        {
            m_initialized = true;
            auto spGPUBuffer = CreateGPUBuffer(pRenderer, EBufferType::StructuredBuffer, totalCells, sizeof(float), 0);
            spGPUBuffer->CopyFromCPU(pRenderer, (uint8_t*)m_sdfData);
            std::any v(spGPUBuffer);
            if (m_drawIndexed)
            {
                m_spMCCountVertsShader->SetCSParam(L"densityField", v);
                m_spMCAllocVertsShader->SetCSParam(L"densityField", v);
                m_spMCEmitVertsShader->SetCSParam(L"densityField", v);
            }
            else
                m_spMCShader->SetCSParam(L"densityField", v);

            spGPUBuffer = CreateGPUBuffer(pRenderer, EBufferType::StructuredBuffer, 256 * 16, sizeof(int32_t), 0);
            spGPUBuffer->CopyFromCPU(pRenderer, (uint8_t*)s_edgeTable);
            std::any ed(spGPUBuffer);
            if (m_drawIndexed)
            {
                m_spMCCountVertsShader->SetCSParam(L"edgeTable", ed);
                m_spMCAllocVertsShader->SetCSParam(L"edgeTable", ed);
                m_spMCEmitVertsShader->SetCSParam(L"edgeTable", ed);

                m_spMCCountVertsShader->SetCSParam(L"subdivisions", std::any(Int(m_subdivisions)));
                m_spMCAllocVertsShader->SetCSParam(L"subdivisions", std::any(Int(m_subdivisions)));
                m_spMCEmitVertsShader->SetCSParam(L"subdivisions", std::any(Int(m_subdivisions)));

                m_spMCCountVertsShader->SetCSParam(L"numCellsX", std::any(Int(m_xRoundedSubdivisions)));
                m_spMCCountVertsShader->SetCSParam(L"numCellsY", std::any(Int(m_yRoundedSubdivisions)));
                m_spMCCountVertsShader->SetCSParam(L"numCellsZ", std::any(Int(m_zRoundedSubdivisions)));

                m_spMCAllocVertsShader->SetCSParam(L"numCellsX", std::any(Int(m_xRoundedSubdivisions)));
                m_spMCAllocVertsShader->SetCSParam(L"numCellsY", std::any(Int(m_yRoundedSubdivisions)));
                m_spMCAllocVertsShader->SetCSParam(L"numCellsZ", std::any(Int(m_zRoundedSubdivisions)));

                m_spMCEmitVertsShader->SetCSParam(L"numCellsX", std::any(Int(m_xRoundedSubdivisions)));
                m_spMCEmitVertsShader->SetCSParam(L"numCellsY", std::any(Int(m_yRoundedSubdivisions)));
                m_spMCEmitVertsShader->SetCSParam(L"numCellsZ", std::any(Int(m_zRoundedSubdivisions)));
            }
            else
            {
                m_spMCShader->SetCSParam(L"edgeTable", ed);
                m_spMCShader->SetCSParam(L"subdivisions", std::any(Int(m_subdivisions)));
                m_spMCShader->SetCSParam(L"numCellsX", std::any(Int(m_xRoundedSubdivisions)));
                m_spMCShader->SetCSParam(L"numCellsY", std::any(Int(m_yRoundedSubdivisions)));
                m_spMCShader->SetCSParam(L"numCellsZ", std::any(Int(m_zRoundedSubdivisions)));
            }
        }

        if (m_drawIndexed)
        {
            auto spGPUCellBuffer = CreateGPUBuffer(pRenderer, EBufferType::RWStructuredBuffer, totalCells, sizeof(uint32_t), 0);
            uint8_t* pCellBuffer = (uint8_t*)new uint32_t[totalCells];
            ZeroMemory(pCellBuffer, totalCells * sizeof(uint32_t));
            spGPUCellBuffer->CopyFromCPU(pRenderer, (uint8_t*)pCellBuffer);
            std::any v(spGPUCellBuffer);
            m_spMCCountVertsShader->SetCSParam(L"cellMasks", v);
            m_spMCAllocVertsShader->SetCSParam(L"cellMasks", v);
            m_spMCEmitVertsShader->SetCSParam(L"cellMasks", v);
        }
        
        struct Counts {
            uint32_t numVertices; // Total number of unique vertices
            uint32_t numIndices; // Total number of indices (will be > numVertices * 3 due to vertex sharing)
            uint32_t numAllocatedVerts; // Index of last allocated vertex
            uint32_t numEmittedIndices; // Index of last emitted index
        };
        Counts *counts = new Counts();
        ZeroMemory(counts, sizeof(Counts));
        auto spGPUCountBuffer = CreateGPUBuffer(pRenderer, EBufferType::RWStructuredBuffer, 1, sizeof(Counts), 0);
        spGPUCountBuffer->CopyFromCPU(pRenderer, (uint8_t*)counts);
        std::any anyCounts(spGPUCountBuffer);
        if (m_drawIndexed)
        {
            m_spMCCountVertsShader->SetCSParam(L"counts", anyCounts);
            m_spMCAllocVertsShader->SetCSParam(L"counts", anyCounts);
            m_spMCEmitVertsShader->SetCSParam(L"counts", anyCounts);
        }
        else
            m_spMCShader->SetCSParam(L"counts", anyCounts);

        // Run first two stages of our pipeline. First stage counts number of referenced vertices. Second stage allocates
        // space for these on a per voxel instance in our vertex buffer.
        if (m_drawIndexed)
        {
            m_spMCCountVertsShader->Dispatch(pRenderer, m_numGroupsX, m_numGroupsY, m_numGroupsZ);
            m_spMCAllocVertsShader->Dispatch(pRenderer, m_numGroupsX, m_numGroupsY, m_numGroupsZ);
        }
        else
        {
            std::any countOnly(Int(1));
            m_spMCShader->SetCSParam(L"countOnly", countOnly);
            m_spMCShader->Dispatch(pRenderer, m_numGroupsX, m_numGroupsY, m_numGroupsZ);
        }

        spGPUCountBuffer->CopyToCPU(pRenderer, (uint8_t*)counts);

        struct Vertex
        {
            float x, y, z;
            float nx, ny, nz;
            float u, v;
        };

        CRefObj<IGPUBuffer> spGPUIndexBuffer;
        CRefObj<IGPUBuffer> spGPUVertexBuffer;
        if (counts->numVertices > 0 && 
            ((m_drawIndexed && counts->numIndices > 0) || !m_drawIndexed))
        {
            spGPUVertexBuffer = CreateGPUBuffer(pRenderer, EBufferType::RWByteAddressBuffer, counts->numVertices, sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER);
            std::any anyVB(spGPUVertexBuffer);

            if (m_drawIndexed)
            {
                m_spMCEmitVertsShader->SetCSParam(L"vertexBuffer", anyVB);
                spGPUIndexBuffer = CreateGPUBuffer(pRenderer, EBufferType::RWByteAddressBuffer, counts->numIndices, sizeof(UINT), D3D11_BIND_INDEX_BUFFER);
                std::any anyIB(spGPUIndexBuffer);
                m_spMCEmitVertsShader->SetCSParam(L"indexBuffer", anyIB);
            }
            else
            {
                m_spMCShader->SetCSParam(L"vertexBuffer", anyVB);
            }

            if (m_drawIndexed)
                m_spMCEmitVertsShader->Dispatch(pRenderer, m_numGroupsX, m_numGroupsY, m_numGroupsZ);
            else
            {
                counts->numVertices = 0;
                spGPUCountBuffer->CopyFromCPU(pRenderer, (uint8_t*)counts);
                std::any countOnly(Int(0));
                m_spMCShader->SetCSParam(L"countOnly", countOnly);
                m_spMCShader->Dispatch(pRenderer, m_numGroupsX, m_numGroupsY, m_numGroupsZ);
            }

            spGPUCountBuffer->CopyToCPU(pRenderer, (uint8_t*)counts);
        }

        if (m_postrenderCallback)
            m_postrenderCallback(pRenderCtx->GetCurrentPass());

        CRefObj<ICausticFactory> spFactory = Caustic::CreateCausticFactory();
        CRefObj<IRenderSubMesh> spRenderSubMesh = spFactory->CreateRenderSubMesh();
        MeshData md;
        if (m_drawIndexed)
        {
            md.m_spIB = spGPUIndexBuffer->GetBuffer();
            md.m_numIndices = counts->numEmittedIndices;
        }
        else
        {
            md.m_spIB = nullptr;
            md.m_numIndices = 0;
        }
        md.m_spVB = spGPUVertexBuffer->GetBuffer();
        md.m_numVertices = counts->numVertices;
        md.m_vertexSize = sizeof(Vertex);
        spRenderSubMesh->SetMeshData(md);
        spRenderSubMesh->SetName("");
        auto spShader = pRenderer->GetShaderMgr()->FindShader(L"Default");
        spRenderSubMesh->Render(pRenderer, pRenderCtx, spShader, nullptr, lights, nullptr);
    }

    void CSceneMarchingCubesElem::SetShaderParam(const wchar_t* pParamName, uint32_t value)
    {
        return;
    }
    
    void CSceneMarchingCubesElem::SetShaderParam(const wchar_t* pParamName, float value)
    {
        return;
    }
}
