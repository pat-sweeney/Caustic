//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\CausticFactory.h"
#include "Renderer.h"
#include "d3dx12.h"
#include <vector>
#include <any>
#include <dxgidebug.h>
#include <d3dcompiler.h>
#include "Base\Core\CritSec.h"
#include <d3d12.h>
#include <algorithm>
#include "Renderable.h"

namespace Caustic
{
	//**********************************************************************
	CRenderer::CRenderer() :
        m_numTextures(0),
        m_fenceValue(0),
        m_currentFrame(0),
        m_width(0),
        m_height(0),
        m_numerator(0),
        m_denominator(0),
        m_backBufferSize(0),
        m_depthStencilBufferSize(0)
    {
	}

	//**********************************************************************
	CRenderer::~CRenderer()
	{
	}

	//**********************************************************************
	void CRenderer::SetupDebugLayer()
	{
		CComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(__uuidof(ID3D12Debug), (void**)&debugController)))
			debugController->EnableDebugLayer();
		else
			OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
		CComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		UINT dxgiFactoryFlags;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue))))
		{
			dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
			DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
			{
				80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
			};
			DXGI_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
		}
		CT(CreateDXGIFactory2(dxgiFactoryFlags, __uuidof(IDXGIFactory5), (void**)&m_spDXGIFactory));
		CComPtr<IDXGIAdapter1> spAdapter;
		CT(m_spDXGIFactory->EnumAdapters1(0, &spAdapter));
		CT(spAdapter->QueryInterface(__uuidof(IDXGIAdapter4), (void**)&m_spAdapter));
		CComPtr<IDXGIOutput> spOutput;
		CT(m_spAdapter->EnumOutputs(0, &spOutput));
		UINT numModes;
		CT(spOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr));
		DXGI_MODE_DESC *modeDesc = new DXGI_MODE_DESC[numModes];
		ZeroMemory(modeDesc, sizeof(modeDesc) * numModes);
		CT(spOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modeDesc));
		int bestDist = MAXINT;
		UINT bestIndex;
		for (UINT i = 0; i < numModes; i++)
		{
			int deltaWidth = modeDesc[i].Width - m_width;
			int deltaHeight = modeDesc[i].Height - m_height;
			int dist = deltaWidth * deltaWidth + deltaHeight * deltaHeight;
			if (dist < bestDist)
			{
				bestDist = dist;
				bestIndex = i;
			}
		}
		m_numerator = modeDesc[bestIndex].RefreshRate.Numerator;
		m_denominator = modeDesc[bestIndex].RefreshRate.Denominator;
		m_width = modeDesc[bestIndex].Width;
		m_height = modeDesc[bestIndex].Height;
		delete[] modeDesc;

		CComPtr<ID3D12Debug> spDebugController0;
		CComPtr<ID3D12Debug1> spDebugController1;
		CT(D3D12GetDebugInterface(IID_PPV_ARGS(&spDebugController0)));
		CT(spDebugController0->QueryInterface(IID_PPV_ARGS(&spDebugController1)));
		spDebugController1->SetEnableGPUBasedValidation(true);
	}

	//**********************************************************************
	void CRenderer::CreateSwapChain(HWND hwnd)
	{
		// Create swap chain
		ZeroMemory(&m_swapDesc, sizeof(m_swapDesc));
		m_swapDesc.BufferCount = 2;
		m_swapDesc.BufferDesc.Width = m_width;
		m_swapDesc.BufferDesc.Height = m_height;
		m_swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		m_swapDesc.Windowed = true;
		m_swapDesc.OutputWindow = hwnd;
		m_swapDesc.BufferDesc.RefreshRate.Numerator = m_numerator;
		m_swapDesc.BufferDesc.RefreshRate.Denominator = m_denominator;
		m_swapDesc.SampleDesc.Count = 1;
		m_swapDesc.SampleDesc.Quality = 0;
		m_swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		m_swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		m_swapDesc.Flags = 0;
		CComPtr<IDXGISwapChain> spSwapChain;
		CT(m_spDXGIFactory->CreateSwapChain(m_spCmdQueue, &m_swapDesc, &spSwapChain));
		CT(spSwapChain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)&m_spSwapChain));
	}

	void CRenderer::AllocateDepthStencilBuffers()
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 2;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		CT(m_spDevice->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_spDepthStencilHeap));
		m_depthStencilBufferSize = m_spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		for (int i = 0; i < c_NumBackBuffers; i++)
		{
			D3D12_CLEAR_VALUE clearValue = {};
			clearValue.Format = DXGI_FORMAT_D32_FLOAT;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;
			m_spDevice->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_width, m_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&clearValue,
				__uuidof(ID3D12Resource), (void**)&m_spDepthStencilBuffers[i]);
			m_spDepthStencilBuffers[i]->SetName(L"m_spDepthStencilBuffer");

			m_hDepthStencilBuffers[i] = m_spDepthStencilHeap->GetCPUDescriptorHandleForHeapStart();
			m_hDepthStencilBuffers[i].ptr = m_hDepthStencilBuffers[0].ptr + i * m_depthStencilBufferSize;
			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
			depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
			m_spDevice->CreateDepthStencilView(m_spDepthStencilBuffers[i], &depthStencilDesc, m_hDepthStencilBuffers[i]);
			m_spDepthStencilBuffers[i]->SetName(L"m_spDepthStencilBuffers");
		}
	}

	void CRenderer::AllocateBackBuffers()
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 2;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		CT(m_spDevice->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_spBackBufferHeap));
		m_backBufferSize = m_spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		for (int i = 0; i < c_NumBackBuffers; i++)
		{
			m_hBackBuffers[i] = m_spBackBufferHeap->GetCPUDescriptorHandleForHeapStart();
			m_hBackBuffers[i].ptr = m_hBackBuffers[0].ptr + i * m_backBufferSize;
			CT(m_spSwapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&m_spBackBuffers[i]));
			m_spDevice->CreateRenderTargetView(m_spBackBuffers[i], NULL, m_hBackBuffers[i]);
			m_spBackBuffers[i]->SetName(L"m_spBackBuffer");
		}
		m_currentFrame = 0;
	}

	void CRenderer::CreateFences()
	{
		for (int i = 0; i < c_NumBackBuffers; i++)
		{
			CT(m_spDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence1), (void**)&m_spFences[i]));
			m_fenceEvents[i] = ::CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
		}
		m_fenceValue = 0;
	}

	void CRenderer::CreateRootSignature()
	{
		D3D12_ROOT_CONSTANTS constants;
		constants.Num32BitValues = 16;
		constants.ShaderRegister = 0;
		constants.RegisterSpace = 0;

		D3D12_ROOT_PARAMETER  rootParameters[2];
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParameters[0].Constants = constants;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

        D3D12_DESCRIPTOR_RANGE  descriptorTableRanges[2];
        descriptorTableRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        descriptorTableRanges[1].NumDescriptors = 2;
        descriptorTableRanges[1].BaseShaderRegister = 0;
        descriptorTableRanges[1].RegisterSpace = 0;
        descriptorTableRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        descriptorTableRanges[0].NumDescriptors = 2;
        descriptorTableRanges[0].BaseShaderRegister = 1;
        descriptorTableRanges[0].RegisterSpace = 0;
        descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
        descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges);
        descriptorTable.pDescriptorRanges = &descriptorTableRanges[0];

        rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        rootParameters[1].DescriptorTable = descriptorTable;
        rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

        // create a static sampler
		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 1,
			&sampler,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		CComPtr<ID3DBlob> spSignatureBlob;
		CT(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &spSignatureBlob, nullptr));
		CT(m_spDevice->CreateRootSignature(0, spSignatureBlob->GetBufferPointer(), spSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_spRootSignature)));
	}

	//**********************************************************************
	void CRenderer::InitializeD3D(HWND hwnd)
	{
		std::unique_ptr<CRenderCtx> spCtx(new CRenderCtx());
		m_spRenderCtx = spCtx.release();

		RECT rect;
		GetClientRect(hwnd, &rect);
		m_width = rect.right - rect.left;
		m_height = rect.bottom - rect.top;

		SetupDebugLayer();

		CT(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device5), (void**)&m_spDevice));

		// Create the command queue
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cmdQueueDesc.NodeMask = 1;
		CT(m_spDevice->CreateCommandQueue(&cmdQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&m_spCmdQueue));

		CT(m_spDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&m_spCommandAllocator));
		CT(m_spDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, m_spCommandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList4), (void**)&m_spCommandList));
		m_spCommandAllocator->SetName(L"m_spCommandAllocator");
		m_spCommandList->SetName(L"m_spCommandList");

		CreateSwapChain(hwnd);
		AllocateBackBuffers();
		AllocateDepthStencilBuffers();
		CreateFences();
		CreateRootSignature();

		//	// Create texture for rendering object IDs
		//   CD3D11_TEXTURE2D_DESC texObjID(DXGI_FORMAT_R32_UINT, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
		//   CT(m_spDevice->CreateTexture2D(&texObjID, NULL, &m_spObjIDTexture));
		//   D3D11_RENDER_TARGET_VIEW_DESC objIDRVDesc;
		//   objIDRVDesc.Format = texObjID.Format;
		//   objIDRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		//   objIDRVDesc.Texture2D.MipSlice = 0;
		//   CT(m_spDevice->CreateRenderTargetView(m_spObjIDTexture, &objIDRVDesc, &m_spObjIDRTView));
	}

	//**********************************************************************
	void CRenderer::LoadShaderBlob(std::wstring &filename, ID3DBlob **ppBlob)
	{
		HANDLE f = ::CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (f != INVALID_HANDLE_VALUE)
		{
			DWORD dwSize = GetFileSize(f, nullptr);
			CT(D3DCreateBlob(dwSize, ppBlob));
			DWORD bytesRead;
			ReadFile(f, (*ppBlob)->GetBufferPointer(), dwSize, &bytesRead, nullptr);
			CloseHandle(f);
		}
	}

	//**********************************************************************
	void CRenderer::LoadShaderInfo(std::wstring &filename, IShaderInfo **ppShaderInfo)
	{
		CCausticFactory::Instance()->CreateShaderInfo(filename.c_str(), ppShaderInfo);
	}

	//**********************************************************************
	void CRenderer::LoadDefaultShaders(const wchar_t *pFolder)
	{
        IShaderMgr *pShaderMgr = CShaderMgr::Instance();
        WIN32_FIND_DATA findData;
		std::wstring fn(pFolder);
		fn += L"\\*.shi";
		HANDLE h = ::FindFirstFile(fn.c_str(), &findData);
		if (h == INVALID_HANDLE_VALUE)
			CT(E_FAIL);
		while (true)
		{
			std::wstring fn(findData.cFileName);
			std::size_t found = fn.rfind(L".shi");
			if (found != std::wstring::npos)
			{
				CComPtr<ID3DBlob> spPixelShaderBlob;
				CComPtr<ID3DBlob> spVertexShaderBlob;
				CComPtr<IShaderInfo> spShaderInfo;
				CComPtr<IShader> spShader;
				std::wstring shaderName(fn.substr(0, found));
				LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_PS.cso", &spPixelShaderBlob);
				LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_VS.cso", &spVertexShaderBlob);
				LoadShaderInfo(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L".shi", &spShaderInfo);
				CCausticFactory::Instance()->CreateShader(this, shaderName.c_str(), spVertexShaderBlob, spPixelShaderBlob, spShaderInfo, &spShader);
                pShaderMgr->RegisterShader(shaderName.c_str(), spShader);
			}
			if (!::FindNextFile(h, &findData))
				break;
		}
	}

	//**********************************************************************
	void CRenderer::LoadBasicGeometry()
	{
	}

    //**********************************************************************
    // Setup is called at the start of the application to initialize
    // the server side of our renderer
    // hwnd HWND to use for drawing
    //**********************************************************************
    void CRenderer::Setup(HWND hwnd, std::wstring &shaderFolder, bool createDebugDevice)
    {
		InitializeD3D(hwnd);
	  
        CRefObj<ICausticFactory> spFactory = CCausticFactory::Instance();

	    // Create a default camera
		spFactory->CreateCamera(false, &m_spCamera);

        // Create our texture manager
        const uint32 c_NumTexUploads = 10;
        const uint64 c_TextureMemSize = 4LL * 1024LL * 1024LL * 1024LL;
        spFactory->CreateTextureMgr(this, c_NumTexUploads, c_TextureMemSize, &m_spTextureMgr);

        AllocateDescriptorHeap();

		LoadDefaultShaders(shaderFolder.c_str());

        IShaderMgr *pShaderMgr = CShaderMgr::Instance();
        pShaderMgr->FindShader(L"Line", &m_spLineShader);

        LoadBasicGeometry();

        CT(m_spCommandList->Close());
    }
    
    //**********************************************************************
    void CRenderer::GetRenderCtx(IRenderCtx **ppCtx)
    {
        (*ppCtx) = m_spRenderCtx;
        if (m_spRenderCtx)
            (*ppCtx)->AddRef();
    }

