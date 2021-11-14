//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string.h>
#include <vector>

export module Base.Math.Matrix.Matrix4x4;
import Base.Core.Core;
import Base.Math.Vector;
import Base.Math.Matrix.Matrix3x3;

// Namespace: Caustic
export namespace Caustic
{
	//**********************************************************************
	// Structure: Matrix4x4
	// A 4x4 matrix
    //
    // Members:
    // float v[4][4] - Elements of the matrix
    //
    // Module:
    // {Link:import Base.Math.Matrix.Matrix4x4;{Base/Math/Matrix4x4.ixx}}
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

        static Matrix4x4 FromRodrigues(Vector3& v)
        {
            Matrix3x3 r = Matrix3x3::FromRodrigues(v);
            return Matrix4x4(
                r.v[0][0], r.v[0][1], r.v[0][2], 0.0f,
                r.v[1][0], r.v[1][1], r.v[1][2], 0.0f,
                r.v[2][0], r.v[2][1], r.v[2][2], 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            );
        }

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

        static void DecomposeShear(double h, double& x, double& y, double& cgamma, double& sgamma, double& ctheta, double& stheta)
        {
            double gamma = 0.5F * atan(2.0F / h);
            cgamma = cos(gamma);
            sgamma = sin(gamma);
            y = sgamma / cgamma;
            x = 1.0F / y;
            ctheta = y * cgamma;
            stheta = x * sgamma;
            return;
        }
        
