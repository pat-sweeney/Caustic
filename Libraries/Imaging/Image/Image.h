//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Interface: IImageBase
	// Base interface shared across all image types
	//**********************************************************************
	struct IImageBase : public IRefCount
	{
		//**********************************************************************
		// Method: GetData
		// Returns a pointer to the image data
		//**********************************************************************
		virtual uint8 *GetData() = 0;

		//**********************************************************************
		// Method: GetWidth
		// Returns the image's width in pixels
		//**********************************************************************
		virtual uint32 GetWidth() = 0;

		//**********************************************************************
		// Method: GetHeight
		// Returns the image's height in pixels
		//**********************************************************************
		virtual uint32 GetHeight() = 0;

		//**********************************************************************
		// Method: GetBPP
		// Returns the image's bites per pixel
		//**********************************************************************
		virtual uint32 GetBPP() = 0;

		//**********************************************************************
		// Method: GetStride
		// Returns the image's stride (bytes/scanline)
		//**********************************************************************
		virtual uint32 GetStride() = 0;

		//**********************************************************************
		// Method: GetBytesPerPixel
		// Returns the number of bytes in each pixel
		//**********************************************************************
		virtual uint32 GetBytesPerPixel() = 0;
	};

	//**********************************************************************
	// Interface: IImage
	// Interface to an image (CPU based)
	//**********************************************************************
	struct IImage : public IImageBase
	{
		//**********************************************************************
		// Method: GaussianBlur
		// Perfoms a gaussian blur on the image
		//
		// Parameters:
		// sigma - sigma for determining width of gaussian filter
		// ppImage - Returns the blurred image
		//**********************************************************************
		virtual void GaussianBlur(float sigma, IImage **ppImage) = 0;

		//**********************************************************************
		// Method: ShowAlpha
		// Outputs an image that has the alpha applied to the image data.
		// The output image is a checkerboard blended with the image data.
		//
		// Parameters:
		// boxSize - size of checkerboard square
		// ppResult - Image with alpha applied
		//**********************************************************************
		virtual void ShowAlpha(int boxSize, IImage **ppResult) = 0;

		//**********************************************************************
		// Method: Clone
		// Makes a copy of the image
		//
		// Parameters:
		// ppImage - Copy of the image
		//**********************************************************************
		virtual void Clone(IImage **ppImage) = 0;

		//**********************************************************************
		// Method: DrawCircle
		// Renders a circle on the image
		//
		// Parameters:
		// center - center of the circle in pixel coordinates
		// radius - radius of the circle (in pixels)
		// color - color used for rendering the circle
		//**********************************************************************
		virtual void DrawCircle(Vector2 &center, uint32 radius, uint8 color[4]) = 0;

		//**********************************************************************
		// Method: DrawLine
		// Renders a line on the image
		//
		// Parameters:
		// v0 - start of line (in pixel coordinates)
		// v1 - end of line (in pixel coordinates)
		// color - color used for rendering the line
		//**********************************************************************
		virtual void DrawLine(Vector2 &v0, Vector2 &v1, uint8 color[4]) = 0;

		//**********************************************************************
		// Method: Clear
		// Erases the image (setting it to black with full alpha)
		//**********************************************************************
		virtual void Clear() = 0;

		//**********************************************************************
		// Method: SetPixel
		// Sets a pixel in the image. This method assumes the underlying image
		// is BGRA.
		//
		// Parameters:
		// x - pixel's X coordinate
		// y - pixel's Y coordinate
		// color - color to set pixel to
		//**********************************************************************
		virtual void SetPixel(uint32 x, uint32 y, uint8 color[4]) = 0;

		//**********************************************************************
		// Method: SetPixel
		// Sets a pixel in the image. This method assumes the underlying image
		// is gray scale.
		//
		// Parameters:
		// x - pixel's X coordinate
		// y - pixel's Y coordinate
		// gray - color to set pixel to
		//**********************************************************************
		virtual void SetPixel(uint32 x, uint32 y, uint8 gray) = 0;

		//**********************************************************************
		// Method: SetPixel
		// Sets a pixel in the image. This method assumes the underlying image
		// is 16b grayscale.
		//
		// Parameters:
		// x - pixel's X coordinate
		// y - pixel's Y coordinate
		// v - color to set pixel to
		//**********************************************************************
		virtual void SetPixel(uint32 x, uint32 y, uint16 v) = 0;
	};

	extern void CreateImage(uint32 width, uint32 height, uint32 bpp, IImage **ppImage);

	//**********************************************************************
	// Interface: IIntegralImage
	// Interface to an integral image (see https://en.wikipedia.org/wiki/Summed-area_table)
	//**********************************************************************
	struct IIntegralImage : public IImageBase
	{
		//**********************************************************************
		// Method: GetSum
		// Returns the sum of the requested area
		//
		// Parameters:
		// channel - which color channel to sum
		// x1 - top left corner's X coordinate (in pixel coordinates)
		// y1 - top left corner's Y coordinate (in pixel coordinates)
		// x2 - bottom right corner's X coordinate (in pixel coordinates)
		// y2 - bottom right corner's Y coordinate (in pixel coordinates)
		//**********************************************************************
		virtual uint32 GetSum(int channel, int x1, int y1, int x2, int y2) = 0;

		//**********************************************************************
		// Method: BoxBlur
		// Blurs the image
		//
		// Parameters:
		// width - width of box filter
		// height - height of box filter
		// ppImage - Returns the blurred image
		//**********************************************************************
		virtual void BoxBlur(int width, int height, IImage **ppImage) = 0;
	};
	extern void CreateIntegralImage(IImage *pImage, IIntegralImage **ppImage);

#undef LoadImage
}
// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Function: LoadImage
	// Loads an image from a file
	//
	// Parameters:
	// pFilename - path to image file
	// ppImage - Returns the loaded image
	//**********************************************************************
	void LoadImage(const wchar_t *pFilename, IImage **ppImage);

	//**********************************************************************
	// Function: StoreImage
	// Saves an image from a file
	//
	// Parameters:
	// pFilename - path to save image file
	// pImage - Image to save
	//**********************************************************************
	void StoreImage(const wchar_t *pFilename, IImage *pImage);
}
