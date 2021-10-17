//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <atlbase.h>
export module Base.Core.ISerialize;
import Base.Core.Core;
import Base.Core.IRefCount;

export namespace Caustic
{
    //**********************************************************************
    // Interface: ISerialize
    // Defines an interface for serializing out objects to some stream. 
	//
	// Module:
	// {Link:import Base.Core.ISerialize;{Base/Core/ISerialize.ixx}}
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
