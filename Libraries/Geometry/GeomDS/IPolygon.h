//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Math\vector.h"
#include "Base\Core\IRefCount.h"
#include "Base\Core\core.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
    struct IPolygon2 : public IRefCount
    {
        virtual uint32 GetNumberPoints() = 0;
        virtual void GetPoint(uint32 index, Vector2 *pt) = 0;
        virtual void AddPoint(Vector2 &pt) = 0;
        virtual void GetBBox(BBox2 *pBBox) = 0;
        virtual void Simplify(float err, float maxLen, IPolygon2 **ppResult) = 0;
        virtual bool ContainsPoint(Vector2 &pt, float *pMinDist = nullptr, float *pMaxDist = nullptr) = 0;
    };

    extern void CreatePolygon2(IPolygon2 **ppPolygon);
}
