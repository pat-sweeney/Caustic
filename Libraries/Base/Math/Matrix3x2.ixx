//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Base.Math.Matrix.Matrix3x2;
import Base.Core.Core;
import Base.Math.Vector;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Structure: Matrix3x2
    // Defines a 3x2 matrix as:
    //   | a  b  |
    //   | c  d  |
    //   | tx ty |
    //
    // Members:
    // float v[3][2] - Elements of the matrix
    //
    // Module:
    // {Link:import Base.Math.Matrix.Matrix3x2;{Base/Math/Matrix3x2.ixx}}
    //**********************************************************************
    struct Matrix3x2
    {
        float v[3][2];

        Matrix3x2()
        {
            v[0][0] = 1.0f;
            v[0][1] = 0.0f;
            v[1][0] = 0.0f;
            v[1][1] = 1.0f;
            v[2][0] = 0.0f;
            v[2][1] = 0.0f;
        }

        Matrix3x2(float v00, float v01, float v10, float v11, float v20, float v21)
        {
            v[0][0] = v00;
            v[0][1] = v01;
            v[1][0] = v10;
            v[1][1] = v11;
            v[2][0] = v20;
            v[2][1] = v21;
        }

        float Determinant()
        {
            return v[0][0] * v[1][1] - v[0][1] * v[1][0];
        }

        bool Inverse();

        Matrix3x2 operator *(const Matrix3x2& r)
        {
            Matrix3x2 m;
            m.v[0][0] = (v[0][0] * r.v[0][0] + v[0][1] * r.v[1][0]);
            m.v[0][1] = (v[0][0] * r.v[0][1] + v[0][1] * r.v[1][1]);
            m.v[1][0] = (v[1][0] * r.v[0][0] + v[1][1] * r.v[1][0]);
            m.v[1][1] = (v[1][0] * r.v[0][1] + v[1][1] * r.v[1][1]);
            m.v[2][0] = (v[2][0] * r.v[0][0] + v[2][1] * r.v[1][0] + r.v[2][0]);
            m.v[2][1] = (v[2][0] * r.v[0][1] + v[2][1] * r.v[1][1] + r.v[2][1]);
            return m;
        }


        friend inline Caustic::Vector2 operator *(const Vector2& p, const Matrix3x2& m)
        {
            Vector2 r;
            r.x = (p.x * m.v[0][0] + p.y * m.v[1][0] + m.v[2][0]);
            r.y = (p.x * m.v[0][1] + p.y * m.v[1][1] + m.v[2][1]);
            return r;
        }

        friend inline Caustic::Vector2 operator *=(Caustic::Vector2& p, Matrix3x2& tm)
        {
            p = p * tm;
            return p;
        }

        //
        // Define multiply operator but without translation component
        //
        friend inline Vector2 operator %(const Vector2& p, const Matrix3x2& m)
        {
            Vector2 r;
            r.x = (p.x * m.v[0][0] + p.y * m.v[1][0]);
            r.y = (p.x * m.v[0][1] + p.y * m.v[1][1]);
            return r;
        }
    };
}
