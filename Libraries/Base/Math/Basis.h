//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.Error;
#include <float.h>
#include <winerror.h>

namespace Caustic
{
	//**********************************************************************
	// Function: BersteinBasis
	// Returns the value of the Nth variable for a cubic polynomial using
	// the Berstein basis functions (i.e. used by cubic Bezier curves)
	//
	// Parameters:
	// index - which variable (0 - 3)
	// u - parameteric value to evaluate at (0.0 - 1.0)
	//
	// Header:
	// [Link:Base/Math/Basis.h]
	//**********************************************************************
	float BersteinBasis(int index, float u)
	{
		float invU = (1 - u);
		switch (index)
		{
		case 0:
			return invU * invU * invU;
		case 1:
			return 3 * u * invU * invU;
		case 2:
			return 3 * u * u * invU;
		case 3:
			return 3 * u * u * u;
		default:
			break;
		}
		CT(E_UNEXPECTED);
		return 0.0f;
	}
}
