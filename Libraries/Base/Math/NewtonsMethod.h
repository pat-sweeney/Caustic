//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
#include <functional>

namespace Caustic
{
    using ObjectiveFunction = std::function<float(float)>;
    using DerivativeFunction = std::function<float(float, ObjectiveFunction)>;

	//**********************************************************************
	// Function: NumericalDifferentiation
	// Computes the 1st derivative of a function via the symmetric difference quotient.
	//
	// Parameters:
	// x - value to compute derivative at
	// objFunc - underlying function being evaluated
	//
	// Header:
	// {Link:#include "Base/Math/NewtonsMethod.h"{Base/Math/NewtonsMethod.h}}
	//**********************************************************************
	extern float NumericalDifferentiation(float x, ObjectiveFunction objFunc);

	//**********************************************************************
	// Function: NewtonsMethod
	// Solves for the root of a function using Newton's method
	//
	// Parameters:
	// initialGuess - initial guess
	// objFunc - underlying function being evaluated
	// derivativeFunc - objFunc's derivative. By default will use numerical differentation to compute.
	//
	// Header:
	// {Link:#include "Base/Math/NewtonsMethod.h"{Base/Math/NewtonsMethod.h}}
	//**********************************************************************
	extern float NewtonsMethod(float initialGuess, ObjectiveFunction objFunc, DerivativeFunction derivativeFunc = NumericalDifferentiation);
}
