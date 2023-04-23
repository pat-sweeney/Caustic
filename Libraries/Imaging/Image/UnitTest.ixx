//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Imaging.Image.UnitTests;
import Base.Core.UnitTests;

export namespace CausticTestSuite
{
    class ImageTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
