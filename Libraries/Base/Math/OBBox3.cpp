//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"
#include "Base\Math\OBBox.h"

namespace Caustic
{
    OBBox3::OBBox3()
    {
        // Initially set bbox to be empty
        origin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
        u = Vector3(0.0F, 0.0F, 0.0F);
        v = Vector3(0.0F, 0.0F, 0.0F);
        n = Vector3(0.0F, 0.0F, 0.0F);
        normu = Vector3(1.0F, 0.0F, 0.0F);
        normv = Vector3(0.0F, 1.0F, 0.0F);
        normn = Vector3(0.0F, 0.0F, 1.0F);
    }

    OBBox3::OBBox3(const OBBox3 &obbox)
    {
        u = obbox.u;
        v = obbox.v;
        n = obbox.n;
        normu = obbox.normu;
        normv = obbox.normv;
        normn = obbox.normn;
        origin = obbox.origin;
    }

    bool OBBox3::Empty()
    {
        if (origin.x == FLT_MAX && origin.y == FLT_MAX && origin.z == FLT_MAX)
            return true;
        return false;
    }

    OBBox3::OBBox3(Vector3 &p1, Vector3 &p2)
    {
        origin = Vector3((p2.x + p1.x) / 2.0F, (p2.y + p1.y) / 2.0F, (p2.z + p1.z) / 2.0F);
        u = Vector3(p2.x - origin.x, 0.0F, 0.0F);
        v = Vector3(0.0F, p2.y - origin.y, 0.0F);
        n = Vector3(0.0F, 0.0F, p2.z - origin.z);
        normu = Vector3(1.0F, 0.0F, 0.0F);
        normv = Vector3(0.0F, 1.0F, 0.0F);
        normn = Vector3(0.0F, 0.0F, 1.0F);
    }

    OBBox3::OBBox3(BBox3 &bb)
    {
        if (bb.Empty())
        {
            origin = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
            u = Vector3(0.0F, 0.0F, 0.0F);
            v = Vector3(0.0F, 0.0F, 0.0F);
            n = Vector3(0.0F, 0.0F, 0.0F);
        }
        else
        {
            origin = Vector3((bb.maxPt.x + bb.minPt.x) / 2.0F, (bb.maxPt.y + bb.minPt.y) / 2.0F, (bb.maxPt.z + bb.minPt.z) / 2.0F);
            u = Vector3(bb.maxPt.x - origin.x, 0.0F, 0.0F);
            v = Vector3(0.0F, bb.maxPt.y - origin.y, 0.0F);
            n = Vector3(0.0F, 0.0F, bb.maxPt.z - origin.z);
        }
        normu = Vector3(1.0F, 0.0F, 0.0F);
        normv = Vector3(0.0F, 1.0F, 0.0F);
        normn = Vector3(0.0F, 0.0F, 1.0F);
    }

    OBBox3::operator OBBox3()
    {
        OBBox3 bb;
        if (!Empty())
        {
            bb.AddPoint(origin.x + u.x + v.x + n.x, origin.y + u.y + v.y + n.y, origin.z + u.z + v.z + n.z);
            bb.AddPoint(origin.x - u.x + v.x + n.x, origin.y - u.y + v.y + n.y, origin.z - u.z + v.z + n.z);
            bb.AddPoint(origin.x - u.x - v.x + n.x, origin.y - u.y - v.y + n.y, origin.z - u.z - v.z + n.z);
            bb.AddPoint(origin.x + u.x - v.x + n.x, origin.y + u.y - v.y + n.y, origin.z + u.z - v.z + n.z);
            bb.AddPoint(origin.x + u.x + v.x - n.x, origin.y + u.y + v.y - n.y, origin.z + u.z + v.z - n.z);
            bb.AddPoint(origin.x - u.x + v.x - n.x, origin.y - u.y + v.y - n.y, origin.z - u.z + v.z - n.z);
            bb.AddPoint(origin.x - u.x - v.x - n.x, origin.y - u.y - v.y - n.y, origin.z - u.z - v.z - n.z);
            bb.AddPoint(origin.x + u.x - v.x - n.x, origin.y + u.y - v.y - n.y, origin.z + u.z - v.z - n.z);
        }
        return bb;
    }

