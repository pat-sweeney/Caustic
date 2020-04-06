//**********************************************************************
// Copyright Patrick Sweeney 1995-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include "Imaging\Image\Image.h"
#include <assert.h>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Class: CImageIter
	// Defines an iterator for walking across the pixels in an image.
	// CImageIter defines the base class. Clients need to used one of the derived
	// classes (CImageIter1, CImageIter8, CImageIter16, CImageIter24, or CImageIter32)
	// to walk across an image.
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
		uint8* m_bp;		// pointer to current byte
		uint32 m_bpl;		// bytes per scan line
	public:
		uint8* PixelData() { return m_bp; }
	};

	//**********************************************************************
	// Class: CImageIter1
	// Defines an iterator for walking across the pixels in an image where
	// each pixel is 1 bit in width.
	//**********************************************************************
	class CImageIter1 : public CImageIter
	{
		uint32 mask;
	public:
		CImageIter1()
		{
		}
		
		CImageIter1(IImage* pImage, int32 x, int32 y)
		{
			mask = 1 << (7 - ((pImage->GetSubX() + x) & 7));
			m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * pImage->GetStride() + (pImage->GetSubX() + x) / 8];
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
	//**********************************************************************
	class CImageIter8 : public CImageIter
	{
	public:
		CImageIter8()
		{
		}

		CImageIter8(IImage* pImage, uint32 x, uint32 y)
		{
			assert(pImage->GetBPP() == 8);
			m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * pImage->GetStride() + (pImage->GetSubX() + x)];
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

		uint8 GetGray() { return m_bp[0]; }
		void SetGray(uint8 v) { m_bp[0] = v; }
	};

	//**********************************************************************
	// Class: CImageIter16
	// Defines an iterator for walking across the pixels in an image where
	// each pixel is 16 bits in width.
	//**********************************************************************
	class CImageIter16 : public CImageIter
	{
	public:
		CImageIter16()
		{
		}

		CImageIter16(IImage* pImage, uint32 x, uint32 y)
		{
			assert(pImage->GetBPP() == 16);
			m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * pImage->GetStride() + (pImage->GetSubX() + x) * 2];
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

		uint16 GetGray() { return ((uint16*)m_bp)[0]; }
		void SetGray(uint16 v) { ((uint16*)m_bp)[0] = v; }
	};

	//**********************************************************************
	// Class: CImageIter24
	// Defines an iterator for walking across the pixels in an image where
	// each pixel is 24 bits in width.
	//**********************************************************************
	class CImageIter24 : public CImageIter
	{
	public:
		CImageIter24()
		{
		}

		CImageIter24(IImage* pImage, uint32 x, uint32 y)
		{
			assert(pImage->GetBPP() == 24);
			m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * pImage->GetStride() + (pImage->GetSubX() + x) * 3];
			m_bpl = pImage->GetStride();
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

		uint8 GetRed() { return m_bp[0]; }
		uint8 GetGreen() { return m_bp[1]; }
		uint8 GetBlue() { return m_bp[2]; }
		void SetRed(uint8 v) { m_bp[0] = v; }
		void SetGreen(uint8 v) { m_bp[1] = v; }
		void SetBlue(uint8 v) { m_bp[2] = v; }
	};

	//**********************************************************************
	// Class: CImageIter32
	// Defines an iterator for walking across the pixels in an image where
	// each pixel is 32 bits in width.
	//**********************************************************************
	class CImageIter32 : public CImageIter
	{
	public:
		CImageIter32()
		{
		}

		CImageIter32(IImage* pImage, uint32 x, uint32 y)
		{
			assert(pImage->GetBPP() == 32);
			m_bp = &pImage->GetData()[(pImage->GetSubY() + y) * pImage->GetStride() + (pImage->GetSubX() + x) * 4];
			m_bpl = pImage->GetStride();
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

		uint8 GetRed() { return m_bp[0]; }
		uint8 GetGreen() { return m_bp[1]; }
		uint8 GetBlue() { return m_bp[2]; }
		uint8 GetAlpha() { return m_bp[3]; }
		void SetRed(uint8 v) { m_bp[0] = v; }
		void SetGreen(uint8 v) { m_bp[1] = v; }
		void SetBlue(uint8 v) { m_bp[2] = v; }
		void SetAlpha(uint8 v) { m_bp[3] = v; }
	};
	
	//**********************************************************************
	// Class: CImageIterGeneric
	// Defines an iterator for walking across the pixels in an image of
	// arbitrary pixel depth. NOTE: These iterators are slower than using
	// the iterator that matches the given image's pixel depth.
	//**********************************************************************
	class CImageIterGeneric : public CImageIter
	{
		CImageIter1 m_iter1;
		CImageIter8 m_iter8;
		CImageIter16 m_iter16;
		CImageIter24 m_iter24;
		CImageIter32 m_iter32;
		int m_bpp;
		int m_x;
		int m_y;
		int m_w;
		int m_h;
	public:
		CImageIterGeneric() :
			m_bpp(0),
			m_x(0),
			m_y(0),
			m_w(0),
			m_h(0)
		{
		}

		CImageIterGeneric(IImage* pImage, int32 x, int32 y)
		{
			m_bpp = pImage->GetBPP();
			if (x < 0)
				x = 0;
			if (x >= (int32)pImage->GetWidth())
				x = (int32)pImage->GetWidth() - 1;
			if (y < 0)
				y = 0;
			if (y >= (int32)pImage->GetHeight())
				y = (int32)pImage->GetHeight() - 1;
			m_x = x;
			m_y = y;
			m_w = pImage->GetWidth();
			m_h = pImage->GetHeight();
			switch (m_bpp)
			{
			case 1:
				m_iter1 = CImageIter1(pImage, x, y);
				break;
			case 8:
				m_iter8 = CImageIter8(pImage, x, y);
				break;
			case 16:
				m_iter16 = CImageIter16(pImage, x, y);
				break;
			case 24:
				m_iter24 = CImageIter24(pImage, x, y);
				break;
			case 32:
				m_iter32 = CImageIter32(pImage, x, y);
				break;
			}
		}

		void Step(CImageIter::EStepDirection dir)
		{
			switch (m_bpp)
			{
			case 1: m_iter1.Step(dir); break;
			case 8: m_iter8.Step(dir); break;
			case 16: m_iter16.Step(dir); break;
			case 24: m_iter24.Step(dir); break;
			case 32: m_iter32.Step(dir); break;
			}
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
			switch (m_bpp)
			{
			case 1: m_iter1.Step(dir); break;
			case 8: m_iter8.Step(dir); break;
			case 16: m_iter16.Step(dir); break;
			case 24: m_iter24.Step(dir); break;
			case 32: m_iter32.Step(dir); break;
			}
			return true;
		}

		uint8 GetRed()
		{
			switch (m_bpp)
			{
			case 1: return m_iter1.GetBit() ? 255 : 0;
			case 8: return m_iter8.GetGray();
			case 16: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
			case 24: return m_iter24.GetRed();
			case 32: return m_iter32.GetRed();
			}
			return 0;
		}

		uint8 GetGreen()
		{
			switch (m_bpp)
			{
			case 1: return m_iter1.GetBit() ? 255 : 0;
			case 8: return m_iter8.GetGray();
			case 16: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
			case 24: return m_iter24.GetGreen();
			case 32: return m_iter32.GetGreen();
			}
			return 0;
		}

		uint8 GetBlue()
		{
			switch (m_bpp)
			{
			case 1: return m_iter1.GetBit() ? 255 : 0;
			case 8: return m_iter8.GetGray();
			case 16: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
			case 24: return m_iter24.GetBlue();
			case 32: return m_iter32.GetBlue();
			}
			return 0;
		}

		uint8 GetAlpha()
		{
			switch (m_bpp)
			{
			case 1: return m_iter1.GetBit() ? 255 : 0;
			case 8: return m_iter8.GetGray();
			case 16: return (m_iter16.GetGray() > 0x7fff) ? 255 : 0;
			case 24: return 255;
			case 32: return m_iter32.GetAlpha();
			}
			return 0;
		}

		void SetRed(uint8 r)
		{
			switch (m_bpp)
			{
			case 1: (r > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
			case 8: m_iter8.SetGray(r); break;
			case 16: m_iter16.SetGray(uint16(r)<<8); break;
			case 24: m_iter24.SetRed(r); break;
			case 32: m_iter32.SetRed(r); break;
			}
		}

		void SetGreen(uint8 g)
		{
			switch (m_bpp)
			{
			case 1: (g > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
			case 8: m_iter8.SetGray(g); break;
			case 16: m_iter16.SetGray(uint16(g) << 8); break;
			case 24: m_iter24.SetGreen(g); break;
			case 32: m_iter32.SetGreen(g); break;
			}
		}

		void SetBlue(uint8 b)
		{
			switch (m_bpp)
			{
			case 1: (b > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
			case 8: m_iter8.SetGray(b); break;
			case 16: m_iter16.SetGray(uint16(b) << 8); break;
			case 24: m_iter24.SetBlue(b); break;
			case 32: m_iter32.SetBlue(b); break;
			}
		}

		void SetAlpha(uint8 a)
		{
			switch (m_bpp)
			{
			case 1: (a > 0x7f) ? m_iter1.SetBit(1) : m_iter1.SetBit(0); break;
			case 8: m_iter8.SetGray(a); break;
			case 16: m_iter16.SetGray(uint16(a) << 8); break;
			case 24: break;
			case 32: m_iter32.SetAlpha(a); break;
			}
		}
	};
}
