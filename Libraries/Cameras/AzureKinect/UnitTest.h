//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\UnitTests.h"

namespace CausticTestSuite
{
    class AzureKinectTests : public UnitTestSuite
    {
    public:
        virtual bool RunUnitTests() override;
    };
};
