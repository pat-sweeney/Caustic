//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
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

        //**********************************************************************
        // Method: Decompose
        // Decomposes the matrix into a scale, shear, rotate and translate matrices.
        // If undoshear is true then the shear is decomposed into a Rotate X Scale X Rotate
        //
        // Returns:
        // tm - returns a list of matrices.
        // if undoshear == true then
        //       tm[0] - scale matrix
        //       tm[1] - ShearXY rotate matrix
        //       tm[2] - ShearXY scale matrix
        //       tm[3] - ShearXY rotate matrix
        //       tm[4] - ShearXZ rotate matrix
        //       tm[5] - ShearXZ scale matrix
        //       tm[6] - ShearXZ rotate matrix
        //       tm[7] - ShearYZ rotate matrix
        //       tm[8] - ShearYZ scale matrix
        //       tm[9] - ShearYZ rotate matrix
        //       tm[10] - RotateX
        //       tm[11] - RotateY
        //       tm[12] - RotateZ
        // else
        //       tm[0] - scale matrix
        //       tm[1] - ShearXY
        //       tm[2] - ShearXZ
        //       tm[3] - ShearYZ
        //       tm[4] - RotateX
        //       tm[5] - RotateY
        //       tm[6] - RotateZ
        //**********************************************************************
        void Decompose(std::vector<Matrix4x4> &tm, bool undoshear);

        //**********************************************************************
        // Method: Zero
        // Zeroes out a matrix
        //**********************************************************************
        void Zero()
        {
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    v[i][j] = 0.0f;
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

#if false
        void JacobiSVD()
        {
        }

        //**********************************************************************
        // Method: FromCorrespondences
        // Creates an affine matrix that transforms one set of points into anther
        // such that the distance between the points is minimized.
        // Based on "Least-Squares Fitting of Two 3-D Point Sets" by Arun, et al
        // https://ieeexplore.ieee.org/document/4767965
        // 
        // Parameters:
        // srcPoints - Points to transform
        // targetPoints - target points
        //**********************************************************************
        void FromCorrespondences(std::vector<Vector3> srcPoints, std::vector<Vector3> targetPoints)
        {
            //**********************************************************************
            // Compute the centroids of the two point clouds
            //**********************************************************************
            float numPoints = (float)srcPoints.size();
            Vector3 centroidA(0.0f, 0.0f, 0.0f);
            Vector3 centroidB(0.0f, 0.0f, 0.0f);
            for (int index = 0; index < numPoints; index++)
            {
                centroidA += Vector3(srcPoints[index].x, srcPoints[index].y, srcPoints[index].z);
                centroidB += Vector3(targetPoints[index].x, targetPoints[index].y, targetPoints[index].z);
            }
            centroidA /= (float)numPoints;
            centroidB /= (float)numPoints;

            //**********************************************************************
            // Compute SVD
            //**********************************************************************
            Matrix3x3 mat;
            mat.Zero();
            for (int i = 0; i < numPoints; i++)
            {
                Vector3 A = Vector3(srcPoints[i].x, srcPoints[i].y, srcPoints[i].z) - centroidA;
                Vector3 B = Vector3(targetPoints[i].x, targetPoints[i].y, targetPoints[i].z) - centroidB;
                Matrix3x3 m = Matrix3x3::FromDotVectors(A, B);
                mat += m;
            }
            Eigen::JacobiSVD<Eigen::Matrix3f> svd(mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
            Matrix3x3 result = svd.matrixV() * svd.matrixU().transpose();

            //**********************************************************************
            // Check if SVD returned a reflection matrix
            //**********************************************************************
            if (result.Determinant() < 0)
            {
                result[2][0] *= -1;
                result[2][1] *= -1;
                result[2][2] *= -1;
            }

            //**********************************************************************
            // Compute the final translation
            //**********************************************************************
            Vector3 translation = centroidB - centroidA * result;

            Matrix4x4 xform = *pXform;
            xform[0][0] = result[0][0];     xform[0][1] = result[1][0];     xform[0][2] = result[2][0];   xform[0][3] = 0.0f;
            xform[1][0] = result[0][1];     xform[1][1] = result[1][1];     xform[1][2] = result[2][1];   xform[1][3] = 0.0f;
            xform[2][0] = result[0][2];     xform[2][1] = result[1][2];     xform[2][2] = result[2][2];   xform[2][3] = 0.0f;
            xform[3][0] = translation.x;    xform[3][1] = translation.y;    xform[3][2] = translation.z;  xform[3][3] = 1.0f;
        }
#endif
    };
}
