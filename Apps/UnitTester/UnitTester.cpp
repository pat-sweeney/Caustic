//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Base\Core\Core.h"
#include "Geometry\Mesh\UnitTest.h"
#include "Cameras\AzureKinect\UnitTest.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    Caustic::uint32 whichTests = 0x0;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (argv[i][0] == '-')
            {
                if (_stricmp(argv[i], "-all") == 0)
                {
                    whichTests = 0xFFFFFFFF;
                    break;
                }
                else if (_stricmp(argv[i], "-mesh") == 0)
                    whichTests |= 0x1;
                else if (_stricmp(argv[i], "-camera") == 0)
                    whichTests |= 0x2;
            }
        }
    }
    if (whichTests & 0x1)
        (new CausticTestSuite::MeshTests())->RunUnitTests();
    if (whichTests & 0x2)
        (new CausticTestSuite::AzureKinectTests())->RunUnitTests();
    return 0;
}
