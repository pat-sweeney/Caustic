//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include <vector>

namespace Caustic
{
	double Covariance(std::vector<double> &xdata, std::vector<double> &ydata, bool useSampleVariance)
	{
		if (xdata.size() != ydata.size())
		{
			CT(E_UNEXPECTED);
		}
		int n = (int)xdata.size();
		double xmean = 0.0;
		double ymean = 0.0;
		for (size_t i = 0; i < n; i++)
		{
			xmean += xdata[i];
			ymean += ydata[i];
		}
		xmean /= ((double)(useSampleVariance) ? n - 1 : n);
		ymean /= ((double)(useSampleVariance) ? n - 1 : n);
		double sum = 0.0;
		for (size_t i = 0; i < n; i++)
		{
			sum = (xdata[i] - xmean) * (ydata[i] - ymean);
		}
		sum /= ((double)(useSampleVariance) ? n - 1 : n);
		return ((sum < 0.0) ? -1.0 : ((sum > 0.0) ? +1.0 : 0.0));
	}
}
