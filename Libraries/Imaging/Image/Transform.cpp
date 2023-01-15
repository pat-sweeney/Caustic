//**********************************************************************
// Copyright Patrick Sweeney 2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <math.h>
#include <any>
#include <string>
#include <map>
#include <windows.h>

module Imaging.Image.ImageFilter.Transform;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Math.Matrix.Matrix3x2;
import Base.Math.Vector;
import Base.Math.BBox;
import Imaging.Image.IImage;
import Imaging.Image.ImageIter;
import Imaging.Image.Image;
import Imaging.Image.ImageFilter;

namespace Caustic
{
    template<typename Iter>
    CRefObj<IImage> CTransformFilter::Transform(IImage* pImage, Matrix3x2& tm)
    {
        //
        // First transform bounding box
        //
        Vector2 bbox[4];
        bbox[0] = Vector2(0.0f, 0.0f);
        bbox[1] = Vector2((float)pImage->GetWidth() - 1.0f, 0.0f);
        bbox[2] = Vector2((float)pImage->GetWidth() - 1.0f, (float)pImage->GetHeight() - 1.0f);
        bbox[3] = Vector2(0.0f, (float)pImage->GetHeight() - 1.0f);
        bbox[0] = bbox[0] * tm;
        bbox[1] = bbox[1] * tm;
        bbox[2] = bbox[2] * tm;
        bbox[3] = bbox[3] * tm;
        
        //
        // Determine destination image's bounding box
        //
        BBox2 dstbbox;
        dstbbox.AddPoint(bbox[0].x, bbox[0].y);
        dstbbox.AddPoint(bbox[1].x, bbox[1].y);
        dstbbox.AddPoint(bbox[2].x, bbox[2].y);
        dstbbox.AddPoint(bbox[3].x, bbox[3].y);
        
        //
        // Determine destination image width and height
        //
        int dstw = (int)(dstbbox.maxPt.x - dstbbox.minPt.x + 1.5f);
        int dsth = (int)(dstbbox.maxPt.y - dstbbox.minPt.y + 1.5f);
        
        //
        // Allocate destination image
        //
        CRefObj<IImage> spXformImage = CreateImageImpl(dstw, dsth, pImage->GetImageType());
        
        //
        // Transform destination image bbox back into source image space
        //
        Matrix3x2 itm = tm;
        if (!itm.Inverse())
            CT(E_FAIL); // transform doesn't have an inverse

        bbox[0] = dstbbox.minPt;
        bbox[1] = Vector2(dstbbox.maxPt.x, dstbbox.minPt.y);
        bbox[2] = dstbbox.maxPt;
        bbox[3] = Vector2(dstbbox.minPt.x, dstbbox.maxPt.y);
        bbox[0] = bbox[0] * itm;
        bbox[1] = bbox[1] * itm;
        bbox[2] = bbox[2] * itm;
        bbox[3] = bbox[3] * itm;

        Iter srcriter(pImage, (int)bbox[0].x, (int)bbox[0].y);
        int rsrcx = (int)bbox[0].x;
        int rsrcy = (int)bbox[0].y;
        Iter dstriter(spXformImage, 0, 0);
        CImageIter::EStepDirection rowxdir = (bbox[0].x < bbox[3].x) ? CImageIter::Right : CImageIter::Left;
        CImageIter::EStepDirection rowydir = (bbox[0].y < bbox[3].y) ? CImageIter::Down : CImageIter::Up;

        Vector2 srcleftdelta = Vector2(bbox[3].x - bbox[0].x, bbox[3].y - bbox[0].y);
        float srcydist = (float)sqrt(srcleftdelta.x * srcleftdelta.x + srcleftdelta.y * srcleftdelta.y);
        float dstydist = (float)dsth;
        float srcyratio = srcydist / dstydist;// amount to step in src for each step in dst
        float delta = (srcleftdelta.x / srcleftdelta.y);
        float ratio2 = srcyratio * srcyratio;
        float srcydy = (float)sqrt(ratio2 / (delta * delta + 1.0f));
        float srcydx = (float)sqrt(ratio2 - srcydy * srcydy);

        Vector2 srcdelta(bbox[1].x - bbox[0].x, bbox[1].y - bbox[0].y);
        float srcxdist = (float)sqrt(srcdelta.x * srcdelta.x + srcdelta.y * srcdelta.y);
        float dstxdist = (float)dstw;
        float srcxratio = srcxdist / dstxdist;// amount to step in src for each step in dst
        delta = (srcdelta.x / srcdelta.y);
        ratio2 = srcxratio * srcxratio;
        float srcxdy = (float)sqrt(ratio2 / (delta * delta + 1.0f));
        float srcxdx = (float)sqrt(ratio2 - srcxdy * srcxdy);

        CImageIter::EStepDirection colxdir = ((bbox[0].x < bbox[1].x) ? CImageIter::Right : CImageIter::Left);
        CImageIter::EStepDirection colydir = ((bbox[0].y < bbox[1].y) ? CImageIter::Down : CImageIter::Up);
        float leftxsteps = 0.0f;
        float leftysteps = 0.0f;
        int steps = 0;
        for (int y = 0; y < dsth; y++)
        {
            Iter srcciter = srcriter;
            Iter dstciter = dstriter;
            int srcx = rsrcx;
            int srcy = rsrcy;
            float xsteps = 0.0f;
            float ysteps = 0.0f;
            for (int x = 0; x < dstw; x++)
            {
                uint8 r, g, b;

                if (srcx >= 0 && srcx < (int)pImage->GetWidth() &&
                    srcy >= 0 && srcy < (int)pImage->GetHeight())
                {
                    r = srcciter.GetRed();
                    g = srcciter.GetGreen();
                    b = srcciter.GetBlue();
                }
                else
                    r = g = b = 0;
                dstciter.SetRed(r);
                dstciter.SetGreen(g);
                dstciter.SetBlue(b);

                //
                // Now step along X
                //
                xsteps += srcxdx;
                steps = (int)xsteps; // Get floor of xsteps
                xsteps -= steps; // Reduce by the floor
                while (steps--)
                {
                    if (colxdir == CImageIter::Right)
                        srcx++;
                    else
                        srcx--;
                    srcciter.Step(colxdir);
                }

                //
                // Now step along Y
                //
                ysteps += srcxdy;
                steps = (int)ysteps;
                ysteps -= steps;
                while (steps--)
                {
                    if (colydir == CImageIter::Down)
                        srcy++;
                    else
                        srcy--;
                    srcciter.Step(colydir);
                }
                dstciter.Step(CImageIter::Right);
            }

            //
            // First step along X
            //
            leftxsteps += srcydx;
            steps = (int)leftxsteps;
            leftxsteps -= (float)steps;
            while (steps--)
            {
                if (rowxdir == CImageIter::Right)
                    rsrcx++;
                else
                    rsrcx--;
                srcriter.Step(rowxdir);
            }

            //
            // First step along Y
            //
            leftysteps += srcydy;
            steps = (int)leftysteps;
            leftysteps -= (float)steps;
            while (steps--)
            {
                if (rowydir == CImageIter::Down)
                    rsrcy++;
                else
                    rsrcy--;
                srcriter.Step(rowydir);
            }
            dstriter.Step(CImageIter::Down);
        }
        return spXformImage;
    }

    //**********************************************************************
    // Method: Apply
    // Transform an image.
    //
    // Parameters:
    // pImage - image to perform filtering on.
    // pParams - filter parameters.
    //
    // Returns:
    // Returns the filtered image.
    //**********************************************************************
    CRefObj<IImage> CTransformFilter::Apply(IImage* pImage, ImageFilterParams* pParams)
    {
        Matrix3x2 tm(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        if (pParams != nullptr)
        {
            std::map<std::string, std::any>::iterator it;
            it = pParams->params.find("Matrix");
            if (it != pParams->params.end())
                tm = std::any_cast<Matrix3x2>(it->second);
        }

        if (pImage->GetBPP() == 24)
        {
            return Transform<CImageIter24>(pImage, tm);
        }
        else if (pImage->GetBPP() == 32)
        {
            return Transform<CImageIter32>(pImage, tm);
        }
        return nullptr;
    }

    bool CTransformFilter::ApplyInPlace(IImage* pImage, ImageFilterParams* pParams)
    {
        return false; // Inplace transformation not supported
    }
}
