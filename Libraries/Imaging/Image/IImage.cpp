//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <wincodec.h>
#include "Geometry\Rast\Bresenham.h"
#include "Geometry\Rast\BresenhamCircle.h"

module Imaging.Image.IImage;
import Base.Core.Core;
import Base.Core.Error;
import Imaging.Image.Image;

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

    CRefObj<IImage> CreateImage(uint32 width, uint32 height, uint32 bpp)
    {
        return CreateImageImpl(width, height, bpp);
    }

    CRefObj<IIntegralImage> CreateIntegralImage(IImage* pImage)
    {
        return CreateIntegralImageImpl(pImage);
    }
}