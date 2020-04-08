//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Quaternion.h"
#include <string.h>
#include <vector>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Structure: Matrix2x2
	// Defines a 2x2 matrix
	//**********************************************************************
	struct Matrix2x2
    {
        float v[2][2];
        // TODO: Fill this in
    };

	//**********************************************************************
	// Structure: Matrix4x3
	//    3-dimensional affine matrix
    // --- Text ---
    //    0,0    0,1    0,2        a  b  c 
    //    1,0    1,1    1,2        d  e  f
    //    2,0    2,1    2,2        g  h  i
    //    3,0    3,1    3,2        tx ty tz
	// ------------
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
        Matrix4x3(const Quaternion &q);
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

	//**********************************************************************
	// Structure: Matrix4x4
	// A 4x4 matrix
	//**********************************************************************
	struct Matrix4x4
    {
        float v[4][4];

        float det3x3(float a0, float b0, float c0,
                     float a1, float b1, float c1,
                     float a2, float b2, float c2);
        void _adjoint(Matrix4x4 &m);
    public:
        Matrix4x4();
        Matrix4x4(float v00, float v01, float v02, float v03,
                  float v10, float v11, float v12, float v13,
                  float v20, float v21, float v22, float v23,
                  float v30, float v31, float v32, float v33);
        Matrix4x4(const Quaternion &q);

        float * operator [](int iIndex)
        {
            return v[iIndex];
        }

        friend inline Caustic::Vector4 operator *(const Caustic::Vector4 &p, const Matrix4x4 &tm)
        {
            Caustic::Vector4 r;
            r.x = (p.x * tm.v[0][0] + p.y * tm.v[1][0] + p.z * tm.v[2][0] + p.w * tm.v[3][0]);
            r.y = (p.x * tm.v[0][1] + p.y * tm.v[1][1] + p.z * tm.v[2][1] + p.w * tm.v[3][1]);
            r.z = (p.x * tm.v[0][2] + p.y * tm.v[1][2] + p.z * tm.v[2][2] + p.w * tm.v[3][2]);
            r.w = (p.x * tm.v[0][3] + p.y * tm.v[1][3] + p.z * tm.v[2][3] + p.w * tm.v[3][3]);
            return r;
        }

        friend inline Caustic::Vector4 operator *(const Caustic::Vector3 &p, const Matrix4x4 &tm)
        {
            Caustic::Vector4 r;
            r.x = (p.x * tm.v[0][0] + p.y * tm.v[1][0] + p.z * tm.v[2][0] + tm.v[3][0]);
            r.y = (p.x * tm.v[0][1] + p.y * tm.v[1][1] + p.z * tm.v[2][1] + tm.v[3][1]);
            r.z = (p.x * tm.v[0][2] + p.y * tm.v[1][2] + p.z * tm.v[2][2] + tm.v[3][2]);
            r.w = (p.x * tm.v[0][3] + p.y * tm.v[1][3] + p.z * tm.v[2][3] + tm.v[3][3]);
            return r;
        }

        friend inline Caustic::Vector4 operator *=(Caustic::Vector4 &p, Matrix4x4 &tm)
        {
            p = p * tm;
            return p;
        }
        Matrix4x4 operator *(const Matrix4x4 &m);

		//**********************************************************************
		// Method: Determinant
		// Returns the determinant of the matrix
		//**********************************************************************
		float Determinant();

		//**********************************************************************
		// Method: Adjoint
		// Returns the adjoint of the matrix
		//**********************************************************************
		Matrix4x4 Adjoint();

		//**********************************************************************
		// Method: Transpose
		// Computes the inplace transpose of the matrix
		//**********************************************************************
		void Transpose();

		//**********************************************************************
		// Method: Transpose
		// Returns the transpose of the matrix
		//**********************************************************************
		void Transpose(Matrix4x4 &tm);

		//**********************************************************************
		// Method: Inverse
		// Computes the inplace inverse of the matrix
		//**********************************************************************
		bool Inverse();

		//**********************************************************************
		// Method: Inverse
		// Returns the inverse of the matrix
		//**********************************************************************
		bool Inverse(Matrix4x4 &tm);

		//**********************************************************************
		// Method: Decompose
		// Decomposes the matrix into a scale, shear, rotate and translate components
		//
		// Returns:
		// scale - Returns the scale factors
		// shear - Returns shear factors
		// rotate - Returns rotation as Euler angles
		// translate - Returns translation components
		//**********************************************************************
		void Decompose(Vector3 *scale, Vector3 *shear, Vector3 *rotate, Vector3 *translate);

		void Decompose(std::vector<Matrix4x4> &tm, bool undoshear);
	};
}
