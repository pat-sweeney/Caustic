//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Rendering.RenderWindow.UnitTests;
import Base.Core.UnitTests;

export namespace CausticTestSuite
{
    class RenderWindowTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
