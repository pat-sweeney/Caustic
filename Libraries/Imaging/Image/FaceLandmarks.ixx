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
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect/face.hpp"
#include "opencv2/objdetect/face.hpp"
#include "opencv2/face/facemark.hpp"

export module Imaging.Image.ImageFilter.FaceLandmarks;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;
import Imaging.Image.ImageFilter.FaceDetector;

export namespace Caustic
{
    //**********************************************************************
    // Class: CFaceLandmarksFilter
    // Image filter locates face landmarks using OpenCV
    //
    // Image Filter Parameters:
    // "Threshold" : <int> - Threshold for determining whether a pixel is an edge
    // "Boost"     : <int> - Amount to boost signal at edge pixels
    //**********************************************************************
    class CFaceLandmarksFilter : public IImageFilter, public CRefCount
    {
        cv::CascadeClassifier m_faceCascade;
        cv::Ptr<cv::face::Facemark> m_facemark;
    public:
        CFaceLandmarksFilter()
        {
            m_faceCascade.load("d:\\data\\haarcascade_frontalface_default.xml");
            m_facemark = cv::face::createFacemarkLBF();
            m_facemark->loadModel("d:\\github\\opencv\\data\\lbfmodel.yaml");
        }

        ~CFaceLandmarksFilter()
        {
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
    // Function: CreateFaceLandmarksFilter
    // Creates a filter for detecting face landmarks using OpenCV.
    //
    // Returns:
    // Returns the filter for performing landmark detection.
    //**********************************************************************
    CRefObj<IImageFilter> CreateFaceLandmarksFilter()
    {
        return CRefObj<IImageFilter>(new CFaceLandmarksFilter());
    }
}
