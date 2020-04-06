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

namespace Caustic
{
    void CImage::Colorize(IImage **ppImage, int maxDepth /* = 8000 */)
    {
        auto Saturate = [](int value) -> int
        {
            if (value < 0)
                return 0;
            if (value > 255)
                return 255;
            return value;
        };
        auto Abs = [](int v)->int
        {
            if (v < 0)
                return -v;
            return v;
        };
        unsigned short maxDepthVal = 0xFFFF;
        unsigned short minDepthVal = 0;
        int bufSize = m_width * m_height * 4;
        uint8* data = new uint8[bufSize];
        unsigned short* pSrcRow = (unsigned short*)m_spData.get();
        uint8* pDstRow = data;
        for (int iy = 0; iy < m_height; iy++)
        {
            unsigned short* pSrcCol = pSrcRow;
            uint8* pDstCol = pDstRow;
            for (int ix = 0; ix < m_width; ix++)
            {
                int depth = pSrcCol[0];
                int normalizedDepth = (depth >= minDepthVal && depth <= maxDepthVal) ? (depth * 1024 / maxDepth) : 0;
                pDstCol[0] = (uint8)Saturate(384 - (int)Abs(normalizedDepth - 256));
                pDstCol[1] = (uint8)Saturate(384 - (int)Abs(normalizedDepth - 512));
                pDstCol[2] = (uint8)Saturate(384 - (int)Abs(normalizedDepth - 768));
                pDstCol[3] = 255;
                pSrcCol++;
                pDstCol += 4;
            }
            pSrcRow += m_width;
            pDstRow += m_width * 4;
        }
        CRefObj<IImage> spImage;
        Caustic::CreateImage(m_width, m_height, 32, &spImage);
        memcpy(spImage->GetData(), data, m_width * m_height * 4);
        *ppImage = spImage.Detach();
    }
}