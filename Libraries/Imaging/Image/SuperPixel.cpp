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
#include <wincodec.h>

namespace Caustic
{
#ifdef TODO
    void CreateSLIC(IImage *pImage, IImage **ppImage)
    {
        std::unique_ptr<CIntegralImage> spIntegralImage(new CIntegralImage(pImage->GetWidth(), pImage->GetHeight()));
        uint32 *pDstRow = (uint32*)spIntegralImage->GetData();
        BYTE *pSrcRow = pImage->GetData();
        int w = (int)spIntegralImage->GetWidth();
        for (uint32 y = 0; y < pImage->GetHeight(); y++)
        {
            uint32 *pDstCol = pDstRow;
            BYTE *pSrcCol = pSrcRow;
            for (uint32 x = 0; x < pImage->GetWidth(); x++)
            {
                uint32 sum[3];
                sum[0] = pSrcCol[0];
                sum[1] = pSrcCol[1];
                sum[2] = pSrcCol[2];
                if (x > 0)
                {
                    sum[0] += pDstCol[-3];
                    sum[1] += pDstCol[-2];
                    sum[2] += pDstCol[-1];
                }
                if (y > 0)
                {
                    sum[0] += pDstCol[-3 * w + 0];
                    sum[1] += pDstCol[-3 * w + 1];
                    sum[2] += pDstCol[-3 * w + 2];
                }
                if (x > 0 && y > 0)
                {
                    sum[0] -= pDstCol[-3 * w + -3];
                    sum[1] -= pDstCol[-3 * w + -2];
                    sum[2] -= pDstCol[-3 * w + -1];
                }
                pDstCol[0] = sum[0];
                pDstCol[1] = sum[1];
                pDstCol[2] = sum[2];
                pSrcCol += pImage->GetBytesPerPixel();
                pDstCol += 3;
            }
            pSrcRow += pImage->GetStride();
            pDstRow += 3 * w;
        }
        spIntegralImage->m_spImage = pImage;
        *ppImage = spIntegralImage.release();
        (*ppImage)->AddRef();
    }
#endif
}
