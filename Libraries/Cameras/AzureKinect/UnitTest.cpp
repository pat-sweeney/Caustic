//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#include "Base\Core\error.h"
#include "Cameras\AzureKinect\IAzureKinect.h"
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IAzureKinect> spAzureKinect;
        AzureKinect::CreateAzureKinect(0, AzureKinect::Color1080p, AzureKinect::Depth1024x1024, AzureKinect::FPS30, &spAzureKinect);
        return true;
    }

    static bool Test2()
    {
        CRefObj<IAzureKinect> spAzureKinect;
        AzureKinect::CreateAzureKinect(0, AzureKinect::Color1080p, AzureKinect::Depth512x512, AzureKinect::FPS30, &spAzureKinect);
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
