//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Base\Core\Core.h"
#include "Geometry\Mesh\UnitTest.h"


int main()
{
    (new CausticTestSuite::MeshTests())->RunUnitTests();
    return 0;
}
