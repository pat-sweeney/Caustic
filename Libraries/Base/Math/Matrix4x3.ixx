//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string.h>
#include <vector>

export module Base.Math.Matrix.Matrix4x3;
import Base.Core.Core;
import Base.Math.Vector;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Structure: Matrix4x3
    //    3-dimensional affine matrix
    // --- Text ---
    //    0,0    0,1    0,2        a  b  c 
    //    1,0    1,1    1,2        d  e  f
    //    2,0    2,1    2,2        g  h  i
    //    3,0    3,1    3,2        tx ty tz
    // ------------
    //
    // Members:
    // float v[4][3] - Elements of the matrix
    //
    // Module:
    // {Link:import Base.Math.Matrix.Matrix4x3;{Base/Math/Matrix4x3.ixx}}
    //**********************************************************************
    struct Matrix4x3
    {
        float v[4][3];
    public:
        Matrix4x3();
        Matrix4x3(float v00, float v01, float v02,
            float v10, float v11, float v12,
            float v20, float v21, float v22,
            float v30, float v31, float v32);
        Matrix4x3 operator *(const Matrix4x3 &r);
        friend Caustic::Vector3 operator *(const Caustic::Vector3 &p, const Matrix4x3 &m);

        float *operator[](int iIndex) { return &v[iIndex][0]; }

        // Define multiply operator but without translation component
        inline friend Caustic::Vector3 operator %(const Caustic::Vector3 &p, const Matrix4x3 &m)
        {
            Caustic::Vector3 r;
            r.x = (p.x * m.v[0][0] + p.y * m.v[1][0] + p.z * m.v[2][0]);
            r.y = (p.x * m.v[0][1] + p.y * m.v[1][1] + p.z * m.v[2][1]);
            r.z = (p.x * m.v[0][2] + p.y * m.v[1][2] + p.z * m.v[2][2]);
            return r;
        }

        bool Inverse(Matrix4x3 &tm);
    };
}
