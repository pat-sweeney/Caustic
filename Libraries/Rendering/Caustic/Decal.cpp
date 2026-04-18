//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.IDecal;
import Base.Core.Core;
import Base.Core.Error;
import Base.Math.Matrix;
import Rendering.Caustic.Decal;
import Rendering.Caustic.IDecal;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: Decal.cpp
// Contains implementation of CreateDecal factory function
//**********************************************************************

namespace Caustic
{
    CRefObj<IDecal> CreateDecal(Matrix4x4& transform, ITexture* pAlbedoTexture, float opacity)
    {
        return CRefObj<IDecal>(new CDecal(transform, pAlbedoTexture, opacity));
    }
};
