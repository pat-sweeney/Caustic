//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"
#include "Base\Math\BBox.h"

namespace Caustic
{
    using namespace Caustic;

    //**********************************************************************
	// Structure: OBBox2
    // Defines a oriented bounding box (in 2D)
    //**********************************************************************
    struct OBBox2
    {
        Vector2 origin;
        Vector2 u, v;
        Vector2 normu, normv; // normalized version of u and v

        OBBox2();
        OBBox2(const OBBox2 &obbox);
        bool Empty();
        OBBox2(Vector2 &p1, Vector2 &p2);
        OBBox2(BBox2 &bb);
        operator OBBox2();
        void AddPoint(float x, float y);
        void AddPoint(const Vector2 &p);
        void AddOBBox(OBBox2 &bb);
        bool PointInside(const Vector2 &p);
        void Transform(Matrix2x2 *tm);
    };

	//**********************************************************************
	// Structure: OBBox3
    // Defines a oriented bounding box (in 3D)
    //**********************************************************************
    struct OBBox3
    {
        Vector3 origin;
        Vector3 u, v, n;
        Vector3 normu, normv, normn; // normalized version of u, v, and n

        OBBox3();
        OBBox3(const OBBox3 &obbox);
        bool Empty();
        OBBox3(Vector3 &p1, Vector3 &p2);
        OBBox3(BBox3 &bb);
        operator OBBox3();
        void AddPoint(float x, float y, float z);
        void AddPoint(const Vector3 &p);
        void AddOBBox(OBBox3 &bb);
        bool PointInside(const Vector3 &p);
        void Transform(Matrix4x4 *tm);
        void Transform(Matrix4x3 *tm);
    };
}
