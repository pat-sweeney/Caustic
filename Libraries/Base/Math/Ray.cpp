//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Math.Ray;
import Base.Math.Matrix;
#include <math.h>

namespace Caustic
{
#define HUGE 1e20F
#define TINY 1e-5F
    
    bool Ray2::IntersectWithLine(Vector2 &p0, Vector2 &p1, RayIntersect2 *pIntersectInfo)
    {
        // First check to collinearity
        Vector2 p1p0 = p1 - p0;
        if (p1p0.Cross(dir) == 0)
            return false; // Lines are either parallel -> (pos - p0).Cross(dir) != 0) or collinear -> (pos - p0).Cross(dir) == 0)
        Vector2 p0pos = p0 - pos;
        float c = dir.Cross(p1p0);
        float t = p0pos.Cross(p1p0) / c;
        float u = p0pos.Cross(dir) / c;
        if (u < 0 || u > 1 || t < 0)
            return false;
        pIntersectInfo->hitPt = p0 + p1p0 * u;
        pIntersectInfo->hitTime = u;
        pIntersectInfo->hitTimeRay = t;
        pIntersectInfo->hitPtRay = pos + dir * t;
        return true;
    }

    bool Ray3::Intersect(const BBox3 &bbox, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo)
    {
        Ray3 genray;
        Vector3 norms[6];
        float thit;

        norms[0].x = 1.0F; norms[0].y = 0.0F; norms[0].z = 0.0F;
        norms[1].x = -1.0F; norms[1].y = 0.0F; norms[1].z = 0.0F;
        norms[2].x = 0.0F; norms[2].y = 1.0F; norms[2].z = 0.0F;
        norms[3].x = 0.0F; norms[3].y = -1.0F; norms[3].z = 0.0F;
        norms[4].x = 0.0F; norms[4].y = 0.0F; norms[4].z = 1.0F;
        norms[5].x = 0.0F; norms[5].y = 0.0F; norms[5].z = -1.0F;
        if (pInvTm)
        {
            Vector4 xformpos = pos * *pInvTm;
            Vector4 xformdir = Vector4(dir.x, dir.y, dir.z, 0.0f) * *pInvTm;
            genray.pos = Vector3(xformpos.x, xformpos.y, xformpos.z);
            genray.dir = Vector3(xformdir.x, xformdir.y, xformdir.z);
        }
        else
        {
            genray.pos = pos;
            genray.dir = dir;
        }
        float tin = -HUGE;
        float tout = HUGE;
        int toutnorm = 0;
        int tinnorm = 0;
        if (genray.dir.x == 0)
        {
            if (1.0F - genray.pos.x < TINY) /* ray outside */
                return false;
        }
        else
        {
            thit = (1.0F - genray.pos.x) / genray.dir.x;
            if (genray.dir.x > TINY)
                tout = thit;
            else
                tin = thit;
        }
        if (-genray.dir.x == 0)
        {
            if ((1.0F + genray.pos.x) < TINY) /* ray outside */
                return false;
        }
        else
        {
            thit = (1.0F + genray.pos.x) / -genray.dir.x;
            if (-genray.dir.x > TINY)
            {
                if (thit < tout)
                {
                    tout = thit;
                    toutnorm = 1;
                }
            }
            else
            {
                if (thit > tin)
                {
                    tin = thit;
                    tinnorm = 1;
                }
            }
        }
        if (genray.dir.y == 0)
        {
            if (1.0F - genray.pos.y < TINY) /* ray outside */
                return false;
        }
        else
        {
            thit = (1.0F - genray.pos.y) / genray.dir.y;
            if (genray.dir.y > TINY)
            {
                if (thit < tout)
                {
                    tout = thit;
                    toutnorm = 2;
                }
            }
            else
            {
                if (thit > tin)
                {
                    tin = thit;
                    tinnorm = 2;
                }
            }
        }
        if (-genray.dir.y == 0)
        {
            if ((1.0F + genray.pos.y) < TINY) /* ray outside */
                return false;
        }
        else
        {
            thit = (1 + genray.pos.y) / -genray.dir.y;
            if (-genray.dir.y > TINY)
            {
                if (thit < tout)
                {
                    tout = thit;
                    toutnorm = 3;
                }
            }
            else
            {
                if (thit > tin)
                {
                    tin = thit;
                    tinnorm = 3;
                }
            }
        }
        if (genray.dir.z == 0)
        {
            if (1.0F - genray.pos.z < TINY) /* ray outside */
                return false;
        }
        else
        {
            thit = (1 - genray.pos.z) / genray.dir.z;
            if (genray.dir.z > TINY)
            {
                if (thit < tout)
                {
                    tout = thit;
                    toutnorm = 4;
                }
            }
            else
            {
                if (thit > tin)
                {
                    tin = thit;
                    tinnorm = 4;
                }
            }
        }
        if (-genray.dir.z == 0)
        {
            if ((1 + genray.pos.z) < TINY) /* ray outside */
                return false;
        }
        else
        {
            float thit = (1 + genray.pos.z) / -genray.dir.z;
            if (-genray.dir.z > TINY)
            {
                if (thit < tout)
                {
                    tout = thit;
                    toutnorm = 5;
                }
            }
            else
            {
                if (thit > tin)
                {
                    tin = thit;
                    tinnorm = 5;
                }
            }
        }
        if (tin < tout)
        {
            if (tin > TINY)
            {
                pIntersectInfo->hitTime = tin;
                pIntersectInfo->hitPt.x = pos.x + dir.x * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.y = pos.y + dir.y * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.z = pos.z + dir.z * pIntersectInfo->hitTime;
                /* compute normal at hit point */
                pIntersectInfo->hitNorm = norms[tinnorm];
                return true;
            }
            if (tout > TINY)
            {
                pIntersectInfo->hitTime = tout;
                pIntersectInfo->hitPt.x = pos.x + dir.x * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.y = pos.y + dir.y * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.z = pos.z + dir.z * pIntersectInfo->hitTime;
                /* compute normal at hit point */
                pIntersectInfo->hitNorm = norms[toutnorm];
                return true;
            }
        }
        return false;
    }


