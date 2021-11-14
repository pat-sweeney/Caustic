//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string.h>
#include <vector>

export module Base.Math.Matrix.Matrix3x3;
import Base.Core.Core;
import Base.Math.Vector;

// Namespace: Caustic
export namespace Caustic
{
	//**********************************************************************
	// Structure: Matrix2x2
	// Defines a 2x2 matrix
    //
    // Members:
    // float v[2][2] - Elements of the matrix
    //
    // Module:
    // {Link:import Base.Math.Matrix.Matrix3x3;{Base/Math/Matrix3x3.ixx}}
    //**********************************************************************
	struct Matrix2x2
    {
        float v[2][2];

        float Determinant()
        {
            return v[0][0] * v[1][1] - v[0][1] * v[1][0];
        }
    };

    //**********************************************************************
    // Structure: Matrix3x3
    // A 3x3 matrix
    //
    // Members:
    // float v[3][3] - Elements of the matrix
    //
    // Module:
    // {Link:import Base.Math.Matrix.Matrix3x3;{Base/Math/Matrix3x3.ixx}}
    //**********************************************************************
    struct Matrix3x3
    {
        float v[3][3];

        void _adjoint(Matrix3x3& m);
    public:
        Matrix3x3();
        Matrix3x3(float v00, float v01, float v02,
            float v10, float v11, float v12,
            float v20, float v21, float v22);
        
        //**********************************************************************
        // Method: FromRodrigues
        // Constructs a 3x3 matrix from a Rodrigues rotation vector
        //
        // Parameters:
        // v - Rodrigues rotation vector
        //
        // Returns:
        // Matrix representing the rotation
        //**********************************************************************
        static Matrix3x3 FromRodrigues(Vector3& v)
        {
            float omega = v.Length();
            v = v / omega;
            float ca = (float)cos(omega);
            float sa = (float)sin(omega);
            Matrix3x3 i(ca, 0.0f, 0.0f,
                0.0f, ca, 0.0f,
                0.0f, 0.0f, ca);
            float ca1 = 1.0f - ca;
            Matrix3x3 r(ca1 * v.x * v.x, ca1 * v.x * v.y, ca1 * v.x * v.z,
                ca1 * v.y * v.x, ca1 * v.y * v.y, ca1 * v.y * v.z,
                ca1 * v.z * v.x, ca1 * v.z * v.y, ca1 * v.z * v.z);
            Matrix3x3 s(0.0f, sa * -v.z, sa * v.y,
                sa * v.z, 0.0f, sa * -v.x,
                sa * -v.y, sa * v.x, 0.0f);
            return i + r + s;
        }

        float det2x2(float a0, float b0, float a1, float b1);
        
        float* operator [](int iIndex)
        {
            return v[iIndex];
        }

        inline Matrix3x3 operator +(const Matrix3x3& r)
        {
            Matrix3x3 nr(
                v[0][0] + r.v[0][0], v[0][1] + r.v[0][1], v[0][2] + r.v[0][2],
                v[1][0] + r.v[1][0], v[1][1] + r.v[1][1], v[1][2] + r.v[1][2],
                v[2][0] + r.v[2][0], v[2][1] + r.v[2][1], v[2][2] + r.v[2][2]);
            return nr;
        }

        friend inline Caustic::Vector3 operator *(const Caustic::Vector3& p, const Matrix3x3& tm)
        {
            Caustic::Vector3 r;
            r.x = (p.x * tm.v[0][0] + p.y * tm.v[1][0] + p.z * tm.v[2][0]);
            r.y = (p.x * tm.v[0][1] + p.y * tm.v[1][1] + p.z * tm.v[2][1]);
            r.z = (p.x * tm.v[0][2] + p.y * tm.v[1][2] + p.z * tm.v[2][2]);
            return r;
        }

        friend inline Caustic::Vector3 operator *(const Caustic::Vector2& p, const Matrix3x3& tm)
        {
            Caustic::Vector3 r;
            r.x = (p.x * tm.v[0][0] + p.y * tm.v[1][0] + tm.v[2][0]);
            r.y = (p.x * tm.v[0][1] + p.y * tm.v[1][1] + tm.v[2][1]);
            r.z = (p.x * tm.v[0][2] + p.y * tm.v[1][2] + tm.v[2][2]);
            return r;
        }

        friend inline Caustic::Vector3 operator *=(Caustic::Vector3& p, Matrix3x3& tm)
        {
            p = p * tm;
            return p;
        }

        Matrix3x3 operator *(const Matrix3x3& m);

        //**********************************************************************
        // Method: Determinant
        // Returns the determinant of the matrix
        //**********************************************************************
        float Determinant();

        //**********************************************************************
        // Method: Adjoint
        // Returns the adjoint of the matrix
        //**********************************************************************
        Matrix3x3 Adjoint();

        //**********************************************************************
        // Method: Transpose
        // Computes the inplace transpose of the matrix
        //**********************************************************************
        void Transpose();

        //**********************************************************************
        // Method: Transpose
        // Returns the transpose of the matrix
        //**********************************************************************
        void Transpose(Matrix3x3& tm);

        //**********************************************************************
        // Method: Inverse
        // Computes the inplace inverse of the matrix
        //**********************************************************************
        bool Inverse();

        //**********************************************************************
        // Method: Inverse
        // Returns the inverse of the matrix
        //**********************************************************************
        bool Inverse(Matrix3x3& tm);

        //**********************************************************************
        // Method: ScalingMatrix
        // Creates a scaling matrix
        //**********************************************************************
        static Matrix3x3 ScalingMatrix(float sx, float sy);

        //**********************************************************************
        // Method: RotationMatrix
        // Creates a rotation matrix
        //**********************************************************************
        static Matrix3x3 RotationMatrix(float ax);

        //**********************************************************************
        // Method: TranslationMatrix
        // Creates a translation matrix
        //**********************************************************************
        static Matrix3x3 TranslationMatrix(float tx, float ty);
    };
}
