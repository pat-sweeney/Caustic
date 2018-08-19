//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include <math.h>

namespace Caustic
{
    struct Matrix4x4;
    struct Matrix4x3;

    struct Quaternion
    {
        float x, y, z, w;
    public:
        Quaternion();
        Quaternion(Matrix4x4 &tm);
        Quaternion(Matrix4x3 &tm);
        Quaternion(Caustic::Vector3 *vec, float angle);
        Quaternion(float _x, float _y, float _z, float _w, bool isvec = false);
        Quaternion operator *(Quaternion &q);
        Quaternion &operator *=(Quaternion &q)
        {
            *this = *this * q;
            return *this;
        }
        Quaternion Conjugate();
        Quaternion Slerp(Quaternion &q1, Quaternion &q2, float t);
    };
}
