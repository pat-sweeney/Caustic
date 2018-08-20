//**********************************************************************
// Copyright Patrick Sweeney 2018
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Rendering\SceneGraph\SceneGraph.h"
#include <Windows.h>

namespace Caustic
{
    struct IRenderWindow : public IRefCount
    {
        virtual CRefObj<ISceneGraph> GetSceneGraph() = 0;
        virtual void MouseDown(int x, int y) = 0;
        virtual void MouseMove(int x, int y, uint32 flags) = 0;
        virtual void MouseUp(int x, int y) = 0;
        virtual void MouseWheel(int factor) = 0;
        virtual void MapKey(uint32 wParam, uint32 lParam) = 0;
    };

    CAUSTICAPI void CreateRenderWindow(HWND hwnd, IRenderWindow **ppRenderWindow);
}