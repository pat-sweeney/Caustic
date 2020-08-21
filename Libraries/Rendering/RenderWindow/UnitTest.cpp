//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "Base\Math\Vector.h"
#include "Base\Core\error.h"
#include "RenderWindow.h"
#include "Rendering/RenderGraph/RenderGraph.h"
#include "UnitTest.h"
#include <stdlib.h>
#include <stdio.h>

using namespace Caustic;
namespace CausticTestSuite
{
    static bool Test1()
    {
        auto spRenderGraphFactory = Caustic::CreateRenderGraphFactory();
        auto spCausticFactory = Caustic::CreateCausticFactory();
        std::wstring shaderFolder; // Intentionally left empty
        auto spRenderWindow = CreateRenderWindow(GetDesktopWindow(), shaderFolder);
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
        auto spRenderWindow = CreateRenderWindow(hwnd, shaderFolder);
        auto spRenderer = spRenderWindow->GetRenderer();
        auto spRenderGraph = spRenderWindow->GetRenderGraph();
        auto spMeshNode = CreateRenderGraphFactory()->CreateMeshNode(Caustic::CreateCube());

        auto spGraphics = spRenderer->GetGraphics();
        auto spMaterialAttrib = spCausticFactory->CreateMaterialAttrib();
        auto spShader = spRenderer->GetShaderMgr()->FindShader(L"textured");
        auto spRenderMaterial = spCausticFactory->CreateRenderMaterial(spGraphics, spMaterialAttrib, spShader);
        auto spVideoTexture = VideoTextureFromWebcam(spGraphics);
        spRenderMaterial->SetTexture(spGraphics, L"diffuseTexture", spVideoTexture, EShaderAccess::PixelShader);
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
