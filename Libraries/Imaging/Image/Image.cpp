//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
#include <wincodec.h>

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

    uint32 CIntegralImage::GetSum(int channel, int x1, int y1, int x2, int y2)
    {
        /*  . . . . . . . . . .
            . . . . o . . . . .
            . . . . . . . . . .
            . . . . . . . . o .
            . . . . . . . . . .
            */
        uint32* pData = (uint32*)GetData();
        x1 = Caustic::Clamp<int>(x1, 0, GetWidth() - 1);
        x2 = Caustic::Clamp<int>(x2, 0, GetWidth() - 1);
        y1 = Caustic::Clamp<int>(y1, 0, GetHeight() - 1);
        y2 = Caustic::Clamp<int>(y2, 0, GetHeight() - 1);
        uint32 w = GetWidth();
        uint32 sum = pData[y2 * 3 * w + 3 * x2 + channel];
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
        std::unique_ptr<CImage> spDstImage(new CImage(GetWidth(), GetHeight(), 32));
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
                colSrc += m_spImage->GetBytesPerPixel();
                colDst += spDstImage->GetBytesPerPixel();
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
        CRefObj<IImage> spImage = CreateImage(GetWidth(), GetHeight(), GetBPP());
        memcpy(spImage->GetData(), GetData(), GetStride() * GetHeight());
        return spImage;
    }

    void CImage::Clear()
    {
        memset(GetData(), 0, GetStride() * GetHeight());
    }

    void CImage::SetPixel(uint32 x, uint32 y, uint8 color[4])
    {
        if (x >= GetWidth() || y >= GetHeight())
            return;
        BYTE* pData = GetData() + y * this->GetStride() + x * m_bytesPerPixel;
        pData[0] = color[2];
        pData[1] = color[1];
        pData[2] = color[0];
        pData[3] = color[3];
    }

    void CImage::SetPixel(uint32 x, uint32 y, uint8 gray)
    {
        if (x >= GetWidth() || y >= GetHeight())
            return;
        BYTE* pData = GetData() + y * this->GetStride() + x * m_bytesPerPixel;
        pData[0] = gray;
    }

    void CImage::SetPixel(uint32 x, uint32 y, uint16 v)
    {
        if (x >= GetWidth() || y >= GetHeight())
            return;
        uint16* pData = (uint16*)(GetData() + y * this->GetStride() + x * m_bytesPerPixel);
        pData[0] = v;
    }

    void CImage::DrawCircle(Vector2& center, uint32 radius, uint8 color[4])
    {
        if (m_bytesPerPixel != 4)
            CT(E_UNEXPECTED);
        BresenhamCircle circle(radius);
        while (!circle.end())
        {
            int32 curx = circle.GetX();
            int32 cury = circle.GetY();
            auto setPixel = [&](int x, int y) {
                int32 nx = (int32)center.x + x;
                int32 ny = (int32)center.y + y;
                if (nx >= 0 && nx < (int32)GetWidth() && ny >= 0 && ny < (int32)GetHeight())
                {
                    BYTE* pData = GetData() + ny * this->GetStride() + nx * m_bytesPerPixel;
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
        uint8 color[4] = { 255, 0, 0, 255 };
        const PathElem* pPrevElem = flatPath->GetElement(0);
        for (int i = 1; i < numElems; i++)
        {
            const PathElem* pCurElem = flatPath->GetElement(i);
            DrawLine(pPrevElem->pt[0], pCurElem->pt[0], color);
            pPrevElem = pCurElem;
        }
    }

    void CImage::DrawLine(const Vector2& v0, const Vector2& v1, uint8 color[4])
    {
        if (m_bytesPerPixel != 4)
            CT(E_UNEXPECTED);
        Bresenham b((int32)v0.x, (int32)v0.y, (int32)v1.x, (int32)v1.y);
        while (!b.eol())
        {
            if (b.get_x() >= 0 && b.get_x() < (int32)GetWidth() && b.get_y() >= 0 && b.get_y() < (int32)GetHeight())
            {
                BYTE* pData = GetData() + b.get_y() * this->GetStride() + b.get_x() * m_bytesPerPixel;
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

        CRefObj<IImage> spImage = CreateImage(w, h, 32);
        uint32 stride = spImage->GetStride();
        uint32 numbytes = stride * h;
        if (guid != GUID_WICPixelFormat32bppBGRA)
        {
            CComPtr<IWICBitmapSource> spNewFrame;
            WICConvertBitmapSource(GUID_WICPixelFormat32bppBGRA, spFrame, &spNewFrame);
            CT(spNewFrame->CopyPixels(nullptr, stride, numbytes, spImage->GetData()));
            spImage->SetRGBOrder(false);
        }
        else
        {
            spFrame->CopyPixels(nullptr, stride, numbytes, spImage->GetData());
            spImage->SetRGBOrder(true);
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
        int bpp = pImage->GetBytesPerPixel();
        if (bpp == 4)
            CT(spBitmapFrame->WritePixels(pImage->GetHeight(), pImage->GetWidth() * 4, pImage->GetHeight() * pImage->GetWidth() * 4, pImage->GetData()));
        else
        {
            CT((bpp == 3) ? S_OK : E_UNEXPECTED);
            uint32 w = pImage->GetWidth();
            uint32 h = pImage->GetHeight();
            uint8* data = new uint8[w * h * 4];
            uint8* pRow = data;
            Caustic::CImageIter24 riter(pImage, 0, 0);
            for (uint32 y = 0; y < h; y++)
            {
                Caustic::CImageIter24 citer = riter;
                uint8* pCol = pRow;
                for (uint32 x = 0; x < w; x++)
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

    CRefObj<IImage> CreateImageImpl(uint32 width, uint32 height, uint32 bpp)
    {
        return CRefObj<IImage>(new CImage(width, height, bpp));
    }

    CRefObj<IIntegralImage> CreateIntegralImageImpl(IImage* pImage)
    {
        std::unique_ptr<CIntegralImage> spIntegralImage(new CIntegralImage(pImage->GetWidth(), pImage->GetHeight()));
        uint32* pDstRow = (uint32*)spIntegralImage->GetData();
        BYTE* pSrcRow = pImage->GetData();
        int w = (int)spIntegralImage->GetWidth();
        for (uint32 y = 0; y < pImage->GetHeight(); y++)
        {
            uint32* pDstCol = pDstRow;
            BYTE* pSrcCol = pSrcRow;
            for (uint32 x = 0; x < pImage->GetWidth(); x++)
            {
                uint32 sum[3];
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
                pSrcCol += pImage->GetBytesPerPixel();
                pDstCol += 3;
            }
            pSrcRow += pImage->GetStride();
            pDstRow += 3 * w;
        }
        spIntegralImage->m_spImage = pImage;
        return CRefObj<IIntegralImage>(spIntegralImage.release());
    }
}
