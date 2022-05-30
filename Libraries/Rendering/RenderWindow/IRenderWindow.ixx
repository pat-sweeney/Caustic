//**********************************************************************
// Copyright Patrick Sweeney 2018-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <functional>

export module Rendering.RenderWindow.IRenderWindow;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IRenderer;
import Rendering.RenderGraph.IRenderGraph;
import Rendering.SceneGraph.ISceneGraph;

export namespace Caustic
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
		// Method: SetSnapPositions
		// Sets the world positions for snapping the camera to when certain keys are pressed
		//
		// Parameters:
		// home - Position to snap to when Home key is pressed
		// xAxis - Position to snap to when X key is pressed
		// yAxis - Position to snap to when Y key is pressed
		// zAxis - Position to snap to when Z key is pressed
		//**********************************************************************
		virtual void SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) = 0;

		//**********************************************************************
		// Method: GetSceneGraph
		// Returns the scene graph associated with this render window
		//**********************************************************************
		virtual CRefObj<ISceneGraph> GetSceneGraph() = 0;

		//**********************************************************************
		// Method: GetRenderer
		// Returns the renderer. This is the marshaling version of the renderer.
		//**********************************************************************
		virtual CRefObj<IRenderer> GetRenderer() = 0;

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
    // shaderFolder - Path to folder containing shaders
	// ppRenderWindow - returns the created render window
	//**********************************************************************
	CRefObj<IRenderWindow> CreateRenderWindow(HWND hwnd, std::wstring &shaderFolder, 
		std::function<void(IRenderer*, IRenderCtx*, int)> callback,
		std::function<void(IRenderer*)> prePresentCallback,
		bool startFrozen = false, int desktopIndex = 0);
} 