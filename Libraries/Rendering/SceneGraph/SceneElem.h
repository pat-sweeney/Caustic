//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include <string>

namespace Caustic
{
	struct ISceneElem;
	struct SceneCtx;

	//**********************************************************************
	// Class: CSceneElem
	// Defines the base class for all our scene graph elements
	//**********************************************************************
	class CSceneElem
	{
	protected:
		std::wstring m_Name; // User provided name for this element
		uint32 m_Flags;
		BBox3 m_BBox;

		void DrawSelected(IRenderer *pRenderer, ISceneElem *pElem, SceneCtx *pSceneCtx);
	public:
		CSceneElem() :
			m_Flags(0)
		{
		}
	};
}
