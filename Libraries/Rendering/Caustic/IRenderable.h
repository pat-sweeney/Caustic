//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Base/Core/IRefCount.h"
#include <vector>

namespace Caustic
{
	struct IRenderable;
	struct IPointLight;
	struct IRenderCtx;

	//**********************************************************************
	// Interface: IRenderable
	// Defines the main interface for rendered objects
	//**********************************************************************
	struct IRenderable : public IRefCount
	{
		//**********************************************************************
		// Method: GetPos
		// Returns the world position of the object
		//**********************************************************************
		virtual Vector3 GetPos() = 0;

		//**********************************************************************
		// Method: Render
		// Called by the renderer to render this object
		//
		// Parameters:
		// pRenderer - Rendering device
		// lights - List of current lights effecting this object
		// pRenderCtx - Current render context
		//**********************************************************************
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx) = 0;

		//**********************************************************************
		// Method: SetTransform
		// Sets the transform on the object
		//
		// Parameters:
		// mat - transform to set
		//**********************************************************************
		virtual void SetTransform(DirectX::XMMATRIX &mat) = 0;

		//**********************************************************************
		// Method: GetTransform
		// Returns the transforms associated with the object
		//**********************************************************************
		virtual DirectX::XMMATRIX &GetTransform() = 0;

		//**********************************************************************
		// Method: InPass
		// Returns whether the object is part of the specified pass
		//
		// Parameters:
		// pass - render pass
		//
		// Returns:
		// Returns True if object is part of the specified pass. False otherwise.
		//**********************************************************************
		virtual bool InPass(int pass) = 0;
	};
}