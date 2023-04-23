//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Base.Math.UnitTests;
import Base.Core.UnitTests;

export namespace CausticTestSuite
{
    class MathTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
