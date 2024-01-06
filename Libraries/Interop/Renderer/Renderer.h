//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
//import Base.Core.IRefCount;
//#include "Rendering\Caustic\Caustic.h"
//#include "Rendering\RenderWindow\IRenderWindow.h"
//#include "Rendering\Caustic\ICausticFactory.h"
//#include "Rendering\SceneGraph\ISceneFactory.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Interop;
using namespace System::Windows;
using namespace System::Windows::Controls;
using namespace System::Windows::Input;
using namespace System::Windows::Media;
using namespace Microsoft::Wpf::Interop::DirectX;

namespace CausticInterop
{
//    using namespace Caustic;
    
#pragma unmanaged
    class CRendererInfo
    {
//        Caustic::CRefObj<Caustic::IRenderWindow> m_spRenderWindow;
//        Caustic::CRefObj<Caustic::ICausticFactory> m_spCausticFactory;
//        Caustic::CRefObj<Caustic::ISceneFactory> m_spSceneFactory;
    public:
        void Setup(void *pShaderPath)
        {
//            m_spSceneFactory = Caustic::CreateSceneFactory();
//            m_spCausticFactory = Caustic::CreateCausticFactory();
//            std::wstring shaderFolder(static_cast<const wchar_t*>(pShaderPath));
//            m_spRenderWindow = Caustic::CreateRenderWindow(nullptr, shaderFolder,
//                [](IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass) {
//                },
//                [](IRenderer*) {
//                });
        }
        
//        void MouseMove(int x, int y, uint32_t flags)
//        {
//            if (m_spRenderWindow)
//                m_spRenderWindow->MouseMove(x, y, flags);
//        }
//
//        void MouseUp(int x, int y, uint32_t button, uint32_t flags)
//        {
//            if (m_spRenderWindow)
//                m_spRenderWindow->MouseUp(x, y, button, flags);
//        }
//
//        void MouseDown(int x, int y, uint32_t button, uint32_t flags)
//        {
//            if (m_spRenderWindow)
//                m_spRenderWindow->MouseDown(x, y, button, flags);
//        }
//
//        void Key(uint32_t wParam, uint32_t lParam)
//        {
//            if (m_spRenderWindow)
//                m_spRenderWindow->MapKey(wParam, lParam);
//        }
//
//        void SetRenderTarget(void *pSurface, bool isNewSurface)
//        {
//            if (m_spRenderWindow)
//            {
//                IUnknown* pTexture = reinterpret_cast<IUnknown*>(pSurface);
//                m_spRenderWindow->GetRenderer()->SetFinalRenderTargetUsingSharedTexture(pTexture);
//            }
//        }
    };
#pragma managed

