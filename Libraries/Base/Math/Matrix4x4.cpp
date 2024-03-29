//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <memory.h>
#include <vector>

module Base.Math.Matrix.Matrix4x4;
import Base.Core.Core;

namespace Caustic
{
    float Matrix4x4::det3x3(float a0, float b0, float c0,
        float a1, float b1, float c1,
        float a2, float b2, float c2)
    {
        return ((a0) * ((b1) * (c2)-(b2) * (c1)) -
            (b0) * ((a1) * (c2)-(a2) * (c1)) +
            (c0) * ((a1) * (b2)-(a2) * (b1)));
    }

    void Matrix4x4::_adjoint(Matrix4x4 &m)
    {
        m.v[0][0] = det3x3(v[1][1], v[1][2], v[1][3],
            v[2][1], v[2][2], v[2][3],
            v[3][1], v[3][2], v[3][3]);
        m.v[1][0] = -det3x3(v[1][0], v[1][2], v[1][3],
            v[2][0], v[2][2], v[2][3],
            v[3][0], v[3][2], v[3][3]);
        m.v[2][0] = det3x3(v[1][0], v[1][1], v[1][3],
            v[2][0], v[2][1], v[2][3],
            v[3][0], v[3][1], v[3][3]);
        m.v[3][0] = -det3x3(v[1][0], v[1][1], v[1][2],
            v[2][0], v[2][1], v[2][2],
            v[3][0], v[3][1], v[3][2]);
        m.v[0][1] = -det3x3(v[0][1], v[0][2], v[0][3],
            v[2][1], v[2][2], v[2][3],
            v[3][1], v[3][2], v[3][3]);
        m.v[1][1] = det3x3(v[0][0], v[0][2], v[0][3],
            v[2][0], v[2][2], v[2][3],
            v[3][0], v[3][2], v[3][3]);
        m.v[2][1] = -det3x3(v[0][0], v[0][1], v[0][3],
            v[2][0], v[2][1], v[2][3],
            v[3][0], v[3][1], v[3][3]);
        m.v[3][1] = det3x3(v[0][0], v[0][1], v[0][2],
            v[2][0], v[2][1], v[2][2],
            v[3][0], v[3][1], v[3][2]);
        m.v[0][2] = det3x3(v[0][1], v[0][2], v[0][3],
            v[1][1], v[1][2], v[1][3],
            v[3][1], v[3][2], v[3][3]);
        m.v[1][2] = -det3x3(v[0][0], v[0][2], v[0][3],
            v[1][0], v[1][2], v[1][3],
            v[3][0], v[3][2], v[3][3]);
        m.v[2][2] = det3x3(v[0][0], v[0][1], v[0][3],
            v[1][0], v[1][1], v[1][3],
            v[3][0], v[3][1], v[3][3]);
        m.v[3][2] = -det3x3(v[0][0], v[0][1], v[0][2],
            v[1][0], v[1][1], v[1][2],
            v[3][0], v[3][1], v[3][2]);
        m.v[0][3] = -det3x3(v[0][1], v[0][2], v[0][3],
            v[1][1], v[1][2], v[1][3],
            v[2][1], v[2][2], v[2][3]);
        m.v[1][3] = det3x3(v[0][0], v[0][2], v[0][3],
            v[1][0], v[1][2], v[1][3],
            v[2][0], v[2][2], v[2][3]);
        m.v[2][3] = -det3x3(v[0][0], v[0][1], v[0][3],
            v[1][0], v[1][1], v[1][3],
            v[2][0], v[2][1], v[2][3]);
        m.v[3][3] = det3x3(v[0][0], v[0][1], v[0][2],
            v[1][0], v[1][1], v[1][2],
            v[2][0], v[2][1], v[2][2]);
    }

    Matrix4x4::Matrix4x4()
    {
        v[0][0] = v[1][1] = v[2][2] = v[3][3] = 1.0F;
        v[0][1] = v[0][2] = v[0][3] = v[1][0] =
            v[1][2] = v[1][3] = v[2][0] = v[2][1] =
            v[2][3] = v[3][0] = v[3][1] = v[3][2] = 0.0F;
    }

