//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <cinttypes>

export module Imaging.Image.IImage;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.GeomDS.IPath2;

// Namespace: Caustic
// Module:
// {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
export namespace Caustic
{
    //**********************************************************************
    // Class: EImageType
    // Defines the types of images we currently support.
    //
    // Parameters:
    // Unknown - type is undefined
    // BW_1bpp - type is black/white image with 1 bit per pixel
    // Gray_8bpp - type is grayscale image with 8 bits per pixel
    // Gray_16bpp - type is grayscale image with 16 bits per pixel
    // RGB_24bpp - type is RGB image with 24 bits per pixel
    // RGBA_32bpp - type is RGBA image with 32 bits per pixel
    // RGBX_32bpp - type is RGBX image with 32 bits per pixel (X is ignored alpha)
    // BGR_24bpp - type is BGR image with 24 bits per pixel
    // BGRA_32bpp - type is BGRA image with 32 bits per pixel
    // BGRX_32bpp - type is BGRX image with 32 bits per pixel (X is ignored alpha)
    // Float_32bpp - type is floating point image
    // Float4_128bpp - type is floating point image with 4 channels
    // NV12_4_2_0 - YUV format using 4:2:0 sampling (see https://en.wikipedia.org/wiki/YUV YUV420)
    //
    // Module:
    // {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
    //**********************************************************************
    enum class EImageType
    {
        Unknown,
        BW_1bpp,
        Gray_8bpp,
        Gray_16bpp,
        RGB_24bpp,
        RGBA_32bpp,
        RGBX_32bpp,
        BGR_24bpp,
        BGRA_32bpp,
        BGRX_32bpp,
        Float1_32bpp,
        Float2_64bpp,
        Float3_96bpp,
        Float4_128bpp,
        NV12_420,
    };

    //**********************************************************************
    // Interface: IImageBase
    // Base interface shared across all image types
    //
    // Module:
    // {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
    //**********************************************************************
    struct IImageBase : public IRefCount
    {
        //**********************************************************************
        // Method: GetImageType
        // Returns the type of data stored in the image
        //**********************************************************************
        virtual EImageType GetImageType() = 0;

        //**********************************************************************
        // Method: GetData
        // Returns a pointer to the image data
        //**********************************************************************
        virtual uint8_t* GetData() = 0;

        //**********************************************************************
        // Method: GetWidth
        // Returns the image's width in pixels
        //**********************************************************************
        virtual uint32_t GetWidth() = 0;

        //**********************************************************************
        // Method: GetHeight
        // Returns the image's height in pixels
        //**********************************************************************
        virtual uint32_t GetHeight() = 0;

        //**********************************************************************
        // Method: GetSubX
        // Returns the X offset into our parent image (if this is a subimage)
        //**********************************************************************
        virtual int32_t GetSubX() = 0;

        //**********************************************************************
        // Method: GetSubY
        // Returns the Y offset into our parent image (if this is a subimage)
        //**********************************************************************
        virtual int32_t GetSubY() = 0;

        //**********************************************************************
        // Method: GetParent
        // Returns the parent image. This is only not nullptr if the image
        // is a subimage of another image.
        //**********************************************************************
        virtual CRefObj<IImageBase> GetParent() = 0;

        //**********************************************************************
        // Method: GetBPP
        // Returns the image's bits per pixel
        //**********************************************************************
        virtual uint32_t GetBPP() = 0;

        //**********************************************************************
        // Method: GetStride
        // Returns the image's stride (bytes/scanline)
        //**********************************************************************
        virtual uint32_t GetStride() = 0;
    };

    //**********************************************************************
    // Interface: IImage
    // Interface to an image (CPU based)
    //
    // Module:
    // {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
    //**********************************************************************
    struct IImage : public IImageBase
    {
        //**********************************************************************
        // Method: Clear
        // Erases the image (setting it to black with full alpha)
        //**********************************************************************
        virtual void Clear() = 0;

        //**********************************************************************
        // Method: Clone
        // Makes a copy of the image
        //
        // Returns:
        // Copy of the image
        //**********************************************************************
        virtual CRefObj<IImage> Clone() = 0;

        //**********************************************************************
        // Method: TakeDataOwnership
        // If the image was created with a pre-existing block of image data,
        // this function will transfer ownership of that data to the image class.
        // After calling this the Image class will be responsible for deleting
        // the data when the image is detroyed. Otherwise, the app will be
        // responsible for freeing the data.
        //**********************************************************************
        virtual void TakeDataOwnership() = 0;

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
        virtual void SetPixel(int32_t x, int32_t y, uint8_t color[4]) = 0;

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
        virtual void SetPixel(int32_t x, int32_t y, uint8_t gray) = 0;

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
        virtual void SetPixel(int32_t x, int32_t y, uint16_t v) = 0;

        //**********************************************************************
        // Method: DrawCircle
        // Renders a circle on the image
        //
        // Parameters:
        // center - center of the circle in pixel coordinates
        // radius - radius of the circle (in pixels)
        // color - color used for rendering the circle
        //**********************************************************************
        virtual void DrawCircle(Vector2& center, uint32_t radius, uint8_t color[4]) = 0;

        //**********************************************************************
        // Method: DrawLine
        // Renders a line on the image
        //
        // Parameters:
        // v0 - start of line (in pixel coordinates)
        // v1 - end of line (in pixel coordinates)
        // color - color used for rendering the line
        //**********************************************************************
        virtual void DrawLine(const Vector2& v0, const Vector2& v1, uint8_t color[4]) = 0;

        //**********************************************************************
        // Method: DrawPath
        // Renders a path on the image
        //
        // Parameters:
        // pPath - path to render
        //**********************************************************************
        virtual void DrawPath(IPath2* pPath) = 0;
    };

    //**********************************************************************
    // Interface: IIntegralImage
    // Interface to an integral image (see https://en.wikipedia.org/wiki/Summed-area_table)
    //
    // Module:
    // {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
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
        virtual uint32_t GetSum(int channel, int x1, int y1, int x2, int y2) = 0;

        //**********************************************************************
        // Method: BoxBlur
        // Blurs the image
        //
        // Parameters:
        // width - width of box filter
        // height - height of box filter
        //
        // Returns:
        // Returns the blurred image
        //**********************************************************************
        virtual CRefObj<IImage> BoxBlur(int width, int height) = 0;
    };

    CRefObj<IImage> CreateImage(uint32_t width, uint32_t height, EImageType imageType);
    CRefObj<IIntegralImage> CreateIntegralImage(IImage* pImage);

    //**********************************************************************
    // Function: LoadImageFile
    // Loads an image from a file
    //
    // Parameters:
    // pFilename - path to image file
    //
    // Returns:
    // Returns the loaded image
    //
    // Module:
    // {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
    //**********************************************************************
    CRefObj<IImage> LoadImageFile(const wchar_t* pFilename);

    //**********************************************************************
    // Function: StoreImage
    // Saves an image from a file
    //
    // Parameters:
    // pFilename - path to save image file
    // pImage - Image to save
    //
    // Module:
    // {Link:import Imaging.Image.Image{Imaging/Image/Image.ixx}}
    //**********************************************************************
    void StoreImage(const wchar_t* pFilename, IImage* pImage);
}
