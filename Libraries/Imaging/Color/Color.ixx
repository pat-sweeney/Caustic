//**********************************************************************
// Copyright Patrick Sweeney 1995-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Imaging.Color;
import Base.Core.Core;
import Base.Math.Vector;

#pragma warning(push)
#pragma warning(disable:26495) // Turn off warnings about uninitialized variables
export namespace Caustic
{
	struct HLSColor;
	struct RGBColor;

	//**********************************************************************
	// Class: YIQColor
	// Defines a color in the YIQ colorspace. See https://en.wikipedia.org/wiki/YIQ for details.
	//
	// Members:
	// <uint8 at Caustic::uint8> y - Y component
	// <uint8 at Caustic::uint8> i - I component
	// <uint8 at Caustic::uint8> q - Q component
	//
	// Module:
	// {Link:import Imaging.Color{Imaging/Color/Color.ixx}}
	//**********************************************************************
	struct YIQColor
	{
		uint8 y, i, q;
	public:
		YIQColor()
		{
		}

		YIQColor(RGBColor& c);
	};

	//**********************************************************************
	// Class: FRGBColor
	// FRGBColor defines an RGB tuple defined in floating point.
	// Each color component is in the range of 0.0 to 1.0
	//
	// Members:
	// <uint8 at Caustic::uint8> r - R component
	// <uint8 at Caustic::uint8> g - G component
	// <uint8 at Caustic::uint8> b - B component
	//
	// Module:
	// {Link:import Imaging.Color{Imaging/Color/Color.ixx}}
	//**********************************************************************
	struct FRGBColor
	{
		float r, g, b;

		FRGBColor()
		{
		}

		FRGBColor(float _r, float _g, float _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		FRGBColor(const Vector3& clr)
		{
			r = clr.x;
			g = clr.y;
			b = clr.z;
		}
	};

	//**********************************************************************
	// Class: FRGBAColor
	// FRGBAColor defines an RGBA tuple defined in floating point.
	// Each color component is in the range of 0.0 to 1.0
	//
	// Members:
	// <uint8 at Caustic::uint8> r - Red component
	// <uint8 at Caustic::uint8> g - Green component
	// <uint8 at Caustic::uint8> b - Blue component
	// <uint8 at Caustic::uint8> a - Alpha component
	//
	// Module:
	// {Link:import Imaging.Color{Imaging/Color/Color.ixx}}
	//**********************************************************************
	struct FRGBAColor
	{
		float r, g, b, a;

		FRGBAColor()
		{
		}

		FRGBAColor(float _r, float _g, float _b, float _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		FRGBAColor(const Vector4& clr)
		{
			r = clr.x;
			g = clr.y;
			b = clr.z;
			a = clr.w;
		}
	};

	//**********************************************************************
	// Class: RGBColor
	// Defines a color in the RGB colorspace. See https://en.wikipedia.org/wiki/RGB_color_model for details.
	// Each color component is in the range of 0 to 255
	//
	// Members:
	// <uint8 at Caustic::uint8> r - R component
	// <uint8 at Caustic::uint8> g - G component
	// <uint8 at Caustic::uint8> b - B component
	//
	// Module:
	// {Link:import Imaging.Color{Imaging/Color/Color.ixx}}
	//**********************************************************************
	struct RGBColor
	{
		uint8 r, g, b;

		int value(int n1, int n2, int hue)
		{
			if (hue > 360)
				hue -= 360;
			else if (hue < 0)
				hue += 360;
			if (hue < 60)
				return n1 + (n2 - n1) * hue / 60;
			if (hue < 180)
				return n2;
			if (hue < 240)
				return n1 + (n2 - n1) * (240 - hue) / 60;
			return n1;
		}
	public:
		RGBColor()
		{
		}

		RGBColor(uint8 _r, uint8 _g, uint8 _b)
		{
			r = _r;
			g = _g;
			b = _b;
		}

		RGBColor(HLSColor& c);
		RGBColor(YIQColor& c);
	};

