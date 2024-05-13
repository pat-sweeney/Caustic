//**********************************************************************
// Copyright Patrick Sweeney 2021-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.ISampler;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ITexture;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.Sampler;

namespace Caustic
{
    CRefObj<ISampler> CreateSampler(IRenderer* pRenderer, ITexture* pTexture)
    {
        return CRefObj<ISampler>(new CSampler(pRenderer, pTexture));
    }
}

