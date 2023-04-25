//**********************************************************************
// Copyright Patrick Sweeney 2018-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <functional>
#include <string>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"

export module Rendering.RenderWindow.IRenderWindow;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.ICamera;
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
        // Method: GetCamera
        // Returns the camera the window is using (which ultimately is set on the renderer)
        //**********************************************************************
        virtual CRefObj<ICamera> GetCamera() = 0;

        //**********************************************************************
        // Method: SetCamera
        // Sets the camera the window is using (which ultimately is set on the renderer)
        //**********************************************************************
        virtual void SetCamera(CRefObj<ICamera> spCamera) = 0;

        //**********************************************************************
        // Method: RecordEvent
        // Records an mouse/keyboard event for processing by the UI later. Only
        // used for ImGui at the moment.
        // 
        // Parameters:
        // hWnd - window event occured in
        // message - integer of message ID
        // wParam - wParam from Windows
        // lParam - lParam from Windows
        //**********************************************************************
        virtual void RecordEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

        //**********************************************************************
        // Method: SetViewport
        // Specifies the viewport for the final render target
        // 
        // Parameters:
        // x0 - top left X coordinate from 0..1 indicating which portion of the output window is used
        // y0 - top left Y coordinate from 0..1 indicating which portion of the output window is used
        // x1 - bottom right X coordinate from 0..1 indicating which portion of the output window is used
        // y1 - bottom right Y coordinate from 0..1 indicating which portion of the output window is used
        //**********************************************************************
        virtual void SetViewport(float x0, float y0, float x1, float y1) = 0;

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
        // Method: SetSceneGraph
        // Sets the render window's scene graph to the specified scene graph
        // 
        // Parameters:
        // pSceneGraph - scene graph to set on render window
        //**********************************************************************
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) = 0;

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
    // viewport - viewport for final render target
    // shaderFolder - Path to folder containing shaders
    // callback - callback during rendering
    // prePresentCallback - callback right before Present() is called
    // startFrozen - should renderer start in frozen state
    // desktopIndex - index of desktop to use
    // 
    // Returns:
    // Returns the created render window
    //**********************************************************************
    CRefObj<IRenderWindow> CreateRenderWindow(HWND hwnd, BBox2& viewport, std::wstring& shaderFolder,
        std::function<void(IRenderer*, IRenderCtx*, int)> callback,
        std::function<void(IRenderer*)> prePresentCallback,
        bool startFrozen = false, int desktopIndex = 0);

    //**********************************************************************
    // Method: CreateImguiRenderWindow
    // Creates an IRenderWindow that is using ImGui for its interface
    // In this case the scene is rendered to a texture and then draw
    // as an image by ImGui.
    //
    // Parameters:
    // hwnd - window to bind IRenderWindow to
    // viewport - viewport for final render target
    // shaderFolder - Path to folder containing shaders
    // renderUI - callback for rendering UI
    // startFrozen - should renderer start in frozen state
    // desktopIndex - index of desktop to use
    // 
    // Returns:
    // Returns the created render window
    //**********************************************************************
    CRefObj<IRenderWindow> CreateImguiRenderWindow(HWND hwnd, BBox2& viewport, std::wstring& shaderFolder,
        std::function<void(Caustic::IRenderer*, ITexture*, ImFont*)> renderUI,
        bool startFrozen = false, int desktopIndex = 0);
}
