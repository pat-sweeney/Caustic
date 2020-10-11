//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Scheme.h"
#include "UnitTest.h"
#include <string>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CSchemeInterpreter interpreter;
        interpreter.Eval("(define add (+ 5 2))");
        return true;
    }

    bool SchemeTests::RunUnitTests()
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
                printf("SchemeTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