    bool Ray3::Intersect(float bottomRadius, float topRadius, float height, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo)
    {
        Ray3 genray;
        if (pInvTm)
        {
            Vector4 xformpos = pos * *pInvTm;
            Vector4 xformdir = Vector4(dir.x, dir.y, dir.z, 0.0f) * *pInvTm;
            genray.pos = Vector3(xformpos.x, xformpos.y, xformpos.z);
            genray.dir = Vector3(xformdir.x, xformdir.y, xformdir.z);
        }
        else
        {
            genray.pos = pos;
            genray.dir = dir;
        }
        float afac = -1.0F;
        float bfac = 1.0F;
        float afac2 = afac * afac;
        float a = (4.0F * genray.dir.x * genray.dir.x + 4.0F * genray.dir.y * genray.dir.y -
            afac2 * genray.dir.z * genray.dir.z);
        float b = (4.0F * genray.dir.x * genray.pos.x + 4.0F * genray.dir.y * genray.pos.y -
            afac2 * genray.dir.z * genray.pos.z - afac * bfac * genray.dir.z);
        float c = (4.0F * genray.pos.x * genray.pos.x + 4.0F * genray.pos.y * genray.pos.y -
            afac2 * genray.pos.z * genray.pos.z - 2.0F * afac * bfac * genray.pos.z - bfac * bfac);
        if (a < -TINY || a > TINY)
        {
            float bb = b * b;
            float discrim = bb - a * c;
            if (discrim >= 0.0F)
            {
                discrim = (float)sqrt(discrim);
                //
                // this may seem like a strange way to solve for the roots of the
                // quadratic equation; however, if a or c are very small then we will
                // end up subtracting almost equal quantities to find one of the roots
                // (i.e. b - discrim will be almost zero). This will result in a very
                // inaccurate solution for one of the roots. This idea was taken from
                // Numerical Recipes in C (Press,Flannery,Teukolsky,Vetterling), section
                // 5.5
                //
                if (b < -TINY)
                    discrim = -(b - discrim);
                else
                    discrim = -(b + discrim);
                float root1 = discrim / a;
                float root2 = (discrim != 0.0) ? c / discrim : root1;
                pIntersectInfo->hitTime = (root1 < root2) ? root1 : root2;
                float hitz = genray.pos.z + genray.dir.z * pIntersectInfo->hitTime;
#if 0
                if (fabs(hitz) <= 1.0F)
#else
                if (fabs(hitz) <= 1.0F && (genray.pos.x + genray.dir.x * pIntersectInfo->hitTime > 0.0F))
#endif
                {
                    // compute hit point and normal at hit point
                    pIntersectInfo->hitPt.x = pos.x + dir.x * pIntersectInfo->hitTime;
                    pIntersectInfo->hitPt.y = pos.y + dir.y * pIntersectInfo->hitTime;
                    pIntersectInfo->hitPt.z = pos.z + dir.z * pIntersectInfo->hitTime;
                    // compute normal at hit point
                    Vector3 hitpt(genray.pos.x + genray.dir.x * pIntersectInfo->hitTime,
                                  genray.pos.y + genray.dir.y * pIntersectInfo->hitTime,
                                  genray.pos.z + genray.dir.z * pIntersectInfo->hitTime);
                    //
                    // to find the normal use our equation for
                    // the cone: x^2+y^2=(((z+1)*t-(z-1)*b)/2)^2
                    // and solve for the gradient using partial
                    // derivatives.
                    //
                    pIntersectInfo->hitNorm.x = 8.0F * hitpt.x;
                    pIntersectInfo->hitNorm.y = 8.0F * hitpt.y;
                    pIntersectInfo->hitNorm.z = -2.0F * hitpt.z * afac2 - 2.0F * afac * bfac;
                    return true;
                }
            }
        }
        //
        // check top and bottom of cone
        //
        // first we check the bottom of the cone. This is a circle
        // of radius 1.0F lying in the z=-1 plane.
        // thus, the hit time for the ray is at
        //    Az + Bz * T = -1
        //       or
        //      T = (-1 - Az) / Bz
        //
        float hitt = (-1.0F - genray.pos.z) / genray.dir.z;
        if (hitt < pIntersectInfo->hitTime)
        {
            float hitx = genray.pos.x + genray.dir.x * hitt;
            float hity = genray.pos.y + genray.dir.y * hitt;
            if (fabs(hitx) <= 1.0F && fabs(hity) <= 1.0F)
            {
                if (hitx * hitx + hity * hity <= 1.0F)
                {
                    // we hit the bottom of the cone
                    pIntersectInfo->hitTime = hitt;
                    pIntersectInfo->hitPt.x = pos.x + dir.x * hitt;
                    pIntersectInfo->hitPt.y = pos.y + dir.y * hitt;
                    pIntersectInfo->hitPt.z = pos.z + dir.z * hitt;
                    // compute normal at hit point
                    pIntersectInfo->hitNorm.x = 0.0F;
                    pIntersectInfo->hitNorm.y = 0.0F;
                    pIntersectInfo->hitNorm.z = -1.0F;
                    return true;
                }
            }
        }
        return false;
    }

