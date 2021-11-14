//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <memory.h>

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

    //
    // The following code was adapted from "Decomposing a Matrix Into Simple Transformations", by
    // Spencer W. Thomas, Graphics Gems II
    //

    //
    // Description: Fmat3h::decompose() breaks an affine matrix into a scale, shear, rotation, and translation.
    // Parameters:    scale - scaling components are returned through this variable
    //
    void Matrix4x4::Decompose(Vector3 *scale, Vector3 *shear, Vector3 *rotate, Vector3 *translate)
    {
#if 0
        Fpt3 row[3];
        Fpt3 cp;
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
        row[0].y /= scale->y;
        row[0].z /= scale->z;
        //
        // Compute XY shear factor and make 2nd row orthonormal to 1st
        //
        shear->xy = row[0].x * row[1].x + row[0].y * row[1].y + row[0].z * row[1].z;
        row[1].x = row[1].x - shear->xy * row[0].x;
        row[1].y = row[1].y - shear->xy * row[0].y;
        row[1].z = row[1].z - shear->xy * row[0].z;
        //
        // Compute Y scale and normalize 2nd row
        //
        scale->y = (float)sqrt(row[1].x * row[1].x + row[1].y * row[1].y + row[1].z * row[1].z);
        row[1].x /= scale->y;
        row[1].y /= scale->y;
        row[1].z /= scale->y;
        shear->xy /= scale->y;
        //
        // Compute XZ and YZ shears, orthogonalize 3rd row
        //
        shear->xz = row[0].x * row[2].x + row[0].y * row[2].y + row[0].z * row[2].z;
        row[2].x = row[2].x - shear->xz * row[0].x;
        row[2].y = row[2].y - shear->xz * row[0].y;
        row[2].z = row[2].z - shear->xz * row[0].z;
        shear->yz = row[1].x * row[2].x + row[1].y * row[2].y + row[1].z * row[2].z;
        row[2].x = row[2].x - shear->yz * row[1].x;
        row[2].y = row[2].y - shear->yz * row[1].y;
        row[2].z = row[2].z - shear->yz * row[1].z;
        //
        // Get Z scale and normalize 3rd row
        //
        scale->z = (float)sqrt(row[2].x * row[2].x + row[2].y * row[2].y + row[2].z * row[2].z);
        row[2].x /= scale->z;
        row[2].y /= scale->z;
        row[2].z /= scale->z;
        shear->xz /= scale->z;
        shear->yz /= scale->z;
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
        // Finally determine rotation factors
        //
        rotate->y = asin(-row[0].z);
        if (cos(rotate->y) != 0.0F)
        {
            rotate->x = atan2(row[1].z, row[2].z);
            rotate->z = atan2(row[0].y, row[0].x);
        }
        else
        {
            rotate->x = atan2(row[1].x, row[1].y);
            rotate->z = 0.0F;
        }
#endif
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
