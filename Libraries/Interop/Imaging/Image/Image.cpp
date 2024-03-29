#include "stdafx.h"

#include "Image.h"

#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;


namespace CausticInterop {

Image^ Image::Clone()
{
#if false
    Image^ image = gcnew Image();
    pin_ptr<Caustic::IImage> spDst = image->m_pSourceImage;
    pin_ptr<Caustic::IImage> spSrc = m_pSourceImage;
    spDst = spSrc->Clone();

    System::Windows::Media::PixelFormat format;
    switch (spSrc->GetBytesPerPixel())
    {
    case 1:
        format = System::Windows::Media::PixelFormats::Gray8;
        break;
    case 3:
        format = System::Windows::Media::PixelFormats::Bgr24;
        break;
    case 4:
        format = System::Windows::Media::PixelFormats::Bgr32;
        break;
    }
    image->m_bitmap = gcnew System::Windows::Media::Imaging::WriteableBitmap(spSrc->GetWidth(), spSrc->GetHeight(), 96.0, 96.0, format, nullptr);
    image->Update(spDst);
    return image;
#else
    return nullptr;
#endif
}

System::Windows::Media::Imaging::WriteableBitmap^ Image::Load(String ^fn)
{
#if false
    IntPtr ip = Marshal::StringToHGlobalUni(fn);
    const wchar_t* str = static_cast<const wchar_t*>(ip.ToPointer());
    pin_ptr<Caustic::IImage> sp = m_pSourceImage;
    sp = Caustic::LoadImageFile(str);
    pin_ptr<Caustic::IImage> spAlpha = m_pAlphaImage;
    spAlpha = m_pSourceImage->Clone();
    Marshal::FreeHGlobal(ip);

    System::Windows::Media::PixelFormat format;
    switch (m_pSourceImage->GetBytesPerPixel())
    {
    case 1:
        format = System::Windows::Media::PixelFormats::Gray8;
        break;
    case 3:
        format = System::Windows::Media::PixelFormats::Bgr24;
        break;
    case 4:
        format = System::Windows::Media::PixelFormats::Bgr32;
        break;
    }
    m_bitmap = gcnew System::Windows::Media::Imaging::WriteableBitmap(m_pSourceImage->GetWidth(), m_pSourceImage->GetHeight(), 96.0, 96.0, format, nullptr);
    Update(m_pAlphaImage);
    return m_bitmap;
#else
    return nullptr;
#endif
}

void Image::Update(void *pImage)
{
#if 0
    m_bitmap->Lock();
    pin_ptr<Caustic::IImage> sp = pImage;
    Caustic::uint8 *pData = sp->GetData();
    System::Windows::Int32Rect rect(0, 0, sp->GetWidth(), sp->GetHeight());
    IntPtr p((void*)pData);
    uint32_t stride = sp->GetStride();
    m_bitmap->WritePixels(rect, p, sp->GetHeight() * stride, stride);
    m_bitmap->Unlock();
#endif
}

#if 0
void InjectPoints(Caustic::IPolygon2 *finalPoints, float spacing)
{
    std::vector<Caustic::Vector2> delanuayPoints;
    for (Caustic::uint32_t i = 0; i < finalPoints->GetNumberPoints(); i++)
    {
        Caustic::Vector2 v;
        finalPoints->GetPoint(i, &v);
        delanuayPoints.push_back(v);
    }
    Caustic::BBox2 bbox;
    finalPoints->GetBBox(&bbox);
    float y = bbox.minPt.y;
    while (y < bbox.maxPt.y)
    {
        float x = bbox.minPt.x;
        while (x < bbox.maxPt.x)
        {
            Caustic::Vector2 pt;
            pt.x = x;
            pt.y = y;
            float minDist, maxDist;
            if (finalPoints->ContainsPoint(pt, &minDist, &maxDist))
            {
                finalPoints->AddPoint(pt);
            }
            x += spacing;
        }
        y += spacing;
    }
}

void Image::ExtractMesh()
{
    Caustic::CRefObj<Caustic::IPolygon2> polygon = Caustic::CreatePolygon2();

    // First scan image until we find first non-alpha pixel
    // We currently only support extracting a single contour.
#ifdef DUMP_CONTOUR
    Caustic::CRefObj<Caustic::IImage> spResults;
    Caustic::CreateImage(m_pSourceImage->GetWidth(), m_pSourceImage->GetHeight(), &spResults);
#endif
    pin_ptr<Caustic::IImage> sp = m_pSourceImage;
    Caustic::uint8 *rowSrc = sp->GetData();
    Caustic::uint32_t bytesPerPixel = sp->GetBytesPerPixel();
    Caustic::uint32_t stride = sp->GetStride();
    bool halt = false;
    int h1 = sp->GetHeight() - 1;
    int w1 = sp->GetWidth() - 1;
    for (Caustic::uint32_t y = 0; !halt && y < sp->GetHeight(); y++)
    {
        Caustic::uint8 *colSrc = rowSrc;
        for (Caustic::uint32_t x = 0; !halt && x < sp->GetWidth(); x++)
        {
            if (colSrc[3] != 0)
            {
                // Use the radial sweep method for walking the contour.
                // For good break down of contour extraction methods see:
                // http://www.imageprocessingplace.com/downloads_V3/root_downloads/tutorials/contour_tracing_Abeer_George_Ghuneim/ray.html
                // Our step directions are as follows:
                //   7 6 5
                //   0 . 4
                //   1 2 3
                // Thus, if we started a position '5' and stepped to '.' our step direction was '1'. In this case we want
                // to next start looking in direction '6' (sweeping clockwise from .->5 to .->6). So our next direction
                // is always (stepDir+3)%8
                Caustic::uint8 *pData = sp->GetData();
#ifdef DUMP_CONTOUR
                Caustic::uint8 *pDstData = spResults->GetData();
#endif
                int startx = x;
                int starty = y;
                int stepDir = 4; // 4 so that our first test step is direction '1'
                int curX = x;
                int curY = y;
                do {
                    int testDir = (stepDir + 5) % 8;
                    for (int j = 0; j < 8; j++, testDir=(testDir+1)%8)
                    {
                        int nextX = curX;
                        int nextY = curY;
                        switch (testDir)
                        {
                        case 0: nextX--; break;
                        case 1: nextX--; nextY++; break;
                        case 2: nextY++; break;
                        case 3: nextX++; nextY++; break;
                        case 4: nextX++; break;
                        case 5: nextX++; nextY--; break;
                        case 6: nextY--; break;
                        case 7: nextX--; nextY--; break;
                        }
                        if (nextX < 0 || nextY < 0 || nextX >= w1 || nextY >= h1)
                            continue;
                        if (pData[nextY * stride + nextX * bytesPerPixel + 3] != 0)
                        {
                            stepDir = testDir;
                            curX = nextX;
                            curY = nextY;
                            break;
                        }
                    }
                    pData[curY * stride + curX * bytesPerPixel + 0] = 0;
                    pData[curY * stride + curX * bytesPerPixel + 1] = 0;
                    pData[curY * stride + curX * bytesPerPixel + 2] = 255;
                    pData[curY * stride + curX * bytesPerPixel + 3] = 255;
#ifdef DUMP_CONTOUR
                    pDstData[curY * stride + curX * bytesPerPixel + 0] = 255;
                    pDstData[curY * stride + curX * bytesPerPixel + 1] = 255;
                    pDstData[curY * stride + curX * bytesPerPixel + 2] = 255;
                    pDstData[curY * stride + curX * bytesPerPixel + 3] = 255;
#endif
                    Caustic::Vector2 v;
                    v.x = (float)curX;
                    v.y = (float)curY;
                    polygon->AddPoint(v);
                } while (curX != startx || curY != starty);
                halt = true;
            }
            colSrc += bytesPerPixel;
        }
        rowSrc += stride;
    }
#ifdef DUMP_CONTOUR
    Caustic::StoreImage(L"d:\\images\\contour.png", spResults);
#endif
    Update(m_pSourceImage);

    // Simplify our polyline
    Caustic::CRefObj<Caustic::IPolygon2> spSimplifiedPolygon = polygon->Simplify(4.0f, 40.0f);

#ifdef DUMP_CONTOUR
    spResults->Clear();
    Caustic::Vector2 lv;
    spSimplifiedPolygon->GetPoint(spSimplifiedPolygon->GetNumberPoints() - 1, &lv);
    for (Caustic::uint32_t j = 0; j < spSimplifiedPolygon->GetNumberPoints(); j++)
    {
        Caustic::Vector2 v;
        spSimplifiedPolygon->GetPoint(j, &v);
        Caustic::uint8 red[4] = { 0, 0, 255, 255 };
        spResults->DrawLine(lv, v, red);
        lv = v;
    }
    Caustic::StoreImage(L"d:\\images\\simplified.png", spResults);
#endif

    Caustic::uint32_t exteriorPointCount = spSimplifiedPolygon->GetNumberPoints();

    InjectPoints(spSimplifiedPolygon, 40);

    Caustic::uint8 *pData = sp->GetData();
    for (Caustic::uint32_t j = 0; j < spSimplifiedPolygon->GetNumberPoints(); j++)
    {
        Caustic::Vector2 v;
        spSimplifiedPolygon->GetPoint(j, &v);
        int curX = (int)v.x;
        int curY = (int)v.y;
        for (int dx = -3; dx < 3; dx++)
            for (int dy = -3; dy < 3; dy++)
            {
                int nx = curX + dx;
                int ny = curY + dy;
                pData[ny * stride + nx * bytesPerPixel + 0] = 255;
                pData[ny * stride + nx * bytesPerPixel + 1] = 255;
                pData[ny * stride + nx * bytesPerPixel + 2] = 255;
                pData[ny * stride + nx * bytesPerPixel + 3] = 255;
            }
    }
    Update(m_pSourceImage);

    Caustic::BBox2 bb;
    bb.minPt.x = 0;
    bb.minPt.y = 0;
    bb.maxPt.x = (float)m_pSourceImage->GetWidth();
    bb.maxPt.y = (float)m_pSourceImage->GetHeight();

    Caustic::CRefObj<Caustic::IDelaunay2> spDelaunay = Caustic::CreateDelaunay2(bb);

    spDelaunay->Open();
    for (Caustic::uint32_t j = 0; j < spSimplifiedPolygon->GetNumberPoints(); j++)
    {
        Caustic::Vector2 v;
        spSimplifiedPolygon->GetPoint(j, &v);
        Caustic::Vector2 uv(v.x / (float)m_pSourceImage->GetWidth(), v.y / (float)m_pSourceImage->GetHeight());
        spDelaunay->AddPoint(v, uv, (j < exteriorPointCount) ? true : false);
    }
    spDelaunay->Close();

    Caustic::uint32_t numTriangles = spDelaunay->GetNumberTriangles();
    for (Caustic::uint32_t j = 0; j < numTriangles; j++)
    {
        Caustic::Vector2 v0, v1, v2;
        bool exterior[3];
        spDelaunay->GetTriangle(j, v0, v1, v2, exterior);
        Caustic::uint8 red[4] = { 255, 0, 0, 255 };
        Caustic::uint8 blue[4] = { 0, 0, 255, 255 };
        m_pSourceImage->DrawLine(v0, v1, (exterior[0]) ? blue : red);
        m_pSourceImage->DrawLine(v1, v2, (exterior[1]) ? blue : red);
        m_pSourceImage->DrawLine(v2, v0, (exterior[2]) ? blue : red);
    }
    Update(m_pSourceImage);

    spDelaunay->WritePLY();
}
#endif
}
