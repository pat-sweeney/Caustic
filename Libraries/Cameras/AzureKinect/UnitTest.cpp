//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Cameras.AzureKinect.AzureKinect;
import Cameras.AzureKinect.IAzureKinect;

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IAzureKinect> spAzureKinect = AzureKinect::CreateAzureKinect(0, AzureKinect::Color1080p, AzureKinect::Depth1024x1024, AzureKinect::FPS30);
        return true;
    }

    static bool Test2()
    {
        CRefObj<IAzureKinect> spAzureKinect = AzureKinect::CreateAzureKinect(0, AzureKinect::Color1080p, AzureKinect::Depth512x512, AzureKinect::FPS30);
        return true;
    }

    bool AzureKinectTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { Test2, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("MathTestSuite: Test %d \n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
