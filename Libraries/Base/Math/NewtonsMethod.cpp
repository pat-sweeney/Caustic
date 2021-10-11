//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "NewtonsMethod.h"
#include <functional>

namespace Caustic
{
    float NumericalDifferentiation(float x, ObjectiveFunction objFunc)
    {
        float h = 0.00001f;
        float x1 = objFunc(x - h);
        float x2 = objFunc(x + h);
        return (x2 - x1) / (2 * h);
    }

    float NewtonsMethod(float initialGuess, ObjectiveFunction objFunc, DerivativeFunction derivativeFunc /*= NumericalDifferentiation*/)
    {
        int i = 0;
        const int maxIterations = 5;
        float Xi = initialGuess;
        while (i++ < maxIterations)
        {
            float objValue = objFunc(Xi);
            if (Caustic::IsZero(objValue))
                break;
            Xi = Xi - objValue / derivativeFunc(Xi, objFunc);
        }
        return Xi;
    }
}