    public ref class Renderer : Image
	{
//        CRendererInfo* m_pRendererInfo;
//        D3D11Image^ m_d3dimage;
//        bool lastVisible;
//        System::TimeSpan^ lastRender;
//        uint32_t lastButton;
//
//        void HandleMouseUp(Object^ sender, MouseButtonEventArgs^ e)
//        {
//            if ((lastButton == c_MiddleButton && e->MiddleButton == MouseButtonState::Released) ||
//                (lastButton == c_LeftButton && e->LeftButton == MouseButtonState::Released) ||
//                (lastButton == c_RightButton && e->RightButton == MouseButtonState::Released))
//            {
//                auto pt = e->GetPosition(this);
//                uint32_t flags = 0;
//                if (Keyboard::IsKeyDown(Key::LeftCtrl) || Keyboard::IsKeyDown(Key::RightCtrl))
//                    flags |= MK_CONTROL;
//                if (Keyboard::IsKeyDown(Key::LeftShift) || Keyboard::IsKeyDown(Key::RightShift))
//                    flags |= MK_SHIFT;
//                m_pRendererInfo->MouseUp((uint32_t)pt.X, (uint32_t)pt.Y, lastButton, flags);
//            }
//        }
//
//        void HandleMouseDown(Object^ sender, MouseButtonEventArgs^ e)
//        {
//            auto pt = e->GetPosition(this);
//            lastButton = 0;
//            if (e->MiddleButton == MouseButtonState::Pressed)
//                lastButton = c_MiddleButton;
//            else if (e->LeftButton == MouseButtonState::Pressed)
//                lastButton = c_LeftButton;
//            else if (e->RightButton == MouseButtonState::Pressed)
//                lastButton = c_RightButton;
//            else
//                return;
//            uint32_t flags = 0;
//            if (Keyboard::IsKeyDown(Key::LeftCtrl) || Keyboard::IsKeyDown(Key::RightCtrl))
//                flags |= MK_CONTROL;
//            if (Keyboard::IsKeyDown(Key::LeftShift) || Keyboard::IsKeyDown(Key::RightShift))
//                flags |= MK_SHIFT;
//            m_pRendererInfo->MouseDown((uint32_t)pt.X, (uint32_t)pt.Y, lastButton, flags);
//        }
//
//        void HandleMouseMove(Object^ sender, MouseEventArgs^ e)
//        {
//            auto pt = e->GetPosition(this);
//            uint32_t flags = 0;
//            if (Keyboard::IsKeyDown(Key::LeftCtrl) || Keyboard::IsKeyDown(Key::RightCtrl))
//                flags |= MK_CONTROL;
//            if (Keyboard::IsKeyDown(Key::LeftShift) || Keyboard::IsKeyDown(Key::RightShift))
//                flags |= MK_SHIFT;
//            m_pRendererInfo->MouseMove((uint32_t)pt.X, (uint32_t)pt.Y, flags);
//        }
//
//        void HandleKeyDown(Object^ sender, KeyEventArgs^ e)
//        {
//            auto vkey = KeyInterop::VirtualKeyFromKey(e->Key);
//            m_pRendererInfo->Key(vkey, 0);
//        }
//
//        void HostLoaded(Object^ sender, RoutedEventArgs^ e)
//        {
//            m_d3dimage = gcnew D3D11Image();
//            this->Source = m_d3dimage;
//            m_d3dimage->SetPixelSize((int)800, (int)800);
//
//            auto win = System::Windows::Application::Current->MainWindow;
//            auto winInteropHelper = gcnew System::Windows::Interop::WindowInteropHelper(win);
//            m_d3dimage->WindowOwner = winInteropHelper->Handle;
//            m_d3dimage->OnRender = gcnew System::Action<IntPtr, bool>(this, &Renderer::RenderFrame);
//
//            if (ShaderPath == nullptr)
//            {
//                auto root = Environment::GetEnvironmentVariable("CausticRoot");
//                if (root == nullptr)
//                    root = "g:/GitHub/Caustic";
//                ShaderPath = root + "/Debug";
//            }
//
//            // Setup the renderer
//            IntPtr ip = Marshal::StringToHGlobalUni(ShaderPath);
//            m_pRendererInfo->Setup(ip.ToPointer());
//            Marshal::FreeHGlobal(ip);
//
//            // Setup mouse event handler
//            this->MouseDown += gcnew System::Windows::Input::MouseButtonEventHandler(this, &Renderer::HandleMouseDown);
//            this->MouseUp += gcnew System::Windows::Input::MouseButtonEventHandler(this, &Renderer::HandleMouseUp);
//            this->MouseMove += gcnew System::Windows::Input::MouseEventHandler(this, &Renderer::HandleMouseMove);
//            this->KeyDown += gcnew System::Windows::Input::KeyEventHandler(this, &Renderer::HandleKeyDown);
//            m_d3dimage->RequestRender();
//        }
//
//        void CompositionTargetRendering(Object^ sender, EventArgs^ e)
//        {
//            RenderingEventArgs^ args = (RenderingEventArgs^)e;
//            if (this->lastRender != args->RenderingTime)
//            {
//                if (m_d3dimage)
//                    m_d3dimage->RequestRender();
//                lastRender = args->RenderingTime;
//            }
//        }
//
//        void HostResized(Object^ sender, SizeChangedEventArgs^ e)
//        {
//            double dpiScale = 1.0; // default value for 96 dpi
//
//            auto hwndTarget = (HwndTarget^)PresentationSource::FromVisual(this)->CompositionTarget;
//            if (hwndTarget != nullptr)
//            {
//                dpiScale = hwndTarget->TransformToDevice.M11;
//            }
//
//            int surfWidth = (int)(this->ActualWidth < 0 ? 0 : Math::Ceiling(this->ActualWidth * dpiScale));
//            int surfHeight = (int)(this->ActualHeight < 0 ? 0 : Math::Ceiling(this->ActualHeight * dpiScale));
//
//            if (m_d3dimage != nullptr)
//                m_d3dimage->SetPixelSize(surfWidth, surfHeight);
//
//            bool isVisible = (surfWidth != 0 && surfHeight != 0);
//            if (lastVisible != isVisible)
//            {
//                lastVisible = isVisible;
//                if (lastVisible)
//                {
//                    CompositionTarget::Rendering += gcnew System::EventHandler(this, &Renderer::CompositionTargetRendering);
//                }
//                else
//                {
//                    CompositionTarget::Rendering -= gcnew System::EventHandler(this, &Renderer::CompositionTargetRendering);
//                }
//            }
//        }
//
//        void RenderFrame(IntPtr surface, bool isNewSurface)
//        {
//            if (isNewSurface)
//                m_pRendererInfo->SetRenderTarget((void*)surface, isNewSurface);
//        }

    public:
        property String^ ShaderPath;

        Renderer()
        {
//            auto Owner = System::Windows::Application::Current->MainWindow;
//            Owner->Loaded += gcnew RoutedEventHandler(this, &Renderer::HostLoaded);
//            Owner->SizeChanged += gcnew SizeChangedEventHandler(this, &Renderer::HostResized);
//            m_pRendererInfo = new CRendererInfo();
//            this->Focusable = true;
        }

        ~Renderer()
        {
//            this->!Renderer();
        }

        !Renderer()
        {
//            this->MouseDown -= gcnew System::Windows::Input::MouseButtonEventHandler(this, &Renderer::HandleMouseDown);
//            this->MouseUp -= gcnew System::Windows::Input::MouseButtonEventHandler(this, &Renderer::HandleMouseUp);
//            this->MouseMove -= gcnew System::Windows::Input::MouseEventHandler(this, &Renderer::HandleMouseMove);
//            this->KeyDown -= gcnew System::Windows::Input::KeyEventHandler(this, &Renderer::HandleKeyDown);
//            CompositionTarget::Rendering -= gcnew System::EventHandler(this, &Renderer::CompositionTargetRendering);
//            delete m_pRendererInfo;
//            m_pRendererInfo = nullptr;
        }
    };
}
