//**********************************************************************
// Copyright Patrick Sweeney 2015-2016
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base\Core\RefCount.h"
#include <functional>
#include <queue>
#include <type_traits>

//**********************************************************************
// File: RendererMarshaller.h
// Contains the declaration for the <CRendererMarshaller> class.
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Class: CRendererMarshaller
    // Implements <IRendererMarshaller>
    //
    // Members:
    // m_cs - Critical section used to control access to the render queue
    // m_spRenderer - Server side renderer
    // m_queue - Queue of client side renderer requests
    // m_exit - Flag to indicate whether the renderer is exiting
    // m_thread - Handle to the renderer thread
    //**********************************************************************
    class CRendererMarshaller :
        public IRenderer,
        public IRendererMarshaller,
        public CRefCount
    {
        CRITICAL_SECTION m_cs;
        CRefObj<IRenderer> m_spRenderer;
        std::queue<std::function<void()> > m_queue;
        bool m_exit;
        HANDLE m_thread;

        void AddLambda(std::function<void()> func);
    public:
        CRendererMarshaller();
        void MainLoop();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRendererMarshaller methods
        //**********************************************************************
        virtual void Initialize(HWND hwnd) override;
        virtual void Shutdown() override;
        virtual void SetMaxCmdLength() override {}
        virtual void GetRenderer(IRenderer **ppRenderer)
        {
            *ppRenderer = this;
            (*ppRenderer)->AddRef();
        }
        virtual void LoadTexture(const wchar_t *pPath, ITexture **ppTexture) override;
        virtual void LoadVideoTexture(const wchar_t *pPath, ITexture **ppTexture) override;
        virtual void SetSceneGraph(ISceneGraph *pSceneGraph) override;
        virtual void SaveScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) override;
        virtual void LoadScene(const wchar_t *pFilename, ISceneGraph *pSceneGraph) override;

        //**********************************************************************
        // IGraphics methods
        //**********************************************************************
        virtual CComPtr<ID3D11Device> GetDevice() override;
        virtual CComPtr<ID3D11DeviceContext> GetContext() override;
        virtual CRefObj<ICamera> GetCamera() override;

        //**********************************************************************
        // IRenderer methods
        //**********************************************************************
        virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) override;
        virtual void DrawMesh(ISubMesh *pMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat) override; // Draws a mesh
        virtual void RenderLoop() override; // Renderer entry point
        virtual void RenderFrame() override; // Have renderer draw and present next frame
        virtual void SetCamera(ICamera *pCamera) override; // Sets camera
        virtual void AddPointLight(IPointLight *pLight) override;
        virtual void GetRenderCtx(IRenderCtx **ppCtx) override;
        virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
        virtual void GetGraphics(IGraphics ** /*ppGraphics*/) override { return; }
    };
}
