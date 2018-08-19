//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"
#include "Base\Math\Matrix.h"

namespace Caustic
{
    struct RayIntersect
    {
        float hitTime;
        Vector3 hitPt;
        Vector3 hitNorm;
    };

    //**********************************************************************
    //! \brief Defines a ray
    //**********************************************************************
    struct Ray3
    {
        Vector3 pos;
        Vector3 dir;

        Ray3() {}
        Ray3(Vector3 &_pos, Vector3 &_dir)
        {
            pos = _pos;
            dir = _dir;
        }

        bool Intersect(const BBox3 &bbox, Matrix4x4 *pInvTm, RayIntersect *pIntersectInfo);
        bool Intersect(float bottomRadius, float topRadius, float height, Matrix4x4 *pInvTm, RayIntersect *pIntersectInfo);
        bool Intersect(float radius, Matrix4x4 *pInvTm, RayIntersect *pIntersectInfo);
        bool Intersect(float radius, float height, Matrix4x4 *pInvTm, RayIntersect *pIntersectInfo);
    };
}
