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
    // Face landmarks are numbered as shown below. This ordering was taken from:
    // https://www.researchgate.net/figure/The-indexes-of-the-68-coordinates-corresponding-to-the-facial-landmarks-selected_fig2_329739701
    // 
    //          19 20 21                    24 25 26
    //       18          22               23        27
    // 
    //                38 39      28         44 45
    //             37       40           43       46
    //                42 41      29         48 47          
    //                           30                       
    //                           31                     
    //                      32        36              
    //                        33 34 35             
    //                                          
    //                                      
    //                      51 52 53 
    //                   50 62 63 64 54
    //  1             49 61         65 55                17
    //   2               60 68 67 66 56                 16
    //    3                 59 58 57                  15
    //     4                                        14
    //       5                                   13
    //         6                             12
    //             7                    11
    //                8            10
    //                      9
    //**********************************************************************
    const int c_FaceLandmark_TipNose = 34;
    const int c_FaceLandmark_NoseBridge_Top = 28;
    const int c_FaceLandmark_NoseBridge_Bottom = 31;
    const int c_FaceLandmark_OuterMouth_RightCorner = 49;
    const int c_FaceLandmark_OuterMouth_LeftCorner = 55;
    const int c_FaceLandmark_OuterMouth_TopMiddle = 52;
    const int c_FaceLandmark_OuterMouth_BottomMiddle = 58;
    const int c_FaceLandmark_InnerMouth_RightCorner = 61;
    const int c_FaceLandmark_InnerMouth_LeftCorner = 65;
    const int c_FaceLandmark_InnerMouth_TopMiddle = 63;
    const int c_FaceLandmark_InnerMouth_BottomMiddle = 67;
    const int c_FaceLandmark_RightEar = 1;
    const int c_FaceLandmark_LeftEar = 17;
    const int c_FaceLandmark_Chin = 9;
    const int c_FaceLandmark_RightEyebrow_RightSide = 18;
    const int c_FaceLandmark_RightEyebrow_LeftSide = 22;
    const int c_FaceLandmark_LeftEyebrow_RightSide = 23;
    const int c_FaceLandmark_LeftEyebrow_LeftSide = 27;
    const int c_FaceLandmark_RightEye_RightSide = 37;
    const int c_FaceLandmark_RightEye_LeftSide = 40;
    const int c_FaceLandmark_LeftEye_RightSide = 43;
    const int c_FaceLandmark_LeftEye_LeftSide = 46;

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
        cv::Mat m_gray;
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
