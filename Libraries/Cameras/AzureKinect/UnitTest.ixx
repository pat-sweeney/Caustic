//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Cameras.AzureKinect.UnitTests;
import Base.Core.UnitTests;

export namespace CausticTestSuite
{
    class AzureKinectTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
