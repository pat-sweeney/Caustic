//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
    extern float Determinant2x2(float a00, float a01, float a10, float a11);
    extern float Determinant3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
    extern void CircumCircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 *pCircumCenter, float *pRadius);
    extern bool PointInTriangleCircumcircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &pt);
}
