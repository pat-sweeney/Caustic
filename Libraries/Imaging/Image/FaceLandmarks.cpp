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

module Imaging.Image.ImageFilter.FaceLandmarks;
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
    using namespace std;
    using namespace cv;

    CRefObj<IImage> CFaceLandmarksFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        auto spOutImage = pImage->Clone();
        ApplyInPlace(spOutImage, pParams);
        return spOutImage;
    }

    bool CFaceLandmarksFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
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

        Vector2 p0, p1;
        std::vector<std::vector<cv::Point2f> > shapes;
        if (m_facemark->fit(src, faces, shapes))
        {
            for (size_t i = 0; i < faces.size(); i++)
            {
                Vector2 p0, p1;
                cv::Point2d ptl = faces[i].tl();
                cv::Point2d pbr = faces[i].br();
                ptl.x = (int)(pImage->GetWidth() * float(ptl.x) / grayW);
                ptl.y = (int)(pImage->GetHeight() * float(ptl.y) / grayH);
                pbr.x = (int)(pImage->GetWidth() * float(pbr.x) / grayW);
                pbr.y = (int)(pImage->GetHeight() * float(pbr.y) / grayH);
                ptl.x = std::min<int>(std::max<int>(ptl.x, 0), pImage->GetWidth() - 1);
                ptl.y = std::min<int>(std::max<int>(ptl.y, 0), pImage->GetHeight() - 1);
                pbr.x = std::min<int>(std::max<int>(pbr.x, 0), pImage->GetWidth() - 1);
                pbr.y = std::min<int>(std::max<int>(pbr.y, 0), pImage->GetHeight() - 1);
                char buf[1024];
                sprintf_s(buf, "Face%d", i);
                BBox2 bbox;
                bbox.minPt.x = ptl.x;
                bbox.minPt.y = ptl.y;
                bbox.maxPt.x = pbr.x;
                bbox.maxPt.y = pbr.y;
                pParams->params.insert(std::make_pair(buf, std::any(bbox)));

                uint8 color[4] = { 255, 0, 0, 255 };
                p0.x = ptl.x; p0.y = ptl.y; p1.x = pbr.x; p1.y = ptl.y;
                pImage->DrawLine(p0, p1, color);
                p0.x = pbr.x; p0.y = ptl.y; p1.x = pbr.x; p1.y = pbr.y;
                pImage->DrawLine(p0, p1, color);
                p0.x = ptl.x; p0.y = pbr.y; p1.x = pbr.x; p1.y = pbr.y;
                pImage->DrawLine(p0, p1, color);
                p0.x = ptl.x; p0.y = ptl.y; p1.x = ptl.x; p1.y = pbr.y;
                pImage->DrawLine(p0, p1, color);
            }
            for (unsigned long i = 0; i < faces.size(); i++)
            {
                for (unsigned long k = 0; k < shapes[i].size(); k++)
                {
                    cv::Point2f pt = shapes[i][k];
                    pt.x = (int)(pImage->GetWidth() * float(pt.x) / grayW);
                    pt.y = (int)(pImage->GetHeight() * float(pt.y) / grayH);
                    pt.x = std::min<int>(std::max<int>(pt.x, 0), pImage->GetWidth() - 1);
                    pt.y = std::min<int>(std::max<int>(pt.y, 0), pImage->GetHeight() - 1);
                    char buf[1024];
                    sprintf_s(buf, "Face%dPoint%d", i, k);
                    Vector2 q;
                    q.x = pt.x;
                    q.y = pt.y;
                    pParams->params.insert(std::make_pair(buf, std::any(q)));

                    uint8 color[4] = { 255, 0, 0, 255 };
                    p0.x = pt.x; p0.y = pt.y;
                    pImage->DrawCircle(p0, 3, color);
                }
            }
        }
        return true;
    }
}
