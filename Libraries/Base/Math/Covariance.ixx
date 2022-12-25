//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Base.Core.Covariance;
import Base.Core.Core;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Function: Covariance
    // Returns the covariance (-1, 0, +1) for a given set of data
    // The covariance measure the linear relationship between a set of data.
    // If the data moves together than it is positive. If it moves in opposite
    // direction than it is negative. If it moves in uncorrelated directions
    // it is 0.
    //
    // Parameters:
    // xdata - x values
    // ydata - y values
    // useSampleVariance - indicates whether to compute the sample or population variance
    //
    // Module:
    // {Link:import Base.Math.Covariance;{Base/Math/Covariance.ixx}}
    //**********************************************************************
    double Covariance(std::vector<double> &xdata, std::vector<double> &ydata, bool useSampleVariance);
}