    bool Ray3::Intersect(float radius, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo)
    {
        //
        // first convert ray into generic coordinate system
        //
        Ray3 genray;
        if (pInvTm)
        {
            Vector4 xformpos = pos * *pInvTm;
            Vector4 xformdir = Vector4(dir.x, dir.y, dir.z, 0.0f) * *pInvTm;
            genray.pos = Vector3(xformpos.x, xformpos.y, xformpos.z);
            genray.dir = Vector3(xformdir.x, xformdir.y, xformdir.z);
        }
        else
        {
            genray.pos = pos;
            genray.dir = dir;
        }
        float dirdir = genray.dir.Dot(genray.dir);
        float startdir = genray.pos.Dot(genray.dir);
        float startstart = genray.pos.Dot(genray.pos) - 1.0F;
        float discrim = startdir * startdir - dirdir * startstart;
        if (discrim >= 0.0F)
        {
            float disc_root = (float)sqrt(discrim);
            if (-startdir > disc_root)
            {
                pIntersectInfo->hitTime = (-startdir - disc_root) / dirdir;
                pIntersectInfo->hitPt.x = pos.x + dir.x * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.y = pos.y + dir.y * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.z = pos.z + dir.z * pIntersectInfo->hitTime;
                //
                // compute normal at hit point
                //
                pIntersectInfo->hitNorm.x = genray.pos.x + genray.dir.x * pIntersectInfo->hitTime;
                pIntersectInfo->hitNorm.y = genray.pos.y + genray.dir.y * pIntersectInfo->hitTime;
                pIntersectInfo->hitNorm.z = genray.pos.z + genray.dir.z * pIntersectInfo->hitTime;
                return true;
            }
            if (startdir > -disc_root)
            {
                pIntersectInfo->hitTime = (-startdir + disc_root) / dirdir;
                pIntersectInfo->hitPt.x = pos.x + dir.x * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.y = pos.y + dir.y * pIntersectInfo->hitTime;
                pIntersectInfo->hitPt.z = pos.z + dir.z * pIntersectInfo->hitTime;
                //
                // compute normal at hit point
                //
                pIntersectInfo->hitNorm.x = genray.pos.x + genray.dir.x * pIntersectInfo->hitTime;
                pIntersectInfo->hitNorm.y = genray.pos.y + genray.dir.y * pIntersectInfo->hitTime;
                pIntersectInfo->hitNorm.z = genray.pos.z + genray.dir.z * pIntersectInfo->hitTime;
                return true;
            }
        }
        return false;
    }

