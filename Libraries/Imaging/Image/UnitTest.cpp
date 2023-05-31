//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <any>
#include <chrono>
#include <stdio.h>

module Imaging.Image.UnitTests;
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
import Imaging.Image.ImageFilter.FaceDetector;
import Imaging.Image.ImageFilter.FaceLandmarks;

using namespace std::chrono;
using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        CRefObj<IImage> spImage = CreateImage(512, 512, EImageType::Gray_8bpp);
        return true;
    }

    static bool Test2()
    {
        CRefObj<IImage> spImage = CreateImage(512, 512, EImageType::RGBA_32bpp);
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

    static bool Test5()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"c:\\temp\\imageIn.png");

        ImageFilterParams params2;
        params2.params["DestFormat"] = std::any(EImageType::RGB_24bpp);
        CConvertFilter cf;
        CRefObj<IImage> pConvertedImage = cf.Apply(pImage, &params2);

        {
            ImageFilterParams params;
            params.params["Degrees"] = std::any((float)90.0f);
            CRotateFilter* filter = new CRotateFilter();
            CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
            StoreImage(L"c:\\temp\\Rotated90.png", pOutImage);
        }

        {
            ImageFilterParams params;
            params.params["Degrees"] = std::any((float)180.0f);
            CRotateFilter* filter = new CRotateFilter();
            CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
            StoreImage(L"c:\\temp\\Rotated180.png", pOutImage);
        }

        {
            ImageFilterParams params;
            params.params["Degrees"] = std::any((float)270.0f);
            CRotateFilter* filter = new CRotateFilter();
            CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
            StoreImage(L"c:\\temp\\Rotated270.png", pOutImage);
        }

        {
            ImageFilterParams params;
            params.params["Degrees"] = std::any((float)45.0f);
            CRotateFilter* filter = new CRotateFilter();
            CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
            StoreImage(L"c:\\temp\\Rotated45.png", pOutImage);
        }
        return true;
    }

    static bool Test6()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"c:\\temp\\imageIn.png");

        ImageFilterParams params2;
        params2.params["DestFormat"] = std::any(EImageType::RGB_24bpp);
        CConvertFilter cf;
        CRefObj<IImage> pConvertedImage = cf.Apply(pImage, &params2);

        ImageFilterParams params;
        Matrix3x2 tm(1.0f, 0.2f, 0.3f, 2.0f, 0.0f, 0.0f);
        params.params["Matrix"] = std::any((Matrix3x2)tm);
        CTransformFilter* filter = new CTransformFilter();
        CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
        StoreImage(L"c:\\temp\\xform24.png", pOutImage);
        return true;
    }

    static bool Test7()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"c:\\temp\\imageIn.png");

        ImageFilterParams params2;
        params2.params["DestFormat"] = std::any(EImageType::RGBA_32bpp);
        CConvertFilter cf;
        CRefObj<IImage> pConvertedImage = cf.Apply(pImage, &params2);

        ImageFilterParams params;
        Matrix3x2 tm(1.0f, 0.2f, 0.3f, 2.0f, 0.0f, 0.0f);
        params.params["Matrix"] = std::any((Matrix3x2)tm);
        CTransformFilter* filter = new CTransformFilter();
        CRefObj<IImage> pOutImage = filter->Apply(pConvertedImage, &params);
        StoreImage(L"c:\\temp\\xform32.png", pOutImage);
        return true;
    }

    static bool Test8()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"d:\\data\\Face.jpg");
        CFaceDetectorFilter cf;
        ImageFilterParams params;
        params.params.insert(std::make_pair("outputImage", std::any(true)));
        params.params.insert(std::make_pair("detectEyes", std::any(true)));
        auto i = cf.Apply(pImage, &params);
        StoreImage(L"d:\\data\\FaceMarked.png", i);
        return true;
    }

    static bool Test9()
    {
        CRefObj<IImage> pImage = LoadImageFile(L"d:\\data\\Face.jpg");
        CFaceLandmarksFilter cf;
        ImageFilterParams params;
        std::any outputImage(false);
        params.params.insert(std::make_pair("outputImage", outputImage));
        auto start = high_resolution_clock::now();
        auto i = cf.Apply(pImage, &params);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        printf("Duration:%d\n", (int)duration.count());
        StoreImage(L"d:\\data\\FaceLandmarks.png", i);
        return true;
    }

    static bool Test10()
    {
    }

    bool ImageTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            //{ Test1, true },
            //{ Test2, true },
            //{ Test3, true },
            //{ Test4, true },
            //{ Test5, true },
            //{ Test6, true },
            //{ Test7, true },
            { Test8, true },
            { Test9, true },
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
