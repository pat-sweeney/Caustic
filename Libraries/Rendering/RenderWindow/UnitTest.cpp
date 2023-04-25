//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#define _USE_MATH_DEFINES
#include <atlbase.h>
#include <Windows.h>
#include <d3d11.h>
#include <string>

module Rendering.RenderWindow.UnitTests;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.UnitTests;
import Base.Math.Vector;
import Base.Math.BBox;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.ICausticFactory;
import Rendering.RenderWindow.RenderWindow;
import Rendering.RenderWindow.IRenderWindow;

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        auto spCausticFactory = Caustic::CreateCausticFactory();
        std::wstring shaderFolder; // Intentionally left empty
        BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
        auto spRenderWindow = CreateRenderWindow(GetDesktopWindow(), viewport, shaderFolder,
            [](IRenderer*, IRenderCtx*, int) {}, [](IRenderer*) {});
        return true;
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_LBUTTONDOWN:
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    
    static bool TestSimpleRenderGraph()
    {
#if 0
        WNDCLASS cw;
        ZeroMemory(&cw, sizeof(cw));
        cw.lpszClassName = L"SampleWindowClass";
        cw.hInstance = GetModuleHandle(NULL);
        cw.lpfnWndProc = WndProc;
        ::RegisterClass(&cw);
        HWND hwnd = ::CreateWindow(L"SampleWindowClass", L"TestRG", WS_OVERLAPPEDWINDOW, 0, 0, 1920, 1080, GetDesktopWindow(), NULL, GetModuleHandle(NULL), NULL);
        ::ShowWindow(hwnd, SW_SHOW);
        auto spRenderGraphFactory = Caustic::CreateRenderGraphFactory();
        auto spCausticFactory = Caustic::CreateCausticFactory();
        std::wstring shaderFolder; // Intentionally left empty
        BBox2 viewport(0.0f, 0.0f, 1.0f, 1.0f);
        auto spRenderWindow = CreateRenderWindow(hwnd, viewport, shaderFolder, [](IRenderer*, IRenderCtx*, int) {});
        auto spRenderer = spRenderWindow->GetRenderer();
        auto spRenderGraph = spRenderWindow->GetRenderGraph();
        auto spMeshNode = CreateRenderGraphFactory()->CreateMeshNode(Caustic::CreateCube());

        auto spRenderer = spRenderer->GetGraphics();
        auto spMaterialAttrib = spCausticFactory->CreateMaterialAttrib();
        auto spShader = spRenderer->GetShaderMgr()->FindShader(L"textured");
        auto spRenderMaterial = spCausticFactory->CreateRenderMaterial(spRenderer, spMaterialAttrib, spShader);
        auto spVideoTexture = VideoTextureFromWebcam(spRenderer);
        spRenderMaterial->SetTexture(spRenderer, L"diffuseTexture", spVideoTexture, EShaderAccess::PixelShader);
        spMeshNode->FindInputPin("frontMaterial")->SetDefaultValue(spRenderMaterial);

        CRefObj<IRenderGraphNode_LightCollection> spLightElem = spRenderGraphFactory->CreateLightCollectionNode();
        spLightElem->FindOutputPin("lights")->LinkTo(spMeshNode->FindInputPin("lights"));

        Vector3 lightPos(0.0f, 0.0f, -10.0f);
        FRGBColor lightColor(1.0f, 1.0f, 1.0f);
        spLightElem->AddLight(spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));

        lightPos = Vector3(-10.0f, 10.0f, 0.0f);
        spLightElem->AddLight(spCausticFactory->CreatePointLight(lightPos, lightColor, 1.0f));

        spRenderGraph->AddChild(spLightElem);
        spRenderGraph->AddChild(spMeshNode);

        MSG msg;
        DWORD dwStartTest = GetTickCount();
        bool testRunning = true;
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            DWORD dwEndTest = GetTickCount();
            if ((dwEndTest - dwStartTest) / 1000 > 4 && testRunning)
            {
                testRunning = false;
                PostQuitMessage(0);
            }
            if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
#endif
        return true;
    }

    bool RenderWindowTests::RunUnitTests()
    {
        typedef bool(*UnitTestFunc)();
        UnitTestEntry tests[] =
        {
            { Test1, true },
            { TestSimpleRenderGraph, true },
        };
        bool result = true;
        m_totalTests = _countof(tests);
        for (int i = 0; i < _countof(tests); i++)
        {
            if (!tests[i].enabled)
                continue;
            if (!(*tests[i].testFunc)())
            {
                printf("RenderWindowTests: Test %d failed\n", i);
                result = false;
            }
            else
                m_testsPassed++;
            m_testsRan++;
        }
        return result;
    }
}
