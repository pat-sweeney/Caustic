//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Vector.h"
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        Vector3 v0(1.0f, 0.0f, 0.0f);
        Vector3 v1(0.0f, 1.0f, 0.0f);
        Vector3 r = v0.cross(v1).Normalize();
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

    bool MathTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { Test2, true }
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
