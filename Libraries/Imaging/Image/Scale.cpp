//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <any>
#include <string>
#include <map>

module Imaging.Image.ImageFilter.Scale;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

namespace Caustic
{
    template <typename Iter>
    void CScaleFilter::Scale(IImage* pSrcImage, IImage* pDstImage, EScaleUpMode scaleUpMode, EScaleDownMode scaleDownMode, float sx, float sy)
    {
        Iter srcciter;
        Iter srcriter;
        Iter dstciter;
        Iter dstriter;
        int xerr, yerr;

        int32_t srcW = (int32_t)pSrcImage->GetWidth();
        int32_t srcH = (int32_t)pSrcImage->GetHeight();
        int32_t dstW = (int32_t)pDstImage->GetWidth();
        int32_t dstH = (int32_t)pDstImage->GetHeight();
        if (sx > 1.0f || sy > 1.0f)
        {
            switch (scaleUpMode)
            {
            case EScaleUpMode::PixelReplicate:
            {
                //
                // First determine relative starting position for initializing error
                //
                IImageBase* p = pSrcImage;
                int startx = 0;
                int starty = 0;
                while (p)
                {
                    startx += p->GetSubX();
                    starty += p->GetSubY();
                    p = p->GetParent();
                }

                //
                // Initialize error
                //
                yerr = -dstH;
                while (starty)
                {
                    yerr += srcH;
                    while (yerr >= 0)
                        yerr -= dstH;
                    starty--;
                }
                int inityerr = yerr;
                xerr = -dstW;
                while (startx)
                {
                    xerr += srcW;
                    while (xerr >= 0)
                        xerr -= dstW;
                    startx--;
                }
                int initxerr = xerr;

                //
                // Now scale image
                //
                srcriter.Setup(pSrcImage, 0, 0);
                dstriter.Setup(pDstImage, 0, 0);
                yerr = inityerr;
                for (int y = 0; y < dstH; y++)
                {
                    srcciter = srcriter;
                    dstciter = dstriter;
                    xerr = initxerr;
                    for (int x = 0; x < dstW; x++)
                    {
                        dstciter.SetRed(srcciter.GetRed());
                        dstciter.SetGreen(srcciter.GetGreen());
                        dstciter.SetBlue(srcciter.GetBlue());
                        xerr += srcW;
                        while (xerr >= 0)
                        {
                            srcciter.Step(CImageIter::Right);
                            xerr -= dstW;
                        }
                        dstciter.Step(CImageIter::Right);
                    }
                    yerr += srcH;
                    while (yerr >= 0)
                    {
                        srcriter.Step(CImageIter::Down);
                        yerr -= dstH;
                    }
                    dstriter.Step(CImageIter::Down);
                }
            }
            break;
            case EScaleUpMode::Bilinear:
            {
                Iter srcriter0;
                Iter srcriter1;
                Iter srcciter00;
                Iter srcciter01;
                Iter srcciter10;
                Iter srcciter11;

                srcriter0.Setup(pSrcImage, 0, 0);
                srcriter1.Setup(pSrcImage, 0, 0);
                dstriter.Setup(pDstImage, 0, 0);
                yerr = -dstH;
                for (int y = 0; y < dstH; y++)
                {
                    srcciter00 = srcriter0;
                    srcciter01 = srcriter0;
                    srcciter10 = srcriter1;
                    srcciter11 = srcriter1;
                    dstciter = dstriter;
                    xerr = -dstW;
                    for (int x = 0; x < dstW; x++)
                    {
                        int r00 = srcciter00.GetRed();
                        int r01 = srcciter01.GetRed();
                        int r10 = srcciter10.GetRed();
                        int r11 = srcciter11.GetRed();
                        int g00 = srcciter00.GetGreen();
                        int g01 = srcciter01.GetGreen();
                        int g10 = srcciter10.GetGreen();
                        int g11 = srcciter11.GetGreen();
                        int b00 = srcciter00.GetBlue();
                        int b01 = srcciter01.GetBlue();
                        int b10 = srcciter10.GetBlue();
                        int b11 = srcciter11.GetBlue();
                        int rdiff0 = r01 - r00;
                        int gdiff0 = g01 - g00;
                        int bdiff0 = b01 - b00;
                        int rdiff1 = r11 - r10;
                        int gdiff1 = g11 - g10;
                        int bdiff1 = b11 - b10;
                        int r0 = r00 + (rdiff0 - (rdiff0 * xerr) / -dstW);
                        int g0 = g00 + (gdiff0 - (gdiff0 * xerr) / -dstW);
                        int b0 = b00 + (bdiff0 - (bdiff0 * xerr) / -dstW);
                        int r1 = r10 + (rdiff1 - (rdiff1 * xerr) / -dstW);
                        int g1 = g10 + (gdiff1 - (gdiff1 * xerr) / -dstW);
                        int b1 = b10 + (bdiff1 - (bdiff1 * xerr) / -dstW);
                        int rdiff = r1 - r0;
                        int gdiff = g1 - g0;
                        int bdiff = b1 - b0;
                        int r = r0 + (rdiff - (rdiff * yerr) / -dstH);
                        int g = g0 + (gdiff - (gdiff * yerr) / -dstH);
                        int b = b0 + (bdiff - (bdiff * yerr) / -dstH);
                        dstciter.SetRed(r);
                        dstciter.SetGreen(g);
                        dstciter.SetBlue(b);
                        xerr += srcW;
                        if (xerr >= 0)
                        {
                            srcciter00 = srcciter01;
                            srcciter10 = srcciter11;
                            while (xerr >= 0)
                            {
                                srcciter01.Step(CImageIter::Right);
                                srcciter11.Step(CImageIter::Right);
                                xerr -= dstW;
                            }
                        }
                        dstciter.Step(CImageIter::Right);
                    }
                    yerr += srcH;
                    if (yerr >= 0)
                    {
                        srcriter0 = srcriter1;
                        while (yerr >= 0)
                        {
                            srcriter1.Step(CImageIter::Down);
                            yerr -= dstH;
                        }
                    }
                    dstriter.Step(CImageIter::Down);
                }
            }
            break;
            }
        }
        else
        {
            switch (scaleDownMode)
            {
            case EScaleDownMode::NearestNeighbor:
                srcriter.Setup(pSrcImage, 0, 0);
                dstriter.Setup(pDstImage, 0, 0);
                yerr = -dstH;
                for (int y = 0; y < dstH; y++)
                {
                    srcciter = srcriter;
                    dstciter = dstriter;
                    xerr = -dstW;
                    for (int x = 0; x < dstW; x++)
                    {
                        int r = srcciter.GetRed();
                        int g = srcciter.GetGreen();
                        int b = srcciter.GetBlue();
                        dstciter.SetRed(r);
                        dstciter.SetGreen(g);
                        dstciter.SetBlue(b);
                        xerr += srcW;
                        while (xerr >= 0)
                        {
                            srcciter.Step(CImageIter::Right);
                            xerr -= dstW;
                        }
                        dstciter.Step(CImageIter::Right);
                    }
                    yerr += srcH;
                    while (yerr >= 0)
                    {
                        srcriter.Step(CImageIter::Down);
                        yerr -= dstH;
                    }
                    dstriter.Step(CImageIter::Down);
                }
                break;
            case EScaleDownMode::Jitter:
                _ASSERT(false); // TODO: Need to add
                break;
            case EScaleDownMode::Average:
            {
                uint32_t sumr, sumg, sumb;
                uint32_t pixels;
                int32_t xerr2, yerr2;
                Iter citer, riter;
                uint8_t r, g, b;

                srcriter.Setup(pSrcImage, 0, 0);
                dstriter.Setup(pDstImage, 0, 0);
                yerr = -dstH;
                for (int y = 0; y < dstH; y++)
                {
                    srcciter = srcriter;
                    dstciter = dstriter;
                    xerr = -dstW;
                    for (int x = 0; x < dstW; x++)
                    {
                        pixels = 0;
                        sumr = sumg = sumb = 0;
                        riter = srcciter;
                        yerr2 = yerr + srcH;
                        if (yerr2 >= 0)
                        {
                            while (yerr2 >= 0)
                            {
                                citer = riter;
                                xerr2 = xerr + srcW;
                                while (xerr2 >= 0)
                                {
                                    sumr += (int)citer.GetRed();
                                    sumg += (int)citer.GetGreen();
                                    sumb += (int)citer.GetBlue();
                                    pixels++;
                                    citer.Step(CImageIter::Right);
                                    xerr2 -= dstW;
                                }
                                riter.Step(CImageIter::Down);
                                yerr2 -= dstH;
                            }
                            r = (uint8_t)(sumr / pixels);
                            g = (uint8_t)(sumg / pixels);
                            b = (uint8_t)(sumb / pixels);
                        }
                        dstciter.SetRed(r);
                        dstciter.SetGreen(g);
                        dstciter.SetBlue(b);

                        xerr += srcW;
                        while (xerr >= 0)
                        {
                            srcciter.Step(CImageIter::Right);
                            xerr -= dstW;
                        }
                        dstciter.Step(CImageIter::Right);
                    }
                    yerr += srcH;
                    while (yerr >= 0)
                    {
                        srcriter.Step(CImageIter::Down);
                        yerr -= dstH;
                    }
                    dstriter.Step(CImageIter::Down);
                }
            }
            break;
            }
        }
    }
    
