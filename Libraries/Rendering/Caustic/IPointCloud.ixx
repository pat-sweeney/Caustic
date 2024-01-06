//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

export module Rendering.Caustic.IPointCloud;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IRenderable;
import Imaging.Image.IImage;
import Base.Math.Matrix;

//**********************************************************************
// File: IPointCloud.h
// This file defines the published interface for point clouds.
// A PointCloud renders a depth map as a set of points.
//**********************************************************************

export namespace Caustic
{
    struct IRenderer;
    struct IPointLight;
    struct IRenderCtx;

    //**********************************************************************
    // Interface: IPointCloud
    // A PointCloud renders a depth map as a set of points.
    //
    // Module:
    // {Link:import Rendering.Caustic.IPointCloud;{Rendering/Caustic/IPointCloud.ixx}}
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
    // Module:
    // {Link:import Rendering.Caustic.IPointCloud;{Rendering/Caustic/IPointCloud.ixx}}
    //**********************************************************************
    CRefObj<IPointCloud> CreatePointCloud(IRenderer* pRenderer, uint32_t width, uint32_t height);
}
