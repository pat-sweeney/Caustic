//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

module Base.Math.UnitTests;
import Base.Core.Core;
import Base.Core.UnitTests;
import Base.Math.Vector;
import Base.Math.NewtonsMethod;
import Base.Math.Quaternion;

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        Vector3 v0(1.0f, 0.0f, 0.0f);
        Vector3 v1(0.0f, 1.0f, 0.0f);
        Vector3 r = v0.Cross(v1).Normalize();
        if (r != Vector3(0.0f, 0.0f, 1.0f)) // Implicitly a right-handed system
        {
            return false;
        }
        return true;
    }

    static bool Test2()
    {
        Vector3 v0(1.0f, 0.0f, 0.0f);
        Vector3 v1(0.0f, 1.0f, 0.0f);
        Vector3 r = v0 + v1;
        if (r != Vector3(1.0f, 1.0f, 0.0f)) // Implicitly a right-handed system1
        {
            return false;
        }
        return true;
    }

    static bool Test3()
    {
        float val = NewtonsMethod(5.0f, [](float x)->float {
            return x * x - 4 * x;
            }, [](float x, ObjectiveFunction f)->float {
                return 2 * x - 4;
            });
        if (!Caustic::IsEq(val, 4.0))
            return false;
        val = NewtonsMethod(5.0f, [](float x)->float {
            return x * x - 4 * x;
            }, NumericalDifferentiation);
        if (!Caustic::IsEq(val, 4.0))
            return false;
        return true;
    }

    static bool QuaternionTest()
    {
        Vector3 axis(0.0f, 1.0f, 0.0f);
        Quaternion q(&axis, Caustic::DegreesToRadians(45.0f));
        Vector3 pt(1.0f, 0.0f, 0.0f);
        pt = q.RotatePoint(pt);
        if (!IsZero(pt.x - 0.707106709f) ||
            !IsZero(pt.y) ||
            !IsZero(pt.z - 0.707106829f))
            return false;

        pt = Vector3(1.0f, 0.0f, 0.0f);
        axis = Vector3(0.0f, 1.0f, 0.0f);
        q = Quaternion(&axis, Caustic::DegreesToRadians(90.0f));
        pt = q.RotatePoint(pt);
        if (!IsZero(pt.x) ||
            !IsZero(pt.y) ||
            !IsZero(1.0f - pt.z))
            return false;
        return true;
    }

    bool MathTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { Test2, true },
            { Test3, true },
            { QuaternionTest, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("MathTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
