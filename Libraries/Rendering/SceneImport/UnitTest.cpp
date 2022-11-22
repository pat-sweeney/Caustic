//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#define _USE_MATH_DEFINES
#include <cmath>
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.Error;
import Base.Math.Matrix.Matrix3x2;
import Base.Math.Vector;
import Geometry.GeomDS.Path2;
import Imaging.Image.IImage;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.Scale;
import Imaging.Image.ImageFilter.Transform;
import Imaging.Image.ImageFilter.Rotate;
import Imaging.Image.ImageFilter.Convert;
import Rendering.SceneImport.Json;
import Base.Core.ConvertStr;
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <any>
#include <memory>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        std::unique_ptr<CJsonImporter> spImporter(new CJsonImporter());
        std::wstring wstr = ExpandEnvironmentVariables(L"$(CausticRoot)\\UnitTestData\\TestScene.json");
        CRefObj<ISceneGraph> spSceneGraph = spImporter->Import(wstr);
        return true;
    }

    bool SceneImportTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            bool succeeded = false;
            try {
                succeeded = (*tests[i].testFunc)();
            }
            catch (...) { }
            if (!succeeded)
            {
                printf("SceneImportTests: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
