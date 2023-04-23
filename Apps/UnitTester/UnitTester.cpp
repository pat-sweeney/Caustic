//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Core.UnitTests;
import Geometry.Mesh.UnitTests;
import Cameras.AzureKinect.UnitTests;
import Imaging.Image.UnitTests;
import Audio.AudioPlayback.UnitTests;
import Base.Math.UnitTests;
import Rendering.RenderWindow.UnitTests;
import Parsers.Lex.UnitTests;
import Rendering.SceneImport.UnitTests;
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
                else if (_stricmp(argv[i], "-imaging") == 0)
                    whichTests |= 0x4;
                else if (_stricmp(argv[i], "-math") == 0)
                    whichTests |= 0x8;
                else if (_stricmp(argv[i], "-renderwindow") == 0)
                    whichTests |= 0x10;
                else if (_stricmp(argv[i], "-lex") == 0)
                    whichTests |= 0x20;
#ifdef SUPPORT_SCHEME
                else if (_stricmp(argv[i], "-scheme") == 0)
                    whichTests |= 0x40;
#endif // SUPPORT_SCHEME
                else if (_stricmp(argv[i], "-sceneimport") == 0)
                    whichTests |= 0x80;
                else if (_stricmp(argv[i], "-audio") == 0)
                    whichTests |= 0x100;
            }
        }
    }
    if (whichTests & 0x1)
        (new CausticTestSuite::MeshTests())->RunUnitTests();
    if (whichTests & 0x2)
        (new CausticTestSuite::AzureKinectTests())->RunUnitTests();
    if (whichTests & 0x4)
        (new CausticTestSuite::ImageTests())->RunUnitTests();
    if (whichTests & 0x8)
        (new CausticTestSuite::MathTests())->RunUnitTests();
    if (whichTests & 0x10)
        (new CausticTestSuite::RenderWindowTests())->RunUnitTests();
    if (whichTests & 0x20)
        (new CausticTestSuite::LexTests())->RunUnitTests();
#ifdef SUPPORT_SCHEME
    if (whichTests & 0x40)
        (new CausticTestSuite::SchemeTests())->RunUnitTests();
#endif // SUPPORT_SCHEME
    if (whichTests & 0x80)
        (new CausticTestSuite::SceneImportTests())->RunUnitTests();
    if (whichTests & 0x100)
        (new CausticTestSuite::AudioPlaybackTests())->RunUnitTests();
    return 0;
}