    Matrix4x4::Matrix4x4(float v00, float v01, float v02, float v03,
        float v10, float v11, float v12, float v13,
        float v20, float v21, float v22, float v23,
        float v30, float v31, float v32, float v33)
    {
        v[0][0] = v00;
        v[0][1] = v01;
        v[0][2] = v02;
        v[0][3] = v03;
        v[1][0] = v10;
        v[1][1] = v11;
        v[1][2] = v12;
        v[1][3] = v13;
        v[2][0] = v20;
        v[2][1] = v21;
        v[2][2] = v22;
        v[2][3] = v23;
        v[3][0] = v30;
        v[3][1] = v31;
        v[3][2] = v32;
        v[3][3] = v33;
    }

    Matrix4x4 Matrix4x4::operator *(const Matrix4x4 &m)
    {
        Matrix4x4 r;
        r.v[0][0] = (v[0][0] * m.v[0][0] + v[0][1] * m.v[1][0] + v[0][2] * m.v[2][0] + v[0][3] * m.v[3][0]);
        r.v[0][1] = (v[0][0] * m.v[0][1] + v[0][1] * m.v[1][1] + v[0][2] * m.v[2][1] + v[0][3] * m.v[3][1]);
        r.v[0][2] = (v[0][0] * m.v[0][2] + v[0][1] * m.v[1][2] + v[0][2] * m.v[2][2] + v[0][3] * m.v[3][2]);
        r.v[0][3] = (v[0][0] * m.v[0][3] + v[0][1] * m.v[1][3] + v[0][2] * m.v[2][3] + v[0][3] * m.v[3][3]);
        r.v[1][0] = (v[1][0] * m.v[0][0] + v[1][1] * m.v[1][0] + v[1][2] * m.v[2][0] + v[1][3] * m.v[3][0]);
        r.v[1][1] = (v[1][0] * m.v[0][1] + v[1][1] * m.v[1][1] + v[1][2] * m.v[2][1] + v[1][3] * m.v[3][1]);
        r.v[1][2] = (v[1][0] * m.v[0][2] + v[1][1] * m.v[1][2] + v[1][2] * m.v[2][2] + v[1][3] * m.v[3][2]);
        r.v[1][3] = (v[1][0] * m.v[0][3] + v[1][1] * m.v[1][3] + v[1][2] * m.v[2][3] + v[1][3] * m.v[3][3]);
        r.v[2][0] = (v[2][0] * m.v[0][0] + v[2][1] * m.v[1][0] + v[2][2] * m.v[2][0] + v[2][3] * m.v[3][0]);
        r.v[2][1] = (v[2][0] * m.v[0][1] + v[2][1] * m.v[1][1] + v[2][2] * m.v[2][1] + v[2][3] * m.v[3][1]);
        r.v[2][2] = (v[2][0] * m.v[0][2] + v[2][1] * m.v[1][2] + v[2][2] * m.v[2][2] + v[2][3] * m.v[3][2]);
        r.v[2][3] = (v[2][0] * m.v[0][3] + v[2][1] * m.v[1][3] + v[2][2] * m.v[2][3] + v[2][3] * m.v[3][3]);
        r.v[3][0] = (v[3][0] * m.v[0][0] + v[3][1] * m.v[1][0] + v[3][2] * m.v[2][0] + v[3][3] * m.v[3][0]);
        r.v[3][1] = (v[3][0] * m.v[0][1] + v[3][1] * m.v[1][1] + v[3][2] * m.v[2][1] + v[3][3] * m.v[3][1]);
        r.v[3][2] = (v[3][0] * m.v[0][2] + v[3][1] * m.v[1][2] + v[3][2] * m.v[2][2] + v[3][3] * m.v[3][2]);
        r.v[3][3] = (v[3][0] * m.v[0][3] + v[3][1] * m.v[1][3] + v[3][2] * m.v[2][3] + v[3][3] * m.v[3][3]);
        return r;
    }

