//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Core.h"

namespace CausticTestSuite
{
    struct UnitTestEntry
    {
        bool(*testFunc)();
        bool enabled;
    };
    
    class UnitTestSuite
    {
    protected:
        Caustic::uint32 m_totalTests;
        Caustic::uint32 m_testsRan;
        Caustic::uint32 m_testsPassed;
    public:
        UnitTestSuite() :
            m_totalTests(0),
            m_testsRan(0),
            m_testsPassed(0)
        {
        }

        Caustic::uint32 TotalTests() { return m_totalTests; }
        Caustic::uint32 TestsRan() { return m_testsRan; }
        Caustic::uint32 TestsPassed() { return m_testsPassed; }
        virtual bool RunUnitTests() = 0;
    };
}
