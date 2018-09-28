//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"

namespace Caustic
{
    struct IImageBase : public IRefCount
    {
        virtual uint8 *GetData() = 0;
        virtual uint32 GetWidth() = 0;
        virtual uint32 GetHeight() = 0;
        virtual uint32 GetStride() = 0;
        virtual uint32 GetBytesPerPixel() = 0;
    };

    struct IImage : public IImageBase
    {
        virtual void GaussianBlur(float sigma, IImage **ppImage) = 0;
        virtual void ShowAlpha(int boxSize, IImage **ppResult) = 0;
        virtual void Clone(IImage **ppImage) = 0;
        virtual void DrawLine(Vector2 &v0, Vector2 &v1, bool exterior) = 0;
    };
    extern void CreateImage(uint32 width, uint32 height, IImage **ppImage);

    struct IIntegralImage : public IImageBase
    {
        virtual uint32 GetSum(int channel, int x1, int y1, int x2, int y2) = 0;
        virtual void BoxBlur(int width, int height, IImage **ppImage) = 0;
    };
    extern void CreateIntegralImage(IImage *pImage, IIntegralImage **ppImage);

#undef LoadImage
    void LoadImage(const wchar_t *pFilename, IImage **ppImage);
    void StoreImage(const wchar_t *pFilename, IImage *pImage);
}
