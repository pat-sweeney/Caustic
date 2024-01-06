//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <cinttypes>

export module Base.Core.UnitTests;
import Base.Core.Core;

export namespace CausticTestSuite
{
    struct UnitTestEntry
    {
        bool(*testFunc)();
        bool enabled;
    };
    
    class UnitTestSuite
    {
    protected:
        uint32_t m_totalTests;
        uint32_t m_testsRan;
        uint32_t m_testsPassed;
    public:
        UnitTestSuite() :
            m_totalTests(0),
            m_testsRan(0),
            m_testsPassed(0)
        {
        }

        uint32_t TotalTests() { return m_totalTests; }
        uint32_t TestsRan() { return m_testsRan; }
        uint32_t TestsPassed() { return m_testsPassed; }
        virtual bool RunUnitTests() = 0;
    };
}
