//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <any>
#include <string>

export module Imaging.Image.ImageFilter.GaussianBlur;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

export namespace Caustic
{
    //**********************************************************************
    // Class: CGaussianBlur
    // Defines an image filter for performing gaussian blurring.
    //
    // Image Filter Parameters:
    // None
    //**********************************************************************
    class CGaussianBlur : public IImageFilter, public CRefCount
    {
        float m_sigma;
    public:
        CGaussianBlur() : CGaussianBlur(1.2f)
        {
        }

        CGaussianBlur(float sigma) :
            m_sigma(sigma)
        {
        }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageFilter
        //**********************************************************************
        virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams* pParams) override;
        virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override;
    };

    //**********************************************************************
    // Function: CreateGaussianBlur
    // Creates a gaussian blur filter.
    //
    // Parameters:
    // sigma - sigma defining the shape of the gaussian.
    //
    // Returns:
    // Returns the newly created filter.
    //**********************************************************************
    CRefObj<IImageFilter> CreateGaussianBlur(float sigma)
    {
        return CRefObj<IImageFilter>(new CGaussianBlur(sigma));
    }

    //**********************************************************************
    // Function: CreateGaussianBlur
    // Creates a gaussian blur filter.
    //
    // Returns:
    // Returns the a Gaussian blur filter.
    //**********************************************************************
    CRefObj<IImageFilter> CreateGaussianBlur()
    {
        return CRefObj<IImageFilter>(new CGaussianBlur());
    }

    //**********************************************************************
    // Method: Apply
    // Defines an image filter for performing gaussian blurring.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pParams - filter parameters.
    //
    // Returns:
    // Returns the filtered image.
    //**********************************************************************
    CRefObj<IImage> CGaussianBlur::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        // First compute kernel
        // A normal distribution is defined by:
        //
        //    f(x|u,s^2) = e^(-(x-u)^2/(2*s*s)) / sqrt(2*s*s*PI)
        // u = average, which is zero since we are centered at 0
        // s = sigma
        // We also know that 255*e^-5.541 ~= 1. Thus we can solve for s such that whatever is our maximum
        // x (i.e. the width of our kernel) will result in a value of 1 in our kernel weight.
        float magic = 5.541f;
        int kw = (int)(sqrtf(magic * 2 * m_sigma * m_sigma));
        int kernelWidth = kw * 2 + 1;
        int* kernelWeights = new int[kernelWidth];
        double* weights = new double[kernelWidth];
        for (int i = -kw; i <= kw; i++)
        {
            double x = (double)i;
            double e = exp(-(x * x) / (2 * m_sigma * m_sigma));
            double weight = e / sqrt(2 * m_sigma * m_sigma * 3.1415926535f);
            weights[i + kw] = weight;
            // FIXX! Need to add other side of kernel to weights
            kernelWeights[i + kw] = (int)(255.0f * weight);
        }

        auto HorizBlurPass = [&](IImage* pSrc, IImage* pDst, uint32_t w, uint32_t h)
        {
            CImageIterGeneric rowSrc(pSrc, 0, 0);
            CImageIterGeneric rowDst(pDst, 0, 0);
            for (uint32_t y = 0; y < h; y++)
            {
                CImageIterGeneric colSrc = rowSrc;
                CImageIterGeneric colDst = rowDst;
                for (uint32_t x = 0; x < w; x++)
                {
                    float sum[3] = { 0.0f , 0.0f , 0.0f };
                    CImageIterGeneric kSrc(pSrc, x - kw, y, true);
                    for (int i = -kw; i <= kw; i++)
                    {
                        if (kSrc.IsInBounds())
                        {
                            sum[0] += float(weights[i + kw] * float(kSrc.GetRed()) / 255.0f);
                            sum[1] += float(weights[i + kw] * float(kSrc.GetGreen()) / 255.0f);
                            sum[2] += float(weights[i + kw] * float(kSrc.GetBlue()) / 255.0f);
                        }
                        kSrc.Step(CImageIter::Right);
                    }
                    int r = (int)(255.0f * sum[0]);
                    int g = (int)(255.0f * sum[1]);
                    int b = (int)(255.0f * sum[2]);
                    colDst.SetRed(r);
                    colDst.SetGreen(g);
                    colDst.SetBlue(b);
                    colDst.SetAlpha(colSrc.GetAlpha());
                    colSrc.Step(CImageIter::EStepDirection::Right);
                    colDst.Step(CImageIter::EStepDirection::Right);
                }
                rowSrc.Step(CImageIter::EStepDirection::Down);
                rowDst.Step(CImageIter::EStepDirection::Down);
            }
        };
        auto VertBlurPass = [&](IImage* pSrc, IImage* pDst, uint32_t w, uint32_t h)
        {
            CImageIterGeneric colSrc(pSrc, 0, 0);
            CImageIterGeneric colDst(pDst, 0, 0);
            for (uint32_t x = 0; x < w; x++)
            {
                CImageIterGeneric rowSrc = colSrc;
                CImageIterGeneric rowDst = colDst;
                for (uint32_t y = 0; y < h; y++)
                {
                    float sum[3] = { 0.0f , 0.0f , 0.0f };
                    CImageIterGeneric kSrc(pSrc, x, y - kw, true);
                    for (int i = -kw; i <= kw; i++)
                    {
                        if (kSrc.IsInBounds())
                        {
                            sum[0] += float(weights[i + kw] * float(kSrc.GetRed()) / 255.0f);
                            sum[1] += float(weights[i + kw] * float(kSrc.GetGreen()) / 255.0f);
                            sum[2] += float(weights[i + kw] * float(kSrc.GetBlue()) / 255.0f);
                        }
                        kSrc.Step(CImageIter::Down);
                    }
                    rowDst.SetRed((int)(255.0f * sum[0]));
                    rowDst.SetGreen((int)(255.0f * sum[1]));
                    rowDst.SetBlue((int)(255.0f * sum[2]));
                    rowDst.SetAlpha(rowSrc.GetAlpha());
                    rowSrc.Step(CImageIter::EStepDirection::Down);
                    rowDst.Step(CImageIter::EStepDirection::Down);
                }
                colSrc.Step(CImageIter::EStepDirection::Right);
                colDst.Step(CImageIter::EStepDirection::Right);
            }
        };
        uint32_t w = pImage->GetWidth();
        uint32_t h = pImage->GetHeight();
        EImageType t = pImage->GetImageType();
        CRefObj<IImage> spImage = CreateImage(w, h, t);
        HorizBlurPass(pImage, spImage, w, h);
        CRefObj<IImage> spImage2 = CreateImage(w, h, t);
        VertBlurPass(spImage, spImage2, w, h);
        return spImage2;
    }

    bool CGaussianBlur::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        return false; // Inplace transformation not supported
    }
}
