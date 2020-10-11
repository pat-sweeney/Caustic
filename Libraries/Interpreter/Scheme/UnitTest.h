//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\UnitTests.h"

namespace CausticTestSuite
{
    class SchemeTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
