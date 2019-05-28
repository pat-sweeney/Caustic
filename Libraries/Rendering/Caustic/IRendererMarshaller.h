//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Math\Vector.h"
#include "Base/Core/IRefCount.h"
#include "Renderer.h"
#include "Rendering/SceneGraph/SceneGraph.h"
#include <Windows.h>
#include <d3d12.h>
#include <functional>

namespace Caustic
{
	//**********************************************************************
	// Interface: IRendererMarshaller
	// Creates a marshaller for render calls. The Renderer runs on its own
	// thread. This interface allows the client to call the renderer from any
	// thread (calls will be marshalled to the renderer thread).
	//**********************************************************************
	struct IRendererMarshaller : public IRenderer
	{
		virtual void Initialize(HWND hwnd, std::wstring &shaderFolder, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) = 0;
		virtual void Shutdown() = 0;
		virtual void SetMaxCmdLength() = 0;
		virtual void GetRenderer(IRenderer **ppRenderer) = 0;
		virtual void LoadTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;
		virtual void LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture) = 0;
		virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;
		virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
		virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) = 0;
		virtual void CallOnRenderThread(std::function<void(IRenderer *pRenderer)> func, bool wait) = 0;
	};
}
