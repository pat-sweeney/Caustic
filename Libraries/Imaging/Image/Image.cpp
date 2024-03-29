//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <wincodec.h>
#include <cinttypes>

module Imaging.Image.Image;
import Base.Core.Core;
import Base.Core.Error;
import Geometry.Rast.Bresenham;
import Geometry.Rast.BresenhamCircle;

namespace Caustic
{
    struct WICImaging
    {
        CComPtr<IWICImagingFactory> m_spFactory;
        WICImaging();
    };
    static WICImaging s_wic;

    WICImaging::WICImaging()
    {
        CoInitialize(NULL);
        CT(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&s_wic.m_spFactory)));
    }

    uint32_t CIntegralImage::GetSum(int channel, int x1, int y1, int x2, int y2)
    {
        /*  . . . . . . . . . .
            . . . . o . . . . .
            . . . . . . . . . .
            . . . . . . . . o .
            . . . . . . . . . .
            */
        uint32_t* pData = (uint32_t*)GetData();
        x1 = Caustic::Clamp<int>(x1, 0, GetWidth() - 1);
        x2 = Caustic::Clamp<int>(x2, 0, GetWidth() - 1);
        y1 = Caustic::Clamp<int>(y1, 0, GetHeight() - 1);
        y2 = Caustic::Clamp<int>(y2, 0, GetHeight() - 1);
        uint32_t w = GetWidth();
        uint32_t sum = pData[y2 * 3 * w + 3 * x2 + channel];
        if (x1 > 0)
            sum -= pData[y2 * 3 * w + 3 * (x1 - 1) + channel];
        if (y1 > 0)
            sum -= pData[(y1 - 1) * 3 * w + 3 * x2 + channel];
        if (x1 > 0 && y1 > 0)
            sum += pData[(y1 - 1) * 3 * w + 3 * (x1 - 1) + channel];
        return sum;
    }

    CRefObj<IImage> CIntegralImage::BoxBlur(int width, int height)
    {
        std::unique_ptr<CImage> spDstImage(new CImage(GetWidth(), GetHeight(), Caustic::EImageType::RGBA_32bpp));
        BYTE* rowSrc = m_spImage->GetData();
        BYTE* rowDst = spDstImage->GetData();
        int miny = -height / 2;
        int maxy = (height - 1) / 2;
        for (int y = 0; y < (int)GetHeight(); y++)
        {
            BYTE* colSrc = rowSrc;
            BYTE* colDst = rowDst;
            int minx = -width / 2;
            int maxx = (width - 1) / 2;
            for (int x = 0; x < (int)GetWidth(); x++)
            {
                colDst[0] = GetSum(0, minx, miny, maxx, maxy) / (width * height);
                colDst[1] = GetSum(1, minx, miny, maxx, maxy) / (width * height);
                colDst[2] = GetSum(2, minx, miny, maxx, maxy) / (width * height);
                colDst[3] = colSrc[3];
                colSrc += (m_spImage->GetBPP() / 8);
                colDst += (spDstImage->GetBPP() / 8);
                minx++;
                maxx++;
            }
            rowSrc += m_spImage->GetStride();
            rowDst += spDstImage->GetStride();
            miny++;
            maxy++;
        }
        return CRefObj<IImage>(spDstImage.release());
    }

    CRefObj<IImage> CImage::Clone()
    {
        CRefObj<IImage> spImage = CreateImage(GetWidth(), GetHeight(), GetImageType());
        memcpy(spImage->GetData(), GetData(), GetStride() * GetHeight());
        return spImage;
    }

    void CImage::Clear()
    {
        memset(GetData(), 0, GetStride() * GetHeight());
    }

    void CImage::SetPixel(int32_t x, int32_t y, uint8_t color[4])
    {
        if (GetImageType() != EImageType::RGBA_32bpp)
            CT(E_INVALIDARG);
        if (x >= (int32_t)GetWidth() || y >= (int32_t)GetHeight())
            return;
        int bytesPerPixel = this->GetBPP() / 8;
        BYTE* pData = GetData() + y * this->GetStride() + x * bytesPerPixel;
        pData[0] = color[2];
        pData[1] = color[1];
        pData[2] = color[0];
        pData[3] = color[3];
    }

    void CImage::SetPixel(int32_t x, int32_t y, uint8_t gray)
    {
        if (GetImageType() != EImageType::Gray_8bpp)
            CT(E_INVALIDARG);
        if (x >= (int32_t)GetWidth() || y >= (int32_t)GetHeight())
            return;
        int bytesPerPixel = this->GetBPP() / 8;
        BYTE* pData = GetData() + y * this->GetStride() + x * bytesPerPixel;
        pData[0] = gray;
    }

    void CImage::SetPixel(int32_t x, int32_t y, uint16_t v)
    {
        if (GetImageType() != EImageType::Gray_16bpp)
            CT(E_INVALIDARG);
        if (x >= (int32_t)GetWidth() || y >= (int32_t)GetHeight())
            return;
        int bytesPerPixel = this->GetBPP() / 8;
        uint16_t* pData = (uint16_t*)(GetData() + y * this->GetStride() + x * bytesPerPixel);
        pData[0] = v;
    }

    void CImage::DrawCircle(Vector2& center, uint32_t radius, uint8_t color[4])
    {
        int bytesPerPixel = this->GetBPP() / 8;
        if (bytesPerPixel != 4)
            CT(E_UNEXPECTED);
        BresenhamCircle circle(radius);
        while (!circle.end())
        {
            int32_t curx = circle.GetX();
            int32_t cury = circle.GetY();
            auto setPixel = [&](int x, int y) {
                int32_t nx = (int32_t)center.x + x;
                int32_t ny = (int32_t)center.y + y;
                if (nx >= 0 && nx < (int32_t)GetWidth() && ny >= 0 && ny < (int32_t)GetHeight())
                {
                    BYTE* pData = GetData() + ny * this->GetStride() + nx * bytesPerPixel;
                    pData[0] = color[2];
                    pData[1] = color[1];
                    pData[2] = color[0];
                    pData[3] = color[3];
                }
            };
            setPixel(+curx, +cury);
            setPixel(+cury, +curx);
            setPixel(+cury, -curx);
            setPixel(+curx, -cury);
            setPixel(-curx, -cury);
            setPixel(-cury, -curx);
            setPixel(-cury, +curx);
            setPixel(-curx, +cury);
            circle.step();
        }
    }

    void CImage::DrawPath(IPath2* path)
    {
        CRefObj<IPath2> flatPath = path->Flatten(1.0f);
        int numElems = flatPath->GetNumberElems();
        uint8_t color[4] = { 255, 0, 0, 255 };
        const PathElem* pPrevElem = flatPath->GetElement(0);
        for (int i = 1; i < numElems; i++)
        {
            const PathElem* pCurElem = flatPath->GetElement(i);
            DrawLine(pPrevElem->pt[0], pCurElem->pt[0], color);
            pPrevElem = pCurElem;
        }
    }

    void CImage::DrawLine(const Vector2& v0, const Vector2& v1, uint8_t color[4])
    {
        int bytesPerPixel = this->GetBPP() / 8;
        if (bytesPerPixel != 4)
            CT(E_UNEXPECTED);
        Bresenham b((int32_t)v0.x, (int32_t)v0.y, (int32_t)v1.x, (int32_t)v1.y);
        while (!b.eol())
        {
            if (b.get_x() >= 0 && b.get_x() < (int32_t)GetWidth() && b.get_y() >= 0 && b.get_y() < (int32_t)GetHeight())
            {
                BYTE* pData = GetData() + b.get_y() * this->GetStride() + b.get_x() * bytesPerPixel;
                pData[0] = color[2];
                pData[1] = color[1];
                pData[2] = color[0];
                pData[3] = color[3];
            }
            b.step();
        }
    }

    CRefObj<IImage> LoadImageFileImpl(const wchar_t* pFilename)
    {
        CComPtr<IWICBitmapDecoder> spDecoder;
        CT(s_wic.m_spFactory->CreateDecoderFromFilename(pFilename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &spDecoder));
        CComPtr<IWICBitmapFrameDecode> spFrame;
        CT(spDecoder->GetFrame(0, &spFrame));
        UINT w, h;
        CT(spFrame->GetSize(&w, &h));

        WICPixelFormatGUID guid;
        CT(spFrame->GetPixelFormat(&guid));

        CRefObj<IImage> spImage;
        if (guid != GUID_WICPixelFormat32bppBGRA)
        {
            spImage = CreateImage(w, h, EImageType::BGRA_32bpp);
            uint32_t stride = spImage->GetStride();
            uint32_t numbytes = stride * h;
            CComPtr<IWICBitmapSource> spNewFrame;
            WICConvertBitmapSource(GUID_WICPixelFormat32bppBGRA, spFrame, &spNewFrame);
            CT(spNewFrame->CopyPixels(nullptr, stride, numbytes, spImage->GetData()));
        }
        else
        {
            spImage = CreateImage(w, h, EImageType::RGBA_32bpp);
            uint32_t stride = spImage->GetStride();
            uint32_t numbytes = stride * h;
            spFrame->CopyPixels(nullptr, stride, numbytes, spImage->GetData());
        }
        return spImage;
    }

    void StoreImageImpl(const wchar_t* pFilename, IImage* pImage)
    {
        CComPtr<IWICStream> spStream;
        CT(s_wic.m_spFactory->CreateStream(&spStream));
        CT(spStream->InitializeFromFilename(pFilename, GENERIC_WRITE));
        CComPtr<IWICBitmapEncoder> spEncoder;
        CT(s_wic.m_spFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &spEncoder));
        CT(spEncoder->Initialize(spStream, WICBitmapEncoderNoCache));
        CComPtr<IPropertyBag2> spPropertyBag;
        CComPtr<IWICBitmapFrameEncode> spBitmapFrame;
        CT(spEncoder->CreateNewFrame(&spBitmapFrame, &spPropertyBag));
        CT(spBitmapFrame->Initialize(spPropertyBag));
        CT(spBitmapFrame->SetSize(pImage->GetWidth(), pImage->GetHeight()));
        WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppRGBA;
        CT(spBitmapFrame->SetPixelFormat(&formatGUID));
        int bpp = pImage->GetBPP() / 8;
        if (bpp == 4)
            CT(spBitmapFrame->WritePixels(pImage->GetHeight(), pImage->GetWidth() * 4, pImage->GetHeight() * pImage->GetWidth() * 4, pImage->GetData()));
        else
        {
            CT((bpp == 3) ? S_OK : E_UNEXPECTED);
            uint32_t w = pImage->GetWidth();
            uint32_t h = pImage->GetHeight();
            uint8_t* data = new uint8_t[w * h * 4];
            uint8_t* pRow = data;
            Caustic::CImageIter24 riter(pImage, 0, 0);
            for (uint32_t y = 0; y < h; y++)
            {
                Caustic::CImageIter24 citer = riter;
                uint8_t* pCol = pRow;
                for (uint32_t x = 0; x < w; x++)
                {
                    pCol[0] = citer.GetRed();
                    pCol[1] = citer.GetGreen();
                    pCol[2] = citer.GetBlue();
                    pCol[3] = 255;
                    pCol += 4;
                    citer.Step(CImageIter::EStepDirection::Right);
                }
                pRow += w * 4;
                riter.Step(CImageIter::EStepDirection::Down);
            }
            CT(spBitmapFrame->WritePixels(h, w * 4, h * w * 4, data));
        }
        CT(spBitmapFrame->Commit());
        CT(spEncoder->Commit());
    }

    CRefObj<IImage> CreateImageImpl(uint32_t width, uint32_t height, EImageType imageType)
    {
        return CRefObj<IImage>(new CImage(width, height, imageType));
    }

    CRefObj<IIntegralImage> CreateIntegralImageImpl(IImage* pImage)
    {
        std::unique_ptr<CIntegralImage> spIntegralImage(new CIntegralImage(pImage->GetWidth(), pImage->GetHeight()));
        uint32_t* pDstRow = (uint32_t*)spIntegralImage->GetData();
        BYTE* pSrcRow = pImage->GetData();
        int w = (int)spIntegralImage->GetWidth();
        int bpp = (pImage->GetBPP() / 8);
        for (uint32_t y = 0; y < pImage->GetHeight(); y++)
        {
            uint32_t* pDstCol = pDstRow;
            BYTE* pSrcCol = pSrcRow;
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
                pSrcCol += bpp;
                pDstCol += 3;
            }
            pSrcRow += pImage->GetStride();
            pDstRow += 3 * w;
        }
        spIntegralImage->m_spImage = pImage;
        return CRefObj<IIntegralImage>(spIntegralImage.release());
    }
}
