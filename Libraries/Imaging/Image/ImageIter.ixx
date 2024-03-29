//**********************************************************************
// Copyright Patrick Sweeney 1995-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <assert.h>
#include <cinttypes>

export module Imaging.Image.ImageIter;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Imaging.Color;
import Imaging.Image.IImage;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Class: CImageIter
    // Defines an iterator for walking across the pixels in an image.
    // CImageIter defines the base class. Clients need to used one of the derived
    // classes (CImageIter1, CImageIter8, CImageIter16, CImageIter24, or CImageIter32)
    // to walk across an image.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //
    // Example:
    // --- Code
    //  CImageIter24 rowSrc(pSrcImage, 0, 0);
    //  CImageIter24 rowDst(pDestImage, 0, 0);
    //  for (int y = 0; y < pSrcImage->GetHeight(); y++)
    //  {
    //      CImageIter24 colSrc = rowSrc;
    //      CImageIter24 colDst = rowDst;
    //        for (int x = 0; x < pSrcImage->GetWidth(); x++)
    //      {
    //          colDst.SetRed(colSrc.GetRed() - 10);
    //          colDst.SetGreen(colSrc.GetGreen() - 10);
    //          colDst.SetBlue(colSrc.GetBlue() - 10);
    //          colSrc.Step(CImageIter::Right);
    //          colDst.Step(CImageIter::Right);
    //      }
    //      rowSrc.Step(CImageIter::Down);
    //      rowDst.Step(CImageIter::Down);
    //  }
    // ---
    //**********************************************************************
    class CImageIter
    {
    public:
        enum EStepDirection
        {
            Right,
            Left,
            Up,
            Down
        };
    protected:
        uint8_t* m_bp;        // pointer to current byte
        uint32_t m_bpl;       // bytes per scan line
        bool m_bgrOrder;    // Is type BGR or RGB?
    public:
        uint8_t* PixelData() { return m_bp; }
    };

    //**********************************************************************
    // Class: CImageIterFloat1
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is floating point value.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIterFloat1 : public CImageIter
    {
    public:
        CImageIterFloat1()
        {
        }

        CImageIterFloat1(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 32);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 4];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::EStepDirection::Left:
                m_bp -= 4;
                break;
            case CImageIter::Right:
                m_bp += 4;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        float1 GetValue() { return float1(((float*)m_bp)[0]); }
        void SetValue(float1 v) { ((float*)m_bp)[0] = v.x; }
        void SetValue(float v) { ((float*)m_bp)[0] = v; }
    };

    //**********************************************************************
    // Class: CImageIterFloat2
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 2 floating point values.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIterFloat2 : public CImageIter
    {
    public:
        CImageIterFloat2()
        {
        }

        CImageIterFloat2(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 64);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 4];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 8;
                break;
            case CImageIter::Right:
                m_bp += 8;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        float2 GetValue() { return float2(((float*)m_bp)[0], ((float*)m_bp)[2]); }
        void SetValue(float2 v) { ((float*)m_bp)[0] = v.x; ((float*)m_bp)[1] = v.y; }
        void SetValue(float x, float y) { ((float*)m_bp)[0] = x; ((float*)m_bp)[1] = y; }
        void SetValueX(float x) { ((float*)m_bp)[0] = x; }
        void SetValueY(float y) { ((float*)m_bp)[1] = y; }
    };

    //**********************************************************************
    // Class: CImageIterFloat3
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 3 floating point values.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIterFloat3 : public CImageIter
    {
    public:
        CImageIterFloat3()
        {
        }

        CImageIterFloat3(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 32);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 4];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 12;
                break;
            case CImageIter::Right:
                m_bp += 12;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        float3 GetValue() { return float3(((float*)m_bp)[0], ((float*)m_bp)[1], ((float*)m_bp)[2]); }
        void SetValue(float3 v) { ((float*)m_bp)[0] = v.x;  ((float*)m_bp)[1] = v.y; ((float*)m_bp)[2] = v.z; }
        void SetValue(float x, float y, float z) { ((float*)m_bp)[0] = x; ((float*)m_bp)[1] = y; ((float*)m_bp)[2] = z; }
        void SetValueX(float x) { ((float*)m_bp)[0] = x; }
        void SetValueY(float y) { ((float*)m_bp)[1] = y; }
        void SetValueZ(float z) { ((float*)m_bp)[2] = z; }
    };

    //**********************************************************************
    // Class: CImageIterFloat4
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 4 floating point values.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIterFloat4 : public CImageIter
    {
    public:
        CImageIterFloat4()
        {
        }

        CImageIterFloat4(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 32);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 4];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 16;
                break;
            case CImageIter::Right:
                m_bp += 16;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        float4 GetValue() { return float4(((float*)m_bp)[0], ((float*)m_bp)[1], ((float*)m_bp)[2], ((float*)m_bp)[3]); }
        void SetValue(float4 v) { ((float*)m_bp)[0] = v.x;  ((float*)m_bp)[1] = v.y; ((float*)m_bp)[2] = v.z; ((float*)m_bp)[3] = v.w; }
        void SetValue(float x, float y, float z, float w) { ((float*)m_bp)[0] = x; ((float*)m_bp)[1] = y; ((float*)m_bp)[2] = z; ((float*)m_bp)[3] = w; }
        void SetValueX(float x) { ((float*)m_bp)[0] = x; }
        void SetValueY(float y) { ((float*)m_bp)[1] = y; }
        void SetValueZ(float z) { ((float*)m_bp)[2] = z; }
        void SetValueW(float w) { ((float*)m_bp)[3] = w; }
    };

    //**********************************************************************
    // Class: CImageIter1
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 1 bit in width.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIter1 : public CImageIter
    {
        uint32_t mask;
    public:
        CImageIter1()
        {
        }
        
        CImageIter1(IImage* pImage, int32_t x, int32_t y)
        {
            Setup(pImage, x, y);
        }

        void Setup(IImage* pImage, int32_t x, int32_t y)
        {
            mask = 1 << (7 - ((pImage->GetSubX() + x) & 7));
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) / 8];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                if (mask == 0x80)
                {
                    m_bp--;
                    mask = 0x01;
                }
                else
                    mask <<= 1;
                break;
            case CImageIter::Right:
                if (mask == 0x01)
                {
                    m_bp++;
                    mask = 0x80;
                }
                else
                    mask >>= 1;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        int GetBit()
        {
            return (*m_bp & mask) ? 1 : 0;
        }

        void SetBit(int v)
        {
            if (v)
                *m_bp |= mask;
            else
                *m_bp &= ~mask;
        }
    };

    //**********************************************************************
    // Class: CImageIter8
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 8 bits in width.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIter8 : public CImageIter
    {
    public:
        CImageIter8()
        {
        }

        CImageIter8(IImage* pImage, int32_t x, int32_t y)
        {
            Setup(pImage, x, y);
        }

        void Setup(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 8);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x)];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp--;
                break;
            case CImageIter::Right:
                m_bp++;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        uint8_t GetGray() { return m_bp[0]; }
        void SetGray(uint8_t v) { m_bp[0] = v; }
    };

    //**********************************************************************
    // Class: CImageIter16
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 16 bits in width.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIter16 : public CImageIter
    {
    public:
        CImageIter16()
        {
        }

        CImageIter16(IImage* pImage, int32_t x, int32_t y)
        {
            Setup(pImage, x, y);
        }

        void Setup(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 16);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 2];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 2;
                break;
            case CImageIter::Right:
                m_bp += 2;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        uint16_t GetGray() { return ((uint16_t*)m_bp)[0]; }
        void SetGray(uint16_t v) { ((uint16_t*)m_bp)[0] = v; }
    };

    //**********************************************************************
    // Class: CImageIter24
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 24 bits in width.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIter24 : public CImageIter
    {
    public:
        CImageIter24()
        {
        }

        CImageIter24(IImage* pImage, int32_t x, int32_t y)
        {
            Setup(pImage, x, y);
        }

        void Setup(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 24);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 3];
            m_bpl = pImage->GetStride();
            m_bgrOrder = (pImage->GetImageType() == EImageType::BGR_24bpp) ? true : false;
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 3;
                break;
            case CImageIter::Right:
                m_bp += 3;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        uint8_t GetRed() { return m_bp[(m_bgrOrder) ? 2 : 0]; }
        uint8_t GetGreen() { return m_bp[1]; }
        uint8_t GetBlue() { return m_bp[(m_bgrOrder) ? 0 : 2]; }
        void SetRed(uint8_t v) { m_bp[(m_bgrOrder) ? 2 : 0] = v; }
        void SetGreen(uint8_t v) { m_bp[1] = v; }
        void SetBlue(uint8_t v) { m_bp[(m_bgrOrder) ? 0 : 2] = v; }
        void SetAlpha(uint8_t /*v*/) {} // Only defined so templatization works
    };

    //**********************************************************************
    // Class: CImageIter32
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 32 bits in width.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIter32 : public CImageIter
    {
    public:
        CImageIter32()
        {
        }

        CImageIter32(IImage* pImage, int32_t x, int32_t y)
        {
            Setup(pImage, x, y);
        }

        void Setup(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 32);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * (int)pImage->GetStride() + (pImage->GetSubX() + x) * 4];
            m_bpl = pImage->GetStride();
            EImageType t = pImage->GetImageType();
            m_bgrOrder = (t == EImageType::BGRA_32bpp || t == EImageType::BGRX_32bpp) ? true : false;
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 4;
                break;
            case CImageIter::Right:
                m_bp += 4;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        uint8_t GetRed() { return m_bp[(m_bgrOrder) ? 2 : 0]; }
        uint8_t GetGreen() { return m_bp[1]; }
        uint8_t GetBlue() { return m_bp[(m_bgrOrder) ? 0 : 2]; }
        uint8_t GetAlpha() { return m_bp[3]; }
        void SetRed(uint8_t v) { m_bp[(m_bgrOrder) ? 2 : 0] = v; }
        void SetGreen(uint8_t v) { m_bp[1] = v; }
        void SetBlue(uint8_t v) { m_bp[(m_bgrOrder) ? 0 : 2] = v; }
        void SetAlpha(uint8_t v) { m_bp[3] = v; }
    };

    //**********************************************************************
    // Class: CImageIter128
    // Defines an iterator for walking across the pixels in an image where
    // each pixel is 128 bits in width (i.e. single float per channel)
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIter128 : public CImageIter
    {
    public:
        CImageIter128()
        {
        }

        CImageIter128(IImage* pImage, int32_t x, int32_t y)
        {
            Setup(pImage, x, y);
        }

        void Setup(IImage* pImage, int32_t x, int32_t y)
        {
            assert(pImage->GetBPP() == 128);
            m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * pImage->GetStride() + (pImage->GetSubX() + x) * 4];
            m_bpl = pImage->GetStride();
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_bp -= 16;
                break;
            case CImageIter::Right:
                m_bp += 16;
                break;
            case CImageIter::Up:
                m_bp -= m_bpl;
                break;
            case CImageIter::Down:
                m_bp += m_bpl;
                break;
            }
            return;
        }

        float GetRed() { return ((float*)m_bp)[0]; }
        float GetGreen() { return ((float*)m_bp)[1]; }
        float GetBlue() { return ((float*)m_bp)[2]; }
        float GetAlpha() { return ((float*)m_bp)[3]; }
        void SetRed(float v) { ((float*)m_bp)[0] = v; }
        void SetGreen(float v) { ((float*)m_bp)[1] = v; }
        void SetBlue(float v) { ((float*)m_bp)[2] = v; }
        void SetAlpha(float v) { ((float*)m_bp)[3] = v; }
    };

    //**********************************************************************
    // Class: CImageIterGeneric
    // Defines an iterator for walking across the pixels in an image of
    // arbitrary pixel depth. NOTE: These iterators are slower than using
    // the iterator that matches the given image's pixel depth.
    //
    // Module:
    // {Link:import Imaging.Image.ImageIter{Imaging/Image/ImageIter.ixx}}
    //**********************************************************************
    class CImageIterGeneric : public CImageIter
    {
        CImageIter1 m_iter1;
        CImageIter8 m_iter8;
        CImageIter16 m_iter16;
        CImageIter24 m_iter24;
        CImageIter32 m_iter32;
        CImageIter128 m_iter128;
        CImageIterFloat1 m_iterFloat1;
        CImageIterFloat2 m_iterFloat2;
        CImageIterFloat3 m_iterFloat3;
        CImageIterFloat4 m_iterFloat4;
        EImageType m_imageType;
        int m_x;
        int m_y;
        int m_w;
        int m_h;
    public:
        CImageIterGeneric() :
            m_imageType(EImageType::RGB_24bpp),
            m_x(0),
            m_y(0),
            m_w(0),
            m_h(0)
        {
        }

        CImageIterGeneric(IImage* pImage, int32_t x, int32_t y, bool allowOutBounds = false)
        {
            m_imageType = pImage->GetImageType();
            if (!allowOutBounds)
            {
                if (x < 0)
                    x = 0;
                if (x >= (int32_t)pImage->GetWidth())
                    x = (int32_t)pImage->GetWidth() - 1;
                if (y < 0)
                    y = 0;
                if (y >= (int32_t)pImage->GetHeight())
                    y = (int32_t)pImage->GetHeight() - 1;
            }
            m_x = x;
            m_y = y;
            m_w = pImage->GetWidth();
            m_h = pImage->GetHeight();
            switch (pImage->GetImageType())
            {
            case EImageType::BW_1bpp:
                m_iter1 = CImageIter1(pImage, x, y);
                break;
            case EImageType::Gray_8bpp:
                m_iter8 = CImageIter8(pImage, x, y);
                break;
            case EImageType::Gray_16bpp:
                m_iter16 = CImageIter16(pImage, x, y);
                break;
            case EImageType::RGB_24bpp:
            case EImageType::BGR_24bpp:
                m_iter24 = CImageIter24(pImage, x, y);
                break;
            case EImageType::RGBA_32bpp:
            case EImageType::RGBX_32bpp:
            case EImageType::BGRA_32bpp:
            case EImageType::BGRX_32bpp:
                m_iter32 = CImageIter32(pImage, x, y);
                break;
            case EImageType::Float1_32bpp:
                m_iterFloat1 = CImageIterFloat1(pImage, x, y);
                break;
            case EImageType::Float2_64bpp:
                m_iterFloat2 = CImageIterFloat2(pImage, x, y);
                break;
            case EImageType::Float3_96bpp:
                m_iterFloat3 = CImageIterFloat3(pImage, x, y);
                break;
            case EImageType::Float4_128bpp:
                m_iterFloat4 = CImageIterFloat4(pImage, x, y);
                break;
            }
        }

        void Step(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                m_x--;
                break;
            case CImageIter::Right:
                m_x++;
                break;
            case CImageIter::Up:
                m_y--;
                break;
            case CImageIter::Down:
                m_y++;
                break;
            }
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: m_iter1.Step(dir); break;
            case EImageType::Gray_8bpp: m_iter8.Step(dir); break;
            case EImageType::Gray_16bpp: m_iter16.Step(dir); break;
            case EImageType::RGB_24bpp: m_iter24.Step(dir); break;
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: m_iter32.Step(dir); break;
            case EImageType::BGR_24bpp: m_iter24.Step(dir); break;
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: m_iter32.Step(dir); break;
            case EImageType::Float1_32bpp: m_iterFloat1.Step(dir); break;
            case EImageType::Float2_64bpp: m_iterFloat2.Step(dir); break;
            case EImageType::Float3_96bpp: m_iterFloat3.Step(dir); break;
            case EImageType::Float4_128bpp: m_iterFloat4.Step(dir); break;
            }
        }

        bool IsInBounds()
        {
            return (m_x >= 0 && m_x < m_w && m_y >= 0 && m_y < m_h) ? true : false;
        }

        bool StepWithBoundsCheck(CImageIter::EStepDirection dir)
        {
            switch (dir)
            {
            case CImageIter::Left:
                if (m_x < 0)
                    return false;
                m_x--;
                break;
            case CImageIter::Right:
                if (m_x >= m_w)
                    return false;
                m_x++;
                break;
            case CImageIter::Up:
                if (m_y < 0)
                    return false;
                m_y--;
                break;
            case CImageIter::Down:
                if (m_y >= m_h)
                    return false;
                m_y++;
                break;
            }
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: m_iter1.Step(dir); break;
            case EImageType::Gray_8bpp: m_iter8.Step(dir); break;
            case EImageType::Gray_16bpp: m_iter16.Step(dir); break;
            case EImageType::RGB_24bpp: m_iter24.Step(dir); break;
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: m_iter32.Step(dir); break;
            case EImageType::BGR_24bpp: m_iter24.Step(dir); break;
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: m_iter32.Step(dir); break;
            case EImageType::Float1_32bpp: m_iterFloat1.Step(dir); break;
            case EImageType::Float2_64bpp: m_iterFloat2.Step(dir); break;
            case EImageType::Float3_96bpp: m_iterFloat3.Step(dir); break;
            case EImageType::Float4_128bpp: m_iterFloat4.Step(dir); break;
            }
            return true;
        }

        uint8_t GetRed()
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: return m_iter1.GetBit() ? 255 : 0;
            case EImageType::Gray_8bpp: return m_iter8.GetGray();
            case EImageType::Gray_16bpp: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
            case EImageType::RGB_24bpp: return m_iter24.GetRed();
            case EImageType::RGBA_32bpp: // Fall through
            case EImageType::RGBX_32bpp: return m_iter32.GetRed();
            case EImageType::BGR_24bpp: return m_iter24.GetRed();
            case EImageType::BGRA_32bpp: // Fall through
            case EImageType::BGRX_32bpp: return m_iter32.GetRed();
            case EImageType::Float1_32bpp: return uint8_t(255.0f * m_iterFloat1.GetValue());
            case EImageType::Float2_64bpp: return uint8_t(255.0f * m_iterFloat2.GetValue().x);
            case EImageType::Float3_96bpp: return uint8_t(255.0f * m_iterFloat3.GetValue().x);
            case EImageType::Float4_128bpp: return uint8_t(255.0f * m_iterFloat4.GetValue().x);
            }
            return 0;
        }

        uint8_t GetGreen()
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: return m_iter1.GetBit() ? 255 : 0;
            case EImageType::Gray_8bpp: return m_iter8.GetGray();
            case EImageType::Gray_16bpp: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
            case EImageType::RGB_24bpp: return m_iter24.GetGreen();
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: return m_iter32.GetGreen();
            case EImageType::BGR_24bpp: return m_iter24.GetGreen();
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: return m_iter32.GetGreen();
            case EImageType::Float1_32bpp: return uint8_t(255.0f * m_iterFloat1.GetValue());
            case EImageType::Float2_64bpp: return uint8_t(255.0f * m_iterFloat2.GetValue().y);
            case EImageType::Float3_96bpp: return uint8_t(255.0f * m_iterFloat3.GetValue().y);
            case EImageType::Float4_128bpp: return uint8_t(255.0f * m_iterFloat4.GetValue().y);
            }
            return 0;
        }

        uint8_t GetBlue()
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: return m_iter1.GetBit() ? 255 : 0;
            case EImageType::Gray_8bpp: return m_iter8.GetGray();
            case EImageType::Gray_16bpp: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
            case EImageType::RGB_24bpp: return m_iter24.GetBlue();
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: return m_iter32.GetBlue();
            case EImageType::BGR_24bpp: return m_iter24.GetBlue();
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: return m_iter32.GetBlue();
            case EImageType::Float1_32bpp: return uint8_t(255.0f * m_iterFloat1.GetValue());
            case EImageType::Float2_64bpp: return uint8_t(255);
            case EImageType::Float3_96bpp: return uint8_t(255.0f * m_iterFloat3.GetValue().z);
            case EImageType::Float4_128bpp: return uint8_t(255.0f * m_iterFloat4.GetValue().z);
            }
            return 0;
        }

        uint8_t GetAlpha()
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: return m_iter1.GetBit() ? 255 : 0;
            case EImageType::Gray_8bpp: return m_iter8.GetGray();
            case EImageType::Gray_16bpp: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
            case EImageType::RGB_24bpp: return m_iter24.GetBlue();
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: return m_iter32.GetBlue();
            case EImageType::BGR_24bpp: return m_iter24.GetBlue();
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: return m_iter32.GetBlue();
            case EImageType::Float1_32bpp: return uint8_t(255.0f * m_iterFloat1.GetValue());
            case EImageType::Float2_64bpp: return uint8_t(255);
            case EImageType::Float3_96bpp: return uint8_t(255);
            case EImageType::Float4_128bpp: return uint8_t(255.0f * m_iterFloat4.GetValue().w);
            }
            return 0;
        }

        int GetGray()
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: return m_iter1.GetBit() ? 255 : 0;
            case EImageType::Gray_8bpp: return m_iter8.GetGray();
            case EImageType::Gray_16bpp: return m_iter16.GetGray();
            case EImageType::RGB_24bpp:
            case EImageType::BGR_24bpp:
            {
                RGBColor rgb(m_iter24.GetRed(), m_iter24.GetGreen(), m_iter24.GetBlue());
                YIQColor yiq(rgb);
                return (int)yiq.y;
            }
            case EImageType::RGBA_32bpp:
            case EImageType::RGBX_32bpp:
            case EImageType::BGRA_32bpp:
            case EImageType::BGRX_32bpp:
            {
                RGBColor rgb(m_iter32.GetRed(), m_iter32.GetGreen(), m_iter32.GetBlue());
                YIQColor yiq(rgb);
                return (int)yiq.y;
            }
            case EImageType::Float1_32bpp: return uint8_t(255.0f * m_iterFloat1.GetValue());
            case EImageType::Float2_64bpp:
            {
                RGBColor rgb(
                    uint8_t(255.0f * m_iterFloat4.GetValue().x),
                    uint8_t(255.0f * m_iterFloat4.GetValue().y),
                    uint8_t(255));
                YIQColor yiq(rgb);
                return (int)yiq.y;
            }
            case EImageType::Float3_96bpp:
            case EImageType::Float4_128bpp:
            {
                RGBColor rgb(
                    uint8_t(255.0f * m_iterFloat4.GetValue().x),
                    uint8_t(255.0f * m_iterFloat4.GetValue().y),
                    uint8_t(255.0f * m_iterFloat4.GetValue().z));
                YIQColor yiq(rgb);
                return (int)yiq.y;
            }
            }
            return 0;
        }

        void SetRed(uint8_t r)
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: (r > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
            case EImageType::Gray_8bpp: m_iter8.SetGray(r); break;
            case EImageType::Gray_16bpp: m_iter16.SetGray(uint16_t(r) << 8); break;
            case EImageType::RGB_24bpp: m_iter24.SetRed(r); break;
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: m_iter32.SetRed(r); break;
            case EImageType::BGR_24bpp: m_iter24.SetRed(r); break;
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: m_iter32.SetRed(r); break;
            case EImageType::Float1_32bpp: m_iterFloat1.SetValue(r / 255.0f); break;
            case EImageType::Float2_64bpp: m_iterFloat2.SetValueX(r / 255.0f); break;
            case EImageType::Float3_96bpp: m_iterFloat3.SetValueX(r / 255.0f); break;
            case EImageType::Float4_128bpp: m_iterFloat4.SetValueX(r / 255.0f); break;
            }
        }

        void SetGreen(uint8_t g)
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: (g > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
            case EImageType::Gray_8bpp: m_iter8.SetGray(g); break;
            case EImageType::Gray_16bpp: m_iter16.SetGray(uint16_t(g) << 8); break;
            case EImageType::RGB_24bpp: m_iter24.SetGreen(g); break;
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: m_iter32.SetGreen(g); break;
            case EImageType::BGR_24bpp: m_iter24.SetGreen(g); break;
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: m_iter32.SetGreen(g); break;
            case EImageType::Float1_32bpp: m_iterFloat1.SetValue(g / 255.0f); break;
            case EImageType::Float2_64bpp: m_iterFloat2.SetValueY(g / 255.0f); break;
            case EImageType::Float3_96bpp: m_iterFloat3.SetValueY(g / 255.0f); break;
            case EImageType::Float4_128bpp: m_iterFloat4.SetValueY(g / 255.0f); break;
            }
        }

        void SetBlue(uint8_t b)
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: (b > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
            case EImageType::Gray_8bpp: m_iter8.SetGray(b); break;
            case EImageType::Gray_16bpp: m_iter16.SetGray(uint16_t(b) << 8); break;
            case EImageType::RGB_24bpp: m_iter24.SetBlue(b); break;
            case EImageType::RGBA_32bpp: // fall through
            case EImageType::RGBX_32bpp: m_iter32.SetBlue(b); break;
            case EImageType::BGR_24bpp: m_iter24.SetBlue(b); break;
            case EImageType::BGRA_32bpp: // fall through
            case EImageType::BGRX_32bpp: m_iter32.SetBlue(b); break;
            case EImageType::Float1_32bpp: m_iterFloat1.SetValue(b / 255.0f); break;
            case EImageType::Float2_64bpp: break;
            case EImageType::Float3_96bpp: m_iterFloat3.SetValueZ(b / 255.0f); break;
            case EImageType::Float4_128bpp: m_iterFloat4.SetValueZ(b / 255.0f); break;
            }
        }

        void SetAlpha(uint8_t a)
        {
            switch (m_imageType)
            {
            case EImageType::BW_1bpp: (a > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
            case EImageType::Gray_8bpp: m_iter8.SetGray(a); break;
            case EImageType::Gray_16bpp: m_iter16.SetGray(uint16_t(a) << 8); break;
            case EImageType::RGB_24bpp: break;
            case EImageType::RGBA_32bpp:
            case EImageType::RGBX_32bpp: m_iter32.SetAlpha(a); break;
            case EImageType::BGR_24bpp: break;
            case EImageType::BGRA_32bpp:
            case EImageType::BGRX_32bpp: m_iter32.SetAlpha(a); break;
            case EImageType::Float1_32bpp: m_iterFloat1.SetValue(a / 255.0f); break;
            case EImageType::Float2_64bpp: break;
            case EImageType::Float3_96bpp: break;
            case EImageType::Float4_128bpp: m_iterFloat4.SetValueW(a / 255.0f); break;
            }
        }
    };
}
