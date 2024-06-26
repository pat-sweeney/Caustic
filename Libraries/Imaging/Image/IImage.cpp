//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <wincodec.h>

module Imaging.Image.IImage;
import Base.Core.Core;
import Base.Core.Error;
import Imaging.Image.Image;
import Geometry.Rast.Bresenham;
import Geometry.Rast.BresenhamCircle;

namespace Caustic
{
    CRefObj<IImage> LoadImageFile(const wchar_t* pFilename)
    {
        return LoadImageFileImpl(pFilename);
    }

    void StoreImage(const wchar_t* pFilename, IImage* pImage)
    {
        StoreImageImpl(pFilename, pImage);
    }

    CRefObj<IImage> CreateImage(uint32_t width, uint32_t height, EImageType imageType)
    {
        return CreateImageImpl(width, height, imageType);
    }

    CRefObj<IIntegralImage> CreateIntegralImage(IImage* pImage)
    {
        return CreateIntegralImageImpl(pImage);
    }
}