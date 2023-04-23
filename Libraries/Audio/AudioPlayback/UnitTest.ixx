//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Audio.AudioPlayback.UnitTests;
import Base.Core.UnitTests;

export namespace CausticTestSuite
{
    class AudioPlaybackTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
