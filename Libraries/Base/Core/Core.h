//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include <math.h>

#ifdef CAUSTIC_EXPORT
#define CAUSTICAPI __declspec(dllexport)
#else
#define CAUSTICAPI extern
#endif

namespace Caustic
{
    // Define the basic types used throughout Caustic
    typedef unsigned char uint8;        //!< An unsigned 8b value
    typedef unsigned short uint16;      //!< An unsigned 16b value
    typedef unsigned int uint32;        //!< An unsigned 32b value
    typedef unsigned long long uint64;  //!< An unsigned 64b value
    typedef char int8;                  //!< A signed 8b value
    typedef short int16;                //!< A signed 16b value
    typedef int int32;                  //!< A signed 32b value
    typedef long long int64;            //!< A signed 64b value

    const float c_Epsilon = 0.0000001f;
    inline bool IsZero(float x) { return (x >= -c_Epsilon && x <= c_Epsilon) ? true : false; }
    inline bool IsEq(float x, float y) { return IsZero(x - y); }
    template <typename T> T clamp(T v, T minv, T maxv)
    {
        if (v < minv)
            return minv;
        if (v > maxv)
            return maxv;
        return v;
    }

    template <typename T>
    void Swap(T &t1, T &t2)
    {
        T tmp = t1;
        t1 = t2;
        t2 = tmp;
    }
}
