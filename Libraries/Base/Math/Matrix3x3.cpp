//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
#include "Base\Math\Matrix.h"

namespace Caustic
{
    float Matrix3x3::det2x2(float a0, float b0, float a1, float b1)
    {
        return (a0 * b1 - b0 * a1);
    }

    void Matrix3x3::_adjoint(Matrix3x3& m)
    {
        m.v[0][0] = det2x2(v[1][1], v[1][2], v[2][1], v[2][2]);
        m.v[1][0] = -det2x2(v[1][0], v[1][2], v[2][0], v[2][2]);
        m.v[2][0] = det2x2(v[1][0], v[1][1], v[2][0], v[2][1]);
        m.v[0][1] = -det2x2(v[0][1], v[0][2], v[2][1], v[2][2]);
        m.v[1][1] = det2x2(v[0][0], v[0][2], v[2][0], v[2][2]);
        m.v[2][1] = -det2x2(v[0][0], v[0][1], v[2][0], v[2][1]);
        m.v[0][2] = det2x2(v[0][1], v[0][2], v[1][1], v[1][2]);
        m.v[1][2] = -det2x2(v[0][0], v[0][2], v[1][0], v[1][2]);
        m.v[2][2] = det2x2(v[0][0], v[0][1], v[1][0], v[1][1]);
    }

    Matrix3x3::Matrix3x3()
    {
        v[0][0] = v[1][1] = v[2][2] = 1.0F;
        v[0][1] = v[0][2] = 
        v[1][0] = v[1][2] =
        v[2][0] = v[2][1] = 0.0f;
    }

    Matrix3x3::Matrix3x3(float v00, float v01, float v02,
        float v10, float v11, float v12,
        float v20, float v21, float v22)
    {
        v[0][0] = v00;
        v[0][1] = v01;
        v[0][2] = v02;
        v[1][0] = v10;
        v[1][1] = v11;
        v[1][2] = v12;
        v[2][0] = v20;
        v[2][1] = v21;
        v[2][2] = v22;
    }

    Matrix3x3 Matrix3x3::operator *(const Matrix3x3& m)
    {
        Matrix3x3 r;
        r.v[0][0] = (v[0][0] * m.v[0][0] + v[0][1] * m.v[1][0] + v[0][2] * m.v[2][0]);
        r.v[0][1] = (v[0][0] * m.v[0][1] + v[0][1] * m.v[1][1] + v[0][2] * m.v[2][1]);
        r.v[0][2] = (v[0][0] * m.v[0][2] + v[0][1] * m.v[1][2] + v[0][2] * m.v[2][2]);
        r.v[1][0] = (v[1][0] * m.v[0][0] + v[1][1] * m.v[1][0] + v[1][2] * m.v[2][0]);
        r.v[1][1] = (v[1][0] * m.v[0][1] + v[1][1] * m.v[1][1] + v[1][2] * m.v[2][1]);
        r.v[1][2] = (v[1][0] * m.v[0][2] + v[1][1] * m.v[1][2] + v[1][2] * m.v[2][2]);
        r.v[2][0] = (v[2][0] * m.v[0][0] + v[2][1] * m.v[1][0] + v[2][2] * m.v[2][0]);
        r.v[2][1] = (v[2][0] * m.v[0][1] + v[2][1] * m.v[1][1] + v[2][2] * m.v[2][1]);
        r.v[2][2] = (v[2][0] * m.v[0][2] + v[2][1] * m.v[1][2] + v[2][2] * m.v[2][2]);
        return r;
    }

    float Matrix3x3::Determinant()
    {
        auto det3x3 = [](float a0, float b0, float c0,
            float a1, float b1, float c1,
            float a2, float b2, float c2) -> float
        {
            return ((a0) * ((b1) * (c2)-(b2) * (c1)) -
                (b0) * ((a1) * (c2)-(a2) * (c1)) +
                (c0) * ((a1) * (b2)-(a2) * (b1)));
        };
        return det3x3(
            v[0][0], v[0][1], v[0][2],
            v[1][0], v[1][1], v[1][2],
            v[2][0], v[2][1], v[2][2]);
    }

    Matrix3x3 Matrix3x3::Adjoint()
    {
        Matrix3x3 r;
        _adjoint(r);
        return r;
    }

    // Compute transpose of matrix placing results back into matrix
    void Matrix3x3::Transpose()
    {
        Swap<float>(v[1][0], v[0][1]);
        Swap<float>(v[2][0], v[0][2]);
        Swap<float>(v[2][1], v[1][2]);
    }

    // Compute transpose of matrix placing results into 'tm'
    void Matrix3x3::Transpose(Matrix3x3& tm)
    {
        tm.v[0][0] = v[0][0];
        tm.v[0][1] = v[1][0];
        tm.v[0][2] = v[2][0];
        tm.v[1][0] = v[0][1];
        tm.v[1][1] = v[1][1];
        tm.v[1][2] = v[2][1];
        tm.v[2][0] = v[0][2];
        tm.v[2][1] = v[1][2];
        tm.v[2][2] = v[2][2];
    }

    // Compute inverse of matrix placing results back into matrix
    bool Matrix3x3::Inverse()
    {
        Matrix3x3 tm;
        if (!Inverse(tm))
            return false;
        memcpy(this, &tm, sizeof(Matrix3x3));
        return true;
    }

    // Compute inverse of matrix placing results in 'tm'
    bool Matrix3x3::Inverse(Matrix3x3& tm)
    {
        float det = Determinant();
        if (IsZero(det))
            return false;
        _adjoint(tm);
        tm.v[0][0] = tm.v[0][0] / det;
        tm.v[0][1] = tm.v[0][1] / det;
        tm.v[0][2] = tm.v[0][2] / det;
        tm.v[1][0] = tm.v[1][0] / det;
        tm.v[1][1] = tm.v[1][1] / det;
        tm.v[1][2] = tm.v[1][2] / det;
        tm.v[2][0] = tm.v[2][0] / det;
        tm.v[2][1] = tm.v[2][1] / det;
        tm.v[2][2] = tm.v[2][2] / det;
        return true;
    }

    Matrix3x3 Matrix3x3::ScalingMatrix(float sx, float sy)
    {
        return Matrix3x3(
            sx, 0.0f, 0.0f,
            0.0f, sy, 0.0f,
            0.0f, 0.0f, 1.0f
        );
    }

    Matrix3x3 Matrix3x3::RotationMatrix(float ax)
    {
        float ca = (float)cos(ax);
        float sa = (float)sin(ax);
        Matrix3x3 rot(
            ca, -sa, 0.0f,
            sa, ca, 0.0f,
            0.0f, 0.0f, 1.0f
        );
        return rot;
    }
    Matrix3x3 Matrix3x3::TranslationMatrix(float tx, float ty)
    {
        return Matrix3x3(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            tx, ty, 1.0f
        );
    }
}