    float Matrix4x4::Determinant()
    {
        return ((v[0][0]) * det3x3(v[1][1], v[1][2], v[1][3],
            v[2][1], v[2][2], v[2][3],
            v[3][1], v[3][2], v[3][3]) -
            (v[0][1]) * det3x3(v[1][0], v[1][2], v[1][3],
                v[2][0], v[2][2], v[2][3],
                v[3][0], v[3][2], v[3][3]) +
                (v[0][2]) * det3x3(v[1][0], v[1][1], v[1][3],
                    v[2][0], v[2][1], v[2][3],
                    v[3][0], v[3][1], v[3][3]) -
                    (v[0][3]) * det3x3(v[1][0], v[1][1], v[1][2],
                        v[2][0], v[2][1], v[2][2],
                        v[3][0], v[3][1], v[3][2]));
    }

    Matrix4x4 Matrix4x4::Adjoint()
    {
        Matrix4x4 r;
        _adjoint(r);
        return r;
    }

    // Compute transpose of matrix placing results back into matrix
    void Matrix4x4::Transpose()
    {
        Swap<float>(v[1][0], v[0][1]);
        Swap<float>(v[2][0], v[0][2]);
        Swap<float>(v[2][1], v[1][2]);
        Swap<float>(v[3][0], v[0][3]);
        Swap<float>(v[3][1], v[1][3]);
        Swap<float>(v[3][2], v[2][3]);
    }

    // Compute transpose of matrix placing results into 'tm'
    void Matrix4x4::Transpose(Matrix4x4 &tm)
    {
        tm.v[0][0] = v[0][0];
        tm.v[0][1] = v[1][0];
        tm.v[0][2] = v[2][0];
        tm.v[0][3] = v[3][0];
        tm.v[1][0] = v[0][1];
        tm.v[1][1] = v[1][1];
        tm.v[1][2] = v[2][1];
        tm.v[1][3] = v[3][1];
        tm.v[2][0] = v[0][2];
        tm.v[2][1] = v[1][2];
        tm.v[2][2] = v[2][2];
        tm.v[2][3] = v[3][2];
        tm.v[3][0] = v[0][3];
        tm.v[3][1] = v[1][3];
        tm.v[3][2] = v[2][3];
        tm.v[3][3] = v[3][3];
    }

    // Compute inverse of matrix placing results back into matrix
    bool Matrix4x4::Inverse()
    {
        Matrix4x4 tm;
        if (!Inverse(tm))
            return false;
        memcpy(this, &tm, sizeof(Matrix4x4));
        return true;
    }

    // Compute inverse of matrix placing results in 'tm'
    bool Matrix4x4::Inverse(Matrix4x4 &tm)
    {
        float det = Determinant();
        if (IsZero(det))
            return false;
        _adjoint(tm);
        tm.v[0][0] = tm.v[0][0] / det;
        tm.v[0][1] = tm.v[0][1] / det;
        tm.v[0][2] = tm.v[0][2] / det;
        tm.v[0][3] = tm.v[0][3] / det;
        tm.v[1][0] = tm.v[1][0] / det;
        tm.v[1][1] = tm.v[1][1] / det;
        tm.v[1][2] = tm.v[1][2] / det;
        tm.v[1][3] = tm.v[1][3] / det;
        tm.v[2][0] = tm.v[2][0] / det;
        tm.v[2][1] = tm.v[2][1] / det;
        tm.v[2][2] = tm.v[2][2] / det;
        tm.v[2][3] = tm.v[2][3] / det;
        tm.v[3][0] = tm.v[3][0] / det;
        tm.v[3][1] = tm.v[3][1] / det;
        tm.v[3][2] = tm.v[3][2] / det;
        tm.v[3][3] = tm.v[3][3] / det;
        return true;
    }

    //**********************************************************************
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

