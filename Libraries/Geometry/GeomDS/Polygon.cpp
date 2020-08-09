//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Polygon.h"
#include "Base\Math\Ray.h"
#include "Base\Math\Helper.h"
#include <memory>

namespace Caustic
{
    //**********************************************************************
    // Function: CreatePolygon2
    // CreatePolygon2 creates a new 2D polygon
    //
    // Returns:
    // Returns new polygon object
    //**********************************************************************
    CRefObj<IPolygon2> CreatePolygon2()
    {
        return CRefObj<IPolygon2>(new CPolygon2());
    }

    //**********************************************************************
    // Method: SimplifyRecursive 
    // SimplifyRecursive is used internally for simplifying a subpiece
    // of the polygon
    //
    // Parameters:
    // pResult - Polygon we are adding the simplified results to
    // start - Starting index for this subpiece
    // end - Ending index for this subpiece
    // err - Tolerance to allow for points we will ignore
    // maxLen - Longest line segment allowed. If maxLen==0 then no limit.
    //**********************************************************************
    void CPolygon2::SimplifyRecursive(IPolygon2 *pResult, int start, int end, float err, float maxLen)
    {
        if (start == end)
            return;
        int maxI = 0;
        float totalLen = 0.0f;
        float maxDist = 0.0f;
        for (int i = start + 1; i < end; i++)
        {
            if (maxLen > 0.0f)
            {
                float p = (m_pts[i] - m_pts[i - 1]).Length();
                totalLen += p;
                if (totalLen > maxLen)
                {
                    if (maxDist == 0.0f)
                        maxI = i;
                    break;
                }
            }
            float dist = DistancePointToLine(m_pts[start], m_pts[end], m_pts[i]);
            if (dist > err && dist > maxDist)
            {
                maxDist = dist;
                maxI = i;
            }
        }
        if (maxI != 0)
        {
            SimplifyRecursive(pResult, start, maxI, err, maxLen);
            pResult->AddPoint(m_pts[maxI]);
            SimplifyRecursive(pResult, maxI, end, err, maxLen);
        }
    }

    //**********************************************************************
    // Method: Simplify
    // Simplify Removes points from a polygon that allow the resulting
    // polygon to still be with 'err' units from the original polygon.
    // This function uses the Ramer-Douglas-Peucker algorithm:
    // https://en.wikipedia.org/wiki/Ramer–Douglas–Peucker_algorithm
    //
    // Parameters:
    // err - Tolerance for deciding whether to remove a point
    // maxLen - Longest line segment allowed. If maxLen==0 then no limit.
    //
    // Returns:
    // Returns the new simplified polygon
    //**********************************************************************
    CRefObj<IPolygon2> CPolygon2::Simplify(float err, float maxLen)
    {
        CRefObj<IPolygon2> spPolygon = CreatePolygon2();

        // Should probably replace this at some point with Zhao&Saalfeld's method which is faster
        // http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.494.7321&rep=rep1&type=pdf
        int start = 0;
        int end = (int)m_pts.size() - 1;
        spPolygon->AddPoint(m_pts[start]);
        SimplifyRecursive(spPolygon, start, end, err, maxLen);
        spPolygon->AddPoint(m_pts[end]);
        return spPolygon;
    }


    //**********************************************************************
    // Method: ContainsPoint
    // ContainsPoint determines whether the specified point is inside
    // the polygon.
    //
    // Parameters:
    // pos - Point to rest
    // pMinDist - Optional argument that returns the closest distance
    // from the point to the polygon
    // pMaxDist - Optional argument that returns the furthest distance
    // from the point to the polygon (Hausdorff distance)
    //**********************************************************************
    bool CPolygon2::ContainsPoint(Vector2 &pos, float *pMinDist = nullptr, float *pMaxDist = nullptr)
    {
        if (pMinDist)
            *pMinDist = FLT_MAX;
        if (pMaxDist)
            *pMaxDist = -FLT_MAX;

        // Intersect ray going from point 'pos' in +X direction and
        // count number of times the ray intersects a line on the polygon.
        Ray2 ray;
        ray.pos = pos;
        ray.dir = Vector2(1, 0);
        int count = 0;
        Vector2 dir(1, 0);
        int li = (int)m_pts.size() - 1;
        float lastT = 0.0f;
        for (int i = 0; i < this->m_pts.size(); i++)
        {
            RayIntersect2 intersectionPoint;
            if (ray.IntersectWithLine(m_pts[li], m_pts[i], &intersectionPoint))
            {
                // Check if the intersection time on the line segment is 0.0
                // and the last segment's intersection time was 1.0. If so, we
                // hit the endpoint of the last line segment and the start point
                // of the current line segment (i.e. counting the same hitPt twice)
                if ((!IsEq(lastT, 1.0f) || !IsZero(intersectionPoint.hitTime)) && IsGreater(intersectionPoint.hitTimeRay, 0.0f))
                {
                    if (pMinDist || pMaxDist)
                    {
                        float dist = DistancePointToLine(m_pts[li], m_pts[i], pos);
                        if (pMinDist && dist < *pMinDist)
                            *pMinDist = dist;
                        if (pMaxDist && dist > *pMaxDist)
                            *pMaxDist = dist;
                    }
                    count++;
                }
                lastT = intersectionPoint.hitTime;
            }
            li = i;
        }
        return (count & 1) ? true : false;
    }
}
