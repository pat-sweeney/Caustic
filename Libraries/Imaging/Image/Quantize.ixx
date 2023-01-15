//**********************************************************************
// Copyright Patrick Sweeney 1996-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <winerror.h>
#include <memory>
#include <any>
#include <map>
#include <string>

export module Imaging.Image.ImageFilter.Quantize;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Geometry.Rast.Bresenham;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.ImageFilter;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Class: CQuantizeFilter
    // Defines an image filter that performs color quantization via K-Means
    //
    // Image Filter Parameters:
    // "NumClusters" : <int> - Number of clusters (i.e. K in K-Means)
    //**********************************************************************
    class CQuantizeFilter : public IImageFilter, public CRefCount
    {
        template <typename T>
        void Quantize(IImage* pImage, IImage* pDestImage, ImageFilterParams* pParams);
    public:
        CQuantizeFilter()
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
        virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams* pParams) override;
        virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override;
    };

    //**********************************************************************
    // Function: CreateQuantizeFilter
    // Creates a filter that performs color quantization via K-Means
    //
    // Returns:
    // Returns filter that applies motion blur to an image
    //**********************************************************************
    CRefObj<IImageFilter> CreateQuantizeFilter()
    {
        return CRefObj<IImageFilter>(new CQuantizeFilter());
    }

    //**********************************************************************
    template <typename T>
    void CQuantizeFilter::Quantize(IImage* pImage, IImage *pDestImage, ImageFilterParams* pParams)
    {
        int K = 256;
        int maxIterations = 5;
        float maxPercent = 0.05f;
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it;
            it = pParams->params.find("NumClusters");
            if (it != pParams->params.end())
                K = std::any_cast<int>(it->second);
            it = pParams->params.find("MaxIterations");
            if (it != pParams->params.end())
                maxIterations = std::any_cast<int>(it->second);
            it = pParams->params.find("PercentChange");
            if (it != pParams->params.end())
                maxPercent = std::any_cast<float>(it->second);
        }
        if (K > 256)
            CT(E_UNEXPECTED);

        struct Cluster
        {
            uint32 meanR, meanG, meanB;
            int numElements;
        };
        Cluster* pClusters = new Cluster[K];
        Cluster* pNewClusters = new Cluster[K];

        int w = pImage->GetWidth();
        int h = pImage->GetHeight();

        // First pick K random points as our cluster centers
        for (int i = 0; i < K; i++)
        {
            int x = rand() % w;
            int y = rand() % h;
            T iter(pImage, x, y);
            pClusters[i].meanR = iter.GetRed();
            pClusters[i].meanG = iter.GetGreen();
            pClusters[i].meanB = iter.GetBlue();
            pClusters[i].numElements = 1;
            pNewClusters[i].meanR = 0;
            pNewClusters[i].meanG = 0;
            pNewClusters[i].meanB = 0;
            pNewClusters[i].numElements = 0;
        }

        CRefObj<IImage> spIndices = CreateImage(w, h, EImageType::Gray_8bpp);
        for (int iter = 0; iter < maxIterations; iter++)
        {
            int pixelsReassigned = 0;
            T srcRow(pImage, 0, 0);
            CImageIter8 indexRow(spIndices, 0, 0);
            for (int y = 0; y < h; y++)
            {
                T srcCol = srcRow;
                CImageIter8 indexCol = indexRow;
                for (int x = 0; x < w; x++)
                {
                    int minDist = INT_MAX;
                    int minCluster = 0;
                    uint8 r = srcCol.GetRed();
                    uint8 g = srcCol.GetGreen();
                    uint8 b = srcCol.GetBlue();
                    for (int j = 0; j < K; j++)
                    {
                        int dr = (int)r - pClusters[j].meanR;
                        int dg = (int)g - pClusters[j].meanG;
                        int db = (int)b - pClusters[j].meanB;
                        int dist = dr * dr + dg * dg + db * db;
                        if (dist < minDist)
                        {
                            minDist = dist;
                            minCluster = j;
                        }
                    }
                    if (indexCol.GetGray() != minCluster)
                        pixelsReassigned++;
                    indexCol.SetGray((uint8)minCluster);
                    pNewClusters[minCluster].meanR += r;
                    pNewClusters[minCluster].meanG += g;
                    pNewClusters[minCluster].meanB += b;
                    pNewClusters[minCluster].numElements++;
                    srcCol.Step(CImageIter::EStepDirection::Right);
                    indexCol.Step(CImageIter::EStepDirection::Right);
                }
                srcRow.Step(CImageIter::EStepDirection::Down);
                indexRow.Step(CImageIter::EStepDirection::Down);
            }
            for (int j = 0; j < K; j++)
            {
                if (pNewClusters[j].numElements != 0)
                {
                    pNewClusters[j].meanR /= pNewClusters[j].numElements;
                    pNewClusters[j].meanG /= pNewClusters[j].numElements;
                    pNewClusters[j].meanB /= pNewClusters[j].numElements;
                }
                pClusters[j].meanR = 0;
                pClusters[j].meanG = 0;
                pClusters[j].meanB = 0;
                pClusters[j].numElements = 0;
            }
            Cluster* pTmp = pClusters;
            pClusters = pNewClusters;
            pNewClusters = pTmp;
            float percentChanged = float(pixelsReassigned) / float(w * h);
            if (percentChanged < maxPercent)
                break;
        }

        // Remap image
        T dstRow(pDestImage, 0, 0);
        CImageIter8 indexRow(spIndices, 0, 0);
        for (int y = 0; y < h; y++)
        {
            T dstCol = dstRow;
            CImageIter8 indexCol = indexRow;
            for (int x = 0; x < w; x++)
            {
                int j = indexCol.GetGray();
                dstCol.SetRed(pClusters[j].meanR);
                dstCol.SetGreen(pClusters[j].meanG);
                dstCol.SetBlue(pClusters[j].meanB);
                indexCol.Step(CImageIter::EStepDirection::Right);
                dstCol.Step(CImageIter::EStepDirection::Right);
            }
            indexRow.Step(CImageIter::EStepDirection::Down);
            dstRow.Step(CImageIter::EStepDirection::Down);
        }
    }

    //**********************************************************************
    // Method: Apply
    // See <IImageFilter::Apply>
    //**********************************************************************
    CRefObj<IImage> CQuantizeFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        CRefObj<IImage> spDestImage = CreateImage(pImage->GetWidth(), pImage->GetHeight(), pImage->GetImageType());
        if (pImage->GetBPP() == 24)
            Quantize<CImageIter24>(pImage, spDestImage, pParams);
        else if (pImage->GetBPP() == 32)
            Quantize<CImageIter32>(pImage, spDestImage, pParams);
        else
        {
            CT(E_NOTIMPL);
            return CRefObj<IImage>(nullptr);
        }
        return spDestImage;
    }

    //**********************************************************************
    // Method: ApplyInPlace
    // See <IImageFilter::ApplyInPlace>
    //**********************************************************************
    bool CQuantizeFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        if (pImage->GetBPP() == 24)
            Quantize<CImageIter24>(pImage, pImage, pParams);
        else if (pImage->GetBPP() == 32)
            Quantize<CImageIter32>(pImage, pImage, pParams);
        else
        {
            CT(E_NOTIMPL);
        }
        return true;
    }
}
