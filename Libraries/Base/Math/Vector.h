//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"

namespace Caustic
{
    //**********************************************************************
    //! Defines a simple vector with 2 components
    //**********************************************************************
    struct Vector2
    {
        float x, y;

        Vector2() {}
        Vector2(float _x, float _y) { x = _x; y = _y; }

        //! \brief IsEq returns whether two points are equivalent (within some tolerance)
        bool IsEq(Vector2 &p)
        {
            if (IsZero(p.x - x) &&
                IsZero(p.y - y))
                return true;
            return false;
        }

        float Length() { return (float)sqrt(x * x + y * y); }
        Vector2 Normalize()
        {
            float len = Length();
            if (!IsZero(len))
            {
                x /= len;
                y /= len;
            }
            return Vector2(x, y);
        }

        Vector2 operator-(Vector2 &rhs) { return Vector2(x - rhs.x, y - rhs.y); }
        Vector2 operator+(Vector2 &rhs) { return Vector2(x + rhs.x, y + rhs.y); }
        Vector2 operator*(float f) { return Vector2(x * f, y * f); }
        bool operator==(Vector2 &rhs) { return this->IsEq(rhs); }
        bool operator!=(Vector2 &rhs) { return !this->IsEq(rhs); }
    };

    //**********************************************************************
    //! Defines a simple vector with 3 components
    //**********************************************************************
    struct Vector3
    {
        float x, y, z;

        Vector3() {}
        Vector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }

        //! \brief Returns -1 or +1 depending on direction vector is pointing
        int Sign()
        {
            int s0 = (x < 0) ? -1 : +1;
            int s1 = (y < 0) ? -1 : +1;
            int s2 = (z < 0) ? -1 : +1;
            return s0 * s1 * s2;
        }

        //! \brief IsEq returns whether two points are equivalent (within some tolerance)
        bool IsEq(Vector3 &p)
        {
            if (IsZero(p.x - x) &&
                IsZero(p.y - y) &&
                IsZero(p.z - z))
                return true;
            return false;
        }

        Vector3 cross(Vector3 &v)
        {
            Vector3 nv;
            
            nv.x = y * v.z - z * v.y;
            nv.y = z * v.x - x * v.z;
            nv.z = x * v.y - y * v.x;
            return nv;
        }
        float dot(Vector3 &v)
        {
            return x * v.x + y * v.y + z * v.z;
        }
        bool operator==(Vector3 &rhs) { return this->IsEq(rhs); }
        bool operator!=(Vector3 &rhs) { return !this->IsEq(rhs); }
        Vector3 operator-(Vector3 &rhs) { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        Vector3 operator-=(Vector3 &rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Vector3 operator+(Vector3 &rhs) { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        Vector3 operator+=(Vector3 &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Vector3 operator*(float s) { return Vector3(x * s, y * s, z * s); }
        Vector3 operator/(float s) { return Vector3(x / s, y / s, z / s); }
        float Length()
        {
            return (float)sqrt(x * x + y * y + z * z);
        }

        Vector3 Normalize()
        {
            float len = Length();
            if (!IsZero(len))
            {
                x /= len;
                y /= len;
                z /= len;
            }
            return Vector3(x, y, z);
        }
    };
#pragma warning(push)
#pragma warning(disable : 4505)
    static Vector3 operator*(float s, Vector3 &v) { return Vector3(v.x * s, v.y * s, v.z * s); }
#pragma warning(pop)

    //**********************************************************************
    //! Defines a simple vector with 4 components
    //**********************************************************************
    struct Vector4
    {
        float x, y, z, w;

        Vector4() {}
        Vector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }

        //! \brief IsEq returns whether two points are equivalent (within some tolerance)
        bool IsEq(Vector4 &p)
        {
            if (IsZero(p.x - x) &&
                IsZero(p.y - y) &&
                IsZero(p.z - z) &&
                IsZero(p.w - w))
                return true;
            return false;
        }

        Vector4 operator-(Vector4 &rhs) { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        Vector4 operator+(Vector4 &rhs) { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        Vector4 operator*(float s) { return Vector4(x * s, y * s, z * s, w * s); }
        bool operator==(Vector4 &rhs) { return this->IsEq(rhs); }
        bool operator!=(Vector4 &rhs) { return !this->IsEq(rhs); }
        float Length()
        {
            return (float)sqrt(x * x + y * y + z * z + w * w);
        }
        void Normalize()
        {
            float len = Length();
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    };
#pragma warning(push)
#pragma warning(disable : 4505)
    static Vector4 operator*(float s, Vector4 &v) { return Vector4(v.x * s, v.y * s, v.z * s, v.w * s); }
#pragma warning(pop)
}
