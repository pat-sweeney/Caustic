//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "Base\Core\Core.h"
#include "Geometry\Mesh\UnitTest.h"


int main()
{
    (new CausticTestSuite::MeshTests())->RunUnitTests();
    return 0;
}
