//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "Base\Math\BBox.h"

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// Function: isqrt
	// Computes the sqrt of an integer via Newtons method without using floating point.
	// See https://en.wikipedia.org/wiki/Integer_square_root for details.
	//**********************************************************************
	inline uint32 isqrt(uint32 n)
	{
		uint32 next = n >> 1;
		uint32 cur;

		if (n <= 1)
			return n;
		while (true)
		{
			cur = next;
			next = (next + n / next) >> 1;
			if (next >= cur)
				break;
		}
		return cur;
	}

	//**********************************************************************
	// Function: Determinant2x2
	// Computes the determinant of a 2x2 matrix.
	//
	// Parameters:
	// Matrix is defined as:
	// ---- Text ---
	//  | a00 a01 |
	//  | a10 a11 |
	// -------------
	//**********************************************************************
	extern float Determinant2x2(float a00, float a01, float a10, float a11);

	//**********************************************************************
	// Function: Determinant3x3
	// Computes the determinant of a 3x3 matrix.
	//
	// Parameters:
	// Matrix is defined as:
	// ---- Text ---
	//  | a00 a01 a02 |
	//  | a10 a11 a12 |
	//  | a20 a21 a22 |
	// -------------
	//**********************************************************************
	extern float Determinant3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);

	//**********************************************************************
	// Function: CircumCircle
	// Given 3 points of a triangle this function will compute the center and radius of the circle passing through those points
	//
	// Parameters:
	// p0 - first point on circle
	// p1 - second point on circle
	// p2 - third point on circle
	// pCircumCenter - returns the center of the circumcircle
	// pRadius - returns the radius of the circumcircle
	//**********************************************************************
	extern void CircumCircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 *pCircumCenter, float *pRadius);

	//**********************************************************************
	// Function: PointInTriangleCircumcircle
	// Tests if a point is inside the specified circumcircle
	//
	// Parameters:
	// p0 - first point on circle
	// p1 - second point on circle
	// p2 - third point on circle
	// pt - Point to test
	//
	// Returns:
	// True if point is in circumcircle. False otherwise.
	//**********************************************************************
	extern bool PointInTriangleCircumcircle(Vector2 &p0, Vector2 &p1, Vector2 &p2, Vector2 &pt);
}
