//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
import Base.Core.Core;
#include "Base\Math\OBBox.h"

namespace Caustic
{
    OBBox2::OBBox2()
    {
        // Initially set bbox to be empty
        origin = Vector2(FLT_MAX, FLT_MAX);
        u = Vector2(0.0F, 0.0F);
        v = Vector2(0.0F, 0.0F);
        normu = Vector2(1.0F, 0.0F);
        normv = Vector2(0.0F, 1.0F);
    }

    OBBox2::OBBox2(const OBBox2 &obbox)
    {
        u = obbox.u;
        v = obbox.v;
        normu = obbox.normu;
        normv = obbox.normv;
        origin = obbox.origin;
    }

    bool OBBox2::Empty()
    {
        if (origin.x == FLT_MAX && origin.y == FLT_MAX)
            return true;
        return false;
    }

    OBBox2::OBBox2(Vector2 &p1, Vector2 &p2)
    {
        origin = Vector2((p2.x + p1.x) / 2.0F, (p2.y + p1.y) / 2.0F);
        u = Vector2(p2.x - origin.x, 0.0F);
        v = Vector2(0.0F, p2.y - origin.y);
        normu = Vector2(1.0F, 0.0F);
        normv = Vector2(0.0F, 1.0F);
    }

    OBBox2::OBBox2(BBox2 &bb)
    {
        if (bb.Empty())
        {
            origin = Vector2(FLT_MAX, FLT_MAX);
            u = Vector2(0.0F, 0.0F);
            v = Vector2(0.0F, 0.0F);
        }
        else
        {
            origin = Vector2((bb.maxPt.x + bb.minPt.x) / 2.0F, (bb.maxPt.y + bb.minPt.y) / 2.0F);
            u = Vector2(bb.maxPt.x - origin.x, 0.0F);
            v = Vector2(0.0F, bb.maxPt.y - origin.y);
        }
        normu = Vector2(1.0F, 0.0F);
        normv = Vector2(0.0F, 1.0F);
    }

    OBBox2::operator OBBox2()
    {
        OBBox2 bb;
        if (!Empty())
        {
            bb.AddPoint(origin.x + u.x + v.x, origin.y + u.y + v.y);
            bb.AddPoint(origin.x - u.x + v.x, origin.y - u.y + v.y);
            bb.AddPoint(origin.x - u.x - v.x, origin.y - u.y - v.y);
            bb.AddPoint(origin.x + u.x - v.x, origin.y + u.y - v.y);
        }
        return bb;
    }

    void OBBox2::AddPoint(float x, float y)
    {
        if (Empty())
        {
            origin = Vector2(x, y);
            return;
        }
        //
        // First determine size of bounding box when it is aligned with world axis
        //
        float xlen = (float)sqrt(u.x * u.x + u.y * u.y);
        float ylen = (float)sqrt(v.x * v.x + v.y * v.y);
        //
        // Rotate/scale point into local coordinate space (bbox space)
        //
        float nx = x * normu.x + y * normu.y - origin.x * normu.x - origin.y * normu.y;
        float ny = x * normv.x + y * normv.y - origin.x * normv.x - origin.y * normv.y;
        //
        // Compute new vector lengths
        //
        if (nx > xlen)
        {
            float offset = (nx - xlen) / 2.0F;
            origin.x += (normu.x * offset);
            origin.y += (normu.y * offset);
            xlen += offset;
        }
        else if (nx < -xlen)
        {
            float offset = (nx + xlen) / 2.0F;
            origin.x += (normu.x * offset);
            origin.y += (normu.y * offset);
            xlen -= offset;
        }
        if (ny > ylen)
        {
            float offset = (ny - ylen) / 2.0F;
            origin.x += (normv.x * offset);
            origin.y += (normv.y * offset);
            ylen += offset;
        }
        else if (ny < -ylen)
        {
            float offset = (ny + ylen) / 2.0F;
            origin.x += (normv.x * offset);
            origin.y += (normv.y * offset);
            ylen -= offset;
        }
        //
        // Recompute vector lengths
        //
        u = normu * xlen;
        v = normv * ylen;
    }

    void OBBox2::AddPoint(const Vector2 &p)
    {
        AddPoint(p.x, p.y);
    }

    void OBBox2::AddOBBox(OBBox2 &bb)
    {
        if (bb.Empty())
            return;
        if (Empty())
        {
            *this = bb;
            return;
        }
        AddPoint(bb.origin.x + bb.u.x + bb.v.x, bb.origin.y + bb.u.y + bb.v.y);
        AddPoint(bb.origin.x - bb.u.x + bb.v.x, bb.origin.y - bb.u.y + bb.v.y);
        AddPoint(bb.origin.x - bb.u.x - bb.v.x, bb.origin.y - bb.u.y - bb.v.y);
        AddPoint(bb.origin.x + bb.u.x - bb.v.x, bb.origin.y + bb.u.y - bb.v.y);
    }

    bool OBBox2::PointInside(const Vector2 &p)
    {
        if (Empty())
            return false;
        //
        // First determine size of bounding box when it is aligned with world axis
        //
        float xlen = (float)sqrt(u.x * u.x + u.y * u.y);
        float ylen = (float)sqrt(v.x * v.x + v.y * v.y);
        //
        // Rotate/scale point into local coordinate space (bbox space)
        //
        float nx = p.x * normu.x + p.y * normu.y - origin.x;
        float ny = p.x * normv.x + p.y * normv.y - origin.y;
        //
        // Compute new vector lengths
        //
        if (nx >= -xlen && nx <= xlen && ny >= -ylen && ny <= ylen)
            return true;
        return false;
    }

    void OBBox2::Transform(Matrix2x2 *tm)
    {
        if (Empty())
            return;
        Vector2 xorigin(origin.x * tm->v[0][0] + origin.y * tm->v[1][0] + tm->v[2][0],
            origin.x * tm->v[0][1] + origin.y * tm->v[1][1] + tm->v[2][1]);
        Vector2 p1(origin.x + u.x, origin.y + u.y);
        Vector2 xp1(p1.x * tm->v[0][0] + p1.y * tm->v[1][0] + tm->v[2][0],
            p1.x * tm->v[0][1] + p1.y * tm->v[1][1] + tm->v[2][1]);
        u = Vector2(xp1.x - xorigin.x, xp1.y - xorigin.y);
        Vector2 p2(origin.x + v.x, origin.y + v.y);
        Vector2 xp2(p2.x * tm->v[0][0] + p2.y * tm->v[1][0] + tm->v[2][0],
            p2.x * tm->v[0][1] + p2.y * tm->v[1][1] + tm->v[2][1]);
        v = Vector2(xp2.x - xorigin.x, xp2.y - xorigin.y);
        origin = xorigin;
        normu = u.Normalize();
        if (IsZero(normu.x) && IsZero(normu.y))
            normu = Vector2(1.0F, 0.0F);
        normv = v.Normalize();
        if (IsZero(normv.x) && IsZero(normv.y))
            normv = Vector2(0.0F, 1.0F);
        return;
    }
}
