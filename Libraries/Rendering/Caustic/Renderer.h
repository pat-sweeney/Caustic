//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\TextureMgr.h"
#include "Geometry\Mesh\Mesh.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\CritSec.h"
#include "Base\Core\Event.h"
#include "RenderMesh.h"
#include "Shader.h"
#include <memory>
#include <any>
#include <vector>
#include <atlbase.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Caustic
{
    const int c_RenderCmd_DrawMesh = 0; // Command ID for rendering a mesh
    const int c_RenderCmd_SetCamera = 1; // Command ID for setting the camera

    const int c_DefaultVertexVerion = 1; // Defines the default vertex version

    const int c_PassFirst = 0;
    const int c_PassObjID = 0;
    const int c_PassShadow = 1;
    const int c_PassOpaque = 2;
    const int c_PassTransparent = 3;
    const int c_PassEnvironment = 4;
    const int c_PassLast = c_PassEnvironment;
    const int c_PassAllPasses = (1 << c_PassLast) - 1;

    struct IRenderMaterial;
	struct IRenderable;

	struct CVertexBuffer
	{
		CComPtr<ID3D12Resource> m_spVB; // Our vertex buffer
		CComPtr<ID3D12Resource> m_spIB; // Our index buffer. Maybe nullptr
		uint32 m_vertexSize; // Size in bytes of each vertex
		uint32 m_numVertices; // Number of vertices
		uint32 m_numIndices; // Number of indices. Maybe 0
	public:
		CVertexBuffer() :
			m_vertexSize(0),
			m_numVertices(0),
			m_numIndices(0)
		{
		}
	};

    //**********************************************************************
    // CLight defines a simple point light
    //**********************************************************************
    class CLight : public IPointLight, public CRefCount
    {
    public:
        void Render(IRenderer * /*pRenderer*/) {}
    };

    //**********************************************************************
    class CRenderCtx : public IRenderCtx, public CRefCount
    {
        uint32 m_debugFlags;
        float m_NormalScale;
        int m_currentPass;
        bool m_passBlendable;

    public:
        friend class CRenderer;
        friend class CRenderable;

        CRenderCtx() :
            m_debugFlags(0),
            m_NormalScale(1.0f),
            m_currentPass(0),
            m_passBlendable(false)
        {
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderCtx
        //**********************************************************************
        virtual void SetDebugFlags(uint32 flags) override { m_debugFlags = flags; }
        virtual uint32 GetDebugFlags() override { return m_debugFlags; }
        virtual void SetNormalScale(float normalScale) override { m_NormalScale = normalScale; };
        virtual float GetNormalScale() override { return m_NormalScale; }
        virtual uint32 GetCurrentPass() override { return m_currentPass; }
        virtual bool PassBlendable() { return m_passBlendable; }
    };

    const int c_MaxShadowMaps = 4;
	static const int c_NumBackBuffers = 2;

    //**********************************************************************
    class CRenderer :
		public CRefCount,
        public IRenderer
    {
		void SetupDebugLayer();
		void CreateSwapChain(HWND hwnd);
		void AllocateBackBuffers();
		void AllocateDepthStencilBuffers();
		void CreateFences();
		void CreateRootSignature();
		void LoadDefaultShaders(const wchar_t *pFilename);
		void LoadShaderBlob(std::wstring &filename, ID3DBlob **ppBlob);
		void LoadShaderInfo(std::wstring &filename, IShaderInfo **ppShaderInfo);
		void LoadBasicGeometry();

		int m_width;
		int m_height;
		UINT m_numerator;
		UINT m_denominator;
		DXGI_SWAP_CHAIN_DESC m_swapDesc;
		CComPtr<IDXGIFactory5> m_spDXGIFactory;
		CComPtr<IDXGIAdapter4> m_spAdapter;
		CComPtr<ID3D12Device5> m_spDevice;
		CComPtr<ID3D12CommandQueue> m_spCmdQueue;
		CComPtr<ID3D12CommandAllocator> m_spCommandAllocator;
		CComPtr<ID3D12GraphicsCommandList4> m_spCommandList;
		CComPtr<ID3D12DescriptorHeap> m_spBackBufferHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hBackBuffers[c_NumBackBuffers];
		CComPtr<ID3D12Resource> m_spBackBuffers[c_NumBackBuffers];
		UINT m_backBufferSize;
		CComPtr<ID3D12DescriptorHeap> m_spDepthStencilHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_hDepthStencilBuffers[c_NumBackBuffers];
		CComPtr<ID3D12Resource> m_spDepthStencilBuffers[c_NumBackBuffers];
		UINT m_depthStencilBufferSize;
		UINT m_currentFrame;
		CComPtr<IDXGISwapChain4> m_spSwapChain;
		CComPtr<ID3D12Fence1> m_spFences[c_NumBackBuffers];
		HANDLE m_fenceEvents[c_NumBackBuffers];
		UINT m_fenceValue;
		CComPtr<ID3D12RootSignature> m_spRootSignature;
    public:

        std::vector<CRefObj<IRenderable>> m_singleObjs;             // List of individual renderable objects (outside scene graph)
        CComPtr<ID3D12Resource> m_spObjIDTexture;                   // Texture for rendering object IDs
        CComPtr<ID3D12Resource> m_spShadowTexture[c_MaxShadowMaps]; // Texture for shadow map

		CRefObj<IShaderMgr> m_spShaderMgr;
		CRefObj<ICamera> m_spCamera;                       // Camera to use for rendering
        CRefObj<ITextureMgr> m_spTextureMgr;
		CRefObj<IRenderCtx> m_spRenderCtx;                 // D3D Render context
		CVertexBuffer m_lineVB;
		CRefObj<IShader> m_spLineShader;
		std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> m_callback;

		void InitializeD3D(HWND hwnd);

        void RenderScene();
        void DrawSceneObjects(int pass);
    public:
        explicit CRenderer();
        virtual ~CRenderer();
        
        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IRenderer
        //**********************************************************************
		virtual DXGI_SAMPLE_DESC GetSampleDesc() override { return m_swapDesc.SampleDesc; }
		virtual DXGI_FORMAT GetFormat() override { return m_swapDesc.BufferDesc.Format; }
		virtual CComPtr<ID3D12RootSignature> GetRootSignature() override { return m_spRootSignature; }
		virtual CComPtr<ID3D12Device5> GetDevice() override { return m_spDevice; }
		virtual CRefObj<ICamera> GetCamera() override { return m_spCamera; }
        virtual CRefObj<ITextureMgr> GetTextureMgr() override { return m_spTextureMgr; }
		virtual uint32 GetFrameIndex() override { return m_currentFrame; }
		virtual CComPtr<ID3D12GraphicsCommandList4> GetCommandList() override { return m_spCommandList; }
		virtual void SetCamera(ICamera *pCamera) override { m_spCamera = pCamera; }
		virtual void Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice) override;
		virtual CRefObj<IShaderMgr> GetShaderMgr() override { return m_spShaderMgr; };
		virtual void BeginFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override;
		virtual void EndFrame() override;
		virtual void RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback) override;

        // Should all this light state be pushed onto the RenderCtx instead??
        virtual void GetRenderCtx(IRenderCtx **ppCtx) override;
		virtual void DrawLine(Vector3 p1, Vector3 p2, Vector4 clr) override;
		virtual void DrawMesh(IRenderMesh *pMesh) override {}
	};
}