#if 0
    void CRenderer::DrawInfinitePlane()
    {
#ifdef SUPPORT_FULLQUAD
        UINT offset = 0;
        UINT vertexSize = sizeof(SVertex_5);
        ID3D11DeviceContext *pContext = GetContext();
        CComPtr<ID3D11RasterizerState> spRasterState;
        D3D11_RASTERIZER_DESC rasDesc;
        ZeroMemory(&rasDesc, sizeof(rasDesc));
        rasDesc.CullMode = D3D11_CULL_NONE;
        rasDesc.DepthClipEnable = TRUE;
        rasDesc.FillMode = D3D11_FILL_SOLID;
        m_spDevice->CreateRasterizerState(&rasDesc, &spRasterState);
        pContext->RSSetState(spRasterState);
        pContext->IASetVertexBuffers(0, 1, &m_spFullQuadVB, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_spFullQuadIB, DXGI_FORMAT_R32_UINT, 0);
        Vector3 eye;
        GetCamera()->GetPosition(&eye, nullptr, nullptr, nullptr, nullptr, nullptr);
        Float3 vEye(eye.x, eye.y, eye.z);
        m_spFullQuadShader->SetPSParam(L"eye", std::any(vEye));
        m_spFullQuadShader->BeginRender(this);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(6, 0, 0);
        m_spFullQuadShader->EndRender(this);
#endif // SUPPORT_FULLQUAD
    }
