//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define _USE_MATH_DEFINES
#include <shlwapi.h>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

module Cameras.VirtualCamera.UnitTests;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        return true;
    }

    bool VirtualCameraTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("VirtualCameraTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
