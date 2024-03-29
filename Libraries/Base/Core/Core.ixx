//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <cinttypes>

#pragma warning(disable:5050)

export module Base.Core.Core;
export import :Part1;

// Namespace: Caustic
// Module:
// {Link:import Base.Core.Core;{Base/Core/Core.ixx}}
export namespace Caustic
{
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
    float Abs(float x) { return (x < 0.0f) ? -x : x; }

    //**********************************************************************
    // Function: DegreesToRadians
    // Converts degrees to radians
    //**********************************************************************
    float DegreesToRadians(float x) { return x * (float)M_PI / 180.0f; }

    //**********************************************************************
    // Function: RadiansToDegrees
    // Converts radians to degrees
    //**********************************************************************
    float RadiansToDegrees(float x) { return x * 180.0f / (float)M_PI; }

    //**********************************************************************
    // Function: IsZero
    // Tests if a given value is close to zero
    //**********************************************************************
    bool IsZero(float x) { return (x >= -c_Epsilon && x <= c_Epsilon) ? true : false; }

    //**********************************************************************
    // Function: IsEq
    // Tests whether two values are sufficiently close to be considered equivalent
    //**********************************************************************
    bool IsEq(float x, float y) { return IsZero(x - y); }

    //**********************************************************************
    // Function: IsLess
    // Tests if x + epsilon < y
    //**********************************************************************
    bool IsLess(float x, float y) { return x < (y - c_Epsilon) ? true : false; }
    
    //**********************************************************************
    // Function: IsGreater
    // Tests if x > y + epsilon
    //**********************************************************************
    bool IsGreater(float x, float y) { return x > (y + c_Epsilon) ? true : false; }

    //**********************************************************************
    // Function: IsLessEq
    // Tests if x + epsilon <= y
    //**********************************************************************
    bool IsLessEq(float x, float y) { return x <= (y - c_Epsilon) ? true : false; }

    //**********************************************************************
    // Function: IsGreaterEq
    // Tests if x >= y + epsilon
    //**********************************************************************
    bool IsGreaterEq(float x, float y) { return x >= (y + c_Epsilon) ? true : false; }

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
    // Function: SmoothStep
    // Returns a normalized value (0..1) for a value 'x' that lies between
    // 'lowerBound' and 'upperBound' using a smooth step function.
    //
    // Parameters:
    // lowerBound - lower bound at which value is 0
    // upperBound - upper bound at which value is 1
    // x - value to smooth step
    //
    // Returns:
    // A value between 0 and 1
    //**********************************************************************
    float SmoothStep(float lowerBound, float upperBound, float x)
    {
        if (x < lowerBound)
            return 0.0f;
        if (x > upperBound)
            return 1.0f;
        float normX = (x - lowerBound) / (upperBound - lowerBound);
        return normX * normX * (3 - 2 * normX);
    }

    //**********************************************************************
    float Gamma(float gamma, float x)
    {
        return powf(x, 1.0f / gamma);
    }

    //**********************************************************************
    float Bias(float b, float x)
    {
        return powf(x, logf(b) / logf(0.5f));
    }

    //**********************************************************************
    float Gain(float g, float x)
    {
        if (x < 0.5f)
            return Bias(1 - g, 2.0f * x) / 2.0f;
        return 1.0f - Bias(1.0f - g, 2.0f - 2.0f * x) / 2.0f;
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

    //**********************************************************************
    // Function: GetCausticRootDirectory
    // Returns the default folder for the Caustic library
    //**********************************************************************
    std::wstring GetCausticRootDirectory();

    //**********************************************************************
    // Function: GetCausticShaderDirectory
    // Returns the default folder for the Caustic shaders
    //**********************************************************************
    std::wstring GetCausticShaderDirectory();
}