    //**********************************************************************
    // Method: Apply
    // Defines an image filter for scaling an image.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pParams - filter parameters.
    //
    // Returns:
    // Returns the filtered image.
    //**********************************************************************
    CRefObj<IImage> CScaleFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        float scaleX = 1.0f;
        float scaleY = 1.0f;
        EScaleUpMode scaleUpMode = EScaleUpMode::Bilinear;
        EScaleDownMode scaleDownMode = EScaleDownMode::Average;
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it;
            it = pParams->params.find("ScaleX");
            if (it != pParams->params.end())
                scaleX = std::any_cast<float>(it->second);
            it = pParams->params.find("ScaleY");
            if (it != pParams->params.end())
                scaleY = std::any_cast<float>(it->second);
            it = pParams->params.find("ScaleUpMode");
            if (it != pParams->params.end())
                scaleUpMode = std::any_cast<EScaleUpMode>(it->second);
            it = pParams->params.find("ScaleDownMode");
            if (it != pParams->params.end())
                scaleDownMode = std::any_cast<EScaleDownMode>(it->second);
        }
        uint32_t newWidth = (int)ceil(scaleX * pImage->GetWidth());
        uint32_t newHeight = (int)ceil(scaleY * pImage->GetHeight());
        CRefObj<IImage> spScaledImage = CreateImageImpl(newWidth, newHeight, pImage->GetImageType());

        switch (pImage->GetBPP())
        {
        case 24:
            Scale<CImageIter24>(pImage, spScaledImage, scaleUpMode, scaleDownMode, scaleX, scaleY);
            break;
        case 32:
            Scale<CImageIter32>(pImage, spScaledImage, scaleUpMode, scaleDownMode, scaleX, scaleY);
            break;
        }
        return spScaledImage;
    }

    bool CScaleFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        return false; // Inplace transformation not supported
    }
}
