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
    //          18 19 20                    23 24 25
    //       17          21               22        26
    // 
    //                37 38      27         43 44
    //             36       39           42       45
    //                41 40      28         47 46          
    //                           29                       
    //                           30                     
    //                      31        35              
    //                        32 33 34             
    //                                          
    //                                      
    //                      50 51 52 
    //                   49 61 62 63 53
    //  0             48 60         64 54                16
    //   1               59 67 66 65 55                 15
    //    2                 58 57 56                  14
    //     3                                        13
    //       4                                   12
    //         5                             11
    //             6                    10
    //                7            9
    //                      8
    //**********************************************************************
    const int c_FaceLandmark_TipNose = 33;
    const int c_FaceLandmark_NoseBridge_Top = 27;
    const int c_FaceLandmark_NoseBridge_Bottom = 30;
    const int c_FaceLandmark_Mouth_FirstIndex = 48;
    const int c_FaceLandmark_Mouth_LastIndex = 67;
    const int c_FaceLandmark_OuterMouth_RightCorner = 48;
    const int c_FaceLandmark_OuterMouth_LeftCorner = 54;
    const int c_FaceLandmark_OuterMouth_TopMiddle = 51;
    const int c_FaceLandmark_OuterMouth_BottomMiddle = 57;
    const int c_FaceLandmark_InnerMouth_FirstIndex = 60;
    const int c_FaceLandmark_InnerMouth_LastIndex = 67;
    const int c_FaceLandmark_InnerMouth_RightCorner = 60;
    const int c_FaceLandmark_InnerMouth_LeftCorner = 64;
    const int c_FaceLandmark_InnerMouth_TopMiddle = 62;
    const int c_FaceLandmark_InnerMouth_BottomMiddle = 66;
    const int c_FaceLandmark_RightEar = 0;
    const int c_FaceLandmark_LeftEar = 16;
    const int c_FaceLandmark_Chin = 8;
    const int c_FaceLandmark_RightEyebrow_RightSide = 17;
    const int c_FaceLandmark_RightEyebrow_LeftSide = 21;
    const int c_FaceLandmark_LeftEyebrow_RightSide = 22;
    const int c_FaceLandmark_LeftEyebrow_LeftSide = 26;
    const int c_FaceLandmark_RightEye_RightSide = 36;
    const int c_FaceLandmark_RightEye_LeftSide = 39;
    const int c_FaceLandmark_LeftEye_RightSide = 42;
    const int c_FaceLandmark_LeftEye_LeftSide = 45;

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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

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
