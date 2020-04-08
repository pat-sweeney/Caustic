//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base/Math/BBox.h"
#include "IRenderMesh.h"
#include "Shader.h"
#include <d3d11.h>

namespace Caustic
{
	extern void BuildVertexBuffer(IGraphics* pGraphics, IShaderInfo* pShaderInfo, std::vector<CGeomVertex>& pts, MeshData* pMeshData);
	extern void BuildIndexBuffer(IGraphics* pGraphics, std::vector<uint32>& faces, MeshData* pMeshData);
}
