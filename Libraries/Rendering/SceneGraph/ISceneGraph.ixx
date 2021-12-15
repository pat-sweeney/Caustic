//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <vector>
#include <string>

export module Rendering.SceneGraph.ISceneGraph;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Imaging.Image.IImage;
import Rendering.Caustic.PathTrace;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.ISceneElem;
import Rendering.SceneGraph.ISceneGroupElem;

export namespace Caustic
{
	//**********************************************************************
	// Interface: ISceneGraph
	// Our main interface for manipulating a scene graph
	//**********************************************************************
	struct ISceneGraph : public ISceneGroupElem
	{
		//**********************************************************************
		// Method: PathTrace
		// Computes view of the scene using path tracing
		//
		// Parameters:
		// pRenderer - renderer
		// pCtx - path context
		// pDest - Image to render results to
		//**********************************************************************
		virtual void PathTrace(IRenderer* pRenderer, PathTraceCtx* pCtx, IImage* pDest) = 0;

		//**********************************************************************
		// Method: Merge
		// Merges a scene graph with another scene graph
		//**********************************************************************
		virtual void Merge(ISceneGraph* pGraph) = 0;

		//**********************************************************************
		// Method: Lock
		// Locks a scene graph so it can be updated
		//**********************************************************************
		virtual void Lock() = 0;

		//**********************************************************************
		// Method: Unlock
		// Unlocks a scene graph allowing the renderer to use it
		//**********************************************************************
		virtual void Unlock() = 0;
	};

	CRefObj<ISceneGraph> CreateSceneGraph();
};
