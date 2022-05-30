//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Base.Math.Matrix.Matrix3x2;
import Base.Core.Core;

namespace Caustic
{
	bool Matrix3x2::Inverse()
	{
		float detm = Determinant();
		if (detm != 0.0f)
		{
			v[0][0] = v[1][1] / detm;
			v[0][1] = -v[0][1] / detm;
			v[1][0] = -v[1][0] / detm;
			v[1][1] = v[0][0] / detm;
			v[2][0] = (-v[2][0] * v[0][0] - v[2][1] * v[1][0]);
			v[2][1] = (-v[2][0] * v[0][1] - v[2][1] * v[1][1]);
			return true;
		}
		return false;
	}
}