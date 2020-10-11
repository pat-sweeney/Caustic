//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#ifdef CAUSTIC_EXPORT
#define CAUSTICAPI __declspec(dllexport)
#else
#define CAUSTICAPI extern
#endif

// Namespace: Caustic
namespace Caustic
{
    // Define the basic types used throughout Caustic

    //**********************************************************************
	// Type: uint8
	// An unsigned 8b value
	//**********************************************************************
	typedef unsigned char uint8;
    
	//**********************************************************************
	// Type: uint16
	// An unsigned 16b value
	//**********************************************************************
	typedef unsigned short uint16;

	//**********************************************************************
	// Type: uint32
	// An unsigned 32b value
	//**********************************************************************
	typedef unsigned int uint32;

	//**********************************************************************
	// Type: uint64
	// An unsigned 64b value
	//**********************************************************************
	typedef unsigned long long uint64;

	//**********************************************************************
	// Type: int8
	// A signed 8b value
	//**********************************************************************
	typedef char int8;

	//**********************************************************************
	// Type: int16
	// A signed 16b value
	//**********************************************************************
	typedef short int16;
    
	//**********************************************************************
	// Type: int32
	// A signed 32b value
	//**********************************************************************
	typedef int int32;

	//**********************************************************************
	// Type: int64
	// A signed 64b value
	//**********************************************************************
	typedef long long int64;

	//**********************************************************************
	// Type: float1
	// A single floating point value. Same as 'float'. Here for symetry with HLSL.
	//**********************************************************************
	typedef struct float1
	{
		float x;

		float1() : x(0.0f) {};
		float1(float f) { x = f; }
		operator float() { return x; }
	} float1;

	//**********************************************************************
	// Type: float2
	// A 2-tuple floating point value.
	//**********************************************************************
	typedef struct float2
	{
		float x;
		float y;

		float2() : x(0.0f), y(0.0f) {};
		float2(float fx, float fy) { x = fx; y = fy; }
	} float2;

	//**********************************************************************
	// Type: float3
	// A 3-tuple floating point value.
	//**********************************************************************
	typedef struct float3
	{
		float x;
		float y;
		float z;

		float3() : x(0.0f), y(0.0f), z(0.0f) {};
		float3(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
	} float3;

	//**********************************************************************
	// Type: float4
	// A 4-tuple floating point value.
	//**********************************************************************
	typedef struct float4
	{
		float x;
		float y;
		float z;
		float w;

		float4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {};
		float4(float fx, float fy, float fz, float fw) { x = fx; y = fy; z = fz; w = fw; }
	} float4;

	//**********************************************************************
	// Constant: c_Epsilon
	// Value considered close to 0.0
	//**********************************************************************
	const float c_Epsilon = 0.0000001f;

	//**********************************************************************
	// Function: Abs
	// Returns the absolute value of 'x'
	//**********************************************************************
	inline float Abs(float x) { return (x < 0.0f) ? -x : x; }

	//**********************************************************************
	// Function: DegreesToRadians
	// Converts degrees to radians
	//**********************************************************************
	inline float DegreesToRadians(float x) { return x * (float)M_PI / 180.0f; }

	//**********************************************************************
	// Function: RadiansToDegrees
	// Converts radians to degrees
	//**********************************************************************
	inline float RadiansToDegrees(float x) { return x * 180.0f / (float)M_PI; }

	//**********************************************************************
	// Function: IsZero
	// Tests if a given value is close to zero
	//**********************************************************************
	inline bool IsZero(float x) { return (x >= -c_Epsilon && x <= c_Epsilon) ? true : false; }

	//**********************************************************************
	// Function: IsEq
	// Tests whether two values are sufficiently close to be considered equivalent
	//**********************************************************************
	inline bool IsEq(float x, float y) { return IsZero(x - y); }

	//**********************************************************************
	// Function: IsLess
	// Tests if x + epsilon < y
	//**********************************************************************
	inline bool IsLess(float x, float y) { return x < (y - c_Epsilon) ? true : false; }
	
	//**********************************************************************
	// Function: IsGreater
	// Tests if x > y + epsilon
	//**********************************************************************
	inline bool IsGreater(float x, float y) { return x > (y + c_Epsilon) ? true : false; }

	//**********************************************************************
	// Function: IsLessEq
	// Tests if x + epsilon <= y
	//**********************************************************************
	inline bool IsLessEq(float x, float y) { return x <= (y - c_Epsilon) ? true : false; }

	//**********************************************************************
	// Function: IsGreaterEq
	// Tests if x >= y + epsilon
	//**********************************************************************
	inline bool IsGreaterEq(float x, float y) { return x >= (y + c_Epsilon) ? true : false; }

	//**********************************************************************
	// Function: Clamp
	// Clamps a value to be between minv and maxv
	//
	// Parameters:
	// v - value to clamp
	// minv - minimum value
	// maxv - maximum value
	//
	// Returns:
	// Returns the clamped value
	//**********************************************************************
	template <typename T> T Clamp(T v, T minv, T maxv)
    {
        if (v < minv)
            return minv;
        if (v > maxv)
            return maxv;
        return v;
    }

	//**********************************************************************
	// Function: Swap
	// Swaps value t1 with t2
	//
	// Parameters:
	// t1 - first value
	// t2 - second value
	//**********************************************************************
	template <typename T>
    void Swap(T &t1, T &t2)
    {
        T tmp = t1;
        t1 = t2;
        t2 = tmp;
    }
}