    bool Ray3::Intersect(float radius, float height, Matrix4x4 *pInvTm, RayIntersect3 *pIntersectInfo)
    {
        //
        // first convert ray into generic coordinate system
        //
        Ray3 genray;
        if (pInvTm)
        {
            Vector4 xformpos = pos * *pInvTm;
            Vector4 xformdir = Vector4(dir.x, dir.y, dir.z, 0.0f) * *pInvTm;
            genray.pos = Vector3(xformpos.x, xformpos.y, xformpos.z);
            genray.dir = Vector3(xformdir.x, xformdir.y, xformdir.z);
        }
        else
        {
            genray.pos = pos;
            genray.dir = dir;
        }
        //
        // our cone may have a top and bottom radius other than
        // 0 and 1 (which would define a cone otherwise it is a
        // tapered cylinder). Thus, the equation for the cone is:
        //
        //        b = bottom radius
        //        t = top radius
        //        h = height of cone
        //        x^2 + y^2 = (((z + 1) * t - (z - 1) * b) / 2)^2
        //
        float afac = 0.0F;
        float bfac = 2.0F;
        float afac2 = afac * afac;
        float a = (4.0F * genray.dir.x * genray.dir.x + 4.0F * genray.dir.y * genray.dir.y -
            afac2 * genray.dir.z * genray.dir.z);
        float b = (4.0F * genray.dir.x * genray.pos.x + 4.0F * genray.dir.y * genray.pos.y -
            afac2 * genray.dir.z * genray.pos.z - afac * bfac * genray.dir.z);
        float c = (4.0F * genray.pos.x * genray.pos.x + 4.0F * genray.pos.y * genray.pos.y -
            afac2 * genray.pos.z * genray.pos.z - 2.0F * afac * bfac * genray.pos.z - bfac * bfac);
        if (a < -TINY || a > TINY)
        {
            float bb = b * b;
            float discrim = bb - a * c;
            if (discrim >= 0.0F)
            {
                discrim = (float)sqrt(discrim);
                //
                // this may seem like a strange way to solve for the roots of the
                // quadratic equation; however, if a or c are very small then we will
                // end up subtracting almost equal quantities to find one of the roots
                // (i.e. b - discrim will be almost zero). This will result in a very
                // inaccurate solution for one of the roots. This idea was taken from
                // Numerical Recipes in C (Press,Flannery,Teukolsky,Vetterling), section
                // 5.5
                //
                if (b < -TINY)
                    discrim = -(b - discrim);
                else
                    discrim = -(b + discrim);
                float root1 = discrim / a;
                float root2 = (discrim != 0.0) ? c / discrim : root1;
                pIntersectInfo->hitTime = (root1 < root2) ? root1 : root2;
                float hitz = genray.pos.z + genray.dir.z * pIntersectInfo->hitTime;
#if 0
                if (ABS(hitz) <= 1.0F)
#else
                if (fabs(hitz) <= 1.0F && (genray.pos.x + genray.dir.x * pIntersectInfo->hitTime > 0.0F))
#endif
                {
                    // compute hit point and normal at hit point
                    pIntersectInfo->hitPt.x = pos.x + dir.x * pIntersectInfo->hitTime;
                    pIntersectInfo->hitPt.y = pos.y + dir.y * pIntersectInfo->hitTime;
                    pIntersectInfo->hitPt.z = pos.z + dir.z * pIntersectInfo->hitTime;
                    // compute normal at hit point
                    Vector3 hitPt(genray.pos.x + genray.dir.x * pIntersectInfo->hitTime,
                        genray.pos.y + genray.dir.y * pIntersectInfo->hitTime,
                        genray.pos.z + genray.dir.z * pIntersectInfo->hitTime);
                    //
                    // to find the normal use our equation for
                    // the cone: x^2+y^2=(((z+1)*t-(z-1)*b)/2)^2
                    // and solve for the gradient using partial
                    // derivatives.
                    //
                    pIntersectInfo->hitNorm.x = 8.0F * hitPt.x;
                    pIntersectInfo->hitNorm.y = 8.0F * hitPt.y;
                    pIntersectInfo->hitNorm.z = -2.0F * hitPt.z * afac2 - 2.0F * afac * bfac;
                    return true;
                }
            }
        }
        //
        // check top and bottom of cone
        //
        //
        // first we check the bottom of the cone. This is a circle
        // of radius cylinder->radius lying in the z=-1 plane.
        // thus, the hit time for the ray is at
        //    Az + Bz * T = -1
        //       or
        //      T = (-1 - Az) / Bz
        //
        float hitt = (-1 - genray.pos.z) / genray.dir.z;
        if (hitt < pIntersectInfo->hitTime)
        {
            float hitx = genray.pos.x + genray.dir.x * hitt;
            float hity = genray.pos.y + genray.dir.y * hitt;
            if (fabs(hitx) <= 1.0F && fabs(hity) <= 1.0F)
            {
                if (hitx * hitx + hity * hity <= 1.0F)
                {
                    // we hit the bottom of the cone
                    pIntersectInfo->hitTime = hitt;
                    pIntersectInfo->hitPt.x = pos.x + dir.x * hitt;
                    pIntersectInfo->hitPt.y = pos.y + dir.y * hitt;
                    pIntersectInfo->hitPt.z = pos.z + dir.z * hitt;
                    // compute normal at hit point
                    pIntersectInfo->hitNorm.x = 0.0F;
                    pIntersectInfo->hitNorm.y = 0.0F;
                    pIntersectInfo->hitNorm.z = -1.0F;
                    return true;
                }
            }
        }
        //
        // now check for the top of the cone
        //
        // use same as bottom of cone except circle lies in
        // z=1 plane with radius cylinder->radius
        //
        hitt = (1.0F - genray.pos.z) / genray.dir.z;
        if (hitt < pIntersectInfo->hitTime)
        {
            float hitx = genray.pos.x + genray.dir.x * hitt;
            float hity = genray.pos.y + genray.dir.y * hitt;
            if (fabs(hitx) <= 1.0F && fabs(hity) <= 1.0F)
            {
                if (hitx * hitx + hity * hity <= 1.0F)
                {
                    // we hit the top of the cone
                    pIntersectInfo->hitTime = hitt;
                    pIntersectInfo->hitPt.x = pos.x + dir.x * hitt;
                    pIntersectInfo->hitPt.y = pos.y + dir.y * hitt;
                    pIntersectInfo->hitPt.z = pos.z + dir.z * hitt;
                    // compute normal at hit point
                    pIntersectInfo->hitNorm.x = 0.0F;
                    pIntersectInfo->hitNorm.y = 0.0F;
                    pIntersectInfo->hitNorm.z = 1.0F;
                    return true;
                }
            }
        }
        return false;
    }

