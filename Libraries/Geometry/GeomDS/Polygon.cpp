//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Polygon.h"
#include "Base\Math\Ray.h"
#include <memory>

namespace Caustic
{
    //**********************************************************************
    //! \brief CreatePolygon2 creates a new 2D polygon
    //! @param[out] ppPolygon New polygon object
    //**********************************************************************
    void CreatePolygon2(IPolygon2 **ppPolygon)
    {
        std::unique_ptr<CPolygon2> spPolygon(new CPolygon2());
        *ppPolygon = spPolygon.release();
        (*ppPolygon)->AddRef();
    }

    //**********************************************************************
    //! \brief DistancePointToLine returns the shortest distance from a given
    //! point to a line.
    //! @param[in] p0 First point defining the line
    //! @param[in] p1 Second point defining the line
    //! @param[in] pt Point to test against line
    //**********************************************************************
    float DistancePointToLine(Caustic::Vector2 &p0, Caustic::Vector2 &p1, Caustic::Vector2 &pt)
    {
        float area = (p1.y - p0.y) * pt.x - (p1.x - p0.x) * pt.y + p1.x * p0.y - p1.y * p0.x;
        if (area < 0)
            area = -area;
        float dx = p1.x - p0.x;
        float dy = p1.y - p0.y;
        float dist = area / sqrt(dx * dx + dy * dy);
        return dist;
    }

    //**********************************************************************
    //! \brief SimplifyRecursive is used internally for simplifying a subpiece
    //! of the polygon
    //! @param[in] pResult Polygon we are adding the simplified results to
    //! @param[in] start Starting index for this subpiece
    //! @param[in] end Ending index for this subpiece
    //! @param[in] err Tolerance to allow for points we will ignore
    //**********************************************************************
    void CPolygon2::SimplifyRecursive(IPolygon2 *pResult, int start, int end, float err)
    {
        if (start == end)
            return;
        float maxDist = 0.0f;
        int maxI = 0;
        for (int i = start + 1; i < end; i++)
        {
            float dist = DistancePointToLine(m_pts[start], m_pts[end], m_pts[i]);
            if (dist > err && dist > maxDist)
            {
                maxDist = dist;
                maxI = i;
            }
        }
        if (maxI != 0)
        {
            SimplifyRecursive(pResult, start, maxI, err);
            pResult->AddPoint(m_pts[maxI]);
            SimplifyRecursive(pResult, maxI, end, err);
        }
    }

    //**********************************************************************
    //! \brief Simplify Removes points from a polygon that allow the resulting
    //! polygon to still be with 'err' units from the original polygon.
    //! This function uses the Ramer-Douglas-Peucker algorithm:
    // https://en.wikipedia.org/wiki/Ramer�Douglas�Peucker_algorithm
    //! @param[in] err Tolerance for deciding whether to remove a point
    //! @param[out] ppResult The new simplified polygon
    //**********************************************************************
    void CPolygon2::Simplify(float err, IPolygon2 **ppResult)
    {
        CRefObj<IPolygon2> spPolygon;
        CreatePolygon2(&spPolygon);

        // Should probably replace this at some point with Zhao&Saalfeld's method which is faster
        // http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.494.7321&rep=rep1&type=pdf
        int start = 0;
        int end = (int)m_pts.size() - 1;
        spPolygon->AddPoint(m_pts[start]);
        SimplifyRecursive(spPolygon.p, start, end, err);
        spPolygon->AddPoint(m_pts[end]);
        *ppResult = spPolygon.Detach();
    }


    //**********************************************************************
    //! \brief ContainsPoint determines whether the specified point is inside
    //! the polygon.
    //! @param[in] pos Point to rest
    //! @param[out] pMinDist Optional argument that returns the closest distance
    //! from the point to the polygon
    //! @param[out] pMaxDist Optional argument that returns the furthest distance
    //! from the point to the polygon (Hausdorff distance)
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
        for (int i = 0; i < this->m_pts.size(); i++)
        {
            RayIntersect2 intersectionPoint;
            {
                wchar_t buf[1024];
                swprintf_s(buf, L"Point: %f,%f   Line:%f,%f..%f,%f\n", pos.x, pos.y, m_pts[li].x, m_pts[li].y, m_pts[i].x, m_pts[i].y);
                OutputDebugString(buf);
            }
            if (ray.IntersectWithLine(m_pts[li], m_pts[i], &intersectionPoint))
            {
                {
                    wchar_t buf[1024];
                    swprintf_s(buf, L"HitPt:%f,%f  Time:%f\n", intersectionPoint.hitPt.x, intersectionPoint.hitPt.y, intersectionPoint.hitTime);
                    OutputDebugString(buf);
                }
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
            li = i;
        }
        return (count & 1) ? true : false;
    }
}
