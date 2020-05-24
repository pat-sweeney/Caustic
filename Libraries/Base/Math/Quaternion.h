//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include <math.h>

namespace Caustic
{
    struct Matrix4x4;
    struct Matrix4x3;

	//**********************************************************************
	// Structure: Quaternion
	// Defines a quaternion
	//
	// Members:
	// float x - X component
	// float y - Y component
	// float z - Z component
	// float w - W component
	//
	// Header:
	// [Link:Base/Math/Quaternion.h]
	//**********************************************************************
	struct Quaternion
    {
        float x, y, z, w;
    public:
		//**********************************************************************
		// Constructor: Quaternion
		// Default ctor
		//**********************************************************************
		Quaternion();

		//**********************************************************************
		// Constructor: Quaternion
		// Constructor from 4x4 matrix
		//**********************************************************************
		Quaternion(Matrix4x4 &tm);

		//**********************************************************************
		// Constructor: Quaternion
		// Constructor from 4x3 matrix
		//**********************************************************************
		Quaternion(Matrix4x3 &tm);

		//**********************************************************************
		// Constructor: Quaternion
		// Constructor from vector and angle
		//
		// Parameters:
		// vec - vector define axis of rotation
		// angle - angle of rotation around axis of rotation
		//**********************************************************************
		Quaternion(Caustic::Vector3 *vec, float angle);

		//**********************************************************************
		// Constructor: Quaternion
		// This ctor creates a Quaternion for a list of floats.
		// If 'isvec' is TRUE then _x,_y,_z form a direction
		// vector and _w is the angle of rotation about the direction vector.
		// Otherwise, _x,_y,_z,_w are treated as the four components
		// of a quaternion.
		//
		// Parameters:
		// x - X component of quaternion or X component of axis of rotation
		// y - Y component of quaternion or Y component of axis of rotation
		// z - Z component of quaternion or Z component of axis of rotation
		// w - W component of quaternion or angle of rotation
		// isvec - specifies whether x/y/z/w are axis/angle or quaternion values
		//**********************************************************************
		Quaternion(float _x, float _y, float _z, float _w, bool isvec = false);

		//**********************************************************************
		// Method: operator*
		// Multiplies two quaternions
		//
		// Returns:
		// Resulting quaternion
		//**********************************************************************
		Quaternion operator *(Quaternion &q);

		//**********************************************************************
		// Method: operator*
		// Multiplies two quaternions
		//
		// Returns:
		// Resulting quaternion
		//**********************************************************************
		Quaternion &operator *=(Quaternion &q)
        {
            *this = *this * q;
            return *this;
        }

		//**********************************************************************
		// Method: Conjugate
		// Computes the conjugate of the quaternion
		//
		// Returns:
		// Conjugate of the quaternion
		//**********************************************************************
		Quaternion Conjugate();

		//**********************************************************************
		// Method: Slerp
		// Computes the spherical linear interpolation between two quaternions
		//
		// Parameters:
		// q1 - first quaternion
		// q2 - second quaternion
		// t - parameter specifying slerp distance between q1 and q2
		//
		// Returns:
		// Resulting quaternion
		//**********************************************************************
		Quaternion Slerp(Quaternion &q1, Quaternion &q2, float t);
    };
}
