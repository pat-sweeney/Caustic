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
#include <chrono>

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
    using namespace chrono;

    CRefObj<IImage> CFaceLandmarksFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        auto spOutImage = pImage->Clone();
        ApplyInPlace(spOutImage, pParams);
        return spOutImage;
    }


    bool CFaceLandmarksFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        assert(pImage->GetImageType() == EImageType::BGR_24bpp || pImage->GetImageType() == EImageType::BGRA_32bpp || pImage->GetImageType() == EImageType::BGRX_32bpp);
        cv::Mat src(pImage->GetHeight(), pImage->GetWidth(), (pImage->GetImageType() == EImageType::BGR_24bpp) ? CV_8UC3 : CV_8UC4, pImage->GetData(), pImage->GetStride());

        bool outputImage = (bool)std::any_cast<bool>(pParams->params["outputImage"]);
        bool downsample = (bool)std::any_cast<bool>(pParams->params["downsample"]);

        std::vector<cv::Rect> faces;
        int grayW;
        int grayH;
        if (!downsample)
        {
            grayW = pImage->GetWidth();
            grayH = pImage->GetHeight();
        }
        else
        {
            grayW = 460;
            grayH = (pImage->GetHeight() * 460) / pImage->GetWidth();
        }
        cv::resize(src, src, cv::Size(grayW, grayH), 0, 0, cv::INTER_LINEAR_EXACT);
        cv::cvtColor(src, m_gray, (pImage->GetImageType() == EImageType::BGR_24bpp) ? cv::COLOR_BGR2GRAY : cv::COLOR_BGRA2GRAY);
        equalizeHist(m_gray, m_gray);
        cv::InputArray graySrc(m_gray);
        m_faceCascade.detectMultiScale(graySrc, faces, 1.1, 3, 0, cv::Size(30, 30));

        Vector2 p0, p1;
        std::vector<std::vector<cv::Point2f> > shapes;
        if (m_facemark->fit(src, faces, shapes))
        {
            if (outputImage)
                pImage->Clear();
            pParams->params.insert(std::make_pair("NumFaces", std::any(faces.size())));
            for (size_t i = 0; i < faces.size(); i++)
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
                sprintf_s(buf, "Face%d", (int)i);
                BBox2 bbox;
                bbox.minPt.x = (float)ptl.x;
                bbox.minPt.y = (float)ptl.y;
                bbox.maxPt.x = (float)pbr.x;
                bbox.maxPt.y = (float)pbr.y;
                pParams->params.insert(std::make_pair(buf, std::any(bbox)));

                if (outputImage)
                {
                    uint8 color[4] = { 255, 0, 0, 255 };
                    p0.x = (float)ptl.x; p0.y = (float)ptl.y; p1.x = (float)pbr.x; p1.y = (float)ptl.y;
                    pImage->DrawLine(p0, p1, color);
                    p0.x = (float)pbr.x; p0.y = (float)ptl.y; p1.x = (float)pbr.x; p1.y = (float)pbr.y;
                    pImage->DrawLine(p0, p1, color);
                    p0.x = (float)ptl.x; p0.y = (float)pbr.y; p1.x = (float)pbr.x; p1.y = (float)pbr.y;
                    pImage->DrawLine(p0, p1, color);
                    p0.x = (float)ptl.x; p0.y = (float)ptl.y; p1.x = (float)ptl.x; p1.y = (float)pbr.y;
                    pImage->DrawLine(p0, p1, color);
                }
            }
            for (unsigned long i = 0; i < faces.size(); i++)
            {
                char buf[1024];
                sprintf_s(buf, "Face%d_NumLandmarks", (int)i);
                pParams->params.insert(std::make_pair(buf, std::any(shapes[i].size())));
                for (unsigned long k = 0; k < shapes[i].size(); k++)
                {
                    cv::Point2f pt = shapes[i][k];
                    pt.x = std::floorf((pImage->GetWidth() - 1) * float(pt.x) / float(grayW - 1));
                    pt.y = std::floorf((pImage->GetHeight() - 1) * float(pt.y) / float(grayH - 1));
                    pt.x = (float)std::min<int>(std::max<int>((int)pt.x, 0), pImage->GetWidth() - 1);
                    pt.y = (float)std::min<int>(std::max<int>((int)pt.y, 0), pImage->GetHeight() - 1);
                    char buf[1024];
                    sprintf_s(buf, "Face%d_Point%d", i, k);
                    Vector2 q;
                    q.x = pt.x;
                    q.y = pt.y;
                    pParams->params.insert(std::make_pair(buf, std::any(q)));
                    
                    if (outputImage)
                    {
                        uint8 color[4] = { 255, 0, 0, 255 };
                        p0.x = pt.x; p0.y = pt.y;
                        pImage->DrawCircle(p0, 3, color);
                    }
                }
            }
        }
        return true;
    }
}
