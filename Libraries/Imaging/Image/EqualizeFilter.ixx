//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <winerror.h>
#include <memory>
#include <any>
#include <string>
#include <map>

export module Imaging.Image.ImageFilter.EqualizeFilter;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.ImageFilter;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Class: CEqualizeFilter
    // Defines an image filter for performing histogram equalization
    //
    // Image Filter Parameters:
    // "UseLuminance"   : <bool> - Indicates whether equalization is done on the luminance or each color channel
    //**********************************************************************
    class CEqualizeFilter: public IImageFilter, public CRefCount
    {
        template <typename T>
        void Equalize(IImage* pImage, ImageFilterParams* pParams);
    public:
        CEqualizeFilter()
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
    // Function: CreateEqualizeFilter
    // Creates a filter that performs histogram equalization on an image
    //
    // Returns:
    // Returns filter that performs histogram equalization
    //**********************************************************************
    CRefObj<IImageFilter> CreateEqualizeFilter()
    {
        return CRefObj<IImageFilter>(new CEqualizeFilter());
    }

    //**********************************************************************
    template <typename T>
    void CEqualizeFilter::Equalize(IImage* pImage, ImageFilterParams* pParams)
    {
        bool useLuminance = false;
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it = pParams->params.find("UseLuminance");
            if (it != pParams->params.end())
                useLuminance = std::any_cast<bool>(it->second);
        }
        uint32_t rhistogram[256] = {};
        uint32_t ghistogram[256] = {};
        uint32_t bhistogram[256] = {};
        uint32_t rmapping[256] = {};
        uint32_t gmapping[256] = {};
        uint32_t bmapping[256] = {};
        bool hasMask = (pParams && pParams->spMask != nullptr) ? true : false;

        T riter(pImage, 0, 0);
        CImageIter1 riter1;
        if (hasMask)
            riter1 = CImageIter1(pParams->spMask, 0, 0);
        uint32_t y = pImage->GetHeight();
        while (y--)
        {
            T citer = riter;
            CImageIter1 citer1 = riter1;
            uint32_t x = pImage->GetWidth();
            if (useLuminance)
            {
                while (x--)
                {
                    RGBColor clr;

                    if (!hasMask || (hasMask && citer1.GetBit()))
                    {
                        clr.r = citer.GetRed();
                        clr.g = citer.GetGreen();
                        clr.b = citer.GetBlue();
                        YIQColor yiq(clr);
                        rhistogram[yiq.y]++;
                    }
                    citer.Step(CImageIter::Right);
                    citer1.Step(CImageIter::Right);
                }
            }
            else
            {
                while (x--)
                {
                    if (!hasMask || (hasMask && citer1.GetBit()))
                    {
                        rhistogram[citer.GetRed()]++;
                        ghistogram[citer.GetGreen()]++;
                        bhistogram[citer.GetBlue()]++;
                    }
                    citer.Step(CImageIter::Right);
                    citer1.Step(CImageIter::Right);
                }
            }
            riter.Step(CImageIter::Down);
            riter1.Step(CImageIter::Down);
        }
        int32_t rprob = 0;
        int32_t gprob = 0;
        int32_t bprob = 0;
        uint32_t npixels = pImage->GetHeight() * pImage->GetWidth();
        for (int k = 0; k < 256; k++)
        {
            rprob += (255 * rhistogram[k] / npixels);
            rmapping[k] = (uint8_t)rprob;
            if (!useLuminance)
            {
                gprob += (255 * ghistogram[k] / npixels);
                gmapping[k] = (uint8_t)gprob;
                bprob += (255 * bhistogram[k] / npixels);
                bmapping[k] = (uint8_t)bprob;
            }
        }
        //
        // Remap image luminance
        //
        riter = T(pImage, 0, 0);
        if (hasMask)
            riter1 = CImageIter1(pParams->spMask, 0, 0);
        y = pImage->GetHeight();
        while (y--)
        {
            T citer = riter;
            CImageIter1 citer1 = riter1;
            int x = pImage->GetWidth();
            if (useLuminance)
            {
                while (x--)
                {
                    RGBColor clr;

                    if (!hasMask || (hasMask && citer1.GetBit()))
                    {
                        clr.r = citer.GetRed();
                        clr.g = citer.GetGreen();
                        clr.b = citer.GetBlue();
                        YIQColor yiq(clr);
                        yiq.y = rmapping[yiq.y];
                        clr = RGBColor(yiq);
                        citer.SetRed(clr.r);
                        citer.SetGreen(clr.g);
                        citer.SetBlue(clr.b);
                    }
                    citer.Step(CImageIter::Right);
                    citer1.Step(CImageIter::Right);
                }
            }
            else
            {
                while (x--)
                {
                    if (!hasMask || (hasMask && citer1.GetBit()))
                    {
                        citer.SetRed(rmapping[citer.GetRed()]);
                        citer.SetGreen(gmapping[citer.GetGreen()]);
                        citer.SetBlue(bmapping[citer.GetBlue()]);
                    }
                    citer.Step(CImageIter::Right);
                    citer1.Step(CImageIter::Right);
                }
            }
            riter.Step(CImageIter::Down);
            riter1.Step(CImageIter::Down);
        }
        return;
    }

    //**********************************************************************
    // Method: Apply
    // See <IImageFilter::Apply>
    //**********************************************************************
    CRefObj<IImage> CEqualizeFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        CRefObj<IImage> spDest = pImage->Clone();
        ApplyInPlace(spDest, pParams);
        return spDest;
    }
    
    //**********************************************************************
    // Method: ApplyInPlace
    // See <IImageFilter::ApplyInPlace>
    //**********************************************************************
    bool CEqualizeFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        if (pImage->GetBPP() == 24)
            Equalize<CImageIter24>(pImage, pParams);
        else if (pImage->GetBPP() == 32)
            Equalize<CImageIter32>(pImage, pParams);
        else
            CT(E_NOTIMPL);
        return true;
    }
}