#endif

    void CRenderer::CreateLineVB()
    {
        m_lineVB.m_numIndices = 0;
        m_lineVB.m_numVertices = 2;
        m_lineVB.m_vertexSize = sizeof(CLineVertex);
        
        // Create our upload buffer
        UINT vbSize = (UINT)(2 * sizeof(CLineVertex));
        CD3DX12_RESOURCE_DESC descUpload = CD3DX12_RESOURCE_DESC::Buffer(vbSize);
        CD3DX12_HEAP_PROPERTIES heapdescUpload(D3D12_HEAP_TYPE_UPLOAD);
        CT(m_spDevice->CreateCommittedResource(&heapdescUpload, D3D12_HEAP_FLAG_NONE,
            &descUpload, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, __uuidof(ID3D12Resource),
            (void**)&m_lineVB.m_spUploadVB));
        CT(m_lineVB.m_spUploadVB->SetName(L"m_lineVB.m_spUploadVB"));

        // Create final buffer
        CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(vbSize);
        CD3DX12_HEAP_PROPERTIES heapdesc(D3D12_HEAP_TYPE_DEFAULT);
        CT(m_spDevice->CreateCommittedResource(&heapdesc, D3D12_HEAP_FLAG_NONE,
            &desc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, __uuidof(ID3D12Resource), (void**)&m_lineVB.m_spVB));
        CT(m_lineVB.m_spVB->SetName(L"m_lineVB.m_spVB"));

        CLineVertex verts[2] = {
            { 0.0f, 0.0f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
        };

        // store vertex buffer in upload heap
        D3D12_SUBRESOURCE_DATA vertexData = {};
        vertexData.pData = reinterpret_cast<BYTE*>(verts);
        vertexData.RowPitch = vbSize;
        vertexData.SlicePitch = vbSize;

        // we are now creating a command with the command list to copy the data from
        // the upload heap to the default heap
        UpdateSubresources(m_spCommandList, m_lineVB.m_spVB, m_lineVB.m_spUploadVB, 0, 0, 1, &vertexData);

        // transition the vertex buffer data from copy destination state to vertex buffer state
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_lineVB.m_spVB, D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
        m_spCommandList->ResourceBarrier(1, &barrier);
    }

    void CRenderer::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        if (m_lineVB.m_spVB == nullptr)
            CreateLineVB();

        UINT offset = 0;
		D3D12_VERTEX_BUFFER_VIEW vbView = {};
		vbView.BufferLocation = m_lineVB.m_spVB->GetGPUVirtualAddress();
		vbView.SizeInBytes = m_lineVB.m_numVertices * m_lineVB.m_vertexSize;
		vbView.StrideInBytes = m_lineVB.m_vertexSize;
		m_spCommandList->IASetVertexBuffers(0, 1, &vbView);
        Matrix m;
        m.x[0] = p2.x - p1.x;     m.x[4] = 0.0f;            m.x[8] = 0.0f;            m.x[12] = 0.0f;
        m.x[1] = 0.0f;            m.x[5] = p2.y - p1.y;     m.x[9] = 0.0f;            m.x[13] = 0.0f;
        m.x[2] = 0.0f;            m.x[6] = 0.0f;            m.x[10] = p2.z - p1.z;    m.x[14] = 0.0f;
        m.x[3] = p1.x;            m.x[7] = p1.y;            m.x[11] = p1.z;           m.x[15] = 1.0f;
        m_spLineShader->SetVSParam(L"endpoints", std::any(m));
        Float4 color(clr.x, clr.y, clr.z, clr.w);
        m_spLineShader->SetPSParam(L"color", std::any(color));
        std::vector<CRefObj<IPointLight>> lights;
        m_spLineShader->BeginRender(this, nullptr, nullptr, lights);
        m_spCommandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		m_spCommandList->DrawInstanced(2, 1, 0, 0);
        m_spLineShader->EndRender(this);
    }
	
	void CRenderer::DrawSceneObjects(int pass)
    {
        // Render any single objects
        std::vector<CRefObj<IPointLight>> lights;
        for (size_t i = 0; i < m_singleObjs.size(); i++)
        {
            if (m_singleObjs[i]->InPass(pass))
                m_singleObjs[i]->Render(this, lights, m_spRenderCtx);
        }
    }

    void CRenderer::RenderScene()
    {
//        DrawInfinitePlane();
		if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWorldAxis)
        {
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 10.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 10.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        }

		if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayLightDir)
        {
          //  for (size_t i = 0; i < m_lights.size(); i++)
          //      DrawLine(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        for (uint32 pass = c_PassFirst; pass <= c_PassLast; pass++)
        {
			if (m_callback != nullptr)
				m_callback(this, m_spRenderCtx, pass);
#ifdef SUPPORT_OBJID
            CRenderCtx *pCtx = (CRenderCtx*)m_spRenderCtx;
            pCtx->m_currentPass = pass;
            pCtx->m_passBlendable = true;
            if (pass == c_PassObjID)
            {
                pCtx->m_passBlendable = false;
                // Setup render target
                m_spContext->OMSetRenderTargets(1, &m_spObjIDRTView, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
#ifdef SUPPORT_SHADOW_MAPPING
            if (pass == c_PassShadow)
            {
                int numShadowPasses = (m_lights.size() < c_MaxShadowMaps) ? m_lights.size() : c_MaxShadowMaps;
                for (int i = 0; i < numShadowPasses; i++)
                {
                    m_spContext->OMSetRenderTargets(1, &m_spShadowRTView[i], m_spStencilView);
                    FLOAT bgClr[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                    m_spContext->ClearRenderTargetView(m_spShadowRTView[i], bgClr);
                    m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
                    // Reset the camera to be from the lights perspective
                    CRefObj<ICamera> spCamera;
                    CreateCamera(true, &spCamera);
                    spCamera->SetPosition(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
                    this->SetCamera(spCamera);
                    DrawSceneObjects(pass);
                    // Restore default render targets
                    m_spContext->OMSetRenderTargets(1, &m_spRTView, m_spStencilView);
                }
            }
            else
#endif // SUPPORT_SHADOW_MAPPING
            if (pass == c_PassTransparent)
            {
                std::vector<int> order;
                order.resize(m_singleObjs.size());
                std::sort(m_singleObjs.begin(), m_singleObjs.end(),
                    [&](CRefObj<IRenderable> &left, CRefObj<IRenderable> &right)->bool
                    {
                        Vector3 cameraPos;
                        GetCamera()->GetPosition(&cameraPos, nullptr, nullptr, nullptr, nullptr, nullptr);
                        float dist1 = (left->GetPos() - cameraPos).Length();
                        float dist2 = (right->GetPos() - cameraPos).Length();
                        if (dist1 < dist2)
                            return true;
                        return false;
                    }
                );
                DrawSceneObjects(pass);
            }

#ifdef SUPPORT_OBJID
            if (pass == c_PassObjID)
            {
                // Restore default render targets
                m_spContext->OMSetRenderTargets(1, &m_spRTView, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
        }
    }

    void CRenderer::SetConstantBuffers(SConstantBuffer *pVertexCB, SConstantBuffer *pPixelCB)
    {
        for (int i = 0; i < 2; i++)
        {
            SConstantBuffer *pBuffer = (i == 0) ? pVertexCB : pPixelCB;
            if (pBuffer->m_bufferSize == 0)
                continue;
            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
            cbvDesc.BufferLocation = pBuffer->m_spBuffer[m_currentFrame]->GetGPUVirtualAddress();
            cbvDesc.SizeInBytes = ((pBuffer->m_bufferSize + 255) / 256) * 256;    // CB size is required to be 256-byte aligned.
            D3D12_CPU_DESCRIPTOR_HANDLE h = m_spDescriptorHeap[m_currentFrame]->GetCPUDescriptorHandleForHeapStart();
            UINT offset = m_spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
            h.ptr += i * offset;
            m_spDevice->CreateConstantBufferView(&cbvDesc, h);
        }
    }
    
    void CRenderer::SetTexture(ID3D12Resource *pTexture)
    {
        D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        ZeroMemory(&srvDesc, sizeof(srvDesc));
        srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        D3D12_CPU_DESCRIPTOR_HANDLE h = m_spDescriptorHeap[m_currentFrame]->GetCPUDescriptorHandleForHeapStart();
        h.ptr += (2 + m_numTextures) * m_spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        m_spDevice->CreateShaderResourceView(pTexture, &srvDesc, h);
        m_numTextures++;
    }

    void CRenderer::AllocateDescriptorHeap()
    {
        // Allocate heap for descriptors and create constant buffer views
        for (int j = 0; j < c_MaxFrames; j++)
        {
            D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
            heapDesc.NumDescriptors = 2 /* pixel+vertex constant buffer */ + c_MaxTextures /* hack! */;
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            heapDesc.NodeMask = 0;
            CT(m_spDevice->CreateDescriptorHeap(&heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_spDescriptorHeap[j]));
        }
    }

	void CRenderer::BeginFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
	{
        m_numTextures = 0;
        m_callback = renderCallback;
		// Reset our command list
		CT(m_spCommandAllocator->Reset());
		CT(m_spCommandList->Reset(m_spCommandAllocator, nullptr));
		m_spCommandList->SetGraphicsRootSignature(m_spRootSignature);
		//ID3D12DescriptorHeap *descriptorHeaps[] = { m_spBackBufferHeap, m_spDepthStencilHeap };
		//m_spCommandList->SetDescriptorHeaps(2, descriptorHeaps);
		//m_spCommandList->SetGraphicsRootDescriptorTable(1, m_spSRVDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

        ID3D12DescriptorHeap *ppHeaps[] = { m_spDescriptorHeap[m_currentFrame] };
        m_spCommandList->SetDescriptorHeaps(1, ppHeaps);
        m_spCommandList->SetGraphicsRootDescriptorTable(1, m_spDescriptorHeap[m_currentFrame]->GetGPUDescriptorHandleForHeapStart());

		// Make sure our backbuffer is in the correct state
		m_spCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_spBackBuffers[m_currentFrame], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		// Setup our render targets
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_hDepthStencilBuffers[m_currentFrame]);
		m_spCommandList->OMSetRenderTargets(1, &m_hBackBuffers[m_currentFrame], false, &dsvHandle);

		// Clear the render target
		static float color[4] = { 1.0f, 0.4f, 0.4f, 1.0f };
		m_spCommandList->ClearRenderTargetView(m_hBackBuffers[m_currentFrame], color, 0, nullptr);
		m_spCommandList->ClearDepthStencilView(m_hDepthStencilBuffers[m_currentFrame], D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH, 1000.0f, 0, 0, nullptr);

		// Establish viewport
		D3D12_VIEWPORT viewport = { 0.0f, 0.0f, (float)m_width, (float)m_height, 0.0f, 1.0f };
		D3D12_RECT scissorRect = { 0, 0, (LONG)m_width, (LONG)m_height };
		m_spCommandList->RSSetViewports(1, &viewport);
		m_spCommandList->RSSetScissorRects(1, &scissorRect);
	}

	void CRenderer::EndFrame()
	{
		m_spCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_spBackBuffers[m_currentFrame], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		CT(m_spCommandList->Close());

		m_spCmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&m_spCommandList.p);

		m_currentFrame = (++m_currentFrame == c_MaxFrames) ? 0 : m_currentFrame;
		m_spSwapChain->Present(1, 0);

		m_fenceValue++;
		CT(m_spCmdQueue->Signal(m_spFences[m_currentFrame], m_fenceValue));
		CT(m_spFences[m_currentFrame]->SetEventOnCompletion(m_fenceValue, m_fenceEvents[m_currentFrame]));
		WaitForSingleObject(m_fenceEvents[m_currentFrame], INFINITE);
	}

    //**********************************************************************
    // RenderFrame is typically called from the render loop to render
    // the next frame.
    //**********************************************************************
    void CRenderer::RenderFrame(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
		BeginFrame(renderCallback);
		// RenderScene();
		EndFrame();
	}
}
