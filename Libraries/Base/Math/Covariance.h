//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include <vector>

namespace Caustic
{
	//**********************************************************************
	//! Returns the covariance (-1, 0, +1) for a given set of data
	//**********************************************************************
	double Covariance(std::vector<double> &xdata, std::vector<double> &ydata, bool useSampleVariance);
}
