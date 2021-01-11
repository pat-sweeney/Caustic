//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <functional>

namespace Caustic
{
    //**********************************************************************
    // Class: Property
    // Defines syntactic sugar for creating C# like properties. Specifically,
    // I want to be able to do things like this:
    //      Vector3 v;
    //      Vector3 q = v.zyx;
    // where 'zyx' is a getter that performs swizzling. I don't want to
    // just expose a public member since that would require storing all
    // the permutations of v.xyz.
    //
    // Ideas in this code were borrowed from stack overflow:
    //  https://stackoverflow.com/questions/4225087/c-like-properties-in-native-c
    //**********************************************************************
    template<typename T>
    class Property
    {
    private:
        std::function<T(void)> getter;
        std::function<void(const T&)> setter;

    public:
        Property()
        {
        }

        Property(std::function<T(void)> get, std::function<void(const T&)> set) :
            getter(get),
            setter(set)
        {
        }

        Property<T>& operator = (const T& val)
        {
            setter(val);
            return *this;
        }

        operator const T () const
        {
            return getter();
        }
        
        T operator() ()
        {
            return getter();
        }
        
        T const operator() () const
        {
            return getter();
        }
    };
}