//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include <Windows.h>
#include <atlbase.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>
#include <any>
#include "Rendering\Caustic\ICausticFactory.h"
#include "Rendering\Caustic\ICamera.h"
#include "Rendering\Caustic\IPointLight.h"
#include "Rendering\Caustic\IRenderable.h"
#include "Rendering\Caustic\IRenderCtx.h"
#include "Rendering\Caustic\IRenderer.h"
#include "Rendering\Caustic\IRenderMaterial.h"
#include "Rendering\Caustic\ISampler.h"
#include "Rendering\Caustic\IShader.h"
#include "Rendering\Caustic\IShaderMgr.h"
#include "Rendering\Caustic\ITexture.h"
#include "Rendering\Caustic\ITrackball.h"
#include "Rendering\Caustic\IShaderInfo.h"
