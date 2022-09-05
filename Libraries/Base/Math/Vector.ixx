//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <functional>

export module Base.Math.Vector;
import Base.Core.Core;
import Base.Core.Property;

export namespace Caustic
{
    //**********************************************************************
    // Structure: Vector2
    // Defines a simple vector with 2 components
    //
    // Members:
    // float x - x component
    // float y - y component
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.ixx}}
    //**********************************************************************
    struct Vector2
    {
        float x, y;

	    //**********************************************************************
	    // Constructor: Vector2
	    // Default contructor
	    //**********************************************************************
	    Vector2() : x(0.0), y(0.0) {}

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
	    bool IsEq(const Vector2 &p) const
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
	    float Length() const { return (float)sqrtf(x * x + y * y); }

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
	    float Cross(const Vector2 &v) const
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
	    float Dot(const Vector2 &v) const
	    {
	        return x * v.x + y * v.y;
	    }

	    Vector2 operator-() const { return Vector2(-x, -y); }
	    Vector2 operator-(const Vector2 &rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
	    Vector2 operator+(const Vector2 &rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
	    Vector2 operator*(float f) const { return Vector2(x * f, y * f); }
	    bool operator==(const Vector2 &rhs) const { return this->IsEq(rhs); }
	    bool operator!=(const Vector2 &rhs) const { return !this->IsEq(rhs); }
    };

    //**********************************************************************
    // Structure: Vector3
    // Defines a simple vector with 3 components
    //
    // Members:
    // float x - x component
    // float y - y component
    // float z - z component
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.ixx}}
    //**********************************************************************
    struct Vector3
    {
        float x, y, z;

		//**********************************************************************
		// Constructor: Vector3
		// Default constructor
		//**********************************************************************
        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

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
        int Sign() const
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
        bool IsEq(const Vector3 &p) const
        {
            if (IsZero(p.x - x) &&
                IsZero(p.y - y) &&
                IsZero(p.z - z))
                return true;
            return false;
        }

		//**********************************************************************
		// Method: Cross
		// Returns the cross product between two vectors
        //
        // Parameters:
        // v - vector to compute cross product against
		//
		// Returns:
		// The cross product
		//**********************************************************************
        Vector3 Cross(const Vector3 &v) const
        {
            Vector3 nv;
            
            nv.x = y * v.z - z * v.y;
            nv.y = z * v.x - x * v.z;
            nv.z = x * v.y - y * v.x;
            return nv;
        }

		//**********************************************************************
		// Method: Dot
		// Returns the scalar product between two vectors
        //
        // Parameters:
        // v - vector to compute dot product against
		//
		// Returns:
		// The scalar product
		//**********************************************************************
        float Dot(const Vector3 &v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }
      
        bool operator==(const Vector3 &rhs) const { return this->IsEq(rhs); }
        bool operator!=(const Vector3 &rhs) const { return !this->IsEq(rhs); }
        Vector3 operator-(const Vector3 &rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        Vector3 operator-=(const Vector3 &rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        Vector3 operator+(const Vector3 &rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        Vector3 operator+=(const Vector3 &rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        Vector3 operator+(float v) const { return Vector3(x + v, y + v, z + v); }
        Vector3 operator-(float v) const { return Vector3(x - v, y - v, z - v); }
        Vector3 operator*(const float s) const { return Vector3(x * s, y * s, z * s); }
        Vector3 operator/(const float s) const { return Vector3(x / s, y / s, z / s); }
        float Length() const
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
    Vector3 operator*(const float s, const Vector3 &v) { return Vector3(v.x * s, v.y * s, v.z * s); }
#pragma warning(pop)

    //**********************************************************************
    // Structure: VectorExt3
    // Defines a variation of Vector3 that also supports swizzling. The reason
    // this isn't part of Vector3 is that construction of all properties is
    // far too heavy to place on the main class. This is only useful in non-critical
    // path (in terms of speed) code. The usage generally looks like:
    //        Vector3 newv = ((Vector3Ext)v).zyx;
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.ixx}}
    //**********************************************************************
    struct Vector3Ext : Vector3
    {
        Vector3Ext(Vector3& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
        }

        Property<Vector3> xyz = Property<Vector3>([this]() -> Vector3 { return Vector3(x, y, z); },
            [this](const Vector3& v) { x = v.x; y = v.y; z = v.z; });
        Property<Vector3> zyx = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(z, y, x); },
            [this](const Vector3& v) { z = v.x; y = v.y; x = v.z; });
        Property<Vector3> xxx = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(x, x, x); },
            [this](const Vector3& v) { x = v.x; });
        Property<Vector3> yyy = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(y, y, y); },
            [this](const Vector3& v) { y = v.x; });
        Property<Vector3> zzz = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(z, z, z); },
            [this](const Vector3& v) { z = v.x; });
        Property<Vector2> xy = Property<Vector2>(
            [this]() -> Vector2 { return Vector2(x, y); },
            [this](const Vector2& v) { x = v.x; y = v.y; });
        Property<Vector2> xz = Property<Vector2>(
            [this]() -> Vector2 { return Vector2(x, z); },
            [this](const Vector2& v) { x = v.x; z = v.y; });
        Property<Vector2> yz = Property<Vector2>(
            [this]() -> Vector2 { return Vector2(y, z); },
            [this](const Vector2& v) { y = v.x; z = v.y; });
    };

    //**********************************************************************
    // Structure: Vector4
    // Defines a simple vector with 4 components
    //
    // Members:
    // float x - x component
    // float y - y component
    // float z - z component
    // float w - w component
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.ixx}}
    //**********************************************************************
    struct Vector4
    {
        float x, y, z, w;

        Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
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

    //**********************************************************************
    // Structure: VectorExt4
    // Defines a variation of Vector4 that also supports swizzling. The reason
    // this isn't part of Vector4 is that construction of all properties is
    // far too heavy to place on the main class. This is only useful in non-critical
    // path (in terms of speed) code. The usage generally looks like:
    //        Vector4 newv = ((Vector4Ext)v).zyx;
    //
    // Module:
    // {Link:Base.Math.Vector{Base/Math/Vector.ixx}}
    //**********************************************************************
    struct Vector4Ext : Vector4
    {
        Vector4Ext(Vector4& v)
        {
            x = v.x;
            y = v.y;
            z = v.z;
            w = v.w;
        }

        // Define swizzle operators
        Property<Vector4> xyzw = Property<Vector4>(
            [this]() -> Vector4 { return Vector4(x, y, z, w); },
            [this](const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; });
        Property<Vector4> wzyx = Property<Vector4>(
            [this]() -> Vector4 { return Vector4(w, z, y, x); },
            [this](const Vector4& v) { w = v.x; z = v.y; y = v.z; x = v.w; });
        Property<Vector4> xxxx = Property<Vector4>(
            [this]() -> Vector4 { return Vector4(x, x, x, x); },
            [this](const Vector4& v) { x = v.x; });
        Property<Vector4> yyyy = Property<Vector4>(
            [this]() -> Vector4 { return Vector4(y, y, y, y); },
            [this](const Vector4& v) { y = v.x; });
        Property<Vector4> zzzz = Property<Vector4>(
            [this]() -> Vector4 { return Vector4(z, z, z, z); },
            [this](const Vector4& v) { z = v.x; });
        Property<Vector4> wwww = Property<Vector4>(
            [this]() -> Vector4 { return Vector4(w, w, w, w); },
            [this](const Vector4& v) { w = v.x; });

        Property<Vector3> xyz = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(x, y, z); },
            [this](const Vector3& v) { x = v.x; y = v.y; z = v.z; });
        Property<Vector3> zyx = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(z, y, x); },
            [this](const Vector3& v) { z = v.x; y = v.y; x = v.z; });
        Property<Vector3> xxx = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(x, x, x); },
            [this](const Vector3& v) { x = v.x; });
        Property<Vector3> yyy = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(y, y, y); },
            [this](const Vector3& v) { y = v.x; });
        Property<Vector3> zzz = Property<Vector3>(
            [this]() -> Vector3 { return Vector3(z, z, z); },
            [this](const Vector3& v) { z = v.x; });

        Property<Vector2> xy = Property<Vector2>(
            [this]() -> Vector2 { return Vector2(x, y); },
            [this](const Vector2& v) { x = v.x; y = v.y; });
        Property<Vector2> xz = Property<Vector2>(
            [this]() -> Vector2 { return Vector2(x, z); },
            [this](const Vector2& v) { x = v.x; z = v.y; });
        Property<Vector2> yz = Property<Vector2>(
            [this]() -> Vector2 { return Vector2(y, z); },
            [this](const Vector2& v) { y = v.x; z = v.y; });
    };
}
