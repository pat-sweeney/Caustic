//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "pch.h"
#include "Base\Core\error.h"
#include "Parsers\Lex\ILex.h"
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<ILex> spLex = CreateLex("", false);
        while (spLex->ReadToken().m_id != c_LexToken_EOF)
            ;
        return true;
    }

    static bool Test2()
    {
        CRefObj<ILex> spLex = CreateLex("", true);
        while (spLex->ReadToken().m_id != c_LexToken_EOF)
            ;
        return true;
    }

    static bool Test3()
    {
        CRefObj<ILex> spLex = CreateLex("now is the time \"For all good men\" 'Test it' 3.4 -4 ; +5 // this is a comment", false);
        struct expectedTokens
        {
            const char* s;
            int id;
        } _ExpectedTokens[] = {
            { "now", c_LexToken_Identifier },
            { "is", c_LexToken_Identifier },
            { "the", c_LexToken_Identifier },
            { "time", c_LexToken_Identifier },
            { "For all good men", c_LexToken_String },
            { "Test it", c_LexToken_String },
            { "3.4", c_LexToken_Float },
            { "-4", c_LexToken_Integer },
            { ";", c_LexToken_Semicolon },
            { "+5", c_LexToken_Integer },
            { "// this is a comment", c_LexToken_Comment },
            { "", c_LexToken_EOF },
        };
        int i = 0;
        while (true)
        {
            LexToken lt = spLex->ReadToken();
            if (lt.m_id != _ExpectedTokens[i].id)
                return false;
            if (_ExpectedTokens[i].id == c_LexToken_EOF)
                break;
            i++;
        }
        return true;
    }

    static bool Test4()
    {
        ParseTableEntry customTokens[] = {
            {"Foobar", c_LexToken_Last + 1},
            {nullptr, 0}
        };
        CRefObj<ILex> spLex = CreateLex("now Foobar is", false);
        spLex->SetParseTable(customTokens);
        struct expectedTokens
        {
            const char* s;
            int id;
        } _ExpectedTokens[] = {
            { "now", c_LexToken_Identifier },
            { "Foobar", c_LexToken_Last + 1 },
            { "is", c_LexToken_Identifier },
            { "", c_LexToken_EOF },
        };
        int i = 0;
        while (true)
        {
            LexToken lt = spLex->ReadToken();
            if (lt.m_id != _ExpectedTokens[i].id)
                return false;
            if (_ExpectedTokens[i].id == c_LexToken_EOF)
                break;
            i++;
        }
        return true;
    }

    bool LexTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { Test2, true },
            { Test3, true },
            { Test4, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("LexTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
