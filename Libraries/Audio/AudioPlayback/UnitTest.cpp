//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#define _USE_MATH_DEFINES
#include <cmath>
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.Error;
import Audio.AudioPlayback.IAudioPlayback;
#include "UnitTest.h"

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IAudioPlayback> spPlayback = CreateAudioPlayback(48000, 16, 2);
        spPlayback->PlayTone(20000);
        return true;
    }
    
    bool AudioPlaybackTests::RunUnitTests()
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
                printf("AudioPlaybackTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
