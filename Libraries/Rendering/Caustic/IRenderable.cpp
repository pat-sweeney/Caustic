//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Rendering.Caustic.IRenderable;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.Renderable;

//**********************************************************************
// File: IRenderable.cpp
// Contains implementation for public portions of CRenderable
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Function: CreateRenderable
    // Creates a renderable object
    //
    // Parameters:
    // pSubMesh - Submesh to create renderable for
    // pFrontMaterial - material for front faces
    // pBackMaterial - material for back faces
    // mat - transform to apply
    //
    // Returns:
    // Returns the created renderable
    // 
    // Module:
    // {Link:import Rendering.Caustic.IRenderable;{Rendering/Caustic/IRenderable.cpp}}
    //**********************************************************************
    CRefObj<IRenderable> CreateRenderable(IRenderSubMesh* pSubMesh, IRenderMaterial* pFrontMaterial, IRenderMaterial* pBackMaterial, DirectX::XMMATRIX& mat)
    {
        return CRefObj<IRenderable>(new CRenderable(pSubMesh, pFrontMaterial, pBackMaterial, mat));
    }
}
