//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Core.h"
#include "IRefCount.h"
#include <atlbase.h>

namespace Caustic
{
    //**********************************************************************
    // Interface: ISerialize
    // Defines an interface for serializing out objects to some stream. 
	//
	// Header:
	// [Link:Base/Core/ISerialize.h]
	//**********************************************************************
    struct ISerialize : public IRefCount
    {
		//**********************************************************************
		// Method: Load
		// Loads an object from the specified stream
		//
		// Parameters:
		// pStream - Stream to read from
		//**********************************************************************
		virtual void Load(IStream *pStream) = 0;

		//**********************************************************************
		// Method: Store
		// Saves an object to the specified stream
		//
		// Parameters:
		// pStream - Stream to write to
		//**********************************************************************
		virtual void Store(IStream *pStream) = 0;
    };
}
