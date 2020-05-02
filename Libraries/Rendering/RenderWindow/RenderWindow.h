//**********************************************************************
// Copyright Patrick Sweeney 2018-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\RenderWindow\IRenderWindow.h"
#include "Rendering\RenderGraph\IRenderGraph.h"
#include "Rendering\RenderGraph\IRenderGraphFactory.h"

namespace Caustic
{
    class CRenderWindow : public IRenderWindow, public CRefCount
    {
        CRefObj<IRendererMarshaller> m_spMarshaller;
        CRefObj<IRenderGraphFactory> m_spRenderGraphFactory;
        CRefObj<IRenderGraph> m_spRenderGraph;
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
    public:
        CRenderWindow(HWND hwnd, std::wstring &shaderFolder);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderWindow
        //**********************************************************************
        virtual CRefObj<IRenderGraph> GetRenderGraph() override { return m_spRenderGraph; }
        virtual void MouseDown(int x, int y, uint32 button, uint32 flags) override;
        virtual void MouseMove(int x, int y, uint32 flags) override;
        virtual void MouseUp(int x, int y, uint32 button, uint32 flags) override;
        virtual void MouseWheel(int factor) override;
        virtual void MapKey(uint32 wParam, uint32 lParam) override;
        virtual CRefObj<IRenderer> GetRenderer() { return m_spMarshaller->GetRenderer(); }
    };
}
