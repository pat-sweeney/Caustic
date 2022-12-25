//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define _USE_MATH_DEFINES 
#include <math.h>
#include <any>
#include <string>
#include <map>

module Imaging.Image.ImageFilter.Rotate;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Matrix.Matrix3x2;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.Transform;

namespace Caustic
{
    template<typename Iter>
    CRefObj<IImage> CRotateFilter::Rotate(IImage *pImage, float degrees)
    {
        while (degrees >= 360.0f)
            degrees -= 360.0f;
        while (degrees < 0.0f)
            degrees += 360.0f;
        if (degrees == 90.0f)
        {
            uint32 bytesPerPixel = pImage->GetBytesPerPixel();
            CRefObj<IImage> spRotImage = CreateImageImpl(pImage->GetHeight(), pImage->GetWidth(), bytesPerPixel * 8);
            //
            //  A                         
            //  +-------------+          +-----------+ B
            //  |             |          |           |
            //  |             |     ==>  |           |  
            //  |             |          |           |
            //  +-------------+ B        |           |
            //                           |           |
            //                         A +-----------+
            //
            Iter rowSrcIter(pImage, 0, 0);
            Iter colDstIter(spRotImage, 0, spRotImage->GetHeight() - 1);
            int y = pImage->GetHeight();
            while (y--)
            {
                Iter colSrcIter = rowSrcIter;
                Iter rowDstIter = colDstIter;
                int x = pImage->GetWidth();
                while (x--)
                {
                    rowDstIter.SetRed(colSrcIter.GetRed());
                    rowDstIter.SetGreen(colSrcIter.GetGreen());
                    rowDstIter.SetBlue(colSrcIter.GetBlue());
                    colSrcIter.Step(CImageIter::Right);
                    rowDstIter.Step(CImageIter::Up);
                }
                rowSrcIter.Step(CImageIter::Down);
                colDstIter.Step(CImageIter::Right);
            }
            return spRotImage;
        }
        if (degrees == 180.0f)
        {
            uint32 bytesPerPixel = pImage->GetBytesPerPixel();
            CRefObj<IImage> spRotImage = CreateImageImpl(pImage->GetWidth(), pImage->GetHeight(), bytesPerPixel * 8);
            //
            //  A                         
            //  +-------------+        B +-------------+
            //  |             |          |             |
            //  |             |    ==>   |             |
            //  |             |          |             |
            //  +-------------+ B        +-------------+ A
            //
            Iter rowSrcIter(pImage, 0, 0);
            Iter rowDstIter(spRotImage, spRotImage->GetWidth() - 1, spRotImage->GetHeight() - 1);
            int y = pImage->GetHeight();
            while (y--)
            {
                Iter colSrcIter = rowSrcIter;
                Iter colDstIter = rowDstIter;
                int x = pImage->GetWidth();
                while (x--)
                {
                    colDstIter.SetRed(colSrcIter.GetRed());
                    colDstIter.SetGreen(colSrcIter.GetGreen());
                    colDstIter.SetBlue(colSrcIter.GetBlue());
                    colSrcIter.Step(CImageIter::Right);
                    colDstIter.Step(CImageIter::Left);
                }
                rowSrcIter.Step(CImageIter::Down);
                rowDstIter.Step(CImageIter::Up);
            }
            return spRotImage;
        }
        else if (degrees == 270.0f)
        {
            uint32 bytesPerPixel = pImage->GetBytesPerPixel();
            CRefObj<IImage> spRotImage = CreateImageImpl(pImage->GetHeight(), pImage->GetWidth(), bytesPerPixel * 8);
            //
            //  A                         
            //  +-------------+          +-----------+ A
            //  |             |          |           |
            //  |             |     ==>  |           |  
            //  |             |          |           |
            //  +-------------+ B        |           |
            //                           |           |
            //                         B +-----------+
            //
            Iter rowSrcIter(pImage, 0, 0);
            Iter colDstIter(spRotImage, spRotImage->GetWidth() - 1, 0);
            int y = pImage->GetHeight();
            while (y--)
            {
                Iter colSrcIter = rowSrcIter;
                Iter rowDstIter = colDstIter;
                int x = pImage->GetWidth();
                while (x--)
                {
                    rowDstIter.SetRed(colSrcIter.GetRed());
                    rowDstIter.SetGreen(colSrcIter.GetGreen());
                    rowDstIter.SetBlue(colSrcIter.GetBlue());
                    colSrcIter.Step(CImageIter::Right);
                    rowDstIter.Step(CImageIter::Down);
                }
                rowSrcIter.Step(CImageIter::Down);
                colDstIter.Step(CImageIter::Left);
            }
            return spRotImage;
        }
        else
        {
            float ca = (float)cos(degrees * M_PI / 180.0f);
            float sa = (float)sin(degrees * M_PI / 180.0f);
            Matrix3x2 tm(ca, sa, -sa, ca, 0.0f, 0.0f);
            CTransformFilter filt;
            ImageFilterParams params;
            params.params["Matrix"] = tm;
            return filt.Apply(pImage, &params);
        }
    }

    //**********************************************************************
    // Method: Apply
    // Rotates an image.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pParams - filter parameters.
    //
    // Returns:
    // Returns the filtered image.
    //**********************************************************************
    CRefObj<IImage> CRotateFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        float degrees = 0.0f;
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it;
            it = pParams->params.find("Degrees");
            if (it != pParams->params.end())
                degrees = std::any_cast<float>(it->second);
        }

        if (pImage->GetBPP() == 24)
        {
            return Rotate<CImageIter24>(pImage, degrees);
        }
        else if (pImage->GetBPP() == 32)
        {
            return Rotate<CImageIter32>(pImage, degrees);
        }
        return nullptr;
    }

    bool CRotateFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        return false; // Inplace transformation not supported
    }
}
