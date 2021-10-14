//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

import Base.Core.Core;
import Base.Core.IRefCount;
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"
#include "Renderable.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

//**********************************************************************
// File: IPointCloud.h
// This file defines the published interface for point clouds.
// A PointCloud renders a depth map as a set of points.
//**********************************************************************

namespace Caustic
{
    struct IRenderer;
    struct IPointLight;
    struct IRenderCtx;

    //**********************************************************************
    // Interface: IPointCloud
    // A PointCloud renders a depth map as a set of points.
    //
    // Header:
    // [Link:Rendering/Caustic/IPointCloud.h]
    //**********************************************************************
    struct IPointCloud : public IRenderable
    {
        //**********************************************************************
        // Method: SetDepthMap
        // Assigns a depth map as the source for the points
        //
        // Parameters:
        // pRenderer - renderer
        // pImage - Depth map image
        //**********************************************************************
        virtual void SetDepthMap(IRenderer* pRenderer, IImage *pImage) = 0;

        //**********************************************************************
        // Method: SetRayMap
        // Assigns a "ray" map (defines a vector for each pixel)
        //
        // Parameters:
        // pRenderer - renderer
        // pImage - Depth map image
        //**********************************************************************
        virtual void SetRayMap(IRenderer* pRenderer, IImage* pImage) = 0;

        //**********************************************************************
        // Method: SetColorMap
        // Sets the color map (image data to color the cloud with) from an <IImage>
        //
        // Parameters:
        // pRenderer - renderer
        // pImage - Color image
        //**********************************************************************
        virtual void SetColorMap(IRenderer* pRenderer, IImage* pImage) = 0;

        //**********************************************************************
        // Method: SetColorExtrinsics
        // Sets the extrinsics matrix for the depth camera from which the depth map was obtained.
        //
        // Parameters:
        // mat - extrinsics matrix
        //**********************************************************************
        virtual void SetColorExtrinsics(Matrix4x4& mat) = 0;

        //**********************************************************************
        // Method: SetColorIntrinsics
        // Sets the intrinsics matrix for the depth camera from which the depth map was obtained.
        //
        // Parameters:
        // mat - intrinsics matrix
        //**********************************************************************
        virtual void SetColorIntrinsics(Matrix3x3& mat) = 0;
    };

    //**********************************************************************
    // Function: CreatePointCloud
    // Creates a point cloud object
    //
    // Parameters:
    // pRenderer - Renderer
    // width - width of depth map
    // height - height of depth map
    //
    // Returns:
    // Returns the created renderable
    //
    // Header:
    // [Link:Rendering/Caustic/IRenderable.h]
    //**********************************************************************
    CRefObj<IPointCloud> CreatePointCloud(IRenderer* pRenderer, uint32 width, uint32 height);
}
