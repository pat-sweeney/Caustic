//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>

module Rendering.Caustic.IPointCloud;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.PointCloud;

//**********************************************************************
// File: IPointCloud.cpp
// This file defines the published implementation for point clouds.
// A PointCloud renders a depth map as a set of points.
//**********************************************************************

namespace Caustic
{
    CRefObj<IPointCloud> CreatePointCloud(IRenderer* pRenderer, uint32 width, uint32 height)
    {
        std::unique_ptr<CPointCloud> spPointCloud(new CPointCloud(pRenderer, width, height));
        return CRefObj<IPointCloud>(spPointCloud.release());
    }
}