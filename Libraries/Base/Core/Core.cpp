//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Core.h"

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Function: GetCausticRootDirectory
	// Returns the default folder for the Caustic library
	//**********************************************************************
	std::wstring GetCausticRootDirectory()
	{
		return std::wstring(CAUSTIC_ROOT_DIRECTORY);
	}

	//**********************************************************************
	// Function: GetCausticShaderDirectory
	// Returns the default folder for the Caustic shaders
	//**********************************************************************
	std::wstring GetCausticShaderDirectory()
	{
		return std::wstring(CAUSTIC_SHADER_DIRECTORY);
	}
}
