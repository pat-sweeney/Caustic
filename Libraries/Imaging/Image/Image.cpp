//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Image.h"
#include "ImageImpl.h"
#include <atlbase.h>
#include <wincodec.h>
#include "Geometry\Rast\Bresenham.h"

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

    void CreateIntegralImage(IImage *pImage, IIntegralImage **ppImage)
    {
        std::unique_ptr<CIntegralImage> spIntegralImage(new CIntegralImage(pImage->GetWidth(), pImage->GetHeight()));
        uint32 *pDstRow = (uint32*)spIntegralImage->GetData();
        BYTE *pSrcRow = pImage->GetData();
        int w = (int)spIntegralImage->GetWidth();
        for (uint32 y = 0; y < pImage->GetHeight(); y++)
        {
            uint32 *pDstCol = pDstRow;
            BYTE *pSrcCol = pSrcRow;
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
        *ppImage = spIntegralImage.release();
        (*ppImage)->AddRef();
    }

    uint32 CIntegralImage::GetSum(int channel, int x1, int y1, int x2, int y2)
    {
        /*  . . . . . . . . . .
            . . . . o . . . . .
            . . . . . . . . . .
            . . . . . . . . o .
            . . . . . . . . . .
            */
        uint32 *pData = (uint32*)GetData();
        x1 = Caustic::clamp<int>(x1, 0, GetWidth() - 1);
        x2 = Caustic::clamp<int>(x2, 0, GetWidth() - 1);
        y1 = Caustic::clamp<int>(y1, 0, GetHeight() - 1);
        y2 = Caustic::clamp<int>(y2, 0, GetHeight() - 1);
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

    void CIntegralImage::BoxBlur(int width, int height, IImage **ppImage)
    {
        std::unique_ptr<CImage> spDstImage(new CImage(GetWidth(), GetHeight()));
        BYTE *rowSrc = m_spImage->GetData();
        BYTE *rowDst = spDstImage->GetData();
        int miny = -height / 2;
        int maxy = (height - 1) / 2;
        for (int y = 0; y < (int)GetHeight(); y++)
        {
            BYTE *colSrc = rowSrc;
            BYTE *colDst = rowDst;
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
        *ppImage = spDstImage.release();
        (*ppImage)->AddRef();
    }

    void CImage::GaussianBlur(float sigma, IImage **ppImage)
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
        int kw = (int)(sqrt(magic * 2 * sigma * sigma));
        int kernelWidth = kw * 2 + 1;
        int *kernelWeights = new int[kernelWidth];
        double *weights = new double[kernelWidth];
        for (int i = -kw; i <= kw; i++)
        {
            double x = (double)i;
            double e = exp(-(x*x) / (2 * sigma*sigma));
            double weight = e / sqrt(2 * sigma * sigma * 3.1415926535f);
            weights[i+kw] = weight;
            // FIXX! Need to add other side of kernel to weights
            kernelWeights[i+kw] = (int)(255.0f * weight);
        }

        auto BlurPass = [&](CImage *pSrc, CImage *pDst, uint32 h, uint32 w, uint32 rowOffset, uint32 colOffset)
        {
            BYTE *rowSrc = pSrc->GetData();
            BYTE *rowDst = pDst->GetData();
            for (uint32 y = 0; y < h; y++)
            {
                BYTE *colSrc = rowSrc;
                BYTE *colDst = rowDst;
                for (uint32 x = 0; x < w; x++)
                {
                    int offset = -kw;
                    float sum[3] = { 0.0f , 0.0f , 0.0f };
                    for (int i = -kw; i <= kw; i++)
                    {
                        int xoffset = (int)x + i;
                        if (xoffset < 0)
                            xoffset = 0;
                        else if (xoffset >= (int)w)
                            xoffset = (int)w - 1;
                        sum[0] += float(weights[i + kw] * float(rowSrc[colOffset * xoffset + 0]) / 255.0f);
                        sum[1] += float(weights[i + kw] * float(rowSrc[colOffset * xoffset + 1]) / 255.0f);
                        sum[2] += float(weights[i + kw] * float(rowSrc[colOffset * xoffset + 2]) / 255.0f);
                    }
                    colDst[0] = (int)(255.0f * sum[0]);
                    colDst[1] = (int)(255.0f * sum[1]);
                    colDst[2] = (int)(255.0f * sum[2]);
                    colDst[3] = colSrc[3];
                    colSrc += colOffset;
                    colDst += colOffset;
                }
                rowSrc += rowOffset;
                rowDst += rowOffset;
            }
        };
        uint32 bytesPerPixel = GetBytesPerPixel();
        uint32 stride = GetStride();
        std::unique_ptr<CImage> spImage(new CImage(GetWidth(), GetHeight()));
        BlurPass(this, spImage.get(), GetHeight(), GetWidth(), stride, bytesPerPixel);
        std::unique_ptr<CImage> spImage2(new CImage(GetWidth(), GetHeight()));
        BlurPass(spImage.get(), spImage2.get(), GetWidth(), GetHeight(), bytesPerPixel, stride);

        *ppImage = spImage2.release();
        (*ppImage)->AddRef();
    }

    void CreateImage(uint32 width, uint32 height, IImage **ppImage)
    {
        std::unique_ptr<CImage> spImage(new CImage());
        spImage->m_width = width;
        spImage->m_height = height;
        spImage->m_bytesPerPixel = 4;
        uint32 stride = width * spImage->m_bytesPerPixel;
        uint32 numbytes = stride * height;
        spImage->m_spData.reset(new BYTE[numbytes]);
        *ppImage = spImage.release();
        (*ppImage)->AddRef();
    }

    void CImage::Clone(IImage **ppImage)
    {
        CRefObj<IImage> spImage;
        CreateImage(GetWidth(), GetHeight(), &spImage);
        memcpy(spImage->GetData(), GetData(), GetStride() * GetHeight());
        *ppImage = spImage.Detach();
    }

    void CImage::DrawLine(Vector2 &v0, Vector2 &v1, bool exterior)
    {
        Bresenham b((int32)v0.x, (int32)v0.y, (int32)v1.x, (int32)v1.y);
        while (!b.eol())
        {
            if (b.get_x() >= 0 && b.get_x() < (int32)GetWidth() && b.get_y() >= 0 && b.get_y() < (int32)GetHeight())
            {
                BYTE *pData = GetData() + b.get_y() * this->GetStride() + b.get_x() * m_bytesPerPixel;
                pData[0] = (exterior) ? 255 : 0;
                pData[1] = 0;
                pData[2] = (exterior) ? 0 : 255;
                pData[3] = 255;
            }
            b.step();
        }
    }

#undef LoadImage
    void LoadImage(const wchar_t *pFilename, IImage **ppImage)
    {
        std::unique_ptr<CImage> spImage(new CImage());
        CComPtr<IWICBitmapDecoder> spDecoder;
        CT(s_wic.m_spFactory->CreateDecoderFromFilename(pFilename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &spDecoder));
        CComPtr<IWICBitmapFrameDecode> spFrame;
        CT(spDecoder->GetFrame(0, &spFrame));
        UINT w, h;
        CT(spFrame->GetSize(&w, &h));

        WICPixelFormatGUID guid;
        CT(spFrame->GetPixelFormat(&guid));

        spImage->m_width = (uint32)w;
        spImage->m_height = (uint32)h;
        spImage->m_bytesPerPixel = 4;
        uint32 stride = w * spImage->m_bytesPerPixel;
        uint32 numbytes = stride * h;
        spImage->m_spData.reset(new BYTE[numbytes]);
        if (guid != GUID_WICPixelFormat32bppBGRA)
        {
            CComPtr<IWICBitmapSource> spNewFrame;
            WICConvertBitmapSource(GUID_WICPixelFormat32bppBGRA, spFrame, &spNewFrame);
            spNewFrame->CopyPixels(nullptr, stride, numbytes, spImage->m_spData.get());
        }
        else
            spFrame->CopyPixels(nullptr, stride, numbytes, spImage->m_spData.get());
        *ppImage = spImage.release();
        (*ppImage)->AddRef();
    }

    void StoreImage(const wchar_t *pFilename, IImage *pImage)
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
        CT(spBitmapFrame->WritePixels(pImage->GetHeight(), pImage->GetWidth() * 4, pImage->GetHeight() * pImage->GetWidth() * 4, pImage->GetData()));
        CT(spBitmapFrame->Commit());
        CT(spEncoder->Commit());
    }
}
