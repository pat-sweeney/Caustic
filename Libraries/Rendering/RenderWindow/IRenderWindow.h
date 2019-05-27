//**********************************************************************
// Copyright Patrick Sweeney 2018-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include <Windows.h>

namespace Caustic
{
    const int c_LeftButton = 0;
    const int c_MiddleButton = 1;
    const int c_RightButton = 2;

	//**********************************************************************
	// Interface: IRenderWindow
	// Defines interface for interacting with a render window. A render window
	// is a window that has a DirectX devince associated with it along with
	// a scene graph.
	//**********************************************************************
	struct IRenderWindow : public IRefCount
    {
		//**********************************************************************
		// Method: GetSceneGraph
		// Returns the scene graph associated with this render window
		//**********************************************************************
		virtual CRefObj<ISceneGraph> GetSceneGraph() = 0;

		//**********************************************************************
		// Method: GetRenderer
		// Returns the renderer associated with this render window
		//**********************************************************************
		virtual CRefObj<IRendererMarshaller> GetRenderer() = 0;
		
		//**********************************************************************
		// Method: MouseDown
		// Called when a mouse down event occurs in the window
		//
		// Parameters:
		// x - X position of mouse in pixels
		// y - Y position of mouse in pixels
		// button - integer indicating which button was pressed
		// flags - flags (control, shift, etc)
		//**********************************************************************
		virtual void MouseDown(int x, int y, uint32 button, uint32 flags) = 0;

		//**********************************************************************
		// Method: MouseMove
		// Called when a mouse move event occurs in the window
		//
		// Parameters:
		// x - X position of mouse in pixels
		// y - Y position of mouse in pixels
		// flags - flags (control, shift, etc)
		//**********************************************************************
		virtual void MouseMove(int x, int y, uint32 flags) = 0;

		//**********************************************************************
		// Method: MouseUp
		// Called when a mouse up event occurs in the window
		//
		// Parameters:
		// x - X position of mouse in pixels
		// y - Y position of mouse in pixels
		// button - integer indicating which button was pressed
		// flags - flags (control, shift, etc)
		//**********************************************************************
		virtual void MouseUp(int x, int y, uint32 button, uint32 flags) = 0;

		//**********************************************************************
		// Method: MouseWheel
		// Called when a mouse wheel event occurs in the window
		//
		// Parameters:
		// factor - number indicating which way the wheel was turned and how far
		//**********************************************************************
		virtual void MouseWheel(int factor) = 0;

		//**********************************************************************
		// Method: MapKey
		// Called when a key press event occurs in the window
		//
		// Parameters:
		// wParam - wParam from WM_KEY event
		// lParam - lParam from WM_KEY event
		//**********************************************************************
		virtual void MapKey(uint32 wParam, uint32 lParam) = 0;
    };


	//**********************************************************************
	// Method: CreateRenderWindow
	// Creates an IRenderWindow
	//
	// Parameters:
	// hwnd - window to bind IRenderWindow to
	// shaderFolder - folder containing shaders
	// ppRenderWindow - returns the created render window
	//**********************************************************************
	CAUSTICAPI void CreateRenderWindow(HWND hwnd, std::wstring &shaderFolder, IRenderWindow **ppRenderWindow);
}