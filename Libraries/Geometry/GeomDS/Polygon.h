//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Math\vector.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\core.h"
#include "Base\Math\BBox.h"
#include <vector>
#include "IPolygon.h"

namespace Caustic
{
    class CPolygon2 : public IPolygon2, public CRefCount
    {
        std::vector<Vector2> m_pts;
        BBox2 m_bbox;

        void SimplifyRecursive(IPolygon2 *pResult, int start, int end, float err, float maxLen);
    public:
        //**********************************************************************
        // IAddRef
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

        void Simplify(float err, float maxLen, IPolygon2 **ppResult) override;
        bool ContainsPoint(Vector2 &pos, float *pMinDist, float *pMaxDist) override;
    };
}