    void Matrix4x4::Decompose(std::vector<Matrix4x4> &tm, bool undoshear)
    {
        double x, y;
        Vector3 scale;
        Vector3 shear;
        Vector3 rotate;
        Vector3 translate;
        Decompose(&scale, &shear, &rotate, &translate);
        int indx;

        tm.resize((undoshear) ? 14 : 8);
        for (int i = 0; i < ((undoshear) ? 14 : 8); i++)
            tm[i] = Matrix4x4();
        
        //
        // Finally break the shear components down into
        // rotation => scale => rotation.
        //
        tm[0].v[0][0] = scale.x;
        tm[0].v[1][1] = scale.y;
        tm[0].v[2][2] = scale.z;

#define SHEAR_XY shear.x
#define SHEAR_XZ shear.y
#define SHEAR_YZ shear.z
        //
        // Decompose the shears into rotation=>scale=>rotation
        //
        if (undoshear)
        {
            double cgamma, sgamma, ctheta, stheta;
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
  
    //
    // The following code was adapted from "Decomposing a Matrix Into Simple Transformations", by
    // Spencer W. Thomas, Graphics Gems II
    //

#undef SHEAR_XY
#undef SHEAR_XZ
#undef SHEAR_YZ
#define SHEAR_XY shear->x
#define SHEAR_XZ shear->y
#define SHEAR_YZ shear->z

    //
    // Description: Fmat3h::decompose() breaks an affine matrix into a scale, shear, rotation, and translation.
    // Parameters:    scale - scaling components are returned through this variable
    //
    void Matrix4x4::Decompose(Vector3 *scale, Vector3 *shear, Vector3 *rotate, Vector3 *translate)
    {
        Vector3 row[3];
        Vector3 cp;

        translate->x = v[3][0];
        translate->y = v[3][1];
        translate->z = v[3][2];
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
        scale->x = (float)sqrt(row[0].x * row[0].x + row[0].y * row[0].y + row[0].z * row[0].z);
        row[0].x /= scale->x;
        row[0].y /= scale->x;
        row[0].z /= scale->x;
        //
        // Compute XY shear factor and make 2nd row orthonormal to 1st
        //
        SHEAR_XY = row[0].x * row[1].x + row[0].y * row[1].y + row[0].z * row[1].z;
        row[1].x = row[1].x - SHEAR_XY * row[0].x;
        row[1].y = row[1].y - SHEAR_XY * row[0].y;
        row[1].z = row[1].z - SHEAR_XY * row[0].z;
        //
        // Compute Y scale and normalize 2nd row
        //
        scale->y = (float)sqrt(row[1].x * row[1].x + row[1].y * row[1].y + row[1].z * row[1].z);
        row[1].x /= scale->y;
        row[1].y /= scale->y;
        row[1].z /= scale->y;
        SHEAR_XY /= scale->y;
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
         scale->z = (float)sqrt(row[2].x * row[2].x + row[2].y * row[2].y + row[2].z * row[2].z);
         row[2].x /= scale->z;
         row[2].y /= scale->z;
         row[2].z /= scale->z;
         SHEAR_XZ /= scale->z;
         SHEAR_YZ /= scale->z;
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
             scale->x *= -1.0F;
             scale->y *= -1.0F;
             scale->z *= -1.0F;
         }
         //
         // Determine rotation factors
         //
         rotate->y = (float)asin(-row[0].z);
         if (cos(rotate->y) != 0.0F)
         {
             rotate->x = (float)atan2(row[1].z, row[2].z);
             rotate->z = (float)atan2(row[0].y, row[0].x);
         }
         else
         {
             rotate->x = (float)atan2(row[1].x, row[1].y);
             rotate->z = 0.0F;
         }
        return;
    }

    Matrix4x4 Matrix4x4::ScalingMatrix(float sx, float sy, float sz)
    {
        return Matrix4x4(
            sx, 0.0f, 0.0f, 0.0f,
            0.0f, sy, 0.0f, 0.0f,
            0.0f, 0.0f, sz, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
    }

    Matrix4x4 Matrix4x4::RotationMatrix(float ax, float ay, float az)
    {
        float ca = (float)cos(ax);
        float sa = (float)sin(ax);
        Matrix4x4 xrot(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, ca, -sa, 0.0f,
            0.0f, sa, ca, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        ca = (float)cos(ay);
        sa = (float)sin(ay);
        Matrix4x4 yrot(
            ca, 0.0f, -sa, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            sa, 0.0f, ca, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        ca = (float)cos(az);
        sa = (float)sin(az);
        Matrix4x4 zrot(
            ca, -sa, 0.0f, 0.0f,
            sa, ca, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        return xrot * yrot * zrot;
    }
    Matrix4x4 Matrix4x4::TranslationMatrix(float tx, float ty, float tz)
    {
        return Matrix4x4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            tx, ty, tz, 1.0f
        );
    }
}
