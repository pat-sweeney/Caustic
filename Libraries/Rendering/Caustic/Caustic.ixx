//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>

export module Rendering.Caustic;
////import Base.Core.Core;
////import Base.Math.BBox;
////import Base.Math.Vector;
////import Rendering.Caustic.ITrackball;
////import Rendering.Caustic.IRenderer;
////import Rendering.Caustic.ICamera;
////import Rendering.Caustic.IRenderCtx;
////import Rendering.Caustic.IDirectionalLight;
////import Rendering.Caustic.IPointLight;
////import Rendering.Caustic.ISpotLight;
////import Rendering.Caustic.IRenderMaterial;
////import Rendering.Caustic.ISampler;
////import Rendering.Caustic.IShaderMgr;
////import Rendering.Caustic.IPointCloud;
////import Rendering.Caustic.IShader;
//////import Rendering.Caustic.IRendererMarshaller;
//////import Rendering.Caustic.IRenderMesh;
////import Rendering.Caustic.IRenderable;
////import Rendering.Caustic.ICausticFactory;
//////import Rendering.Caustic.ITexture;

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************
namespace Caustic
{
//	CRefObj<IRenderSubMesh> BuildRenderSubMesh(IRenderer* pRenderer,
//		std::vector<CGeomFace>& faces, std::vector<CGeomVertex>& verts,
//		IShader* pShader);
//	CRefObj<IRenderSubMesh> CreateDepthGridMesh(IRenderer* pRenderer,
//		uint32 width, uint32 height, IShader* pShader);
}
