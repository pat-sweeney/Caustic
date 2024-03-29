//**********************************************************************
// Copyright Patrick Sweeney 1995-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Geometry.GeomDS.Path2;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.GeomDS.IPath2;

export namespace Caustic
{
    class CPath2 : public IPath2, public CRefCount
    {
        std::vector<PathElem> m_elems;

        void BezierFlatten(Vector2* p0, Vector2* p1, Vector2* p2, Vector2* p3, float tolerance);
    public:
        CPath2()
        {
        }

        CPath2(uint32_t numElems)
        {
            m_elems.resize(numElems);
        }

        static CRefObj<IPath2> CreatePath2();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // IPath2
        //**********************************************************************
        virtual uint32_t GetNumberElems() override { return (uint32_t)m_elems.size(); }
        virtual void AddPathElem(EPathElem elemType, Vector2* p0 = nullptr, Vector2* p1 = nullptr, Vector2* p2 = nullptr) override;
        virtual void AppendPathElems(EPathElem elemType, ...) override;
        virtual const PathElem* GetElement(int indx) override { return &m_elems[indx]; }
        virtual CRefObj<IPath2> Flatten(float flatness, BBox2* bb = nullptr) override;
        virtual bool PointOnPath(Vector2& pt, Vector2* closestpt, float tolerance) override;
        virtual BBox2 GetBBox() override;
        virtual bool PathInsideBbox(BBox2& bbox) override;
    };
}
