//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>

export module Base.Math.NewtonsMethod;
import Base.Core.Core;

export namespace Caustic
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
	// Module:
	// {Link:import Base.Math.NewtonsMethod;{Base/Math/NewtonsMethod.ixx}}
	//**********************************************************************
	float NumericalDifferentiation(float x, ObjectiveFunction objFunc);

	//**********************************************************************
	// Function: NewtonsMethod
	// Solves for the root of a function using Newton's method
	//
	// Parameters:
	// initialGuess - initial guess
	// objFunc - underlying function being evaluated
	// derivativeFunc - objFunc's derivative. By default will use numerical differentation to compute.
	//
	// Module:
	// {Link:import Base.Math.NewtonsMethod;{Base/Math/NewtonsMethod.ixx}}
	//**********************************************************************
	float NewtonsMethod(float initialGuess, ObjectiveFunction objFunc, DerivativeFunction derivativeFunc = NumericalDifferentiation);
}
