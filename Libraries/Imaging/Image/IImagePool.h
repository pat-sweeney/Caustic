//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Interface: IImagePool
	// Interface that allocates a pool of images
	//
	// Header:
	// {Link:#include "Imaging/Image/IImagePool.h"{Imaging/Image/IImagePool.h}}
	//**********************************************************************
	struct IImagePool : public IRefCount
	{
		//**********************************************************************
		// Method: Acquire
		// Acquires a new image from the pool, either by allocating an image
		// or reusing an existing image
		//
		// Parameters:
		// width - width of desired image in pixels
		// height - height of desired image in pixels
		// bpp - bits per pixel
		//**********************************************************************
		virtual CRefObj<IImage> Acquire(uint32 width, uint32 height, uint32 bpp) = 0;
	};

	CRefObj<IImagePool> CreateImagePool(uint32 maxImages, uint32 width, uint32 height, uint32 bpp);
}