	//**********************************************************************
	// Class: HLSColor
	// Defines a color in the HLS colorspace. See https://en.wikipedia.org/wiki/YIQ for details.
	// Each color component is in the range of 0 to 255
	//
	// Members:
	// <uint8 at Caustic::uint8> r - R component
	// <uint8 at Caustic::uint8> g - G component
	// <uint8 at Caustic::uint8> b - B component
	//
	// Module:
	// {Link:import Imaging.Color{Imaging/Color/Color.ixx}}
	//**********************************************************************
	struct HLSColor
	{
		uint8 h, l, s;
	public:
		HLSColor()
		{
		}

		HLSColor(RGBColor& c)
		{
			int r = (int)c.r;
			int g = (int)c.g;
			int b = (int)c.b;
			int max = r;
			if (g > max) max = g;
			if (b > max) max = b;
			int min = r;
			if (g < min) min = g;
			if (b < min) min = b;
			int _l = (max + min) / 2;
			int _h, _s;
			if (max == min)
			{
				_s = 0;
				_h = 360;
			}
			else
			{
				int delta = max - min;
				if (_l <= 127)
					_s = (delta * 255) / (max + min);
				else
					_s = (delta * 255) / (510 - max - min);
				if (r == max)
					_h = (60 * (g - b)) / (255 * delta);
				else if (g == max)
					_h = 120 + (60 * (b - r)) / (255 * delta);
				else if (b == max)
					_h = 240 + (60 * (r - g)) / (255 * delta);
				if (_h < 0)
					_h += 360;
			}
			_h = (255 * _h) / 360;
			h = (uint8)_h;
			l = (uint8)_l;
			s = (uint8)_s;
		}
	};

	inline RGBColor::RGBColor(HLSColor& c)
	{
		int m2;
		if (c.l <= 127)
			m2 = c.l + (c.s * c.l) / 255;
		else
			m2 = c.l + c.s - (c.l * c.s) / 255;
		int m1 = 2 * c.l - m2;
		if (c.s == 0)
			r = g = b = c.l;
		else
		{
			r = (uint8)value(m1, m2, (360 * c.h) / 255 + 120);
			g = (uint8)value(m1, m2, (360 * c.h) / 255);
			b = (uint8)value(m1, m2, (360 * c.h) / 255 - 120);
		}
	}

	//
	// rgb -> yiq:
	// 
	// 	[ 0.299	 0.587	 0.114  ]  [ R ]
	// 	[ 0.596	 -0.275	 -0.321 ]  [ G ]
	// 	[ 0.212	 -0.528	 0.311  ]  [ B ]
	// 
	// yiq -> rgb:
	// 	[ 1.003089  0.954849   0.617860  ] [ Y ]
	// 	[ 0.996776  -0.270706  -0.644788 ] [ I ]
	// 	[ 1.008498  -1.110485  1.699568  ] [ Q ]
	//
	inline YIQColor::YIQColor(RGBColor& c)
	{
		int _y = ((int)c.r * 299 + (int)c.g * 587 + (int)c.b * 114) / 1000;
		int _i = ((int)c.r * 596 + (int)c.g * -275 + (int)c.b * -321) / 1000;
		int _q = ((int)c.r * 212 + (int)c.g * -528 + (int)c.b * 311) / 1000;
		y = (uint8)Caustic::Clamp(_y, 0, 255);
		i = (uint8)Caustic::Clamp(_i, 0, 255);
		q = (uint8)Caustic::Clamp(_q, 0, 255);
	}

	inline RGBColor::RGBColor(YIQColor& c)
	{
		int _r = ((int)c.y * 1003 + (int)c.i * 955 + (int)c.q * 618) / 1000;
		int _g = ((int)c.y * 997 + (int)c.i * -271 + (int)c.q * -645) / 1000;
		int _b = ((int)c.y * 1008 + (int)c.i * -1110 + (int)c.q * 1700) / 1000;
		r = (uint8)Caustic::Clamp(_r, 0, 255);
		g = (uint8)Caustic::Clamp(_g, 0, 255);
		b = (uint8)Caustic::Clamp(_b, 0, 255);
	}
}
#pragma warning(pop)
