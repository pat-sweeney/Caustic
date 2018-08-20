//**********************************************************************
// Copyright Patrick Sweeney 2018
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\RefCount.h"
#include "Rendering\RenderWindow\IRenderWindow.h"

namespace Caustic
{
    class CRenderWindow : public IRenderWindow, public CRefCount
    {
        CRefObj<IRendererMarshaller> m_spMarshaller;
        CRefObj<ISceneGraph> m_spSceneGraph;
        CRefObj<ICamera> m_spCamera;
        CRefObj<ITrackball> m_spTrackball;
        bool m_tracking;
        Vector3 m_eye;
        Vector3 m_look;
        Vector3 m_up;
        HWND m_hwnd;
        DirectX::XMMATRIX m_view;
        DirectX::XMMATRIX m_invview;
    public:
        CRenderWindow(HWND hwnd);
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderWindow
        //**********************************************************************
        virtual CRefObj<ISceneGraph> GetSceneGraph() override { return m_spSceneGraph; }
        virtual void MouseDown(int x, int y) override;
        virtual void MouseMove(int x, int y, uint32 flags) override;
        virtual void MouseUp(int x, int y) override;
        virtual void MouseWheel(int factor) override;
        virtual void MapKey(uint32 wParam, uint32 lParam) override;
    };
}
