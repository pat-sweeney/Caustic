//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"

namespace Caustic
{
    //**********************************************************************
    // Structure: Vector2
    // Defines a simple vector with 2 components
    //**********************************************************************
    struct Vector2
    {
        float x, y;

	//**********************************************************************
	// Constructor: Vector2
	// Default contructor
	//**********************************************************************
	Vector2() {}

	//**********************************************************************
	// Constructor: Vector2
	// Contructor for a 2D vector
	//
	// Parameters:
	// _x - X coordinate
	// _y - Y coordinate
	//**********************************************************************
	Vector2(float _x, float _y) { x = _x; y = _y; }

	//**********************************************************************
	// Method: IsEq
	// Returns whether two points are equivalent (within some tolerance)
	//
	// Parameters:
	// p - Point to compare against
	//
	// Returns:
	// True if points are equivalent. False otherwise.
	//**********************************************************************
	bool IsEq(const Vector2 &p)
	{
	    if (IsZero(p.x - x) && IsZero(p.y - y))
		    return true;
	    return false;
	}

	//**********************************************************************
	// Method: Length
	// Returns the L2 norm for the vector
	//
	// Returns:
	// Length of vector
	//**********************************************************************
	float Length() { return (float)sqrtf(x * x + y * y); }

	//**********************************************************************
	// Method: Normalize
	// Normalizes the vector
	//
	// Returns:
	// Normalized vector
	//**********************************************************************
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

	//**********************************************************************
	// Method: Cross
	// Computes cross product between two vectors
	//
	// Returns:
	// Cross product vector
	//**********************************************************************
	float Cross(const Vector2 &v)
	{
	    return x * v.y - y * v.x;
	}

	//**********************************************************************
	// Method: Dot
	// Computes dot product between two vectors
	//
	// Returns:
	// Dot product
	//**********************************************************************
	float Dot(const Vector2 &v)
	{
	    return x * v.x + y * v.y;
	}

	Vector2 operator-() { return Vector2(-x, -y); }
	Vector2 operator-(Vector2 &rhs) { return Vector2(x - rhs.x, y - rhs.y); }
	Vector2 operator+(Vector2 &rhs) { return Vector2(x + rhs.x, y + rhs.y); }
	Vector2 operator*(float f) { return Vector2(x * f, y * f); }
	bool operator==(Vector2 &rhs) { return this->IsEq(rhs); }
	bool operator!=(Vector2 &rhs) { return !this->IsEq(rhs); }
    };

    //**********************************************************************
    // Structure: Vector3
    // Defines a simple vector with 3 components
    //**********************************************************************
    struct Vector3
    {
        float x, y, z;

		//**********************************************************************
		// Constructor: Vector3
		// Default constructor
		//**********************************************************************
        Vector3() {}

		//**********************************************************************
		// Constructor: Vector3
		// Constructor
		//
		// Parameters:
		// _x - X coordinate
		// _y - Y coordinate
		// _z - Z coordinate
		//**********************************************************************
        Vector3(const float _x, const float _y, const float _z) { x = _x; y = _y; z = _z; }

		//**********************************************************************
		// Method: Sign
		// Returns -1 or +1 depending on direction vector is pointing
		//**********************************************************************
        int Sign()
        {
            int s0 = (x < 0) ? -1 : +1;
            int s1 = (y < 0) ? -1 : +1;
            int s2 = (z < 0) ? -1 : +1;
            return s0 * s1 * s2;
        }

		//**********************************************************************
		// Method: IsEq
		// Returns whether two points are equivalent (within some tolerance)
        //
        // Parameters:
        // p - point to test against
		//
		// Returns:
		// True if points are considered equivalent. False otherwise.
		//**********************************************************************
        bool IsEq(const Vector3 &p)
        {
            if (IsZero(p.x - x) &&
                IsZero(p.y - y) &&
                IsZero(p.z - z))
                return true;
            return false;
        }

		//**********************************************************************
		// Method: cross
		// Returns the cross product between two vectors
        //
        // Parameters:
        // v - vector to compute cross product against
		//
		// Returns:
		// The cross product
		//**********************************************************************
        Vector3 cross(Vector3 &v)
        {
            Vector3 nv;
            
            nv.x = y * v.z - z * v.y;
            nv.y = z * v.x - x * v.z;
            nv.z = x * v.y - y * v.x;
            return nv;
        }

		//**********************************************************************
		// Method: dot
		// Returns the dot product between two vectors
        //
        // Parameters:
        // v - vector to compute dot product against
		//
		// Returns:
		// The dot product
		//**********************************************************************
        float dot(const Vector3 &v)
        {
            return x * v.x + y * v.y + z * v.z;
        }
      
        bool operator==(const Vector3 &rhs) { return this->IsEq(rhs); }
        bool operator!=(const Vector3 &rhs) { return !this->IsEq(rhs); }
        Vector3 operator-(const Vector3 &rhs) { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        Vector3 operator-=(const Vector3 &rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Vector3 operator+(const Vector3 &rhs) { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        Vector3 operator+=(const Vector3 &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Vector3 operator*(const float s) { return Vector3(x * s, y * s, z * s); }
        Vector3 operator/(const float s) { return Vector3(x / s, y / s, z / s); }
        float Length()
        {
            return (float)sqrtf(x * x + y * y + z * z);
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
    static Vector3 operator*(const float s, const Vector3 &v) { return Vector3(v.x * s, v.y * s, v.z * s); }
#pragma warning(pop)

    //**********************************************************************
    // Structure: Vector4
    // Defines a simple vector with 4 components
    //**********************************************************************
    struct Vector4
    {
        float x, y, z, w;

        Vector4() {}
        Vector4(const float _x, const float _y, const float _z, const float _w) { x = _x; y = _y; z = _z; w = _w; }

        //**********************************************************************
        // Method: IsEq
        // Returns whether two points are equivalent (within some tolerance)
        //
        // Parameters:
        // p - Point to test against
        //**********************************************************************
        bool IsEq(const Vector4 &p)
        {
            if (IsZero(p.x - x) &&
                IsZero(p.y - y) &&
                IsZero(p.z - z) &&
                IsZero(p.w - w))
                return true;
            return false;
        }

        Vector4 operator-(const Vector4 &rhs) { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        Vector4 operator+(const Vector4 &rhs) { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        Vector4 operator*(const float s) { return Vector4(x * s, y * s, z * s, w * s); }
        bool operator==(const Vector4 &rhs) { return this->IsEq(rhs); }
        bool operator!=(const Vector4 &rhs) { return !this->IsEq(rhs); }
        float Length()
        {
            return (float)sqrtf(x * x + y * y + z * z + w * w);
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
    static Vector4 operator*(const float s, const Vector4 &v) { return Vector4(v.x * s, v.y * s, v.z * s, v.w * s); }
#pragma warning(pop)
}
