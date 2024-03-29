//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Core.Error;
import Imaging.Image.IImage;
import Imaging.Image.Image;
#include <atlbase.h>
#include <wincodec.h>

namespace Caustic
{
#ifdef TODO
    void CreateSLIC(IImage *pImage, IImage **ppImage)
    {
        std::unique_ptr<CIntegralImage> spIntegralImage(new CIntegralImage(pImage->GetWidth(), pImage->GetHeight()));
        uint32_t *pDstRow = (uint32_t*)spIntegralImage->GetData();
        BYTE *pSrcRow = pImage->GetData();
        int w = (int)spIntegralImage->GetWidth();
        for (uint32_t y = 0; y < pImage->GetHeight(); y++)
        {
            uint32_t *pDstCol = pDstRow;
            BYTE *pSrcCol = pSrcRow;
            for (uint32_t x = 0; x < pImage->GetWidth(); x++)
            {
                uint32_t sum[3];
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
