//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Image.h"
#include "ImageImpl.h"
#include <atlbase.h>

namespace Caustic
{
    void CImage::ShowAlpha(int boxSize, IImage **ppResult)
    {
        CreateImage(GetWidth(), GetHeight(), ppResult);
        BYTE *rowSrc = GetData();
        BYTE *rowDst = (*ppResult)->GetData();
        uint32 bytesPerPixel = GetBytesPerPixel();
        uint32 stride = GetStride();
        for (uint32 y = 0; y < GetHeight(); y++)
        {
            BYTE *colSrc = rowSrc;
            BYTE *colDst = rowDst;
            for (uint32 x = 0; x < GetWidth(); x++)
            {
                int clr = (((x / boxSize) & 1) == ((y / boxSize) & 1)) ? 255 : 127;
                colDst[0] = (colSrc[0] * colSrc[3] + clr * (255 - colSrc[3])) / 255;
                colDst[1] = (colSrc[1] * colSrc[3] + clr * (255 - colSrc[3])) / 255;
                colDst[2] = (colSrc[2] * colSrc[3] + clr * (255 - colSrc[3])) / 255;
                colDst[3] = 255;
                colSrc += bytesPerPixel;
                colDst += bytesPerPixel;
            }
            rowSrc += stride;
            rowDst += stride;
        }
    }
}
