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
#include <stdio.h>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

module Imaging.Image.ImageFilter.FaceDetector;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.BBox;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Color;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

namespace Caustic
{
    CRefObj<IImage> CFaceDetectorFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        auto spMarkedImage = pImage->Clone();
        ApplyInPlace(spMarkedImage, pParams);
        return spMarkedImage;
    }

    bool CFaceDetectorFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        assert(pImage->GetImageType() == EImageType::BGR_24bpp || pImage->GetImageType() == EImageType::BGRA_32bpp);
        cv::Mat src(pImage->GetHeight(), pImage->GetWidth(), (pImage->GetImageType() == EImageType::BGR_24bpp) ? CV_8UC3 : CV_8UC4, pImage->GetData(), pImage->GetStride());

        std::vector<cv::Rect> faces;
        int grayW = 460;
        int grayH = (pImage->GetHeight() * 460) / pImage->GetWidth();
        cv::resize(src, src, cv::Size(grayW, grayH), 0, 0, cv::INTER_LINEAR_EXACT);
        cv::Mat gray;
        cv::cvtColor(src, gray, (pImage->GetImageType() == EImageType::BGR_24bpp) ? cv::COLOR_BGR2GRAY : cv::COLOR_BGRA2GRAY);
        equalizeHist(gray, gray);
        cv::InputArray graySrc(gray);
        m_faceCascade.detectMultiScale(graySrc, faces, 1.1, 3, 0, cv::Size(30, 30));
        int numFaces = (int)faces.size();
        pParams->params.insert(std::make_pair("NumFaces", std::any(numFaces)));
        bool outputImage = (bool)std::any_cast<bool>(pParams->params["outputImage"]);
        bool detectEyes = (bool)std::any_cast<bool>(pParams->params["detectEyes"]);
        for (int i = 0; i < (int)faces.size(); i++)
        {
            Vector2 p0, p1;
            cv::Point2d ptl = faces[i].tl();
            cv::Point2d pbr = faces[i].br();
            ptl.x = (int)(pImage->GetWidth() * float(ptl.x) / grayW);
            ptl.y = (int)(pImage->GetHeight() * float(ptl.y) / grayH);
            pbr.x = (int)(pImage->GetWidth() * float(pbr.x) / grayW);
            pbr.y = (int)(pImage->GetHeight() * float(pbr.y) / grayH);
            ptl.x = std::min<int>(std::max<int>((int)ptl.x, 0), pImage->GetWidth() - 1);
            ptl.y = std::min<int>(std::max<int>((int)ptl.y, 0), pImage->GetHeight() - 1);
            pbr.x = std::min<int>(std::max<int>((int)pbr.x, 0), pImage->GetWidth() - 1);
            pbr.y = std::min<int>(std::max<int>((int)pbr.y, 0), pImage->GetHeight() - 1);
            char buf[1024];
            sprintf_s(buf, "Face%d", i);
            BBox2 bbox;
            bbox.minPt.x = (float)ptl.x;
            bbox.minPt.y = (float)ptl.y;
            bbox.maxPt.x = (float)pbr.x;
            bbox.maxPt.y = (float)pbr.y;
            pParams->params.insert(std::make_pair(buf, std::any(bbox)));

            if (outputImage)
            {
                uint8_t color[4] = { 255, 0, 0, 255 };
                p0.x = (float)ptl.x; p0.y = (float)ptl.y; p1.x = (float)pbr.x; p1.y = (float)ptl.y;
                pImage->DrawLine(p0, p1, color);
                p0.x = (float)pbr.x; p0.y = (float)ptl.y; p1.x = (float)pbr.x; p1.y = (float)pbr.y;
                pImage->DrawLine(p0, p1, color);
                p0.x = (float)ptl.x; p0.y = (float)pbr.y; p1.x = (float)pbr.x; p1.y = (float)pbr.y;
                pImage->DrawLine(p0, p1, color);
                p0.x = (float)ptl.x; p0.y = (float)ptl.y; p1.x = (float)ptl.x; p1.y = (float)pbr.y;
                pImage->DrawLine(p0, p1, color);
            }

            if (detectEyes)
            {
                cv::Mat eyeROI = gray(faces[i]);
                std::vector<cv::Rect> eyes;
                m_eyeCascade.detectMultiScale(gray, eyes);
                for (int j = 0; j < (int)eyes.size(); j++)
                {
                    Vector2 pt;
                    cv::Point2d ptl = eyes[j].tl();
                    cv::Point2d pbr = eyes[j].br();
                    ptl.x = (int)(pImage->GetWidth() * float(ptl.x) / grayW);
                    ptl.y = (int)(pImage->GetHeight() * float(ptl.y) / grayH);
                    pbr.x = (int)(pImage->GetWidth() * float(pbr.x) / grayW);
                    pbr.y = (int)(pImage->GetHeight() * float(pbr.y) / grayH);
                    pt.x = float(ptl.x + pbr.x) / 2.0f;
                    pt.y = float(ptl.y + pbr.y) / 2.0f;
                    if (outputImage)
                    {
                        uint8_t color[4] = { 0, 0, 255, 255 };
                        pImage->DrawCircle(pt, 5, color);
                    }
                }
            }
        }
        return true;
    }
}