    //**********************************************************************
    // Method: Intersect
    // Calculates the intersection of a ray with an plane. If pIntersectInfo
    // is not nullptr then the intersection data is returned via this pointer.
    //
    // Parameters:
    // n - normal for plane
    // d - plane offset
    // pIntersectInfo - Returns the interesection info. Maybe nullptr.
    //
    // Returns:
    // True if ray interesects plane, otherwise false
    //**********************************************************************
    bool Ray3::Intersect(Vector3 n, float d, RayIntersect3* pIntersectInfo)
    {
        float dp = n.Dot(dir);
        if (IsZero(dp))
            return false;
        float t = d - n.Dot(pos) / dp;
        if (t < 0.0f)
            return false;
        if (pIntersectInfo)
        {
            pIntersectInfo->hitNorm = n;
            pIntersectInfo->hitPt = pos + dir * t;
            pIntersectInfo->hitTime = t;
        }
        return true;
    }

    //**********************************************************************
    // Method: Intersect
    // Calculates the intersection of a ray with an triangle. If pIntersectInfo
    // is not nullptr then the intersection data is returned via this pointer.
    //
    // Parameters:
    // p0, p1, p2 - coordinates for triangle's vertices
    // pIntersectInfo - Returns the interesection info. Maybe nullptr.
    //
    // Returns:
    // True if ray interesects triangle, otherwise false
    //**********************************************************************
    bool Ray3::Intersect(Vector3 p0, Vector3 p1, Vector3 p2, RayIntersect3* pIntersectInfo)
    {
        // Compute normal for triangle
        Vector3 d0 = (p1 - p0).Normalize();
        Vector3 d1 = (p2 - p0).Normalize();
        Vector3 n = d0.Cross(d1);
        float dp = n.Dot(dir);
        if (IsZero(dp))
            return false;
        float t = n.Dot(p0) - n.Dot(pos) / dp;
        Vector3 ip = pos + dir * t;
        // Perform inside/outside test
        if ((p1 - p0).Cross(ip - p0).Dot(n) < 0.0f ||
            (p2 - p1).Cross(ip - p1).Dot(n) < 0.0f ||
            (p0 - p2).Cross(ip - p2).Dot(n) < 0.0f)
            return false;
        if (pIntersectInfo)
        {
            pIntersectInfo->hitNorm = n;
            pIntersectInfo->hitPt = ip;
            pIntersectInfo->hitTime = t;
        }
        return true;
    }
}