    void OBBox3::AddPoint(float x, float y, float z)
    {
        if (Empty())
        {
            origin = Vector3(x, y, z);
            return;
        }
        //
        // First determine size of bounding box when it is aligned with world axis
        //
        float xlen = (float)sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
        float ylen = (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        float zlen = (float)sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        //
        // Rotate/scale point into local coordinate space (bbox space)
        //
#if 0
        float nx = x * normu.x + y * normu.y + z * normu.z - origin.x;
        float ny = x * normv.x + y * normv.y + z * normv.z - origin.y;
        float nz = x * normn.x + y * normn.y + z * normn.z - origin.z;
#else
        float nx = x * normu.x + y * normu.y + z * normu.z - origin.x * normu.x - origin.y * normu.y - origin.z * normu.z;
        float ny = x * normv.x + y * normv.y + z * normv.z - origin.x * normv.x - origin.y * normv.y - origin.z * normv.z;
        float nz = x * normn.x + y * normn.y + z * normn.z - origin.x * normn.x - origin.y * normn.y - origin.z * normn.z;
#endif
        //
        // Compute new vector lengths
        //
        if (nx > xlen)
        {
            float offset = (nx - xlen) / 2.0F;
            origin.x += (normu.x * offset);
            origin.y += (normu.y * offset);
            origin.z += (normu.z * offset);
            xlen += offset;
        }
        else if (nx < -xlen)
        {
            float offset = (nx + xlen) / 2.0F;
            origin.x += (normu.x * offset);
            origin.y += (normu.y * offset);
            origin.z += (normu.z * offset);
            xlen -= offset;
        }
        if (ny > ylen)
        {
            float offset = (ny - ylen) / 2.0F;
            origin.x += (normv.x * offset);
            origin.y += (normv.y * offset);
            origin.z += (normv.z * offset);
            ylen += offset;
        }
        else if (ny < -ylen)
        {
            float offset = (ny + ylen) / 2.0F;
            origin.x += (normv.x * offset);
            origin.y += (normv.y * offset);
            origin.z += (normv.z * offset);
            ylen -= offset;
        }
        if (nz > zlen)
        {
            float offset = (nz - zlen) / 2.0F;
            origin.x += (normn.x * offset);
            origin.y += (normn.y * offset);
            origin.z += (normn.z * offset);
            zlen += offset;
        }
        else if (nz < -zlen)
        {
            float offset = (nz + zlen) / 2.0F;
            origin.x += (normn.x * offset);
            origin.y += (normn.y * offset);
            origin.z += (normn.z * offset);
            zlen -= offset;
        }
        //
        // Recompute vector lengths
        //
        u = normu * xlen;
        v = normv * ylen;
        n = normn * zlen;
        return;
    }

    void OBBox3::AddPoint(const Vector3 &p)
    {
        AddPoint(p.x, p.y, p.z);
        return;
    }

    void OBBox3::AddOBBox(OBBox3 &bb)
    {
        if (bb.Empty())
            return;
        if (Empty())
        {
            *this = bb;
            return;
        }
        AddPoint(bb.origin.x + bb.u.x + bb.v.x + bb.n.x,
                 bb.origin.y + bb.u.y + bb.v.y + bb.n.y,
                 bb.origin.z + bb.u.z + bb.v.z + bb.n.z);
        AddPoint(bb.origin.x - bb.u.x + bb.v.x + bb.n.x,
                 bb.origin.y - bb.u.y + bb.v.y + bb.n.y,
                 bb.origin.z - bb.u.z + bb.v.z + bb.n.z);
        AddPoint(bb.origin.x - bb.u.x - bb.v.x + bb.n.x,
                 bb.origin.y - bb.u.y - bb.v.y + bb.n.y,
                 bb.origin.z - bb.u.z - bb.v.z + bb.n.z);
        AddPoint(bb.origin.x + bb.u.x - bb.v.x + bb.n.x,
                 bb.origin.y + bb.u.y - bb.v.y + bb.n.y,
                 bb.origin.z + bb.u.z - bb.v.z + bb.n.z);
        AddPoint(bb.origin.x + bb.u.x + bb.v.x - bb.n.x,
                 bb.origin.y + bb.u.y + bb.v.y - bb.n.y,
                 bb.origin.z + bb.u.z + bb.v.z - bb.n.z);
        AddPoint(bb.origin.x - bb.u.x + bb.v.x - bb.n.x,
                 bb.origin.y - bb.u.y + bb.v.y - bb.n.y,
                 bb.origin.z - bb.u.z + bb.v.z - bb.n.z);
        AddPoint(bb.origin.x - bb.u.x - bb.v.x - bb.n.x,
                 bb.origin.y - bb.u.y - bb.v.y - bb.n.y,
                 bb.origin.z - bb.u.z - bb.v.z - bb.n.z);
        AddPoint(bb.origin.x + bb.u.x - bb.v.x - bb.n.x,
                 bb.origin.y + bb.u.y - bb.v.y - bb.n.y,
                 bb.origin.z + bb.u.z - bb.v.z - bb.n.z);
        return;
    }

    bool OBBox3::PointInside(const Vector3 &p)
    {
        if (Empty())
            return false;
        //
        // First determine size of bounding box when it is aligned with world axis
        //
        float xlen = (float)sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
        float ylen = (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        float zlen = (float)sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
        //
        // Rotate/scale point into local coordinate space (bbox space)
        //
        float nx = p.x * normu.x + p.y * normu.y + p.z * normu.z - origin.x;
        float ny = p.x * normv.x + p.y * normv.y + p.z * normv.z - origin.y;
        float nz = p.x * normn.x + p.y * normn.y + p.z * normn.z - origin.z;
        //
        // Compute new vector lengths
        //
        if (nx >= -xlen && nx <= xlen && ny >= -ylen && ny <= ylen && nz >= -zlen && nz <= zlen)
            return true;
        return false;
    }

    void OBBox3::Transform(Matrix4x4 *tm)
    {
        if (Empty())
            return;
        Vector3 xorigin(origin.x * tm->v[0][0] + origin.y * tm->v[1][0] + origin.z * tm->v[2][0] + tm->v[3][0],
            origin.x * tm->v[0][1] + origin.y * tm->v[1][1] + origin.z * tm->v[2][1] + tm->v[3][1],
            origin.x * tm->v[0][2] + origin.y * tm->v[1][2] + origin.z * tm->v[2][2] + tm->v[3][2]);
        Vector3 p1(origin.x + u.x, origin.y + u.y, origin.z + u.z);
        Vector3 xp1(p1.x * tm->v[0][0] + p1.y * tm->v[1][0] + p1.z * tm->v[2][0] + tm->v[3][0],
            p1.x * tm->v[0][1] + p1.y * tm->v[1][1] + p1.z * tm->v[2][1] + tm->v[3][1],
            p1.x * tm->v[0][2] + p1.y * tm->v[1][2] + p1.z * tm->v[2][2] + tm->v[3][2]);
        u = Vector3(xp1.x - xorigin.x, xp1.y - xorigin.y, xp1.z - xorigin.z);
        Vector3 p2(origin.x + v.x, origin.y + v.y, origin.z + v.z);
        Vector3 xp2(p2.x * tm->v[0][0] + p2.y * tm->v[1][0] + p2.z * tm->v[2][0] + tm->v[3][0],
            p2.x * tm->v[0][1] + p2.y * tm->v[1][1] + p2.z * tm->v[2][1] + tm->v[3][1],
            p2.x * tm->v[0][2] + p2.y * tm->v[1][2] + p2.z * tm->v[2][2] + tm->v[3][2]);
        v = Vector3(xp2.x - xorigin.x, xp2.y - xorigin.y, xp2.z - xorigin.z);
        Vector3 p3(origin.x + n.x, origin.y + n.y, origin.z + n.z);
        Vector3 xp3(p3.x * tm->v[0][0] + p3.y * tm->v[1][0] + p3.z * tm->v[2][0] + tm->v[3][0],
            p3.x * tm->v[0][1] + p3.y * tm->v[1][1] + p3.z * tm->v[2][1] + tm->v[3][1],
            p3.x * tm->v[0][2] + p3.y * tm->v[1][2] + p3.z * tm->v[2][2] + tm->v[3][2]);
        n = Vector3(xp3.x - xorigin.x, xp3.y - xorigin.y, xp3.z - xorigin.z);
        origin = xorigin;
        normu = u.Normalize();
        if (IsZero(normu.x) && IsZero(normu.y) && IsZero(normu.z))
            normu = Vector3(1.0F, 0.0F, 0.0F);
        normv = v.Normalize();
        if (IsZero(normv.x) && IsZero(normv.y) && IsZero(normv.z))
            normv = Vector3(0.0F, 1.0F, 0.0F);
        normn = n.Normalize();
        if (IsZero(normn.x) && IsZero(normn.y) && IsZero(normn.z))
            normn = Vector3(0.0F, 0.0F, 1.0F);
        return;
    }

    void OBBox3::Transform(Matrix4x3 *tm)
    {
        if (Empty())
            return;
        Vector3 xorigin(origin.x * tm->v[0][0] + origin.y * tm->v[1][0] + origin.z * tm->v[2][0] + tm->v[3][0],
            origin.x * tm->v[0][1] + origin.y * tm->v[1][1] + origin.z * tm->v[2][1] + tm->v[3][1],
            origin.x * tm->v[0][2] + origin.y * tm->v[1][2] + origin.z * tm->v[2][2] + tm->v[3][2]);
        Vector3 p1(origin.x + u.x, origin.y + u.y, origin.z + u.z);
        Vector3 xp1(p1.x * tm->v[0][0] + p1.y * tm->v[1][0] + p1.z * tm->v[2][0] + tm->v[3][0],
            p1.x * tm->v[0][1] + p1.y * tm->v[1][1] + p1.z * tm->v[2][1] + tm->v[3][1],
            p1.x * tm->v[0][2] + p1.y * tm->v[1][2] + p1.z * tm->v[2][2] + tm->v[3][2]);
        u = Vector3(xp1.x - xorigin.x, xp1.y - xorigin.y, xp1.z - xorigin.z);
        Vector3 p2(origin.x + v.x, origin.y + v.y, origin.z + v.z);
        Vector3 xp2(p2.x * tm->v[0][0] + p2.y * tm->v[1][0] + p2.z * tm->v[2][0] + tm->v[3][0],
            p2.x * tm->v[0][1] + p2.y * tm->v[1][1] + p2.z * tm->v[2][1] + tm->v[3][1],
            p2.x * tm->v[0][2] + p2.y * tm->v[1][2] + p2.z * tm->v[2][2] + tm->v[3][2]);
        v = Vector3(xp2.x - xorigin.x, xp2.y - xorigin.y, xp2.z - xorigin.z);
        Vector3 p3(origin.x + n.x, origin.y + n.y, origin.z + n.z);
        Vector3 xp3(p3.x * tm->v[0][0] + p3.y * tm->v[1][0] + p3.z * tm->v[2][0] + tm->v[3][0],
            p3.x * tm->v[0][1] + p3.y * tm->v[1][1] + p3.z * tm->v[2][1] + tm->v[3][1],
            p3.x * tm->v[0][2] + p3.y * tm->v[1][2] + p3.z * tm->v[2][2] + tm->v[3][2]);
        n = Vector3(xp3.x - xorigin.x, xp3.y - xorigin.y, xp3.z - xorigin.z);
        origin = xorigin;
        normu = u.Normalize();
        if (IsZero(normu.x) && IsZero(normu.y) && IsZero(normu.z))
            normu = Vector3(1.0F, 0.0F, 0.0F);
        normv = v.Normalize();
        if (IsZero(normv.x) && IsZero(normv.y) && IsZero(normv.z))
            normv = Vector3(0.0F, 1.0F, 0.0F);
        normn = n.Normalize();
        if (IsZero(normn.x) && IsZero(normn.y) && IsZero(normn.z))
            normn = Vector3(0.0F, 0.0F, 1.0F);
        return;
    }
}
