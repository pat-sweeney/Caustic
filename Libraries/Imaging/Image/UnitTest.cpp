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
import Base.Math.Vector;
import Geometry.GeomDS.Path2;
import Imaging.Image.IImage;
import Imaging.Image.ImageFilter.Scale;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.Convert;
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <any>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IImage> spImage = CreateImage(512, 512, 8);
        return true;
    }

    static bool Test2()
    {
        CRefObj<IImage> spImage = CreateImage(512, 512, 32);
        CRefObj<IPath2> spPath = CPath2::CreatePath2();
        spPath->AppendPathElems(
            EPathElem::MoveTo, 50.0, 50.0,
            EPathElem::CurveTo, 200.0, 50.0, 200.0, 100.0, 50.0, 100.0,
            EPathElem::Null);
        CRefObj<IPath2> spFlatPath = spPath->Flatten(1.0f, nullptr);
        spImage->DrawPath(spFlatPath);
        StoreImage(L"c:\\temp\\Curve.jpg", spImage);
        return true;
    }

    static bool Test3()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"c:\\temp\\imageIn.png");

        ImageFilterParams params2;
        params2.params["DestFormat"] = std::any(EImageType::RGB_24bpp);
        CConvertFilter cf;
        CRefObj<IImage> pConvertedImage = cf.Apply(pImage, &params2);

        ImageFilterParams params;
        params.params["ScaleX"] = std::any((float)2.0f);
        params.params["ScaleY"] = std::any((float)2.0f);
        params.params["ScaleUpMode"] = std::any(EScaleUpMode::Bilinear);
        CScaleFilter* filter = new CScaleFilter();
        StoreImage(L"c:\\temp\\convimg.png", pConvertedImage);
        CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
        StoreImage(L"c:\\temp\\imageOut.png", pOutImage);
        return true;
    }

    static bool Test4()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"c:\\temp\\imageIn.png");

        ImageFilterParams params2;
        params2.params["DestFormat"] = std::any(EImageType::RGB_24bpp);
        CConvertFilter cf;
        CRefObj<IImage> pConvertedImage = cf.Apply(pImage, &params2);

        ImageFilterParams params;
        params.params["ScaleX"] = std::any((float)0.1f);
        params.params["ScaleY"] = std::any((float)0.1f);
        params.params["ScaleDownMode"] = std::any(EScaleDownMode::Average);
        CScaleFilter* filter = new CScaleFilter();
        StoreImage(L"c:\\temp\\convimg.png", pConvertedImage);
        CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
        StoreImage(L"c:\\temp\\imageOut.png", pOutImage);
        return true;
    }

    bool ImageTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { Test2, true },
            { Test3, true },
            { Test4, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("ImageTestSuite: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
