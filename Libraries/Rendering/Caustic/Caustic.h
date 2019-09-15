//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Geometry\Mesh\Mesh.h"
#include "Rendering\Caustic\IGraphics.h"
#include "Rendering\Caustic\ITexture.h"
#include "Rendering\Caustic\ISampler.h"
#include "Rendering\Caustic\ICamera.h"
#include "Rendering\Caustic\ITrackball.h"
#include "Rendering\Caustic\IShaderMgr.h"
#include "Rendering\Caustic\IShader.h"
#include "Rendering\Caustic\IRenderer.h"
#include "Rendering\Caustic\IRenderCtx.h"
#include "Rendering\Caustic\IPointLight.h"
#include "Rendering\Caustic\IRenderMaterial.h"
#include "Rendering\Caustic\IRendererMarshaller.h"
#include "Rendering\Caustic\IRenderable.h"
#include "Rendering\Caustic\ICausticFactory.h"

//**********************************************************************
// File: Caustic.h
// This file defines the published interface for talking to the Caustic Renderer.
// This include is usually the top level include that clients will use.
//**********************************************************************
