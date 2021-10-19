//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Geometry.GeomDS.Polygon;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.GeomDS.IPolygon;
export namespace Caustic
{
    class CPolygon2 : public IPolygon2, public CRefCount
    {
        std::vector<Vector2> m_pts;
        BBox2 m_bbox;

        void SimplifyRecursive(IPolygon2 *pResult, int start, int end, float err, float maxLen);
    public:
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // IPolygon2
        //**********************************************************************
        uint32 GetNumberPoints() override { return (uint32)m_pts.size(); }
        void GetPoint(uint32 index, Vector2 *pt) override { *pt = m_pts[index]; }
        void AddPoint(Vector2 &pt) override
        {
            m_bbox.AddPoint(pt);
            m_pts.push_back(pt);
        }

        void GetBBox(BBox2 *pBBox) override
        {
            *pBBox = m_bbox;
        }

        CRefObj<IPolygon2> Simplify(float err, float maxLen) override;
        bool ContainsPoint(Vector2 &pos, float *pMinDist, float *pMaxDist) override;
    };
}
