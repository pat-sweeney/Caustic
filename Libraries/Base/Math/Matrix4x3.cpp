//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
#include "Base\Math\Matrix.h"

namespace Caustic
{
    Matrix4x3::Matrix4x3()
    {
        v[0][0] = v[1][1] = v[2][2] = 1.0F;
        v[0][1] = v[0][2] = v[1][0] =
        v[1][2] = v[2][0] = v[2][1] =
        v[3][0] = v[3][1] = v[3][2] = 0.0F;
    }

    Matrix4x3::Matrix4x3(float v00, float v01, float v02,
                         float v10, float v11, float v12,
                         float v20, float v21, float v22,
                         float v30, float v31, float v32)
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
        v[3][0] = v30;
        v[3][1] = v31;
        v[3][2] = v32;
    }

    Matrix4x3::Matrix4x3(const Quaternion &q)
    {
        float nq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
        float s = (nq > 0.0F) ? (2.0F / nq) : 0.0F;
        float xs = q.x * s;
        float ys = q.y * s;
        float zs = q.z * s;
        float wx = q.w * xs;
        float wy = q.w * ys;
        float wz = q.w * zs;
        float xx = q.x * xs;
        float xy = q.x * ys;
        float xz = q.x * zs;
        float yy = q.y * ys;
        float yz = q.y * zs;
        float zz = q.z * zs;

        v[0][0] = 1.0F - (yy + zz);
        v[1][0] = xy - wz;
        v[2][0] = xz + wy;
        v[0][1] = xy + wz;
        v[1][1] = 1.0F - (xx + zz);
        v[2][1] = yz - wx;
        v[0][2] = xz - wy;
        v[1][2] = yz + wx;
        v[2][2] = 1.0F - (xx + yy);
        v[3][0] = 0.0F;
        v[3][1] = 0.0F;
        v[3][2] = 0.0F;
    }

    Matrix4x3 Matrix4x3::operator *(const Matrix4x3 &r)
    {
        Matrix4x3 m;

        m.v[0][0] = (v[0][0] * r.v[0][0] + v[0][1] * r.v[1][0] + v[0][2] * r.v[2][0]);
        m.v[0][1] = (v[0][0] * r.v[0][1] + v[0][1] * r.v[1][1] + v[0][2] * r.v[2][1]);
        m.v[0][2] = (v[0][0] * r.v[0][2] + v[0][1] * r.v[1][2] + v[0][2] * r.v[2][2]);
        m.v[1][0] = (v[1][0] * r.v[0][0] + v[1][1] * r.v[1][0] + v[1][2] * r.v[2][0]);
        m.v[1][1] = (v[1][0] * r.v[0][1] + v[1][1] * r.v[1][1] + v[1][2] * r.v[2][1]);
        m.v[1][2] = (v[1][0] * r.v[0][2] + v[1][1] * r.v[1][2] + v[1][2] * r.v[2][2]);
        m.v[2][0] = (v[2][0] * r.v[0][0] + v[2][1] * r.v[1][0] + v[2][2] * r.v[2][0]);
        m.v[2][1] = (v[2][0] * r.v[0][1] + v[2][1] * r.v[1][1] + v[2][2] * r.v[2][1]);
        m.v[2][2] = (v[2][0] * r.v[0][2] + v[2][1] * r.v[1][2] + v[2][2] * r.v[2][2]);
        m.v[3][0] = (v[3][0] * r.v[0][0] + v[3][1] * r.v[1][0] + v[3][2] * r.v[2][0] + r.v[3][0]);
        m.v[3][1] = (v[3][0] * r.v[0][1] + v[3][1] * r.v[1][1] + v[3][2] * r.v[2][1] + r.v[3][1]);
        m.v[3][2] = (v[3][0] * r.v[0][2] + v[3][1] * r.v[1][2] + v[3][2] * r.v[2][2] + r.v[3][2]);
        return m;
    }

    inline Vector3 operator *(const Vector3 &p, const Matrix4x3 &m)
    {
        Vector3 r;
        r.x = (p.x * m.v[0][0] + p.y * m.v[1][0] + p.z * m.v[2][0] + m.v[3][0]);
        r.y = (p.x * m.v[0][1] + p.y * m.v[1][1] + p.z * m.v[2][1] + m.v[3][1]);
        r.z = (p.x * m.v[0][2] + p.y * m.v[1][2] + p.z * m.v[2][2] + m.v[3][2]);
        return r;
    }

    bool Matrix4x3::Inverse(Matrix4x3 &tm)
    {
        float det;

        det = ((v[0][0])  * ((v[1][1])  * (v[2][2]) - (v[1][2])  * (v[2][1])) -
            (v[0][1])  * ((v[1][0])  * (v[2][2]) - (v[1][2])  * (v[2][0])) +
            (v[0][2])  * ((v[1][0])  * (v[2][1]) - (v[1][1])  * (v[2][0])));
        if (det == 0.0)
            return false;
        det = 1 / det;
        Matrix4x3 result;
        result.v[0][0] = (v[1][1] * v[2][2] - v[1][2] * v[2][1]) * det;
        result.v[0][1] = -(v[0][1] * v[2][2] - v[0][2] * v[2][1]) * det;
        result.v[0][2] = (v[0][1] * v[1][2] - v[0][2] * v[1][1]) * det;
        result.v[1][0] = -(v[1][0] * v[2][2] - v[1][2] * v[2][0]) * det;
        result.v[1][1] = (v[0][0] * v[2][2] - v[0][2] * v[2][0]) * det;
        result.v[1][2] = -(v[0][0] * v[1][2] - v[0][2] * v[1][0]) * det;
        result.v[2][0] = (v[1][0] * v[2][1] - v[1][1] * v[2][0]) * det;
        result.v[2][1] = -(v[0][0] * v[2][1] - v[0][1] * v[2][0]) * det;
        result.v[2][2] = (v[0][0] * v[1][1] - v[0][1] * v[1][0]) * det;
        result.v[3][0] = -(v[3][0] * result.v[0][0] + v[3][1] * result.v[1][0] + v[3][2] * result.v[2][0]);
        result.v[3][1] = -(v[3][0] * result.v[0][1] + v[3][1] * result.v[1][1] + v[3][2] * result.v[2][1]);
        result.v[3][2] = -(v[3][0] * result.v[0][2] + v[3][1] * result.v[1][2] + v[3][2] * result.v[2][2]);
        return true;
    }
}
