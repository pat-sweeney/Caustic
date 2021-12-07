//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.ICausticFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.CausticFactory;

//**********************************************************************
// File: ICausticFactory.cpp
//**********************************************************************

namespace Caustic
{
	CRefObj<ICausticFactory> CCausticFactory::factory;

	//**********************************************************************
	// Function: CreateCausticFactory
	// Global function for creating the main Caustic factory.
	//
	// Returns:
	// ppFactory - Returns the newly created Caustic factory.
	// 
	// Module:
	// {Link:import Rendering.Caustic.ICausticFactory;{Rendering/Caustic/ICausticFactory.cpp}}
	//**********************************************************************
	CRefObj<ICausticFactory> CreateCausticFactory()
	{
		return CRefObj<ICausticFactory>(new CCausticFactory());
	}
}