        void Decompose(std::vector<Matrix4x4> &tm, bool undoshear)
        {
            double x, y, cgamma, sgamma, ctheta, stheta;
            Vector3 scale, shear, rotate, translate;
            Vector3 row[3];
            Vector3 cp;
            int indx;

            tm.resize((undoshear) ? 14 : 8);
            for (int i = 0; i < ((undoshear) ? 14 : 8); i++)
                tm[i] = Matrix4x4();
            translate.x = v[3][0];
            translate.y = v[3][1];
            translate.z = v[3][2];
            row[0].x = v[0][0];
            row[0].y = v[0][1];
            row[0].z = v[0][2];
            row[1].x = v[1][0];
            row[1].y = v[1][1];
            row[1].z = v[1][2];
            row[2].x = v[2][0];
            row[2].y = v[2][1];
            row[2].z = v[2][2];
            //
            // Compute X scale factor and normalize first row
            //
            scale.x = (float)sqrt(row[0].x * row[0].x + row[0].y * row[0].y + row[0].z * row[0].z);
            row[0].x /= scale.x;
            row[0].y /= scale.x;
            row[0].z /= scale.x;
            //
            // Compute XY shear factor and make 2nd row orthonormal to 1st
            //
#define SHEAR_XY shear.x
#define SHEAR_XZ shear.y
#define SHEAR_YZ shear.z
            SHEAR_XY = row[0].x * row[1].x + row[0].y * row[1].y + row[0].z * row[1].z;
            row[1].x = row[1].x - SHEAR_XY * row[0].x;
            row[1].y = row[1].y - SHEAR_XY * row[0].y;
            row[1].z = row[1].z - SHEAR_XY * row[0].z;
            //
            // Compute Y scale and normalize 2nd row
            //
            scale.y = (float)sqrt(row[1].x * row[1].x + row[1].y * row[1].y + row[1].z * row[1].z);
            row[1].x /= scale.y;
            row[1].y /= scale.y;
            row[1].z /= scale.y;
            SHEAR_XY /= scale.y;
            //
            // Compute XZ and YZ shears, orthogonalize 3rd row
            //
            SHEAR_XZ = row[0].x * row[2].x + row[0].y * row[2].y + row[0].z * row[2].z;
            row[2].x = row[2].x - SHEAR_XZ * row[0].x;
            row[2].y = row[2].y - SHEAR_XZ * row[0].y;
            row[2].z = row[2].z - SHEAR_XZ * row[0].z;
            SHEAR_YZ = row[1].x * row[2].x + row[1].y * row[2].y + row[1].z * row[2].z;
            row[2].x = row[2].x - SHEAR_YZ * row[1].x;
            row[2].y = row[2].y - SHEAR_YZ * row[1].y;
            row[2].z = row[2].z - SHEAR_YZ * row[1].z;
            //
            // Get Z scale and normalize 3rd row
            //
            scale.z = (float)sqrt(row[2].x * row[2].x + row[2].y * row[2].y + row[2].z * row[2].z);
            row[2].x /= scale.z;
            row[2].y /= scale.z;
            row[2].z /= scale.z;
            SHEAR_XZ /= scale.z;
            SHEAR_YZ /= scale.z;
            //
            // At this point, the matrix (in rows[]) is orthonormal.
            // Check for a coordinate system flip. If the determinant
            // is -1, then negate the matrix and the scaling factors.
            //
            cp.x = row[1].y * row[2].z - row[1].z * row[2].y;
            cp.y = row[1].z * row[2].x - row[1].x * row[2].z;
            cp.z = row[1].x * row[2].y - row[1].y * row[2].x;
            if (row[0].x * cp.x + row[0].y * cp.y + row[0].z * cp.z < 0.0F)
            {
                row[0].x *= -1.0F;
                row[0].y *= -1.0F;
                row[0].z *= -1.0F;
                row[1].x *= -1.0F;
                row[1].y *= -1.0F;
                row[1].z *= -1.0F;
                row[2].x *= -1.0F;
                row[2].y *= -1.0F;
                row[2].z *= -1.0F;
                scale.x *= -1.0F;
                scale.y *= -1.0F;
                scale.z *= -1.0F;
            }
            //
            // Determine rotation factors
            //
            rotate.y = (float)asin(-row[0].z);
            if (cos(rotate.y) != 0.0F)
            {
                rotate.x = (float)atan2(row[1].z, row[2].z);
                rotate.z = (float)atan2(row[0].y, row[0].x);
            }
            else
            {
                rotate.x = (float)atan2(row[1].x, row[1].y);
                rotate.z = 0.0F;
            }
            //
            // Finally break the shear components down into
            // rotation => scale => rotation.
            //
            tm[0].v[0][0] = scale.x;
            tm[0].v[1][1] = scale.y;
            tm[0].v[2][2] = scale.z;
            //
            // Decompose the shears into rotation=>scale=>rotation
            //
            if (undoshear)
            {
                DecomposeShear(SHEAR_XY, x, y, cgamma, sgamma, ctheta, stheta);
                tm[1].v[0][0] = (float)ctheta;         // Rotate
                tm[1].v[0][1] = (float)-stheta;
                tm[1].v[1][0] = (float)stheta;
                tm[1].v[1][1] = (float)ctheta;
                tm[2].v[0][0] = (float)x;              // Scale
                tm[2].v[1][1] = (float)y;
                tm[3].v[0][0] = (float)cgamma;         // Rotate
                tm[3].v[0][1] = (float)sgamma;
                tm[3].v[1][0] = (float)-sgamma;
                tm[3].v[1][1] = (float)cgamma;
                DecomposeShear(SHEAR_XZ, x, y, cgamma, sgamma, ctheta, stheta);
                tm[4].v[0][0] = (float)ctheta;         // Rotate
                tm[4].v[0][2] = (float)-stheta;
                tm[4].v[2][0] = (float)stheta;
                tm[4].v[2][2] = (float)ctheta;
                tm[5].v[0][0] = (float)x;              // Scale
                tm[5].v[2][2] = (float)y;
                tm[6].v[0][0] = (float)cgamma;         // Rotate
                tm[6].v[0][2] = (float)sgamma;
                tm[6].v[2][0] = (float)-sgamma;
                tm[6].v[2][2] = (float)cgamma;
                DecomposeShear(SHEAR_YZ, x, y, cgamma, sgamma, ctheta, stheta);
                tm[7].v[1][1] = (float)ctheta;         // Rotate
                tm[7].v[1][2] = (float)-stheta;
                tm[7].v[2][1] = (float)stheta;
                tm[7].v[2][2] = (float)ctheta;
                tm[8].v[1][1] = (float)x;              // Scale
                tm[8].v[2][2] = (float)y;
                tm[9].v[1][1] = (float)cgamma;         // Rotate
                tm[9].v[1][2] = (float)sgamma;
                tm[9].v[2][1] = (float)-sgamma;
                tm[9].v[2][2] = (float)cgamma;
                indx = 10;
            }
            else
            {
                tm[1].v[1][0] = SHEAR_XY;
                tm[2].v[2][0] = SHEAR_XZ;
                tm[3].v[2][1] = SHEAR_YZ;
                indx = 4;
            }
            //
            // Rotate about X
            //
            tm[indx].v[1][1] = (float)cos(rotate.x);
            tm[indx].v[1][2] = (float)sin(rotate.x);
            tm[indx].v[2][1] = (float)-sin(rotate.x);
            tm[indx].v[2][2] = (float)cos(rotate.x);
            indx++;
            //
            // Rotate about Y
            //
            tm[indx].v[0][0] = (float)cos(rotate.y);
            tm[indx].v[0][2] = (float)-sin(rotate.y);
            tm[indx].v[2][0] = (float)sin(rotate.y);
            tm[indx].v[2][2] = (float)cos(rotate.y);
            indx++;
            //
            // Rotate about Z
            //
            tm[indx].v[0][0] = (float)cos(rotate.z);
            tm[indx].v[0][1] = (float)sin(rotate.z);
            tm[indx].v[1][0] = (float)-sin(rotate.z);
            tm[indx].v[1][1] = (float)cos(rotate.z);
            indx++;
            //
            // Translate
            //
            tm[indx].v[3][0] = translate.x;
            tm[indx].v[3][1] = translate.y;
            tm[indx].v[3][2] = translate.z;
            return;
        }

        //**********************************************************************
        // Method: ScalingMatrix
        // Creates a scaling matrix
        //**********************************************************************
        static Matrix4x4 ScalingMatrix(float sx, float sy, float sz);

        //**********************************************************************
        // Method: RotationMatrix
        // Creates a rotation matrix
        //
        // Parameters:
        // ax - rotation about X axis specified in radians
        // ay - rotation about Y axis specified in radians
        // az - rotation about Z axis specified in radians
        //**********************************************************************
        static Matrix4x4 RotationMatrix(float ax, float ay, float az);

        //**********************************************************************
        // Method: TranslationMatrix
        // Creates a translation matrix
        //**********************************************************************
        static Matrix4x4 TranslationMatrix(float tx, float ty, float tz);
    };
}
