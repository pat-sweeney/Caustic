//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Math.Matrix.Matrix4x3;

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

    bool Matrix4x3::Inverse(Matrix4x3 &result)
    {
        float det;

        det = ((v[0][0])  * ((v[1][1])  * (v[2][2]) - (v[1][2])  * (v[2][1])) -
            (v[0][1])  * ((v[1][0])  * (v[2][2]) - (v[1][2])  * (v[2][0])) +
            (v[0][2])  * ((v[1][0])  * (v[2][1]) - (v[1][1])  * (v[2][0])));
        if (det == 0.0)
            return false;
        det = 1 / det;
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
