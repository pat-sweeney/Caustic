//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base/Core/IRefCount.h"
#include "IShader.h"
#include "IMaterialAttrib.h"
#include "IRenderer.h"
#include "ITexture.h"
#include <vector>

namespace Caustic
{
	struct IShader;
	struct IMaterialAttrib;
	struct IRenderer;
	struct ITexture;

	//**********************************************************************
	// Interface: IRenderMaterial
	// Defines a material that has its associated
	// render element (as opposed to an IMaterialAttrib which only defines
	// the attributes of the material).
	//**********************************************************************
	struct IRenderMaterial : public IRefCount
	{
		virtual void SetShader(IShader *pShader) = 0;
		virtual void GetShader(IShader **ppShader) = 0;
		virtual void SetMaterial(IMaterialAttrib *pMaterial) = 0;
		virtual void GetMaterial(IMaterialAttrib **ppMaterial) = 0;
		virtual void SetDiffuseTexture(IRenderer *pRenderer, ITexture *pTexture) = 0;
		virtual void SetSpecularTexture(IRenderer *pRenderer, ITexture *pTexture) = 0;
		virtual void SetAmbientTexture(IRenderer *pRenderer, ITexture *pTexture) = 0;
		virtual CRefObj<ITexture> GetDiffuseTexture() = 0;
		virtual CRefObj<ITexture> GetSpecularTexture() = 0;
		virtual CRefObj<ITexture> GetAmbientTexture() = 0;
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IShader *pOverrideShader) = 0;
	};
}
