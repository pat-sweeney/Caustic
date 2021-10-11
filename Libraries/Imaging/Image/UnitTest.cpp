//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Base\Math\Vector.h"
#include "Base\Core\error.h"
#include "Image.h"
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IImage> spImage = CreateImage(512, 512, 8);
        return true;
    }

    static bool Test2()
    {
        CRefObj<IImage> spImage = CreateImage(512, 512, 32);
        CRefObj<IPath2> spPath = CreatePath2();
        spPath->AppendPathElems(
            EPathElem::MoveTo, 50.0, 50.0,
            EPathElem::CurveTo, 200.0, 50.0, 200.0, 100.0, 50.0, 100.0,
            EPathElem::Null);
        CRefObj<IPath2> spFlatPath = spPath->Flatten(1.0f);
        spImage->DrawPath(spFlatPath);
        StoreImage(L"c:\\temp\\Curve.jpg", spImage);
        return true;
    }

    bool ImageTests::RunUnitTests()
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
                printf("ImageTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
