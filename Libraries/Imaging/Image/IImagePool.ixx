//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Imaging.Image.IImagePool;
import Base.Core.Core;
import Base.Core.IRefCount;
import Imaging.Image.IImage;

// Namespace: Caustic
export namespace Caustic
{
	//**********************************************************************
	// Interface: IImagePool
	// Interface that allocates a pool of images
	//
	// Module:
	// {Link: import Imaging.Image.IImagePool{Imaging/Image/IImagePool.ixx}}
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
}
