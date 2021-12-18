//**********************************************************************
// Copyright Patrick Sweeney 2018-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include "Rendering\RenderGraph\IRenderGraph.h"
#include "Rendering\RenderGraph\IRenderGraphFactory.h"
#include <functional>

export module Rendering.RenderWindow.RenderWindow;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.RendererMarshaller.IRendererMarshaller;
import Rendering.Caustic.ITrackball;
import Rendering.SceneGraph.ISceneGraph;
import Rendering.SceneGraph.ISceneFactory;
import Rendering.RenderWindow.IRenderWindow;

export namespace Caustic
{
    class CRenderWindow : public IRenderWindow, public CRefCount
    {
        CRefObj<IRendererMarshaller> m_spMarshaller;
        CRefObj<IRenderGraphFactory> m_spRenderGraphFactory;
        CRefObj<IRenderGraph> m_spRenderGraph;
        CRefObj<ISceneFactory> m_spSceneFactory;
        CRefObj<ISceneGraph> m_spSceneGraph;
        CRefObj<IPointLight> m_spPointLight;
        CRefObj<ICamera> m_spCamera;
        CRefObj<ITrackball> m_spTrackball;
        bool m_tracking;
        Vector3 m_eye;
        Vector3 m_look;
        Vector3 m_up;
        Vector3 m_upAsPt;
        HWND m_hwnd;
        DirectX::XMMATRIX m_view;
        DirectX::XMMATRIX m_invview;
        int m_startx, m_starty;
        int m_winwidth, m_winheight;
        bool m_useRenderGraph;
        Vector3 m_snapPosHome;
        Vector3 m_snapPosX;
        Vector3 m_snapPosY;
        Vector3 m_snapPosZ;
        std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*, int)> m_callback;
        std::function<void(Caustic::IRenderer*)> m_prePresentCallback;
    public:
        CRenderWindow(HWND hwnd, std::wstring &shaderFolder,
            std::function<void(Caustic::IRenderer*, Caustic::IRenderCtx*, int)> callback,
            std::function<void(Caustic::IRenderer*)> prePresentCallback,
            bool useRenderGraph = false, bool startFrozen = false, int desktopIndex = 0);
        ~CRenderWindow();
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderWindow
        //**********************************************************************
        virtual void SetSnapPositions(const Vector3& home, const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis) override;
        virtual CRefObj<ISceneGraph> GetSceneGraph() override { return m_spSceneGraph; }
        virtual void MouseDown(int x, int y, uint32 button, uint32 flags) override;
        virtual void MouseMove(int x, int y, uint32 flags) override;
        virtual void MouseUp(int x, int y, uint32 button, uint32 flags) override;
        virtual void MouseWheel(int factor) override;
        virtual void MapKey(uint32 wParam, uint32 lParam) override;
        virtual CRefObj<IRenderer> GetRenderer() { return m_spMarshaller->GetRenderer(); }
    };
}
