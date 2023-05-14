//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Cameras.VirtualCamera.UnitTests;
import Base.Core.UnitTests;

export namespace CausticTestSuite
{
    class VirtualCameraTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
