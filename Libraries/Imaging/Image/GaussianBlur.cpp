//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Image.h"
#include "ImageImpl.h"
#include "ImageFilter.h"
#include "ImageIter.h"

namespace Caustic
{
    //**********************************************************************
    // Class: CGaussianBlur
    // Defines an image filter for performing gaussian blurring.
    //**********************************************************************
    class CGaussianBlur : public IImageFilter, public CRefCount
    {
        float m_sigma;
    public:
        CGaussianBlur(float sigma) :
            m_sigma(sigma)
        {
        }
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageFilter
        //**********************************************************************
        virtual void Apply(IImage* pImage, IImage* pMask, IImage** ppResult) override;
        virtual bool ApplyInPlace(IImage* pImage, IImage* pMask) override;
    };

    //**********************************************************************
    // Function: CreateGaussianBlur
    // Creates a gaussian blur filter.
    //
    // Parameters:
    // sigma - sigma defining the shape of the gaussian.
    // ppFilter - returns the newly created filter.
    //**********************************************************************
    void CreateGaussianBlur(float sigma, IImageFilter** ppFilter)
    {
        std::unique_ptr<CGaussianBlur> spFilter(new CGaussianBlur(sigma));
        *ppFilter = spFilter.release();
        (*ppFilter)->AddRef();
    }

    //**********************************************************************
    // Method: Apply
    // Defines an image filter for performing gaussian blurring.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pMask - unused.
    // ppResult - returns the filtered image.
    //**********************************************************************
    void CGaussianBlur::Apply(IImage* pImage, IImage* pMask, IImage** ppResult)
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
        int kw = (int)(sqrt(magic * 2 * m_sigma * m_sigma));
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

        auto BlurPass = [&](IImage* pSrc, IImage* pDst, uint32 h, uint32 w, uint32 rowOffset, uint32 colOffset)
        {
            CImageIterGeneric rowSrc(pSrc, 0, 0);
            CImageIterGeneric rowDst(pDst, 0, 0);
            for (uint32 y = 0; y < h; y++)
            {
                CImageIterGeneric colSrc = rowSrc;
                CImageIterGeneric colDst = rowDst;
                for (uint32 x = 0; x < w; x++)
                {
                    float sum[3] = { 0.0f , 0.0f , 0.0f };
                    CImageIterGeneric kSrc(pSrc, x - kw, y);
                    for (int i = -kw; i <= kw; i++)
                    {
                        sum[0] += float(weights[i + kw] * float(kSrc.GetRed()) / 255.0f);
                        sum[1] += float(weights[i + kw] * float(kSrc.GetGreen()) / 255.0f);
                        sum[2] += float(weights[i + kw] * float(kSrc.GetBlue()) / 255.0f);
                        kSrc.StepWithBoundsCheck(CImageIter::Right);
                    }
                    colDst.SetRed((int)(255.0f * sum[0]));
                    colDst.SetGreen((int)(255.0f * sum[1]));
                    colDst.SetBlue((int)(255.0f * sum[2]));
                    colDst.SetAlpha(colSrc.GetAlpha());
                    colSrc.Step(CImageIter::EStepDirection::Right);
                    colDst.Step(CImageIter::EStepDirection::Right);
                }
                rowSrc.Step(CImageIter::EStepDirection::Down);
                rowDst.Step(CImageIter::EStepDirection::Down);
            }
        };
        uint32 bytesPerPixel = pImage->GetBytesPerPixel();
        uint32 stride = pImage->GetStride();
        CRefObj<IImage> spImage;
        CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP(), &spImage);
        BlurPass(pImage, spImage, pImage->GetHeight(), pImage->GetWidth(), stride, bytesPerPixel);
        CRefObj<IImage> spImage2;
        CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetBPP(), &spImage2);
        BlurPass(spImage, spImage2, pImage->GetWidth(), pImage->GetHeight(), bytesPerPixel, stride);
        *ppResult = spImage2.Detach();
    }

    bool CGaussianBlur::ApplyInPlace(IImage* pImage, IImage* pMask)
    {
        return false; // Inplace transformation not supported
    }
}
