//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <any>
#include <string>
#include <memory>
#include <map>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

export module Imaging.Image.ImageFilter.FaceDetector;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

using namespace cv;
using namespace std;

export namespace Caustic
{
    //**********************************************************************
    // Class: CFaceDetectorFilter
    // Image filter locates face using OpenCV
    //
    // Image Filter Parameters:
    // "Threshold" : <int> - Threshold for determining whether a pixel is an edge
    // "Boost"     : <int> - Amount to boost signal at edge pixels
    //**********************************************************************
    class CFaceDetectorFilter : public IImageFilter, public CRefCount
    {
        cv::CascadeClassifier m_faceCascade;
    public:
        CFaceDetectorFilter()
        {
            m_faceCascade.load("d:\\data\\haarcascade_frontalface_default.xml");
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IImageFilter
        //**********************************************************************
        virtual CRefObj<IImage> Apply(IImage* pImage, ImageFilterParams* pParams) override;
        virtual bool ApplyInPlace(IImage* pImage, ImageFilterParams* pParams) override;
    };

    //**********************************************************************
    // Function: CreateFaceDetectorFilter
    // Creates a filter for detecting faces using OpenCV.
    //
    // Returns:
    // Returns the filter for performing face detection.
    //**********************************************************************
    CRefObj<IImageFilter> CreateFaceDetectorFilter()
    {
        return CRefObj<IImageFilter>(new CFaceDetectorFilter());
    }
}
