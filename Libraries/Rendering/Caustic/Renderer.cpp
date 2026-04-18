//**********************************************************************
// Copyright Patrick Sweeney 2015-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <any>
#include <d3d11.h>
#include <d3d11_4.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <algorithm>
#include <cfloat>
#include <dxgi1_6.h>
#include <string>
#include <atlbase.h>
#include <memory>
#include <chrono>

module Rendering.Caustic.Renderer;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.CritSec;
import Base.Core.ConvertStr;
import Base.Math.BBox;
import Rendering.Caustic.Texture;
import Rendering.Caustic.Renderable;
import Rendering.Caustic.Sampler;
import Rendering.Caustic.ShaderMgr;
import Rendering.Caustic.CausticFactory;
import Rendering.Caustic.ICausticFactory;
import Rendering.Caustic.ISpotLight;
import Rendering.Caustic.IAreaLight;
import Rendering.Caustic.IParticleSystem;
import Rendering.Caustic.ICamera;
import Geometry.Mesh.RenderTypes;

//**********************************************************************
// File: Renderer.cpp
// Contains code for the main renderer
//**********************************************************************

namespace Caustic
{
    //**********************************************************************
    // Constructor: CRenderer
    //**********************************************************************
    CRenderer::CRenderer() :
        m_waitForShutdown(true, true),
        m_exitThread(false),
        m_depthTestEnabled(true),
        m_postProcessEnabled(true),
        m_bloomEnabled(true),
        m_fxaaEnabled(true),
        m_ssaoEnabled(true),
        m_bloomThreshold(1.0f),
        m_bloomIntensity(0.5f),
        m_exposure(1.0f),
        m_frustumCullingEnabled(true),
        m_fogEnabled(false),
        m_fogFrameCounter(0),
        m_fogDensity(0.02f),
        m_fogColor(0.5f, 0.6f, 0.7f),
        m_fogHeightFalloff(0.1f),
        m_fogScattering(0.3f),
        m_fogMaxDistance(200.0f),
        m_fogStartHeight(0.0f)
    {
        m_freezeEvent = CreateEvent(nullptr, true, false, nullptr);
        m_freeze = 0;
        m_finalViewport = BBox2(0.0f, 0.0f, 1.0f, 1.0f);
    }

    //**********************************************************************
    // Destructor: ~CRenderer
    //**********************************************************************
    CRenderer::~CRenderer()
    {
        m_exitThread = true;
        m_waitForShutdown.Wait(INFINITE);
    }

    //**********************************************************************
    CRefObj<IRenderMesh> CRenderer::ToRenderMesh(IMesh* pMesh, IShader* pShader)
    {
        CRefObj<ICausticFactory> spFactory = CreateCausticFactory();
        CRefObj<IShaderInfo> spShaderInfo = pShader->GetShaderInfo();
        CRefObj<IRenderMesh> spRenderMesh = spFactory->CreateRenderMesh();
        std::vector<MeshData> mdv = pMesh->ToMeshData(GetDevice(), spShaderInfo->VertexLayout(), spShaderInfo->GetVertexSize());
        for (MeshData md : mdv)
        {
            CRefObj<IRenderSubMesh> spRenderSubMesh = spFactory->CreateRenderSubMesh();
            spRenderSubMesh->SetMeshData(md);
            spRenderSubMesh->SetName(md.m_name.c_str());
            spRenderMesh->AddSubMesh(spRenderSubMesh);
        }
        return spRenderMesh;
    }

    void CRenderer::ToRenderMaterials(IMesh* pMesh, IShader* pShader, IRenderMesh* pRenderMesh, IMaterialAttrib* pDefaultMaterial)
    {
        CRefObj<ICausticFactory> spFactory = CreateCausticFactory();
        for (uint32_t i = 0; i < pMesh->NumberSubMeshes(); i++)
        {
            ISubMesh* pSubMesh = pMesh->GetSubMesh(i);
            CRefObj<IRenderSubMesh> spRenderSubMesh = pRenderMesh->GetSubMesh(i);

            // Assign appropriate materials
            CRefObj<IMaterialAttrib> spMaterialAttrib = pMesh->GetMaterial(pSubMesh->GetMaterialID());
            CRefObj<IRenderMaterial> spRenderMaterial = spFactory->CreateRenderMaterial(this, (spMaterialAttrib) ? spMaterialAttrib.p : pDefaultMaterial, pShader);
            spRenderSubMesh->SetFrontMaterial(spRenderMaterial);

            // For now assume no back material
        }
    }

    //**********************************************************************
    // Method: DeviceWindowResized
    // Set <IRenderer::DeviceWindowResized>.
    //**********************************************************************
    void CRenderer::DeviceWindowResized(uint32_t width, uint32_t height)
    {
        CGraphicsBase::DeviceWindowResizedInternal(width, height);

        // Create half-res fog render target
        {
            uint32_t fogW = m_BBDesc.Width / 2;
            uint32_t fogH = m_BBDesc.Height / 2;
            fogW = (fogW < 1) ? 1 : fogW;
            fogH = (fogH < 1) ? 1 : fogH;
            CComPtr<ID3D11Texture2D> spFogTex;
            CD3D11_TEXTURE2D_DESC fogDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, fogW, fogH,
                1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&fogDesc, NULL, &spFogTex));
            m_spFogRTV = nullptr;
            CT(m_spDevice->CreateRenderTargetView(spFogTex, NULL, &m_spFogRTV));
            CComPtr<ID3D11ShaderResourceView> spFogSRV;
            CT(m_spDevice->CreateShaderResourceView(spFogTex, NULL, &spFogSRV));
            m_spFogRT = CRefObj<ITexture>(new CTexture(spFogTex, spFogSRV));
        }

        // Create depth copy texture for decal pass (avoids DSV/SRV conflict)
        {
            CComPtr<ID3D11Texture2D> spDepthCopyTex;
            CD3D11_TEXTURE2D_DESC depthCopyDesc(DXGI_FORMAT_R32_FLOAT, m_BBDesc.Width, m_BBDesc.Height,
                1, 1, D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&depthCopyDesc, NULL, &spDepthCopyTex));
            CComPtr<ID3D11ShaderResourceView> spDepthCopySRV;
            CT(m_spDevice->CreateShaderResourceView(spDepthCopyTex, NULL, &spDepthCopySRV));
            m_spDepthCopy = CRefObj<ITexture>(new CTexture(spDepthCopyTex, spDepthCopySRV));
        }
    }

    //**********************************************************************
    // Method: InitializeD3D
    // Initializes the server side renderer. Clients should call this method indirectly (via <IRenderer::Setup>)
    // at application startup.
    //
    // Parameters:
    // hwnd - HWND to use for drawing
    // viewport - viewport to render final viewport into
    //**********************************************************************
    void CRenderer::InitializeD3D(HWND hwnd, BBox2 &viewport)
    {
        CGraphicsBase::InitializeD3D(hwnd, viewport);

#ifdef SUPPORT_OBJECT_IDS
        // Create texture for rendering object IDs
        CD3D11_TEXTURE2D_DESC texObjID(DXGI_FORMAT_R32_UINT, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
        CT(m_spDevice->CreateTexture2D(&texObjID, NULL, &m_spObjIDTexture));
        D3D11_RENDER_TARGET_VIEW_DESC objIDRVDesc;
        objIDRVDesc.Format = texObjID.Format;
        objIDRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        objIDRVDesc.Texture2D.MipSlice = 0;
        CT(m_spDevice->CreateRenderTargetView(m_spObjIDTexture, &objIDRVDesc, &m_spObjIDRTView));
#endif // SUPPORT_OBJECT_IDS

        // Make sure rasterizer is setup the way we expect it to be
        //   - BackFace Culling on
        //   - Front Faces are counter clockwise
        D3D11_RASTERIZER_DESC desc;
        desc.FrontCounterClockwise = false;
        desc.CullMode = D3D11_CULL_BACK;
        desc.AntialiasedLineEnable = false;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0.0f;
        desc.DepthClipEnable = true;
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.MultisampleEnable = true;
        desc.SlopeScaledDepthBias = 0.0f;
        desc.ScissorEnable = false;
        CT(m_spDevice->CreateRasterizerState(&desc, &m_spRasterizerState));
    }
    
    //**********************************************************************
    // Method: Setup
    // See <IRenderer::Setup>.
    //**********************************************************************
    void CRenderer::Setup(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool createDebugDevice, bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        if (startFrozen)
            Freeze();
        m_renderThreadId = GetCurrentThreadId();
        CGraphicsBase::Setup(hwnd, viewport, createDebugDevice, desktopIndex);

        if (shaderFolder.empty())
        {
            auto wstrExePath = GetExecutablePath();
            shaderFolder = wstrExePath + L"\\Shaders";
        }
        m_spShaderMgr = CRefObj<IShaderMgr>(new CShaderMgr());
        LoadShaders(shaderFolder.c_str());

        m_spLineShader = m_spShaderMgr->FindShader(L"Line");
        m_spQuadShader = m_spShaderMgr->FindShader(L"ScreenQuad");

        // Load post-processing shaders (these may not exist yet; fail gracefully)
        try { m_spBloomExtractShader = m_spShaderMgr->FindShader(L"BloomExtract"); } catch (...) {}
        try { m_spBloomBlurShader = m_spShaderMgr->FindShader(L"BloomBlur"); } catch (...) {}
        try { m_spBloomCompositeShader = m_spShaderMgr->FindShader(L"BloomComposite"); } catch (...) {}
        try { m_spFXAAShader = m_spShaderMgr->FindShader(L"FXAA"); } catch (...) {}
        try { m_spSSAOShader = m_spShaderMgr->FindShader(L"SSAO"); } catch (...) {}
        try { m_spSSAOBlurShader = m_spShaderMgr->FindShader(L"SSAOBlur"); } catch (...) {}

        // Create point-light shadow cubemaps
        m_numPointShadowLights = 0;
        m_iblDirty = false;
        for (int i = 0; i < c_MaxPointShadowLights; i++)
        {
            m_spPointShadowCubemap[i] = CreateCubemapDepthTexture(this, c_PointShadowMapSize);
        }

        // Create spot-light shadow maps
        m_numSpotShadowLights = 0;
        for (int i = 0; i < c_MaxSpotShadowLights; i++)
        {
            CComPtr<ID3D11Texture2D> spTex;
            CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R32_TYPELESS, c_SpotShadowMapSize, c_SpotShadowMapSize,
                1, 1, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&texDesc, NULL, &spTex));
            CD3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D32_FLOAT);
            CT(m_spDevice->CreateDepthStencilView(spTex, &dsvDesc, &m_spSpotShadowDSV[i]));
            CComPtr<ID3D11ShaderResourceView> spSRV;
            CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R32_FLOAT);
            CT(m_spDevice->CreateShaderResourceView(spTex, &srvDesc, &spSRV));
            m_spSpotShadowMap[i] = CRefObj<ITexture>(new CTexture(spTex, spSRV));
        }

        // Load IBL shaders
        try { m_spBRDFLUTShader = m_spShaderMgr->FindShader(L"BRDFLUT"); } catch (...) {}
        try { m_spIrradianceShader = m_spShaderMgr->FindShader(L"IrradianceConvolution"); } catch (...) {}
        try { m_spPrefilterShader = m_spShaderMgr->FindShader(L"PrefilterEnvMap"); } catch (...) {}
        try { m_spTileCullShader = m_spShaderMgr->FindShader(L"TileLightCull"); } catch (...) {}
        try { m_spSSRShader = m_spShaderMgr->FindShader(L"SSR"); } catch (...) {}
        try { m_spFogShader = m_spShaderMgr->FindShader(L"VolumetricFog"); } catch (...) {}
        try { m_spFogCompositeShader = m_spShaderMgr->FindShader(L"FogComposite"); } catch (...) {}
        try { m_spDecalShader = m_spShaderMgr->FindShader(L"Decal"); } catch (...) {}
        m_tiledLightingEnabled = false;
        m_ssrEnabled = false;

        // Create decal blend state (SrcAlpha/InvSrcAlpha) and rasterizer state (front-face cull)
        {
            D3D11_BLEND_DESC blendDesc = {};
            blendDesc.RenderTarget[0].BlendEnable = TRUE;
            blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
            blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            CT(m_spDevice->CreateBlendState(&blendDesc, &m_spDecalBlendState));

            D3D11_RASTERIZER_DESC rastDesc = {};
            rastDesc.FillMode = D3D11_FILL_SOLID;
            rastDesc.CullMode = D3D11_CULL_FRONT; // Render back faces so decals work when camera is inside
            rastDesc.FrontCounterClockwise = FALSE;
            rastDesc.DepthClipEnable = TRUE;
            CT(m_spDevice->CreateRasterizerState(&rastDesc, &m_spDecalRastState));
        }

        // Generate BRDF LUT (one-time, doesn't depend on environment map)
        if (m_spBRDFLUTShader != nullptr)
        {
            m_spBRDFLUT = CreateTexture(this, 512, 512, DXGI_FORMAT_R16G16_FLOAT,
                (D3D11_CPU_ACCESS_FLAG)0, (D3D11_BIND_FLAG)(D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE));

            // Render BRDF LUT
            CComPtr<ID3D11RenderTargetView> spBRDFRTV;
            CT(m_spDevice->CreateRenderTargetView(m_spBRDFLUT->GetD3DTexture(), NULL, &spBRDFRTV));

            CComPtr<ID3D11RenderTargetView> spOldRT;
            CComPtr<ID3D11DepthStencilView> spOldDS;
            m_spContext->OMGetRenderTargets(1, &spOldRT, &spOldDS);
            D3D11_VIEWPORT oldVP = m_viewport;

            m_spContext->OMSetRenderTargets(1, &spBRDFRTV.p, nullptr);
            D3D11_VIEWPORT brdfVP = {};
            brdfVP.Width = 512.0f;
            brdfVP.Height = 512.0f;
            brdfVP.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &brdfVP);

            std::vector<CRefObj<ILight>> emptyLights;
            m_spBRDFLUTShader->SetVSParamFloat(L"minu", 0.0f);
            m_spBRDFLUTShader->SetVSParamFloat(L"minv", 0.0f);
            m_spBRDFLUTShader->SetVSParamFloat(L"maxu", 1.0f);
            m_spBRDFLUTShader->SetVSParamFloat(L"maxv", 1.0f);
            m_spBRDFLUTShader->BeginRender(this, nullptr, emptyLights, nullptr);
            UINT brdfOffset = 0;
            UINT brdfVertexSize = sizeof(CQuadVertex);
            m_spContext->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &brdfVertexSize, &brdfOffset);
            m_spContext->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT_R32_UINT, 0);
            m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            m_spContext->DrawIndexed(6, 0, 0);
            m_spBRDFLUTShader->EndRender(this);

            m_spContext->OMSetRenderTargets(1, &spOldRT.p, spOldDS);
            m_viewport = oldVP;
            m_spContext->RSSetViewports(1, &m_viewport);
        }

        //**********************************************************************
        // Create vertex buffer used to draw lines
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(CLineVertex) * 2, D3D11_BIND_VERTEX_BUFFER);
            CLineVertex *pVertexBuffer = new CLineVertex[2];
            pVertexBuffer[0].x = 0.0f;
            pVertexBuffer[0].y = 0.0f;
            pVertexBuffer[0].z = 0.0f;
            pVertexBuffer[1].x = 1.0f;
            pVertexBuffer[1].y = 1.0f;
            pVertexBuffer[1].z = 1.0f;
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = pVertexBuffer;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&bufdesc, &data, &m_spLineVB));
        }

        //**********************************************************************
        // Create vertex buffer used to draw screen space quads
        //**********************************************************************
        {
            CD3D11_BUFFER_DESC bufdesc(sizeof(CQuadVertex) * 4, D3D11_BIND_VERTEX_BUFFER);
            CQuadVertex quadPts[4] = {
                { -1.0f, -1.0f, 0.9f, 0.0f, 1.0f },
                { -1.0f, +1.0f, 0.9f, 0.0f, 0.0f, },
                { +1.0f, +1.0f, 0.9f, 1.0f, 0.0f, },
                { +1.0f, -1.0f, 0.9f, 1.0f, 1.0f },
            };
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = quadPts;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&bufdesc, &data, &m_spQuadVB));

            CD3D11_BUFFER_DESC indexbufdesc(sizeof(UINT) * 6, D3D11_BIND_INDEX_BUFFER);
            UINT quadIndices[2][3] = {
                { 0, 2, 1 },
                { 0, 3, 2 },
            };
            data.pSysMem = quadIndices;
            data.SysMemPitch = 0;
            data.SysMemSlicePitch = 0;
            CT(m_spDevice->CreateBuffer(&indexbufdesc, &data, &m_spQuadIB));
        }

        //**********************************************************************
        // Create vertex/index buffer for decal unit cube [-0.5, +0.5]^3
        //**********************************************************************
        {
            struct CubeVertex { float x, y, z; };
            CubeVertex cubeVerts[8] = {
                { -0.5f, -0.5f, -0.5f }, { -0.5f, +0.5f, -0.5f },
                { +0.5f, +0.5f, -0.5f }, { +0.5f, -0.5f, -0.5f },
                { -0.5f, -0.5f, +0.5f }, { -0.5f, +0.5f, +0.5f },
                { +0.5f, +0.5f, +0.5f }, { +0.5f, -0.5f, +0.5f }
            };
            CD3D11_BUFFER_DESC cubeVBDesc(sizeof(cubeVerts), D3D11_BIND_VERTEX_BUFFER);
            D3D11_SUBRESOURCE_DATA cubeVBData = { cubeVerts, 0, 0 };
            CT(m_spDevice->CreateBuffer(&cubeVBDesc, &cubeVBData, &m_spCubeVB));

            UINT cubeIndices[36] = {
                0,1,2, 0,2,3, // front
                4,6,5, 4,7,6, // back
                0,4,5, 0,5,1, // left
                3,2,6, 3,6,7, // right
                1,5,6, 1,6,2, // top
                0,3,7, 0,7,4  // bottom
            };
            CD3D11_BUFFER_DESC cubeIBDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
            D3D11_SUBRESOURCE_DATA cubeIBData = { cubeIndices, 0, 0 };
            CT(m_spDevice->CreateBuffer(&cubeIBDesc, &cubeIBData, &m_spCubeIB));
        }
    }

    //**********************************************************************
    // Method: EnableDepthTest
    // See <IRenderer::EnableDepthTest>
    //**********************************************************************
    bool CRenderer::EnableDepthTest(bool enable)
    {
        CheckThread();
        bool oldEnable = m_depthTestEnabled;
        
        CComPtr<ID3D11DepthStencilState> oldState;
        UINT oldStencil;
        m_spContext->OMGetDepthStencilState(&oldState, &oldStencil);
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        oldState->GetDesc(&depthStencilDesc);
        depthStencilDesc.DepthEnable = enable;
        CComPtr<ID3D11DepthStencilState> spState;
        CT(m_spDevice->CreateDepthStencilState(&depthStencilDesc, &spState));
        m_spContext->OMSetDepthStencilState(spState, oldStencil);
        m_depthTestEnabled = enable;
        return oldEnable;
    }
    
    //**********************************************************************
    // Method: Freeze
    // See <IRenderer::Freeze>
    //**********************************************************************
    void CRenderer::Freeze()
    {
        m_freeze++;
        if (m_freeze == 1)
            ResetEvent(m_freezeEvent);
    }

    //**********************************************************************
    // Method: Unfreeze
    // See <IRenderer::Unfreeze>
    //**********************************************************************
    void CRenderer::Unfreeze()
    {
        if (m_freeze > 0)
        {
            m_freeze--;
            if (m_freeze == 0)
                SetEvent(m_freezeEvent);
        }
    }

    //**********************************************************************
    // Method: IsFrozen
    // See <IRenderer::IsFrozen>
    //**********************************************************************
    bool CRenderer::IsFrozen()
    {
        return (m_freeze == 0) ? false : true;
    }

    //**********************************************************************
    // Method: RunOnRenderer
    // See <IRenderer>
    //**********************************************************************
    void CRenderer::RunOnRenderer(std::function<void(IRenderer*)> callback, bool wait /* = false */)
    {
        callback(this);
    }

    //**********************************************************************
    // Method: LoadShaderBlob
    // Loads the shader into a blob from the specified file.
    //
    // Parameters:
    // filename - Name of file to load from
    // ppBlob - Returns the created shader blob
    //**********************************************************************
    void CRenderer::LoadShaderBlob(const std::wstring &filename, ID3DBlob **ppBlob)
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
    // Method: LoadShaderInfo
    // Loads the shader info (*.shi) file (describes the layout and parameters of a given shader)
    //
    // Parameters:
    // filename - Name of shader info file (*.shi)
    //
    // Returns:
    // Returns the new shader info object
    //**********************************************************************
    CRefObj<IShaderInfo> CRenderer::LoadShaderInfo(const std::wstring &filename)
    {
        return CCausticFactory::Instance()->CreateShaderInfo(filename.c_str());
    }

    //**********************************************************************
    // Method: DrawScreenQuadWithCustomShader
    // See <IRenderer::DrawScreenQuadWithCustomShader>
    //**********************************************************************
    void CRenderer::DrawScreenQuadWithCustomShader(IShader *pShader, float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler* pSampler, bool disableDepth /* = false */)
    {
        CheckThread();
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
#endif
#ifdef _DEBUG
        spCtx2->BeginEventInt(L"DrawScreenQuadWithCustomShader", 0);
#endif
        ID3D11DeviceContext* pContext = GetContext();
        D3D11_RASTERIZER_DESC desc;
        desc.FrontCounterClockwise = false;
        desc.CullMode = D3D11_CULL_NONE;
        desc.AntialiasedLineEnable = false;
        desc.DepthBias = 0;
        desc.DepthBiasClamp = 0.0f;
        desc.DepthClipEnable = true;
        desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        desc.MultisampleEnable = true;
        desc.SlopeScaledDepthBias = 0.0f;
        desc.ScissorEnable = false;
        CComPtr<ID3D11RasterizerState> spRasterizerState;
        CT(m_spDevice->CreateRasterizerState(&desc, &spRasterizerState));
        pContext->RSSetState(spRasterizerState);

        CComPtr<ID3D11DepthStencilState> oldState;
        UINT oldStencil;
        if (disableDepth)
        {
            pContext->OMGetDepthStencilState(&oldState, &oldStencil);

            D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
            ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
            depthStencilDesc.DepthEnable = false;
            CComPtr<ID3D11DepthStencilState> spState;
            CT(m_spDevice->CreateDepthStencilState(&depthStencilDesc, &spState));
            pContext->OMSetDepthStencilState(spState, 0);
        }

        UINT offset = 0;
        UINT vertexSize = sizeof(CQuadVertex);
        pContext->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &vertexSize, &offset);
        pContext->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
        pShader->SetPSParam(L"tex", std::any(CRefObj<ITexture>(pTexture)));
        std::vector<CRefObj<ILight>> lights;

        CRefObj<ISampler> spSampler;
        if (pSampler == nullptr)
        {
            spSampler = CCausticFactory::Instance()->CreateSampler(this, pTexture);
            pSampler = spSampler.p;
        }
        pShader->SetPSParam(L"s", std::any(CSamplerRef(pSampler)));


        pShader->SetVSParamFloat(L"minu", minU);
        pShader->SetVSParamFloat(L"minv", minV);
        pShader->SetVSParamFloat(L"maxu", maxU);
        pShader->SetVSParamFloat(L"maxv", maxV);

        pShader->BeginRender(this, nullptr, lights, nullptr);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->DrawIndexed(6, 0, 0);
        pShader->EndRender(this);

        if (disableDepth)
            pContext->OMSetDepthStencilState(oldState, oldStencil);

#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: DrawScreenQuad
    // See <IRenderer::DrawScreenQuad>
    //**********************************************************************
    void CRenderer::DrawScreenQuad(float minU, float minV, float maxU, float maxV, ITexture* pTexture, ISampler *pSampler, bool disableDepth /* = false */)
    {
        CheckThread();
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
#endif
#ifdef _DEBUG
        spCtx2->BeginEventInt(L"DrawScreenQuad", 0);
#endif
        DrawScreenQuadWithCustomShader(m_spQuadShader, minU, minV, maxU, maxV, pTexture, pSampler, disableDepth);

#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: LoadShaders
    // Loads all the shaders found in the specified folder.
    //
    // Parameters:
    // pFolder - path to shaders to load
    //**********************************************************************
    void CRenderer::LoadShaders(const wchar_t* pFolder)
    {
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
                CComPtr<ID3DBlob> spComputeShaderBlob;
                CRefObj<IShaderInfo> spShaderInfo;
                CRefObj<IShader> spShader;
                std::wstring shaderName(fn.substr(0, found));
                spShaderInfo = LoadShaderInfo(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L".shi");
                if (spShaderInfo->HasShader(EShaderType::TypePixelShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_PS.cso", &spPixelShaderBlob);
                if (spShaderInfo->HasShader(EShaderType::TypeVertexShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_VS.cso", &spVertexShaderBlob);
                if (spShaderInfo->HasShader(EShaderType::TypeComputeShader))
                    LoadShaderBlob(std::wstring(const_cast<wchar_t*>(pFolder)) + std::wstring(L"\\") + shaderName + L"_CS.cso", &spComputeShaderBlob);
                spShader = CCausticFactory::Instance()->CreateShader(this, shaderName.c_str(), spVertexShaderBlob, spPixelShaderBlob, spComputeShaderBlob, spShaderInfo);
                m_spShaderMgr->RegisterShader(shaderName.c_str(), spShader);
            }
            if (!::FindNextFile(h, &findData))
                break;
        }
    }

    //**********************************************************************
    // Method: DrawMesh
    // See <IRenderer::DrawMesh>
    //**********************************************************************
    void CRenderer::DrawMesh(IRenderSubMesh *pSubMesh, IMaterialAttrib *pMaterial, ITexture *pTexture, IShader *pShader, DirectX::XMMATRIX &mat)
    {
        CRefObj<IRenderMaterial> spFrontMaterial;
        spFrontMaterial = CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader);
        spFrontMaterial->SetTexture(this, L"diffuseTexture", pTexture, EShaderAccess::PixelShader);
        CRefObj<IRenderMaterial> spBackMaterial;
        if (pSubMesh->GetMeshFlags() & EMeshFlags::TwoSided)
        {
            spBackMaterial = CCausticFactory::Instance()->CreateRenderMaterial(this, pMaterial, pShader);
            spBackMaterial->SetTexture(this, L"diffuseTexture", pTexture, EShaderAccess::PixelShader);
        }
        std::unique_ptr<CRenderable> spRenderable(new CRenderable(pSubMesh, spFrontMaterial, spBackMaterial, mat));
        m_singleObjs.push_back(spRenderable.release());
    }

    //**********************************************************************
    // Method: AddPointLight
    // See <IRenderer::AddPointLight>
    //**********************************************************************
    void CRenderer::AddPointLight(IPointLight *pLight)
    {
        CheckThread();
        m_lights.push_back(CRefObj<ILight>(pLight));
    }

    //**********************************************************************
    // Method: GetRenderCtx
    // See <IRenderer::GetRenderCtx>
    //**********************************************************************
    CRefObj<IRenderCtx> CRenderer::GetRenderCtx()
    {
        CheckThread();
        return CRefObj<IRenderCtx>(m_spRenderCtx);
    }

    //**********************************************************************
    // Method: ClearDepth
    // See <IRenderer::ClearDepth>
    //**********************************************************************
    void CRenderer::ClearDepth()
    {
        CheckThread();
        m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    //**********************************************************************
    // Method: DrawLine
    // See <IRenderer::DrawLine>
    //**********************************************************************
    void CRenderer::DrawLine(Vector3 p1, Vector3 p2, Vector4 clr)
    {
        CheckThread();
        UINT offset = 0;
        UINT vertexSize = sizeof(CLineVertex);
        ID3D11DeviceContext *pContext = GetContext();
        pContext->IASetVertexBuffers(0, 1, &m_spLineVB.p, &vertexSize, &offset);
        Matrix m;
        m.x[0] = p2.x - p1.x;    m.x[4] = 0.0f;           m.x[8] = 0.0f;           m.x[12] = 0.0f;
        m.x[1] = 0.0f;           m.x[5] = p2.y - p1.y;    m.x[9] = 0.0f;           m.x[13] = 0.0f;
        m.x[2] = 0.0f;           m.x[6] = 0.0f;           m.x[10] = p2.z - p1.z;   m.x[14] = 0.0f;
        m.x[3] = p1.x;           m.x[7] = p1.y;           m.x[11] = p1.z;          m.x[15] = 1.0f;
        m_spLineShader->SetVSParam(L"endpoints", std::any(m));
        Float4 color(clr.x, clr.y, clr.z, clr.w);
        m_spLineShader->SetPSParam(L"color", std::any(color));
        std::vector<CRefObj<ILight>> lights;
        m_spLineShader->BeginRender(this, nullptr, lights, nullptr);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        pContext->Draw(2, 0);
        m_spLineShader->EndRender(this);
    }

    //**********************************************************************
    // Method: ComputeCascadeSplits
    // Computes the split depths for cascaded shadow maps using a practical
    // split scheme that blends logarithmic and uniform distributions.
    //
    // Parameters:
    // nearClip - camera near clip distance
    // farClip - camera far clip distance
    // splitDepths - output array of c_NumCascades split depths
    //**********************************************************************
    void CRenderer::ComputeCascadeSplits(float nearClip, float farClip, float splitDepths[c_NumCascades])
    {
        float clipRange = farClip - nearClip;
        float ratio = farClip / nearClip;
        for (int i = 0; i < c_NumCascades; i++)
        {
            float p = (float)(i + 1) / (float)c_NumCascades;
            float logSplit = nearClip * powf(ratio, p);
            float uniformSplit = nearClip + clipRange * p;
            splitDepths[i] = c_CascadeSplitLambda * logSplit + (1.0f - c_CascadeSplitLambda) * uniformSplit;
        }
    }

    //**********************************************************************
    // Method: ComputeCascadeViewProj
    // Computes the orthographic light view-projection matrix for a single
    // cascade by fitting to the camera frustum slice.
    //
    // Parameters:
    // pCamera - the main camera
    // lightDir - normalized direction of the directional light
    // nearSplit - near depth of this cascade slice
    // farSplit - far depth of this cascade slice
    // outViewProj - resulting light view-projection matrix
    //**********************************************************************
    void CRenderer::ComputeCascadeViewProj(ICamera* pCamera, const Vector3& lightDir, float nearSplit, float farSplit, DirectX::XMMATRIX& outViewProj)
    {
        float fov, aspectRatio, nearZ, farZ;
        pCamera->GetParams(&fov, &aspectRatio, &nearZ, &farZ);

        // Compute frustum corners in world space for this cascade slice
        DirectX::XMMATRIX viewMatrix = pCamera->GetView();
        DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearSplit, farSplit);
        DirectX::XMMATRIX viewProjInv = DirectX::XMMatrixInverse(nullptr, viewMatrix * projMatrix);

        // NDC corners of a unit cube
        DirectX::XMFLOAT3 ndcCorners[8] = {
            { -1.0f, -1.0f, 0.0f }, { -1.0f,  1.0f, 0.0f },
            {  1.0f, -1.0f, 0.0f }, {  1.0f,  1.0f, 0.0f },
            { -1.0f, -1.0f, 1.0f }, { -1.0f,  1.0f, 1.0f },
            {  1.0f, -1.0f, 1.0f }, {  1.0f,  1.0f, 1.0f }
        };

        // Transform NDC corners to world space
        DirectX::XMFLOAT3 worldCorners[8];
        DirectX::XMFLOAT3 center = { 0.0f, 0.0f, 0.0f };
        for (int i = 0; i < 8; i++)
        {
            DirectX::XMVECTOR corner = DirectX::XMVector3TransformCoord(
                DirectX::XMLoadFloat3(&ndcCorners[i]), viewProjInv);
            DirectX::XMStoreFloat3(&worldCorners[i], corner);
            center.x += worldCorners[i].x;
            center.y += worldCorners[i].y;
            center.z += worldCorners[i].z;
        }
        center.x /= 8.0f;
        center.y /= 8.0f;
        center.z /= 8.0f;

        // Build light view matrix looking at the frustum center from the light direction
        DirectX::XMVECTOR vLightDir = DirectX::XMVectorSet(lightDir.x, lightDir.y, lightDir.z, 0.0f);
        vLightDir = DirectX::XMVector3Normalize(vLightDir);
        DirectX::XMVECTOR vCenter = DirectX::XMLoadFloat3(&center);
        DirectX::XMVECTOR vUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

        // If light direction is nearly parallel to up, use a different up vector
        float dotUp = fabsf(DirectX::XMVectorGetX(DirectX::XMVector3Dot(vLightDir, vUp)));
        if (dotUp > 0.99f)
            vUp = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

        DirectX::XMVECTOR vEye = DirectX::XMVectorSubtract(vCenter, DirectX::XMVectorScale(vLightDir, 100.0f));
        DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(vEye, vCenter, vUp);

        // Find min/max in light space to compute tight orthographic bounds
        float minX = FLT_MAX, maxX = -FLT_MAX;
        float minY = FLT_MAX, maxY = -FLT_MAX;
        float minZ = FLT_MAX, maxZ = -FLT_MAX;
        for (int i = 0; i < 8; i++)
        {
            DirectX::XMVECTOR cornerLS = DirectX::XMVector3TransformCoord(
                DirectX::XMLoadFloat3(&worldCorners[i]), lightView);
            DirectX::XMFLOAT3 ls;
            DirectX::XMStoreFloat3(&ls, cornerLS);
            minX = (std::min)(minX, ls.x);
            maxX = (std::max)(maxX, ls.x);
            minY = (std::min)(minY, ls.y);
            maxY = (std::max)(maxY, ls.y);
            minZ = (std::min)(minZ, ls.z);
            maxZ = (std::max)(maxZ, ls.z);
        }

        // Stabilize the shadow map: snap to texel-aligned increments to prevent shimmer
        float cascadeTexelSize = (maxX - minX) / 4096.0f; // 4096 = per-cascade resolution
        minX = floorf(minX / cascadeTexelSize) * cascadeTexelSize;
        maxX = floorf(maxX / cascadeTexelSize) * cascadeTexelSize;
        minY = floorf(minY / cascadeTexelSize) * cascadeTexelSize;
        maxY = floorf(maxY / cascadeTexelSize) * cascadeTexelSize;

        // Extend depth range to catch shadow casters behind the frustum
        float zPadding = (maxZ - minZ) * 2.0f;
        minZ -= zPadding;

        DirectX::XMMATRIX lightProj = DirectX::XMMatrixOrthographicOffCenterLH(minX, maxX, minY, maxY, minZ, maxZ);
        outViewProj = lightView * lightProj;
    }

    //**********************************************************************
    // Method: DrawSceneObjects
    // First calls the renderCallback provided. Then renders each Renderable.
    //
    // Parameters:
    // pass - which pass are we rendering
    // renderCallback - Render callback
    //**********************************************************************
    //**********************************************************************
    // Method: IsBoxInFrustum
    // Tests whether an AABB intersects the view frustum defined by 6 planes.
    // Uses the "positive vertex" test (conservative — no false negatives).
    //**********************************************************************
    bool CRenderer::IsBoxInFrustum(const BBox3& bbox, const DirectX::XMVECTOR frustumPlanes[6])
    {
        for (int i = 0; i < 6; i++)
        {
            // Find the "positive vertex" — the corner most in the direction of the plane normal
            float px = (DirectX::XMVectorGetX(frustumPlanes[i]) >= 0.0f) ? bbox.maxPt.x : bbox.minPt.x;
            float py = (DirectX::XMVectorGetY(frustumPlanes[i]) >= 0.0f) ? bbox.maxPt.y : bbox.minPt.y;
            float pz = (DirectX::XMVectorGetZ(frustumPlanes[i]) >= 0.0f) ? bbox.maxPt.z : bbox.minPt.z;
            DirectX::XMVECTOR pVertex = DirectX::XMVectorSet(px, py, pz, 1.0f);
            if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(frustumPlanes[i], pVertex)) < 0.0f)
                return false; // entirely outside this plane
        }
        return true;
    }

    void CRenderer::DrawSceneObjects(int pass, std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
        if (renderCallback)
            (renderCallback)(this, m_spRenderCtx, pass);

        // Render any single objects
        if (pass == c_PassShadow)
        {
            // Find the first directional light that casts shadows
            int shadowLightIndex = -1;
            for (int i = 0; i < (int)m_lights.size(); i++)
            {
                if (m_lights[i]->GetType() == ELightType::DirectionalLight &&
                    m_lights[i]->GetCastsShadows())
                {
                    shadowLightIndex = i;
                    break;
                }
            }

            if (shadowLightIndex >= 0)
            {
                Vector3 lightDir = m_lights[shadowLightIndex]->GetDirection();
                float nearClip = m_spCamera->GetNear();
                float farClip = m_spCamera->GetFar();

                // Compute cascade split depths
                ComputeCascadeSplits(nearClip, farClip, m_cascadeData.cascadeSplitDepths);

                // Clear the entire shadow atlas once
                m_spContext->ClearDepthStencilView(m_spShadowMapStencilView[c_HiResShadowMap], D3D11_CLEAR_DEPTH, 1.0f, 0);

                // Render each cascade
                for (int cascade = 0; cascade < c_NumCascades; cascade++)
                {
                    float cascadeNear = (cascade == 0) ? nearClip : m_cascadeData.cascadeSplitDepths[cascade - 1];
                    float cascadeFar = m_cascadeData.cascadeSplitDepths[cascade];

                    // Compute tight orthographic projection for this cascade
                    ComputeCascadeViewProj(m_spCamera, lightDir, cascadeNear, cascadeFar,
                        m_cascadeData.cascadeViewProj[cascade]);

                    // Set up render target and viewport for this cascade tile (2x2 grid)
                    ShadowMapRenderState rs;
                    m_spContext->OMGetRenderTargets(1, &rs.m_spOldRT, &rs.m_spOldStencil);
                    rs.m_spOldCamera = m_spCamera;
                    rs.m_viewport = m_viewport;
                    m_shadowMapRenderState.push(rs);
                    m_spContext->OMSetRenderTargets(0, nullptr, m_spShadowMapStencilView[c_HiResShadowMap].p);

                    // Set viewport to the cascade tile in 2x2 grid (each tile is 4096x4096)
                    int tileX = cascade % 2;
                    int tileY = cascade / 2;
                    ZeroMemory(&m_viewport, sizeof(m_viewport));
                    m_viewport.TopLeftX = (float)(tileX * 4096);
                    m_viewport.TopLeftY = (float)(tileY * 4096);
                    m_viewport.Width = 4096.0f;
                    m_viewport.Height = 4096.0f;
                    m_viewport.MinDepth = 0.0f;
                    m_viewport.MaxDepth = 1.0f;
                    m_spContext->RSSetViewports(1, &m_viewport);

                    // Create a camera from the cascade's light view-projection
                    // We extract the view/proj from the combined matrix by constructing
                    // a camera that matches. Since the shadow shader uses worldViewProj
                    // directly, we set the camera to identity and use the cascade VP.
                    CRefObj<ICamera> spShadowCamera = CreateCamera(true);
                    Vector3 lightPos = m_lights[shadowLightIndex]->GetPosition();
                    spShadowCamera->SetPosition(lightPos, lightDir, Vector3(0.0f, 1.0f, 0.0f));

                    // Override with our computed orthographic projection
                    float fov, aspect, nearZ, farZ;
                    spShadowCamera->GetParams(&fov, &aspect, &nearZ, &farZ);
                    spShadowCamera->SetParams(fov, aspect, nearZ, farZ);
                    this->SetCamera(spShadowCamera);

                    // Render all shadow-casting objects
                    for (size_t j = 0; j < m_singleObjs.size(); j++)
                    {
                        if (m_singleObjs[j]->InPass(pass))
                            m_singleObjs[j]->Render(this, m_lights, m_spRenderCtx);
                    }

                    // Restore state
                    ShadowMapRenderState rsPop = m_shadowMapRenderState.top();
                    this->SetCamera(rsPop.m_spOldCamera);
                    m_viewport = rsPop.m_viewport;
                    m_spContext->RSSetViewports(1, &m_viewport);
                    m_shadowMapRenderState.pop();
                    m_spContext->OMSetRenderTargets(1, &rsPop.m_spOldRT.p, rsPop.m_spOldStencil);
                }
            }

            // Render point-light shadow cubemaps
            RenderPointShadows(pass, renderCallback);
            RenderSpotShadows(pass, renderCallback);
        }
        else
        {
            // Extract frustum planes for culling (if enabled)
            DirectX::XMVECTOR frustumPlanes[6];
            if (m_frustumCullingEnabled)
            {
                DirectX::XMMATRIX vp = DirectX::XMMatrixMultiply(
                    m_spCamera->GetView(), m_spCamera->GetProjection());
                DirectX::XMFLOAT4X4 m;
                DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(vp));
                // Left:   row3 + row0
                frustumPlanes[0] = DirectX::XMVectorSet(m._14 + m._11, m._24 + m._21, m._34 + m._31, m._44 + m._41);
                // Right:  row3 - row0
                frustumPlanes[1] = DirectX::XMVectorSet(m._14 - m._11, m._24 - m._21, m._34 - m._31, m._44 - m._41);
                // Bottom: row3 + row1
                frustumPlanes[2] = DirectX::XMVectorSet(m._14 + m._12, m._24 + m._22, m._34 + m._32, m._44 + m._42);
                // Top:    row3 - row1
                frustumPlanes[3] = DirectX::XMVectorSet(m._14 - m._12, m._24 - m._22, m._34 - m._32, m._44 - m._42);
                // Near:   row2
                frustumPlanes[4] = DirectX::XMVectorSet(m._13, m._23, m._33, m._43);
                // Far:    row3 - row2
                frustumPlanes[5] = DirectX::XMVectorSet(m._14 - m._13, m._24 - m._23, m._34 - m._33, m._44 - m._43);
                for (int i = 0; i < 6; i++)
                    frustumPlanes[i] = DirectX::XMPlaneNormalize(frustumPlanes[i]);
            }

            for (size_t i = 0; i < m_singleObjs.size(); i++)
            {
                if (m_singleObjs[i]->InPass(pass))
                {
                    if (m_frustumCullingEnabled)
                    {
                        BBox3 bbox;
                        if (m_singleObjs[i]->GetBBox(&bbox) && !IsBoxInFrustum(bbox, frustumPlanes))
                            continue; // culled
                    }
                    m_singleObjs[i]->Render(this, m_lights, m_spRenderCtx);
                }
            }
        }
    }
    
    //**********************************************************************
    CRefObj<ITexture> CRenderer::GetShadowmapTexture(int whichShadowMap)
    {
        return CRefObj<ITexture>(new CTexture(m_spShadowTexture[whichShadowMap], m_spShadowSRView[whichShadowMap]));
    }

    //**********************************************************************
    void CRenderer::SetShadowmapViewport(int whichShadowMap, int lightMapIndex)
    {
        int lx = lightMapIndex % m_shadowMapLightWidth[whichShadowMap];
        int ly = lightMapIndex / m_shadowMapLightHeight[whichShadowMap];
        ZeroMemory(&m_viewport, sizeof(m_viewport));
        m_viewport.TopLeftX = float(lx * m_shadowMapWidth[whichShadowMap] / m_shadowMapLightWidth[whichShadowMap]);
        m_viewport.TopLeftY = float(ly * m_shadowMapHeight[whichShadowMap] / m_shadowMapLightHeight[whichShadowMap]);
        m_viewport.Width = (float)(m_shadowMapWidth[whichShadowMap] / m_shadowMapLightWidth[whichShadowMap]);
        m_viewport.Height = (float)(m_shadowMapHeight[whichShadowMap] / m_shadowMapLightHeight[whichShadowMap]);
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_spContext->RSSetViewports(1, &m_viewport);
    }

    //**********************************************************************
    // We create 3 different shadow maps:
    //     high res - 8192x8192 x 16 lights
    //     medium res - 4096x2048 x 8 lights
    //     low res - 2048x2048 x 4 lights
    // Each of these shadow maps are broken down into tiles for each light
    // in the scene. The high res map is broken down into 16 submaps (one
    // for each light in the scene), the medium res map for 8 lights, and
    // the low res map for 4 lights.
    //    <------------- 8192 ----------->      <------------- 4096 ----------->     <-- 2048 ------>
    // ^ +-------+-------+-------+-------+   ^ +-------+-------+-------+-------+  ^ +-------+-------+
    // | |       |       |       |       |   | |       |       |       |       |  | |       |       |
    // | |   0   |   1   |   2   |   3   |     |   0   |   1   |   2   |   3   |    |   0   |   1   |
    // | |       |       |       |       |   2 |       |       |       |       |  2 |       |       |
    // | |-------+-------+-------+-------+   0 |-------+-------+-------+-------+  0 |-------+-------+
    //   |       |       |       |       |   4 |       |       |       |       |  4 |       |       |
    // 8 |   4   |   5   |   6   |   7   |   8 |   4   |   5   |   6   |   7   |  8 |   2   |   3   |
    // 1 |       |       |       |       |   | |       |       |       |       |  | |       |       |
    // 9 +-------+-------+-------+-------+   v +-------+-------+-------+-------+  v +-------+-------+
    // 2 |       |       |       |       |
    //   |   8   |   9   |  10   |  11   |
    // | |       |       |       |       |
    // | |-------+-------+-------+-------+
    // | |       |       |       |       |
    // | |  12   |  13   |  14   |  15   |
    // | |       |       |       |       |
    // v +-------+-------+-------+-------+
    //**********************************************************************
    void CRenderer::SelectShadowmap(int whichShadowMap, int lightMapIndex, std::vector<CRefObj<ILight>>& lights, IShader* pShader)
    {
        if (lights.size() == 0)
            return;
        pShader->SetPSParam(L"shadowMapTexture", std::any(m_spShadowTexture[whichShadowMap]));

        // Pass cascade view-projection matrices and split depths to shaders
        for (int i = 0; i < c_NumCascades; i++)
        {
            // Convert XMMATRIX to Caustic::Matrix for the shader param system
            DirectX::XMFLOAT4X4 f4x4;
            DirectX::XMStoreFloat4x4(&f4x4, m_cascadeData.cascadeViewProj[i]);
            Matrix mat(reinterpret_cast<float*>(&f4x4));
            pShader->SetVSParam(L"cascadeViewProj", i, std::any(mat));
            Float4 splitDepth(m_cascadeData.cascadeSplitDepths[i], 0.0f, 0.0f, 0.0f);
            pShader->SetPSParam(L"cascadeSplitDepths", i, std::any(splitDepth));
        }

        // Bind point-light shadow cubemap if available
        if (m_numPointShadowLights > 0 && m_spPointShadowCubemap[0] != nullptr)
        {
            pShader->SetPSParam(L"pointShadowMap", std::any(m_spPointShadowCubemap[0]));
            // Find the point light's range for depth comparison
            for (int i = 0; i < (int)lights.size(); i++)
            {
                if (lights[i]->GetType() == ELightType::PointLight && lights[i]->GetCastsShadows())
                {
                    float range = lights[i]->GetRange();
                    pShader->SetPSParamFloat(L"pointShadowFarPlane", range);
                    break;
                }
            }
        }

        // Bind IBL textures if available
        if (m_spIrradianceMap != nullptr && m_spPrefilteredMap != nullptr && m_spBRDFLUT != nullptr)
        {
            pShader->SetPSParam(L"irradianceMap", std::any(m_spIrradianceMap));
            pShader->SetPSParam(L"prefilteredMap", std::any(m_spPrefilteredMap));
            pShader->SetPSParam(L"brdfLUT", std::any(m_spBRDFLUT));
            pShader->SetPSParamInt(L"useIBL", 1);
        }
        else
        {
            pShader->SetPSParamInt(L"useIBL", 0);
        }

        // Bind tiled lighting data if available
        if (m_tiledLightingEnabled && m_spLightBuffer != nullptr && m_spTileLightBuffer != nullptr)
        {
            pShader->SetPSParam(L"lightBuffer", std::any(m_spLightBuffer));
            pShader->SetPSParam(L"tileLightData", std::any(m_spTileLightBuffer));
            pShader->SetPSParamInt(L"useTiledLighting", 1);
            int numTilesX = (m_BBDesc.Width + 15) / 16;
            pShader->SetPSParamInt(L"numTilesX", numTilesX);
        }
        else
        {
            pShader->SetPSParamInt(L"useTiledLighting", 0);
        }

        // Bind spot-light shadow maps
        pShader->SetPSParamInt(L"numSpotShadowLights", m_numSpotShadowLights);
        for (int i = 0; i < m_numSpotShadowLights; i++)
        {
            wchar_t paramName[64];
            swprintf_s(paramName, L"spotShadowMap[%d]", i);
            pShader->SetPSParam(paramName, std::any(m_spSpotShadowMap[i]));
            swprintf_s(paramName, L"spotShadowViewProj[%d]", i);
            DirectX::XMFLOAT4X4 f4x4;
            DirectX::XMStoreFloat4x4(&f4x4, m_spotShadowViewProj[i]);
            Matrix mat(reinterpret_cast<float*>(&f4x4));
            pShader->SetPSParam(paramName, std::any(mat));
        }

        // Upload area light data
        int numAreaLights = 0;
        for (int i = 0; i < (int)lights.size() && numAreaLights < 4; i++)
        {
            if (lights[i]->GetType() != ELightType::AreaLight)
                continue;
            IAreaLight* pArea = dynamic_cast<IAreaLight*>(lights[i].p);
            if (pArea == nullptr)
                continue;
            Vector3 corners[4];
            pArea->GetCorners(corners);
            for (int c = 0; c < 4; c++)
            {
                wchar_t paramName[64];
                swprintf_s(paramName, L"areaLightCorners[%d]", numAreaLights * 4 + c);
                Float4 corner(corners[c].x, corners[c].y, corners[c].z, 0.0f);
                pShader->SetPSParam(paramName, std::any(corner));
            }
            {
                wchar_t paramName[64];
                FRGBColor clr = pArea->GetColor();
                swprintf_s(paramName, L"areaLightColor[%d]", numAreaLights);
                Float4 color(clr.r, clr.g, clr.b, 0.0f);
                pShader->SetPSParam(paramName, std::any(color));
                swprintf_s(paramName, L"areaLightIntensity[%d]", numAreaLights);
                Float4 inten(pArea->GetIntensity(), 0.0f, 0.0f, 0.0f);
                pShader->SetPSParam(paramName, std::any(inten));
            }
            numAreaLights++;
        }
        pShader->SetPSParamInt(L"numAreaLights", numAreaLights);
    }

    //**********************************************************************
    void CRenderer::BeginShadowmapPass(int whichShadowMap)
    {
        // Clear shadow map
        CComPtr<ID3D11RenderTargetView> spOldRT;
        CComPtr<ID3D11DepthStencilView> spOldStencil;
        m_spContext->OMGetRenderTargets(1, &spOldRT, &spOldStencil);
        m_spContext->OMSetRenderTargets(0, nullptr, m_spShadowMapStencilView[whichShadowMap].p);
        FLOAT bgClr[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        m_spContext->ClearDepthStencilView(m_spShadowMapStencilView[whichShadowMap], D3D11_CLEAR_DEPTH, 1.0f, 0);
        m_spContext->OMSetRenderTargets(1, &spOldRT.p, spOldStencil);
    }


    //**********************************************************************
    void CRenderer::EndShadowmapPass(int whichShadowMap)
    {
    }


    //**********************************************************************
    void CRenderer::PushShadowmapRT(int whichShadowMap, int lightMapIndex, const Vector3& lightPos, const Vector3 &lightDir)
    {
        ShadowMapRenderState rs;
        m_spContext->OMGetRenderTargets(1, &rs.m_spOldRT, &rs.m_spOldStencil);
        rs.m_spOldCamera = m_spCamera;
        rs.m_viewport = m_viewport;
        m_shadowMapRenderState.push(rs);
        m_spContext->OMSetRenderTargets(0, nullptr, m_spShadowMapStencilView[whichShadowMap].p);

        // Reset the camera to be from the lights perspective
        CRefObj<ICamera> spCamera = CreateCamera(true);
        spCamera->SetPosition(lightPos, lightDir, Vector3(0.0f, 1.0f, 0.0f));
        this->SetCamera(spCamera);
        SetShadowmapViewport(whichShadowMap, lightMapIndex);
    }
    
    void CRenderer::PopShadowmapRT()
    {
        // Restore default render targets
        ShadowMapRenderState rs = m_shadowMapRenderState.top();
        this->SetCamera(rs.m_spOldCamera);
        m_viewport = rs.m_viewport;
        m_spContext->RSSetViewports(1, &m_viewport);
        m_shadowMapRenderState.pop();
        m_spContext->OMSetRenderTargets(1, &rs.m_spOldRT.p, rs.m_spOldStencil);
    }
    
    void CRenderer::AddRenderable(IRenderable* pRenderable)
    {
        m_singleObjs.push_back(pRenderable);
    }

    //**********************************************************************
    // Method: RenderScene
    // Renders current scene (both scene graph and any renderables currently
    // attached to the renderer)
    //**********************************************************************
    void CRenderer::RenderScene(std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback)
    {
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        spCtx2->BeginEventInt(L"RenderScene", 0);

        m_spRenderCtx->SetDebugFlags(RenderCtxFlags::c_DisplayWorldAxis);
#endif
        m_spContext->RSSetState(m_spRasterizerState);
        if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayWorldAxis)
        {
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 100.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
            DrawLine(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 100.0f), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
        }

        if (m_spRenderCtx->GetDebugFlags() & RenderCtxFlags::c_DisplayLightDir &&
            m_lights.size() > 0)
        {
            for (size_t i = 0; i < m_lights.size(); i++)
                DrawLine(m_lights[i]->GetPosition(), Vector3(0.0f, 0.0f, 0.0f), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
        }

        for (uint32_t pass = c_PassFirst; pass <= c_PassLast; pass++)
        {
            CRenderCtx *pCtx = (CRenderCtx*)m_spRenderCtx.p;
#ifdef _DEBUG
            CComPtr<ID3D11DeviceContext2> spCtx2;
            CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
#endif
            pCtx->m_currentPass = pass;
            pCtx->m_passBlendable = true;
            if (pass == c_PassOpaque)
            {
#ifdef _DEBUG
                spCtx2->BeginEventInt(L"OpaquePass", 0);
#endif
                DispatchTileLightCull();
                DrawSceneObjects(pass, renderCallback);

                // Render decals after opaque objects
                if (!m_decals.empty() && m_spDecalShader != nullptr && m_spDepthCopy != nullptr)
                {
#ifdef _DEBUG
                    spCtx2->BeginEventInt(L"DecalPass", 0);
#endif
                    // Copy depth buffer to separate texture (avoid DSV/SRV conflict)
                    m_spContext->CopyResource(m_spDepthCopy->GetD3DTexture(), m_spDepthStencilBuffer);

                    // Save current blend/raster/dsv state
                    CComPtr<ID3D11BlendState> spOldBlend;
                    float oldBlendFactor[4];
                    UINT oldSampleMask;
                    m_spContext->OMGetBlendState(&spOldBlend, oldBlendFactor, &oldSampleMask);
                    CComPtr<ID3D11RasterizerState> spOldRast;
                    m_spContext->RSGetState(&spOldRast);

                    // Set decal blend and rasterizer state
                    m_spContext->OMSetBlendState(m_spDecalBlendState, nullptr, 0xffffffff);
                    m_spContext->RSSetState(m_spDecalRastState);

                    // Unbind DSV, keep HDR RT with alpha blend
                    ID3D11RenderTargetView* pHDRRT = m_spHDRRTView;
                    m_spContext->OMSetRenderTargets(1, &pHDRRT, nullptr);

                    // Bind decal cube mesh
                    UINT cubeVertSize = sizeof(float) * 3;
                    UINT cubeOffset = 0;
                    m_spContext->IASetVertexBuffers(0, 1, &m_spCubeVB.p, &cubeVertSize, &cubeOffset);
                    m_spContext->IASetIndexBuffer(m_spCubeIB, DXGI_FORMAT_R32_UINT, 0);
                    m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                    // Compute viewProjInv
                    DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(GetCamera()->GetView(), GetCamera()->GetProjection());
                    DirectX::XMMATRIX viewProjInv = DirectX::XMMatrixInverse(nullptr, viewProj);
                    DirectX::XMFLOAT4X4 f4x4;
                    DirectX::XMStoreFloat4x4(&f4x4, viewProjInv);
                    Matrix viewProjInvMat(reinterpret_cast<float*>(&f4x4));

                    for (auto& spDecal : m_decals)
                    {
                        Matrix4x4 decalWorld = spDecal->GetTransform();
                        // Compute decalWorldInv
                        DirectX::XMMATRIX xmDecalWorld = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&decalWorld));
                        DirectX::XMMATRIX xmDecalWorldInv = DirectX::XMMatrixInverse(nullptr, xmDecalWorld);
                        DirectX::XMStoreFloat4x4(&f4x4, xmDecalWorldInv);
                        Matrix decalWorldInvMat(reinterpret_cast<float*>(&f4x4));

                        // worldViewProj for VS
                        DirectX::XMMATRIX wvp = DirectX::XMMatrixMultiply(xmDecalWorld, viewProj);
                        DirectX::XMStoreFloat4x4(&f4x4, wvp);
                        Matrix wvpMat(reinterpret_cast<float*>(&f4x4));

                        m_spDecalShader->SetVSParam(L"worldViewProj", std::any(wvpMat));

                        m_spDecalShader->SetPSParam(L"viewProjInv", std::any(viewProjInvMat));
                        m_spDecalShader->SetPSParam(L"decalWorldInv", std::any(decalWorldInvMat));
                        m_spDecalShader->SetPSParam(L"depthTexture", std::any(m_spDepthCopy));
                        if (spDecal->GetAlbedoTexture() != nullptr)
                            m_spDecalShader->SetPSParam(L"decalTexture", std::any(spDecal->GetAlbedoTexture()));
                        m_spDecalShader->SetPSParamFloat(L"decalOpacity", spDecal->GetOpacity());
                        m_spDecalShader->SetPSParamFloat(L"screenWidth", (float)m_BBDesc.Width);
                        m_spDecalShader->SetPSParamFloat(L"screenHeight", (float)m_BBDesc.Height);

                        std::vector<CRefObj<ILight>> emptyLights;
                        m_spDecalShader->BeginRender(this, nullptr, emptyLights, nullptr);
                        m_spContext->DrawIndexed(36, 0, 0);
                        m_spDecalShader->EndRender(this);
                    }

                    // Restore state
                    m_spContext->OMSetBlendState(spOldBlend, oldBlendFactor, oldSampleMask);
                    m_spContext->RSSetState(spOldRast);
                    // Re-bind DSV
                    m_spContext->OMSetRenderTargets(1, &pHDRRT, m_spStencilView);
#ifdef _DEBUG
                    spCtx2->EndEvent();
#endif
                }
#ifdef _DEBUG
                spCtx2->EndEvent();
#endif
            }
#ifdef SUPPORT_OBJID
            else if (pass == c_PassObjID)
            {
                pCtx->m_passBlendable = false;
                // Setup render target
                m_spContext->OMSetRenderTargets(1, &m_spObjIDRTView, m_spStencilView);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
            else if (pass == c_PassShadow)
            {
#ifdef _DEBUG
                spCtx2->BeginEventInt(L"ShadowPass", 0);
#endif
                DrawSceneObjects(pass, renderCallback);
#ifdef _DEBUG
                spCtx2->EndEvent();
#endif
            }
            else if (pass == c_PassTransparent)
            {
#ifdef _DEBUG
                spCtx2->BeginEventInt(L"TransparentPass", 0);
#endif
                // Setup blend state
                CComPtr<ID3D11BlendState> spBlendState;
                D3D11_BLEND_DESC blendState;
                ZeroMemory(&blendState, sizeof(D3D11_BLEND_DESC));
                blendState.AlphaToCoverageEnable = false;
                blendState.IndependentBlendEnable = false;
                for (int i = 0; i < 8; i++)
                {
                    if (i == 0)
                    {
                        blendState.RenderTarget[i].BlendEnable = true;
                        blendState.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
                        blendState.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
                        blendState.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
                        blendState.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendState.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
                        blendState.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendState.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
                    }
                    else
                    {
                        blendState.RenderTarget[i].BlendEnable = false;
                        blendState.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
                        blendState.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
                        blendState.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
                        blendState.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
                        blendState.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
                        blendState.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
                        blendState.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
                    }
                }
                m_spDevice->CreateBlendState(&blendState, &spBlendState);
                CComPtr<ID3D11BlendState> spOldBlendState;
                float oldBlendFactor[4];
                UINT oldSampleMask;
                m_spContext->OMGetBlendState(&spOldBlendState, oldBlendFactor, &oldSampleMask);
                m_spContext->OMSetBlendState(spBlendState, nullptr, 0xffffffff);

                std::vector<int> order;
                order.resize(m_singleObjs.size());
                std::sort(m_singleObjs.begin(), m_singleObjs.end(),
                    [&](IRenderable *left, IRenderable *right)->bool
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
                DrawSceneObjects(pass, renderCallback);
                m_spContext->OMSetBlendState(spOldBlendState, oldBlendFactor, oldSampleMask);

                // Render particle systems (additive blend, depth test on, depth write off)
                for (auto& ps : m_particleSystems)
                    ps->Render(this);
#ifdef _DEBUG
                spCtx2->EndEvent();
#endif
            }

#ifdef SUPPORT_OBJID
            if (pass == c_PassObjID)
            {
                // Restore default render targets
                ID3D11RenderTargetView* pView = (m_spFinalRTView) ? m_spFinalRTView : m_spRTView;
                ID3D11DepthStencilView* pStencil = (m_spFinalRTView) ? m_spFinalStencilView : m_spStencilView;
                m_spContext->OMSetRenderTargets(1, &pView, pStencil);
                FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spObjIDRTView, bgClr);
                m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
            }
#endif // SUPPORT_OBJID
        }
#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: RenderFrame
    // See <IRenderer::RenderFrame>
    //**********************************************************************
    void CRenderer::RenderFrame(
        std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback,
        std::function<void(IRenderer* pRenderer)> prePresentCallback
        )
    {
        if (m_freeze > 0)
            WaitForSingleObject(m_freezeEvent, INFINITE);

        // Determine scene render target: if post-processing is enabled, render to HDR RT
        bool usePostProcess = m_postProcessEnabled && m_spHDRRTView != nullptr;
        ID3D11RenderTargetView* pSceneRTV;
        ID3D11DepthStencilView* pStencilView;
        if (usePostProcess)
        {
            pSceneRTV = m_spHDRRTView;
            pStencilView = m_spStencilView;
        }
        else
        {
            pSceneRTV = (m_spFinalRTView) ? m_spFinalRTView : m_spRTView;
            pStencilView = (m_spFinalRTView) ? m_spFinalStencilView : m_spStencilView;
        }

        m_spContext->OMSetRenderTargets(1, &pSceneRTV, nullptr);

        FLOAT bgClr[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
        m_spContext->ClearRenderTargetView(pSceneRTV, bgClr);
        if (!usePostProcess && m_spFinalRTView != nullptr)
            m_spContext->ClearRenderTargetView(m_spFinalRTView, bgClr);
        m_spContext->ClearDepthStencilView(pStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        m_spContext->ClearDepthStencilView(m_spStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        CD3D11_DEPTH_STENCIL_DESC depthDesc(D3D11_DEFAULT);
        depthDesc.DepthEnable = true;
        depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
        CComPtr<ID3D11DepthStencilState> spDepthStencilState;
        CT(m_spDevice->CreateDepthStencilState(&depthDesc, &spDepthStencilState));
        m_spContext->OMSetDepthStencilState(spDepthStencilState, 1);

        m_spContext->OMSetRenderTargets(1, &pSceneRTV, pStencilView);

        // Set up MRT for SSR normal buffer if enabled
        if (m_ssrEnabled && m_spNormalRTV != nullptr)
        {
            FLOAT blackNorm[4] = { 0.5f, 0.5f, 0.0f, 0.0f };
            m_spContext->ClearRenderTargetView(m_spNormalRTV, blackNorm);
            ID3D11RenderTargetView* rtvs[2] = { pSceneRTV, m_spNormalRTV };
            m_spContext->OMSetRenderTargets(2, rtvs, pStencilView);
        }

        // Generate IBL maps if environment map changed
        if (m_iblDirty)
        {
            GenerateIBLMaps();
            m_spContext->OMSetRenderTargets(1, &pSceneRTV, pStencilView);
        }

        // Update particle systems
        {
            auto now = std::chrono::high_resolution_clock::now();
            if (m_hasLastFrameTime)
            {
                float dt = std::chrono::duration<float>(now - m_lastFrameTime).count();
                dt = (dt > 0.1f) ? 0.1f : dt; // clamp to avoid spiral of death
                for (auto& ps : m_particleSystems)
                    ps->Update(this, dt);
            }
            m_lastFrameTime = now;
            m_hasLastFrameTime = true;
        }

        RenderScene(renderCallback);

        // Run post-processing chain
        if (usePostProcess)
        {
            // Unbind MRT and depth so post-process can read them
            m_spContext->OMSetRenderTargets(1, &pSceneRTV, nullptr);
            RunPostProcessing();
        }
        else
        {
            if (m_spFinalRTView != nullptr)
                m_spContext->OMSetRenderTargets(1, &m_spRTView.p, m_spStencilView);
        }

        if (prePresentCallback)
            (prePresentCallback)(this);
        m_spSwapChain->Present(1, 0);
    }

    //**********************************************************************
    // Method: RenderLoop
    // See <IRenderer::RenderLoop>
    //**********************************************************************
    void CRenderer::RenderLoop(
        std::function<void(IRenderer *pRenderer, IRenderCtx *pRenderCtx, int pass)> renderCallback,
        std::function<void(IRenderer* pRenderer)> prePresentCallback
    )
    {
        m_waitForShutdown.Clear();
        while (!m_exitThread)
        {
            RenderFrame(renderCallback, prePresentCallback);
        }
        m_waitForShutdown.Set();
    }

    //**********************************************************************
    // Method: SetViewport
    // See <IRenderer>
    //**********************************************************************
    void CRenderer::SetViewport(float x0, float y0, float x1, float y1)
    {
        m_finalViewport.minPt.x = x0;
        m_finalViewport.minPt.y = y0;
        m_finalViewport.maxPt.x = x1;
        m_finalViewport.maxPt.y = y1;
    }

    //**********************************************************************
    // Method: SetFinalRenderTarget
    // See <IRenderer>
    //**********************************************************************
    void CRenderer::SetFinalRenderTarget(ID3D11Texture2D* pTexture)
    {
        Freeze();
        if (m_spFinalRTView)
        {
            m_spContext->OMSetRenderTargets(0, nullptr, nullptr);
            m_spFinalRTView = nullptr;
            m_spFinalStencilView = nullptr;
        }
        
        // Create depth buffer
        D3D11_TEXTURE2D_DESC desc;
        pTexture->GetDesc(&desc);
        CD3D11_TEXTURE2D_DESC texDesc2D(DXGI_FORMAT_D24_UNORM_S8_UINT, desc.Width, desc.Height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
        CT(m_spDevice->CreateTexture2D(&texDesc2D, NULL, &m_spFinalDepthStencilBuffer));

        // Create the depth buffer ressource view
        CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
        CT(m_spDevice->CreateDepthStencilView(m_spFinalDepthStencilBuffer, &stencilDesc, &m_spFinalStencilView));

        CT(m_spDevice->CreateRenderTargetView(pTexture, nullptr, &m_spFinalRTView));
        
        uint32_t width = m_viewRect.right - m_viewRect.left + 1;
        uint32_t height = m_viewRect.bottom - m_viewRect.top + 1;
        if (desc.Width != width || desc.Height != height)
        {
            ZeroMemory(&m_viewport, sizeof(m_viewport));
            m_viewport.TopLeftX = m_finalViewport.minPt.x * (float)(desc.Width);
            m_viewport.TopLeftY = m_finalViewport.minPt.y * (float)(desc.Height);
            m_viewport.Width =  (m_finalViewport.maxPt.x - m_finalViewport.minPt.x) * (float)(desc.Width);
            m_viewport.Height = (m_finalViewport.maxPt.y - m_finalViewport.minPt.y) * (float)(desc.Height);
            m_viewport.MinDepth = 0.0f;
            m_viewport.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &m_viewport);
        }
        Unfreeze();
    }


    //**********************************************************************
    // Method: SetFinalRenderTargetUsingSharedTexture
    // See <IRenderer>
    //**********************************************************************
    void CRenderer::SetFinalRenderTargetUsingSharedTexture(IUnknown* pTexture)
    {
        IDXGIResource* pDXGIResource;
        CT(pTexture->QueryInterface(__uuidof(IDXGIResource), (void**)&pDXGIResource));
        HANDLE sharedHandle;
        CT(pDXGIResource->GetSharedHandle(&sharedHandle));
        pDXGIResource->Release();

        CComPtr<ID3D11Device> spDevice = GetDevice();
        CComPtr<IUnknown> tempResource11;
        CT(spDevice->OpenSharedResource(sharedHandle, __uuidof(ID3D11Resource), (void**)(&tempResource11)));

        CComPtr<ID3D11Texture2D> spOutputResource;
        CT(tempResource11->QueryInterface(__uuidof(ID3D11Texture2D), (void**)(&spOutputResource)));
        SetFinalRenderTarget(spOutputResource);
    }

    //**********************************************************************
    // Method: CopyFrameBackbuffer
    // See <IRenderer>
    //**********************************************************************
    void CRenderer::CopyFrameBackBuffer(IImage* pImage)
    {
        // Copy rendered image to an IImage for processing on the CPU
        int bpp = 32;
        switch (m_BBDesc.Format)
        {
        case DXGI_FORMAT::DXGI_FORMAT_R8_UNORM:
            bpp = 8;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_R16_UINT:
            bpp = 16;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
        case DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM:
            bpp = 32;
            break;
        case DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT:
            bpp = 128;
            break;
        }
        if (m_spCPUBackBuffer == nullptr)
        {
            CD3D11_TEXTURE2D_DESC texdesc(m_BBDesc.Format, m_BBDesc.Width, m_BBDesc.Height);
            texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            texdesc.BindFlags = (D3D11_BIND_FLAG)0;
            texdesc.Usage = D3D11_USAGE_STAGING;
            texdesc.MipLevels = 1;
            texdesc.SampleDesc.Count = 1;
            texdesc.SampleDesc.Quality = 0;
            CT(m_spDevice->CreateTexture2D(&texdesc, nullptr, &m_spCPUBackBuffer));
        }
        m_spContext->CopyResource(m_spCPUBackBuffer, m_spBackBuffer);
        D3D11_MAPPED_SUBRESOURCE ms;
        CT(m_spContext->Map(m_spCPUBackBuffer, 0, D3D11_MAP_READ, 0, &ms));
        BYTE* pSrc = reinterpret_cast<BYTE*>(ms.pData);
        BYTE* pDst = pImage->GetData();
        int stride = pImage->GetStride();
        if (ms.RowPitch == stride)
        {
            CopyMemory(pDst, pSrc, m_BBDesc.Height * stride);
        }
        else
        {
            for (int y = 0; y < (int)m_BBDesc.Height; y++)
            {
                memcpy(pDst, pSrc, m_BBDesc.Width * bpp / 8);
                pSrc += ms.RowPitch;
                pDst += stride;
            }
        }
        m_spContext->Unmap(m_spCPUBackBuffer, 0);
    }

    //**********************************************************************
    // Function: CreateRendererInternal
    // Creates a renderer
    //
    // Parameters:
    // hwnd - window to attach renderer to
    // viewport - viewport for final render target
    // shaderFolder - path to directory containing shaders
    // startFrozen - should renderer be started in a frozen state?
    // desktopIndex - index indicating which desktop should be used with duplication service
    //      (used by Caustic::CreateDesktopTexture())
    //
    // Returns:
    // Returns the created renderer
    //**********************************************************************
    CRefObj<IRenderer> CreateRendererInternal(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool startFrozen /* = false */, int desktopIndex /* = 0 */)
    {
        std::unique_ptr<CRenderer> spRenderer(new CRenderer());
        spRenderer->Setup(hwnd, viewport, shaderFolder, true, startFrozen, desktopIndex);

        CRefObj<ICamera> spCamera = CCausticFactory::Instance()->CreateCamera(true);
        spRenderer->SetCamera(spCamera);
        return CRefObj<IRenderer>(spRenderer.release());
    }

    //**********************************************************************
    // Method: SetCamera
    // See <IRenderer::SetCamera>
    //**********************************************************************
    void CGraphicsBase::SetCamera(ICamera* pCamera)
    {
        pCamera->BuildMatrices();
        m_spCamera = pCamera;
    }

    void CGraphicsBase::DeviceWindowResizedInternal(uint32_t width, uint32_t height)
    {
        m_spBackBuffer = nullptr;
        m_spRTView = nullptr;
        CT(m_spSwapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0));
        CT(m_spSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_spBackBuffer)));
        m_spBackBuffer->GetDesc(&m_BBDesc);
        CT(m_spDevice->CreateRenderTargetView(m_spBackBuffer, nullptr, &m_spRTView));

        // Recreate depth buffer at new size
        m_spStencilView = nullptr;
        m_spDepthSRView = nullptr;
        m_spDepthStencilBuffer = nullptr;
        m_spDepthTextureObj = nullptr;
        {
            CD3D11_TEXTURE2D_DESC texDesc2D(DXGI_FORMAT_R32_TYPELESS, m_BBDesc.Width, m_BBDesc.Height, 1, 1,
                D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&texDesc2D, NULL, &m_spDepthStencilBuffer));
            CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D32_FLOAT);
            CT(m_spDevice->CreateDepthStencilView(m_spDepthStencilBuffer, &stencilDesc, &m_spStencilView));
            CD3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R32_FLOAT);
            CT(m_spDevice->CreateShaderResourceView(m_spDepthStencilBuffer, &depthSRVDesc, &m_spDepthSRView));
            m_spDepthTextureObj = CRefObj<ITexture>(new CTexture(m_spDepthStencilBuffer, m_spDepthSRView));
        }

        // Recreate HDR render target
        m_spHDRRTView = nullptr;
        m_spHDRTexture = nullptr;
        m_spHDRTextureObj = nullptr;
        {
            CD3D11_TEXTURE2D_DESC hdrDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, m_BBDesc.Width, m_BBDesc.Height,
                1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&hdrDesc, NULL, &m_spHDRTexture));
            CT(m_spDevice->CreateRenderTargetView(m_spHDRTexture, NULL, &m_spHDRRTView));
            CComPtr<ID3D11ShaderResourceView> spHDRSRV;
            CT(m_spDevice->CreateShaderResourceView(m_spHDRTexture, NULL, &spHDRSRV));
            m_spHDRTextureObj = CRefObj<ITexture>(new CTexture(m_spHDRTexture, spHDRSRV));
        }

        // Recreate ping-pong post-processing render targets
        for (int i = 0; i < 2; i++)
        {
            m_spPostProcessRTV[i] = nullptr;
            m_spPostProcessRT[i] = nullptr;
            m_spPostProcessTexObj[i] = nullptr;
            CD3D11_TEXTURE2D_DESC ppDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, m_BBDesc.Width, m_BBDesc.Height,
                1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&ppDesc, NULL, &m_spPostProcessRT[i]));
            CT(m_spDevice->CreateRenderTargetView(m_spPostProcessRT[i], NULL, &m_spPostProcessRTV[i]));
            CComPtr<ID3D11ShaderResourceView> spSRV;
            CT(m_spDevice->CreateShaderResourceView(m_spPostProcessRT[i], NULL, &spSRV));
            m_spPostProcessTexObj[i] = CRefObj<ITexture>(new CTexture(m_spPostProcessRT[i], spSRV));
        }

        // Create normal buffer for SSR (R16G16B16A16: normalVS.xy, roughness, metallic)
        m_spNormalRTV = nullptr;
        m_spNormalBufferObj = nullptr;
        {
            CComPtr<ID3D11Texture2D> spNormalTex;
            CD3D11_TEXTURE2D_DESC normalDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, m_BBDesc.Width, m_BBDesc.Height,
                1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&normalDesc, NULL, &spNormalTex));
            CT(m_spDevice->CreateRenderTargetView(spNormalTex, NULL, &m_spNormalRTV));
            CComPtr<ID3D11ShaderResourceView> spNormalSRV;
            CT(m_spDevice->CreateShaderResourceView(spNormalTex, NULL, &spNormalSRV));
            m_spNormalBufferObj = CRefObj<ITexture>(new CTexture(spNormalTex, spNormalSRV));
        }

        // Recreate bloom mip chain
        {
            uint32_t mipW = m_BBDesc.Width / 2;
            uint32_t mipH = m_BBDesc.Height / 2;
            for (int i = 0; i < c_BloomMipCount; i++)
            {
                m_spBloomMipRTV[i] = nullptr;
                m_spBloomMipTexture[i] = nullptr;
                m_spBloomMipTexObj[i] = nullptr;
                mipW = (mipW < 1) ? 1 : mipW;
                mipH = (mipH < 1) ? 1 : mipH;
                CD3D11_TEXTURE2D_DESC bloomDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, mipW, mipH,
                    1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
                CT(m_spDevice->CreateTexture2D(&bloomDesc, NULL, &m_spBloomMipTexture[i]));
                CT(m_spDevice->CreateRenderTargetView(m_spBloomMipTexture[i], NULL, &m_spBloomMipRTV[i]));
                CComPtr<ID3D11ShaderResourceView> spSRV;
                CT(m_spDevice->CreateShaderResourceView(m_spBloomMipTexture[i], NULL, &spSRV));
                m_spBloomMipTexObj[i] = CRefObj<ITexture>(new CTexture(m_spBloomMipTexture[i], spSRV));
                mipW /= 2;
                mipH /= 2;
            }
        }

        AdjustViewport();
    }

    //**********************************************************************
    // Method: InitializeD3D
    // Called at the start of the application to initialize the server side of our renderer.
    //
    // Parameters:
    // hwnd - HWND to use for drawing
    // viewport - viewport to render final viewport into
    //**********************************************************************
    void CGraphicsBase::InitializeD3D(HWND hwnd, BBox2 &viewport)
    {
        m_hwnd = hwnd;
        std::unique_ptr<CRenderCtx> spCtx(new CRenderCtx());
        m_spRenderCtx = spCtx.release();

        CT(m_spSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_spBackBuffer)));
        m_spBackBuffer->GetDesc(&m_BBDesc);
        CT(m_spDevice->CreateRenderTargetView(m_spBackBuffer, nullptr, &m_spRTView));

#ifdef SUPPORT_GRAPHICS_CAPTURE
        CT(DXGIGetDebugInterface1(0, __uuidof(m_spGraphicsAnalysis), reinterpret_cast<void**>(&m_spGraphicsAnalysis)));
#endif // SUPPORT_GRAPHICS_CAPTURE

        // Create texture for rendering shadow map
        for (int i = 0; i < c_MaxShadowMaps; i++)
        {
            switch (i)
            {
            case c_HiResShadowMap:
                m_shadowMapWidth[i] = 8192;
                m_shadowMapHeight[i] = 8192;
                m_shadowMapLightWidth[i] = 2;  // 2x2 grid for 4 cascades
                m_shadowMapLightHeight[i] = 2;
                break;
            case c_MidResShadowMap:
                m_shadowMapWidth[i] = 4096;
                m_shadowMapHeight[i] = 2048;
                m_shadowMapLightWidth[i] = 4;
                m_shadowMapLightHeight[i] = 2;
                break;
            case c_LowResShadowMap:
                m_shadowMapWidth[i] = 2048;
                m_shadowMapHeight[i] = 2048;
                m_shadowMapLightWidth[i] = 2;
                m_shadowMapLightHeight[i] = 2;
                break;
            }

            // Create shadow map texture
            CD3D11_TEXTURE2D_DESC texDesc(DXGI_FORMAT_R32_TYPELESS, m_shadowMapWidth[i], m_shadowMapHeight[i],
                1, 1, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&texDesc, NULL, &m_spShadowTexture[i]));

            // Create the view onto shadow map texture for use as depth buffer
            CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D32_FLOAT);
            CT(m_spDevice->CreateDepthStencilView(m_spShadowTexture[i], &stencilDesc, &m_spShadowMapStencilView[i]));

            // Create the view onto shadow map texture for use as shader resource
            CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R32_FLOAT);
            CT(m_spDevice->CreateShaderResourceView(m_spShadowTexture[i], &srvDesc, &m_spShadowSRView[i]));
        }

        // Create depth buffer (typeless for SRV access by post-processing effects like SSAO)
        CD3D11_TEXTURE2D_DESC texDesc2D(DXGI_FORMAT_R32_TYPELESS, m_BBDesc.Width, m_BBDesc.Height, 1, 1, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE);
        CT(m_spDevice->CreateTexture2D(&texDesc2D, NULL, &m_spDepthStencilBuffer));

        // Create the depth buffer stencil view
        CD3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D32_FLOAT);
        CT(m_spDevice->CreateDepthStencilView(m_spDepthStencilBuffer, &stencilDesc, &m_spStencilView));

        // Create shader resource view for reading depth in post-processing
        CD3D11_SHADER_RESOURCE_VIEW_DESC depthSRVDesc(D3D11_SRV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R32_FLOAT);
        CT(m_spDevice->CreateShaderResourceView(m_spDepthStencilBuffer, &depthSRVDesc, &m_spDepthSRView));

        // Create HDR scene render target (R16G16B16A16_FLOAT)
        {
            CD3D11_TEXTURE2D_DESC hdrDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, m_BBDesc.Width, m_BBDesc.Height,
                1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&hdrDesc, NULL, &m_spHDRTexture));
            CT(m_spDevice->CreateRenderTargetView(m_spHDRTexture, NULL, &m_spHDRRTView));
            CComPtr<ID3D11ShaderResourceView> spHDRSRV;
            CT(m_spDevice->CreateShaderResourceView(m_spHDRTexture, NULL, &spHDRSRV));
            m_spHDRTextureObj = CRefObj<ITexture>(new CTexture(m_spHDRTexture, spHDRSRV));
        }

        // Create ping-pong post-processing render targets
        for (int i = 0; i < 2; i++)
        {
            CD3D11_TEXTURE2D_DESC ppDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, m_BBDesc.Width, m_BBDesc.Height,
                1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
            CT(m_spDevice->CreateTexture2D(&ppDesc, NULL, &m_spPostProcessRT[i]));
            CT(m_spDevice->CreateRenderTargetView(m_spPostProcessRT[i], NULL, &m_spPostProcessRTV[i]));
            CComPtr<ID3D11ShaderResourceView> spSRV;
            CT(m_spDevice->CreateShaderResourceView(m_spPostProcessRT[i], NULL, &spSRV));
            m_spPostProcessTexObj[i] = CRefObj<ITexture>(new CTexture(m_spPostProcessRT[i], spSRV));
        }

        // Create bloom downsample mip chain
        {
            uint32_t mipW = m_BBDesc.Width / 2;
            uint32_t mipH = m_BBDesc.Height / 2;
            for (int i = 0; i < c_BloomMipCount; i++)
            {
                mipW = (mipW < 1) ? 1 : mipW;
                mipH = (mipH < 1) ? 1 : mipH;
                CD3D11_TEXTURE2D_DESC bloomDesc(DXGI_FORMAT_R16G16B16A16_FLOAT, mipW, mipH,
                    1, 1, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
                CT(m_spDevice->CreateTexture2D(&bloomDesc, NULL, &m_spBloomMipTexture[i]));
                CT(m_spDevice->CreateRenderTargetView(m_spBloomMipTexture[i], NULL, &m_spBloomMipRTV[i]));
                CComPtr<ID3D11ShaderResourceView> spSRV;
                CT(m_spDevice->CreateShaderResourceView(m_spBloomMipTexture[i], NULL, &spSRV));
                m_spBloomMipTexObj[i] = CRefObj<ITexture>(new CTexture(m_spBloomMipTexture[i], spSRV));
                mipW /= 2;
                mipH /= 2;
            }
        }

        // Create ITexture wrapper for depth SRV
        m_spDepthTextureObj = CRefObj<ITexture>(new CTexture(m_spDepthStencilBuffer, m_spDepthSRView));

        m_finalViewport = viewport;
        AdjustViewport();
    }

    void CGraphicsBase::AdjustViewport()
    {
        ::GetClientRect(m_hwnd, &m_viewRect);
        ZeroMemory(&m_viewport, sizeof(m_viewport));
        int w = m_viewRect.right - m_viewRect.left;
        int h = m_viewRect.bottom - m_viewRect.top;
        m_viewport.TopLeftX = m_finalViewport.minPt.x * (float)w;
        m_viewport.TopLeftY = m_finalViewport.minPt.y * (float)h;
        m_viewport.Width = (m_finalViewport.maxPt.x - m_finalViewport.minPt.x) * (float)w;
        m_viewport.Height = (m_finalViewport.maxPt.y - m_finalViewport.minPt.y) * (float)h;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_spContext->RSSetViewports(1, &m_viewport);
    }

    /*static*/ LRESULT CALLBACK OffscreenWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    //**********************************************************************
    // Method: Setup
    // See <IRenderer::Setup>
    //**********************************************************************
    void CGraphicsBase::Setup(HWND hwnd, BBox2 &viewport, bool createDebugDevice, int desktopIndex)
    {
        if (hwnd == nullptr)
        {
            WNDCLASS wc = {};
            wc.lpfnWndProc = OffscreenWndProc;
            wc.cbWndExtra = 0;
            wc.hInstance = nullptr;
            wc.hCursor = LoadCursor(NULL, IDC_ARROW);
            wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wc.style = CS_SAVEBITS | CS_DROPSHADOW;
            wc.lpszClassName = L"OffscreenWindowClass";
            RegisterClass(&wc);
            hwnd = CreateWindow(L"OffscreenWindowClass", L"OffscreenWindow", 0, 0, 0, 100, 100, nullptr, nullptr, nullptr, nullptr);
        }
        DXGI_SWAP_CHAIN_DESC desc = { 0 };
        desc.BufferDesc.Width = 0;
        desc.BufferDesc.Height = 0;
        desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BufferDesc.RefreshRate.Numerator = 1;
        desc.BufferDesc.RefreshRate.Denominator = 30;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 2;
        desc.OutputWindow = hwnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        desc.Flags = 0;

        CT(D3D11CreateDeviceAndSwapChain(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // software module
#ifdef _DEBUG
            D3D11_CREATE_DEVICE_DEBUG, // flags
#else
            0, // flags
#endif
            nullptr, // pFeatureLevels
            0, // numFeatureLevels
            D3D11_SDK_VERSION,
            &desc, &m_spSwapChain, &m_spDevice, &m_featureLevel, &m_spContext));
        //**********************************************************************
        // Setup Windows Duplication service
        //**********************************************************************
        CComPtr<IDXGIDevice> spDXGIDevice;
        CT(m_spDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&spDXGIDevice));
        CComPtr<IDXGIAdapter> spDXGIAdapter;
        CT(spDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&spDXGIAdapter));

        // Walk our list of adapters and find the correct display
        static bool enableDuplication = false;
        if (enableDuplication)
        {
            int outputIndex = 0;
            CComPtr<IDXGIOutput> spOutput;
            while (spDXGIAdapter->EnumOutputs(outputIndex, &spOutput) != DXGI_ERROR_NOT_FOUND)
            {
                if (outputIndex == desktopIndex)
                {
                    CComPtr<IDXGIOutput1> spOutput1;
                    HRESULT hr = spOutput->QueryInterface(__uuidof(IDXGIOutput1), (void**)&spOutput1);
                    if (SUCCEEDED(hr))
                    {
                        hr = spOutput1->DuplicateOutput(m_spDevice, &m_spDuplication);
                        if (SUCCEEDED(hr))
                            break;
                    }
                }
                outputIndex++;
                spOutput = nullptr;
            }
        }

        InitializeD3D(hwnd, viewport);

        // Create a default camera
        m_spCamera = CCausticFactory::Instance()->CreateCamera(false);
    }

    //**********************************************************************
    // Method: RunPostProcessing
    // Executes the post-processing chain after scene rendering.
    // The scene has been rendered to m_spHDRRTView. This method runs
    // SSAO, bloom, tonemapping, and FXAA, then blits the final result
    // to the backbuffer (or final RT override).
    //**********************************************************************
    void CRenderer::RunPostProcessing()
    {
#ifdef _DEBUG
        CComPtr<ID3D11DeviceContext2> spCtx2;
        CT(m_spContext->QueryInterface<ID3D11DeviceContext2>(&spCtx2));
        spCtx2->BeginEventInt(L"PostProcessing", 0);
#endif

        // Disable depth testing for all post-processing passes
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.DepthEnable = false;
        CComPtr<ID3D11DepthStencilState> spNoDepthState;
        CT(m_spDevice->CreateDepthStencilState(&depthStencilDesc, &spNoDepthState));
        m_spContext->OMSetDepthStencilState(spNoDepthState, 0);

        // Set up rasterizer for fullscreen quads
        D3D11_RASTERIZER_DESC rastDesc;
        rastDesc.FrontCounterClockwise = false;
        rastDesc.CullMode = D3D11_CULL_NONE;
        rastDesc.AntialiasedLineEnable = false;
        rastDesc.DepthBias = 0;
        rastDesc.DepthBiasClamp = 0.0f;
        rastDesc.DepthClipEnable = true;
        rastDesc.FillMode = D3D11_FILL_SOLID;
        rastDesc.MultisampleEnable = false;
        rastDesc.SlopeScaledDepthBias = 0.0f;
        rastDesc.ScissorEnable = false;
        CComPtr<ID3D11RasterizerState> spRasterizerState;
        CT(m_spDevice->CreateRasterizerState(&rastDesc, &spRasterizerState));
        m_spContext->RSSetState(spRasterizerState);

        // Bind quad geometry
        UINT offset = 0;
        UINT vertexSize = sizeof(CQuadVertex);
        m_spContext->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &vertexSize, &offset);
        m_spContext->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT_R32_UINT, 0);
        m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Track which ITexture holds the current scene image
        // Start with HDR scene render
        CRefObj<ITexture> pCurrentTex = m_spHDRTextureObj;
        int pingPongIndex = 0; // next write target

        std::vector<CRefObj<ILight>> emptyLights;

        //**********************************************************************
        // Pass: SSAO
        //**********************************************************************
        if (m_ssaoEnabled && m_spSSAOShader != nullptr)
        {
#ifdef _DEBUG
            spCtx2->BeginEventInt(L"SSAO", 0);
#endif
            // SSAO pass: read scene color + depth, write AO-modulated result
            m_spContext->OMSetRenderTargets(1, &m_spPostProcessRTV[pingPongIndex].p, nullptr);
            FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            m_spContext->ClearRenderTargetView(m_spPostProcessRTV[pingPongIndex], black);

            D3D11_VIEWPORT ppVP = {};
            ppVP.Width = (float)m_BBDesc.Width;
            ppVP.Height = (float)m_BBDesc.Height;
            ppVP.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &ppVP);

            m_spSSAOShader->SetPSParam(L"sceneTexture", std::any(pCurrentTex));
            m_spSSAOShader->SetPSParam(L"depthTexture", std::any(m_spDepthTextureObj));
            m_spSSAOShader->SetPSParamFloat(L"screenWidth", (float)m_BBDesc.Width);
            m_spSSAOShader->SetPSParamFloat(L"screenHeight", (float)m_BBDesc.Height);

            m_spSSAOShader->SetVSParamFloat(L"minu", 0.0f);
            m_spSSAOShader->SetVSParamFloat(L"minv", 0.0f);
            m_spSSAOShader->SetVSParamFloat(L"maxu", 1.0f);
            m_spSSAOShader->SetVSParamFloat(L"maxv", 1.0f);

            m_spSSAOShader->BeginRender(this, nullptr, emptyLights, nullptr);
            m_spContext->DrawIndexed(6, 0, 0);
            m_spSSAOShader->EndRender(this);

            pCurrentTex = m_spPostProcessTexObj[pingPongIndex];
            pingPongIndex = 1 - pingPongIndex;

            // SSAO blur pass
            if (m_spSSAOBlurShader != nullptr)
            {
                m_spContext->OMSetRenderTargets(1, &m_spPostProcessRTV[pingPongIndex].p, nullptr);
                m_spContext->ClearRenderTargetView(m_spPostProcessRTV[pingPongIndex], black);

                m_spSSAOBlurShader->SetPSParam(L"tex", std::any(pCurrentTex));
                m_spSSAOBlurShader->SetPSParamFloat(L"texelWidth", 1.0f / (float)m_BBDesc.Width);
                m_spSSAOBlurShader->SetPSParamFloat(L"texelHeight", 1.0f / (float)m_BBDesc.Height);

                m_spSSAOBlurShader->SetVSParamFloat(L"minu", 0.0f);
                m_spSSAOBlurShader->SetVSParamFloat(L"minv", 0.0f);
                m_spSSAOBlurShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spSSAOBlurShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spSSAOBlurShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spSSAOBlurShader->EndRender(this);

                pCurrentTex = m_spPostProcessTexObj[pingPongIndex];
                pingPongIndex = 1 - pingPongIndex;
            }
#ifdef _DEBUG
            spCtx2->EndEvent();
#endif
        }

        //**********************************************************************
        // Pass: SSR (Screen-Space Reflections)
        //**********************************************************************
        if (m_ssrEnabled && m_spSSRShader != nullptr && m_spNormalBufferObj != nullptr)
        {
#ifdef _DEBUG
            spCtx2->BeginEventInt(L"SSR", 0);
#endif
            m_spContext->OMSetRenderTargets(1, &m_spPostProcessRTV[pingPongIndex].p, nullptr);
            FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
            m_spContext->ClearRenderTargetView(m_spPostProcessRTV[pingPongIndex], black);

            D3D11_VIEWPORT ppVP = {};
            ppVP.Width = (float)m_BBDesc.Width;
            ppVP.Height = (float)m_BBDesc.Height;
            ppVP.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &ppVP);

            m_spSSRShader->SetPSParam(L"sceneTexture", std::any(pCurrentTex));
            m_spSSRShader->SetPSParam(L"depthTexture", std::any(m_spDepthTextureObj));
            m_spSSRShader->SetPSParam(L"normalTexture", std::any(m_spNormalBufferObj));

            // Pass projection matrix and inverse
            DirectX::XMFLOAT4X4 f4x4;
            DirectX::XMStoreFloat4x4(&f4x4, GetCamera()->GetProjection());
            Matrix projMat(reinterpret_cast<float*>(&f4x4));
            m_spSSRShader->SetPSParam(L"projMatrix", std::any(projMat));
            DirectX::XMStoreFloat4x4(&f4x4, DirectX::XMMatrixInverse(nullptr, GetCamera()->GetProjection()));
            Matrix projInvMat(reinterpret_cast<float*>(&f4x4));
            m_spSSRShader->SetPSParam(L"projInv", std::any(projInvMat));

            m_spSSRShader->SetPSParamFloat(L"screenWidth", (float)m_BBDesc.Width);
            m_spSSRShader->SetPSParamFloat(L"screenHeight", (float)m_BBDesc.Height);
            m_spSSRShader->SetPSParamFloat(L"maxDistance", 50.0f);
            m_spSSRShader->SetPSParamFloat(L"thickness", 0.5f);
            m_spSSRShader->SetVSParamFloat(L"minu", 0.0f);
            m_spSSRShader->SetVSParamFloat(L"minv", 0.0f);
            m_spSSRShader->SetVSParamFloat(L"maxu", 1.0f);
            m_spSSRShader->SetVSParamFloat(L"maxv", 1.0f);

            m_spSSRShader->BeginRender(this, nullptr, emptyLights, nullptr);
            m_spContext->DrawIndexed(6, 0, 0);
            m_spSSRShader->EndRender(this);

            pCurrentTex = m_spPostProcessTexObj[pingPongIndex];
            pingPongIndex = 1 - pingPongIndex;
#ifdef _DEBUG
            spCtx2->EndEvent();
#endif
        }

        //**********************************************************************
        // Pass: Volumetric Fog (half-res ray-march then bilateral composite)
        //**********************************************************************
        if (m_fogEnabled && m_spFogShader != nullptr && m_spFogCompositeShader != nullptr && m_spFogRT != nullptr)
        {
#ifdef _DEBUG
            spCtx2->BeginEventInt(L"VolumetricFog", 0);
#endif
            // Step 1: Half-res fog ray-march
            {
                uint32_t fogW = m_BBDesc.Width / 2;
                uint32_t fogH = m_BBDesc.Height / 2;
                fogW = (fogW < 1) ? 1 : fogW;
                fogH = (fogH < 1) ? 1 : fogH;
                D3D11_VIEWPORT fogVP = {};
                fogVP.Width = (float)fogW;
                fogVP.Height = (float)fogH;
                fogVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &fogVP);
                m_spContext->OMSetRenderTargets(1, &m_spFogRTV.p, nullptr);
                FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                m_spContext->ClearRenderTargetView(m_spFogRTV, black);

                m_spFogShader->SetPSParam(L"depthTexture", std::any(m_spDepthTextureObj));

                // Bind directional shadow map if available
                m_spFogShader->SetPSParam(L"shadowMapTexture", std::any(GetShadowmapTexture(0)));

                // Camera matrices
                DirectX::XMFLOAT4X4 f4x4;
                DirectX::XMStoreFloat4x4(&f4x4, DirectX::XMMatrixInverse(nullptr, GetCamera()->GetView()));
                Matrix viewInvMat(reinterpret_cast<float*>(&f4x4));
                m_spFogShader->SetPSParam(L"viewInv", std::any(viewInvMat));

                DirectX::XMStoreFloat4x4(&f4x4, DirectX::XMMatrixInverse(nullptr, GetCamera()->GetProjection()));
                Matrix projInvMat(reinterpret_cast<float*>(&f4x4));
                m_spFogShader->SetPSParam(L"projInv", std::any(projInvMat));

                DirectX::XMStoreFloat4x4(&f4x4, GetCamera()->GetView());
                Matrix viewMat(reinterpret_cast<float*>(&f4x4));
                m_spFogShader->SetPSParam(L"view", std::any(viewMat));

                // Cascade shadow data
                for (int c = 0; c < c_NumCascades; c++)
                {
                    DirectX::XMStoreFloat4x4(&f4x4, m_cascadeData.cascadeViewProj[c]);
                    Matrix cascMat(reinterpret_cast<float*>(&f4x4));
                    m_spFogShader->SetPSParam(L"cascadeViewProj", c, std::any(cascMat));
                    Float4 splitDepth(m_cascadeData.cascadeSplitDepths[c], 0.0f, 0.0f, 0.0f);
                    m_spFogShader->SetPSParam(L"cascadeSplitDepths", c, std::any(splitDepth));
                }

                // Camera position
                Vector3 camPos;
                GetCamera()->GetPosition(&camPos, nullptr, nullptr, nullptr, nullptr, nullptr);
                m_spFogShader->SetPSParamFloat(L"cameraPosX", camPos.x);
                m_spFogShader->SetPSParamFloat(L"cameraPosY", camPos.y);
                m_spFogShader->SetPSParamFloat(L"cameraPosZ", camPos.z);

                // Fog parameters
                m_spFogShader->SetPSParamFloat(L"fogDensity", m_fogDensity);
                m_spFogShader->SetPSParam(L"fogColor", std::any(Float3(m_fogColor.r, m_fogColor.g, m_fogColor.b)));
                m_spFogShader->SetPSParamFloat(L"fogHeightFalloff", m_fogHeightFalloff);
                m_spFogShader->SetPSParamFloat(L"fogScattering", m_fogScattering);
                m_spFogShader->SetPSParamFloat(L"fogMaxDistance", m_fogMaxDistance);
                m_spFogShader->SetPSParamFloat(L"fogStartHeight", m_fogStartHeight);

                // Light direction and color (use first directional light)
                m_spFogShader->SetPSParam(L"lightDirWS", std::any(Float3(0.0f, -1.0f, 0.0f)));
                m_spFogShader->SetPSParamFloat(L"lightIntensity", 1.0f);
                m_spFogShader->SetPSParam(L"lightColor", std::any(Float3(1.0f, 1.0f, 1.0f)));

                m_spFogShader->SetPSParamFloat(L"screenWidth", (float)m_BBDesc.Width);
                m_spFogShader->SetPSParamFloat(L"screenHeight", (float)m_BBDesc.Height);

                // Camera
                m_spFogShader->SetPSParam(L"cameraPosWS", std::any(Float3(camPos.x, camPos.y, camPos.z)));
                m_spFogShader->SetPSParamFloat(L"nearPlane", 0.1f);
                m_spFogShader->SetPSParamFloat(L"farPlane", 1000.0f);

                m_spFogShader->SetVSParamFloat(L"minu", 0.0f);
                m_spFogShader->SetVSParamFloat(L"minv", 0.0f);
                m_spFogShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spFogShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spFogShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spFogShader->EndRender(this);
            }

            // Step 2: Bilateral upsample + composite
            {
                D3D11_VIEWPORT ppVP = {};
                ppVP.Width = (float)m_BBDesc.Width;
                ppVP.Height = (float)m_BBDesc.Height;
                ppVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &ppVP);
                m_spContext->OMSetRenderTargets(1, &m_spPostProcessRTV[pingPongIndex].p, nullptr);

                m_spFogCompositeShader->SetPSParam(L"sceneTexture", std::any(pCurrentTex));
                m_spFogCompositeShader->SetPSParam(L"fogTexture", std::any(m_spFogRT));
                m_spFogCompositeShader->SetPSParam(L"depthTexture", std::any(m_spDepthTextureObj));
                m_spFogCompositeShader->SetPSParamFloat(L"screenWidth", (float)m_BBDesc.Width);
                m_spFogCompositeShader->SetPSParamFloat(L"screenHeight", (float)m_BBDesc.Height);
                m_spFogCompositeShader->SetPSParamFloat(L"fogTexWidth", (float)(m_BBDesc.Width / 2));
                m_spFogCompositeShader->SetPSParamFloat(L"fogTexHeight", (float)(m_BBDesc.Height / 2));
                m_spFogCompositeShader->SetPSParamFloat(L"depthThreshold", 0.01f);

                m_spFogCompositeShader->SetVSParamFloat(L"minu", 0.0f);
                m_spFogCompositeShader->SetVSParamFloat(L"minv", 0.0f);
                m_spFogCompositeShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spFogCompositeShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spFogCompositeShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spFogCompositeShader->EndRender(this);

                pCurrentTex = m_spPostProcessTexObj[pingPongIndex];
                pingPongIndex = 1 - pingPongIndex;
            }
#ifdef _DEBUG
            spCtx2->EndEvent();
#endif
        }

        //**********************************************************************
        // Pass: Bloom
        //**********************************************************************
        if (m_bloomEnabled && m_spBloomExtractShader != nullptr && m_spBloomBlurShader != nullptr && m_spBloomCompositeShader != nullptr)
        {
#ifdef _DEBUG
            spCtx2->BeginEventInt(L"Bloom", 0);
#endif
            // Step 1: Bright-pass extraction into first bloom mip
            {
                D3D11_TEXTURE2D_DESC mipDesc;
                m_spBloomMipTexture[0]->GetDesc(&mipDesc);
                D3D11_VIEWPORT bloomVP = {};
                bloomVP.Width = (float)mipDesc.Width;
                bloomVP.Height = (float)mipDesc.Height;
                bloomVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &bloomVP);
                m_spContext->OMSetRenderTargets(1, &m_spBloomMipRTV[0].p, nullptr);
                FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                m_spContext->ClearRenderTargetView(m_spBloomMipRTV[0], black);

                m_spBloomExtractShader->SetPSParam(L"tex", std::any(pCurrentTex));
                m_spBloomExtractShader->SetPSParamFloat(L"threshold", m_bloomThreshold);
                m_spBloomExtractShader->SetVSParamFloat(L"minu", 0.0f);
                m_spBloomExtractShader->SetVSParamFloat(L"minv", 0.0f);
                m_spBloomExtractShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spBloomExtractShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spBloomExtractShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spBloomExtractShader->EndRender(this);
            }

            // Step 2: Progressive downsample + blur
            for (int i = 1; i < c_BloomMipCount; i++)
            {
                D3D11_TEXTURE2D_DESC mipDesc;
                m_spBloomMipTexture[i]->GetDesc(&mipDesc);
                D3D11_VIEWPORT bloomVP = {};
                bloomVP.Width = (float)mipDesc.Width;
                bloomVP.Height = (float)mipDesc.Height;
                bloomVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &bloomVP);
                m_spContext->OMSetRenderTargets(1, &m_spBloomMipRTV[i].p, nullptr);
                FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                m_spContext->ClearRenderTargetView(m_spBloomMipRTV[i], black);

                D3D11_TEXTURE2D_DESC prevDesc;
                m_spBloomMipTexture[i - 1]->GetDesc(&prevDesc);
                m_spBloomBlurShader->SetPSParam(L"tex", std::any(m_spBloomMipTexObj[i - 1]));
                m_spBloomBlurShader->SetPSParamFloat(L"texelWidth", 1.0f / (float)prevDesc.Width);
                m_spBloomBlurShader->SetPSParamFloat(L"texelHeight", 1.0f / (float)prevDesc.Height);
                m_spBloomBlurShader->SetVSParamFloat(L"minu", 0.0f);
                m_spBloomBlurShader->SetVSParamFloat(L"minv", 0.0f);
                m_spBloomBlurShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spBloomBlurShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spBloomBlurShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spBloomBlurShader->EndRender(this);
            }

            // Step 3: Bloom composite + ACES tonemap to post-process RT
            {
                D3D11_VIEWPORT ppVP = {};
                ppVP.Width = (float)m_BBDesc.Width;
                ppVP.Height = (float)m_BBDesc.Height;
                ppVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &ppVP);
                m_spContext->OMSetRenderTargets(1, &m_spPostProcessRTV[pingPongIndex].p, nullptr);
                FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
                m_spContext->ClearRenderTargetView(m_spPostProcessRTV[pingPongIndex], black);

                m_spBloomCompositeShader->SetPSParam(L"sceneTexture", std::any(pCurrentTex));
                m_spBloomCompositeShader->SetPSParam(L"bloom0", std::any(m_spBloomMipTexObj[0]));
                m_spBloomCompositeShader->SetPSParam(L"bloom1", std::any(m_spBloomMipTexObj[1]));
                m_spBloomCompositeShader->SetPSParam(L"bloom2", std::any(m_spBloomMipTexObj[2]));
                m_spBloomCompositeShader->SetPSParam(L"bloom3", std::any(m_spBloomMipTexObj[3]));
                m_spBloomCompositeShader->SetPSParamFloat(L"bloomIntensity", m_bloomIntensity);
                m_spBloomCompositeShader->SetPSParamFloat(L"exposure", m_exposure);
                m_spBloomCompositeShader->SetVSParamFloat(L"minu", 0.0f);
                m_spBloomCompositeShader->SetVSParamFloat(L"minv", 0.0f);
                m_spBloomCompositeShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spBloomCompositeShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spBloomCompositeShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spBloomCompositeShader->EndRender(this);

                pCurrentTex = m_spPostProcessTexObj[pingPongIndex];
                pingPongIndex = 1 - pingPongIndex;
            }
#ifdef _DEBUG
            spCtx2->EndEvent();
#endif
        }
        else
        {
            // No bloom: just do basic ACES tonemap using BloomComposite with zero bloom
            if (m_spBloomCompositeShader != nullptr)
            {
                D3D11_VIEWPORT ppVP = {};
                ppVP.Width = (float)m_BBDesc.Width;
                ppVP.Height = (float)m_BBDesc.Height;
                ppVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &ppVP);
                m_spContext->OMSetRenderTargets(1, &m_spPostProcessRTV[pingPongIndex].p, nullptr);

                m_spBloomCompositeShader->SetPSParam(L"sceneTexture", std::any(pCurrentTex));
                m_spBloomCompositeShader->SetPSParam(L"bloom0", std::any(pCurrentTex)); // dummy
                m_spBloomCompositeShader->SetPSParam(L"bloom1", std::any(pCurrentTex));
                m_spBloomCompositeShader->SetPSParam(L"bloom2", std::any(pCurrentTex));
                m_spBloomCompositeShader->SetPSParam(L"bloom3", std::any(pCurrentTex));
                m_spBloomCompositeShader->SetPSParamFloat(L"bloomIntensity", 0.0f);
                m_spBloomCompositeShader->SetPSParamFloat(L"exposure", m_exposure);
                m_spBloomCompositeShader->SetVSParamFloat(L"minu", 0.0f);
                m_spBloomCompositeShader->SetVSParamFloat(L"minv", 0.0f);
                m_spBloomCompositeShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spBloomCompositeShader->SetVSParamFloat(L"maxv", 1.0f);

                m_spBloomCompositeShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spBloomCompositeShader->EndRender(this);

                pCurrentTex = m_spPostProcessTexObj[pingPongIndex];
                pingPongIndex = 1 - pingPongIndex;
            }
        }

        //**********************************************************************
        // Pass: FXAA
        //**********************************************************************
        if (m_fxaaEnabled && m_spFXAAShader != nullptr)
        {
#ifdef _DEBUG
            spCtx2->BeginEventInt(L"FXAA", 0);
#endif
            // FXAA writes to the final backbuffer directly
            ID3D11RenderTargetView* pFinalRTV = (m_spFinalRTView) ? m_spFinalRTView : m_spRTView;
            m_spContext->OMSetRenderTargets(1, &pFinalRTV, nullptr);

            D3D11_VIEWPORT ppVP = {};
            ppVP.Width = (float)m_BBDesc.Width;
            ppVP.Height = (float)m_BBDesc.Height;
            ppVP.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &ppVP);

            m_spFXAAShader->SetPSParam(L"tex", std::any(pCurrentTex));
            m_spFXAAShader->SetPSParamFloat(L"rcpFrameX", 1.0f / (float)m_BBDesc.Width);
            m_spFXAAShader->SetPSParamFloat(L"rcpFrameY", 1.0f / (float)m_BBDesc.Height);
            m_spFXAAShader->SetVSParamFloat(L"minu", 0.0f);
            m_spFXAAShader->SetVSParamFloat(L"minv", 0.0f);
            m_spFXAAShader->SetVSParamFloat(L"maxu", 1.0f);
            m_spFXAAShader->SetVSParamFloat(L"maxv", 1.0f);

            m_spFXAAShader->BeginRender(this, nullptr, emptyLights, nullptr);
            m_spContext->DrawIndexed(6, 0, 0);
            m_spFXAAShader->EndRender(this);
#ifdef _DEBUG
            spCtx2->EndEvent();
#endif
        }
        else
        {
            // No FXAA: blit current result to backbuffer
            ID3D11RenderTargetView* pFinalRTV = (m_spFinalRTView) ? m_spFinalRTView : m_spRTView;
            m_spContext->OMSetRenderTargets(1, &pFinalRTV, nullptr);

            D3D11_VIEWPORT ppVP = {};
            ppVP.Width = (float)m_BBDesc.Width;
            ppVP.Height = (float)m_BBDesc.Height;
            ppVP.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &ppVP);

            m_spQuadShader->SetPSParam(L"tex", std::any(pCurrentTex));
            m_spQuadShader->SetVSParamFloat(L"minu", 0.0f);
            m_spQuadShader->SetVSParamFloat(L"minv", 0.0f);
            m_spQuadShader->SetVSParamFloat(L"maxu", 1.0f);
            m_spQuadShader->SetVSParamFloat(L"maxv", 1.0f);

            m_spQuadShader->BeginRender(this, nullptr, emptyLights, nullptr);
            m_spContext->DrawIndexed(6, 0, 0);
            m_spQuadShader->EndRender(this);
        }

        // Restore normal viewport
        m_spContext->RSSetViewports(1, &m_viewport);
        m_spContext->RSSetState(m_spRasterizerState);

#ifdef _DEBUG
        spCtx2->EndEvent();
#endif
    }

    //**********************************************************************
    // Method: RenderPointShadows
    // Renders shadow cubemaps for point lights that cast shadows.
    // For each shadow-casting point light, renders the scene depth
    // into 6 cube faces using 90° FOV perspective projection.
    //**********************************************************************
    void CRenderer::RenderPointShadows(int pass, std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback)
    {
        using namespace DirectX;

        m_numPointShadowLights = 0;

        // Cube face view directions and up vectors
        // Order: +X, -X, +Y, -Y, +Z, -Z
        static const XMVECTORF32 faceDirs[6] = {
            {{ 1.0f,  0.0f,  0.0f, 0.0f}},  // +X
            {{-1.0f,  0.0f,  0.0f, 0.0f}},  // -X
            {{ 0.0f,  1.0f,  0.0f, 0.0f}},  // +Y
            {{ 0.0f, -1.0f,  0.0f, 0.0f}},  // -Y
            {{ 0.0f,  0.0f,  1.0f, 0.0f}},  // +Z
            {{ 0.0f,  0.0f, -1.0f, 0.0f}}   // -Z
        };
        static const XMVECTORF32 faceUps[6] = {
            {{ 0.0f, 1.0f,  0.0f, 0.0f}},  // +X
            {{ 0.0f, 1.0f,  0.0f, 0.0f}},  // -X
            {{ 0.0f, 0.0f, -1.0f, 0.0f}},  // +Y
            {{ 0.0f, 0.0f,  1.0f, 0.0f}},  // -Y
            {{ 0.0f, 1.0f,  0.0f, 0.0f}},  // +Z
            {{ 0.0f, 1.0f,  0.0f, 0.0f}}   // -Z
        };

        for (int li = 0; li < (int)m_lights.size() && m_numPointShadowLights < c_MaxPointShadowLights; li++)
        {
            if (m_lights[li]->GetType() != ELightType::PointLight || !m_lights[li]->GetCastsShadows())
                continue;

            int shadowIdx = m_numPointShadowLights;
            Vector3 lightPos = m_lights[li]->GetPosition();
            float lightRange = m_lights[li]->GetRange();
            XMVECTOR lightPosV = XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 1.0f);

            // 90° FOV perspective projection
            XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, 0.1f, lightRange);

            // Render each face
            for (uint32_t face = 0; face < 6; face++)
            {
                // Save current state
                ShadowMapRenderState rs;
                m_spContext->OMGetRenderTargets(1, &rs.m_spOldRT, &rs.m_spOldStencil);
                rs.m_spOldCamera = m_spCamera;
                rs.m_viewport = m_viewport;
                m_shadowMapRenderState.push(rs);

                // Set face DSV as render target
                CComPtr<ID3D11DepthStencilView> spFaceDSV = m_spPointShadowCubemap[shadowIdx]->GetFaceDSV(face);
                m_spContext->OMSetRenderTargets(0, nullptr, spFaceDSV);
                m_spContext->ClearDepthStencilView(spFaceDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

                // Set viewport
                D3D11_VIEWPORT vp = {};
                vp.Width = (float)c_PointShadowMapSize;
                vp.Height = (float)c_PointShadowMapSize;
                vp.MinDepth = 0.0f;
                vp.MaxDepth = 1.0f;
                m_viewport = vp;
                m_spContext->RSSetViewports(1, &m_viewport);

                // Create shadow camera for this face
                XMMATRIX viewMatrix = XMMatrixLookToLH(lightPosV, faceDirs[face], faceUps[face]);
                CRefObj<ICamera> spShadowCamera = CreateCamera(true);
                XMFLOAT3 target;
                XMStoreFloat3(&target, XMVectorAdd(lightPosV, faceDirs[face]));
                Vector3 dir(target.x - lightPos.x, target.y - lightPos.y, target.z - lightPos.z);
                XMFLOAT3 upF;
                XMStoreFloat3(&upF, faceUps[face]);
                Vector3 up(upF.x, upF.y, upF.z);
                spShadowCamera->SetPosition(lightPos, dir, up);
                spShadowCamera->SetParams(90.0f, 1.0f, 0.1f, lightRange);
                this->SetCamera(spShadowCamera);

                // Render shadow-casting objects
                for (size_t j = 0; j < m_singleObjs.size(); j++)
                {
                    if (m_singleObjs[j]->InPass(pass))
                        m_singleObjs[j]->Render(this, m_lights, m_spRenderCtx);
                }

                // Restore state
                ShadowMapRenderState rsPop = m_shadowMapRenderState.top();
                this->SetCamera(rsPop.m_spOldCamera);
                m_viewport = rsPop.m_viewport;
                m_spContext->RSSetViewports(1, &m_viewport);
                m_shadowMapRenderState.pop();
                m_spContext->OMSetRenderTargets(1, &rsPop.m_spOldRT.p, rsPop.m_spOldStencil);
            }

            m_numPointShadowLights++;
        }
    }

    //**********************************************************************
    // Method: RenderSpotShadows
    // Renders shadow maps for shadow-casting spot lights. Each spot light
    // uses a single perspective frustum aligned to the light's direction
    // with the outer cone angle as the FOV.
    //**********************************************************************
    void CRenderer::RenderSpotShadows(int pass, std::function<void(IRenderer* pRenderer, IRenderCtx* pRenderCtx, int pass)> renderCallback)
    {
        m_numSpotShadowLights = 0;
        for (size_t i = 0; i < m_lights.size() && m_numSpotShadowLights < c_MaxSpotShadowLights; i++)
        {
            if (m_lights[i]->GetType() != ELightType::SpotLight || !m_lights[i]->GetCastsShadows())
                continue;

            int idx = m_numSpotShadowLights;
            Vector3 lightPos = m_lights[i]->GetPosition();
            Vector3 lightDir = m_lights[i]->GetDirection();
            float lightRange = m_lights[i]->GetRange();

            // Get outer angle for FOV (stored as float2 via ISpotLight::GetAngles)
            // We need to cast to ISpotLight — but since we know it's a spot light, use the outer angle from PushLights
            // The angles are inner.x, outer.y in degrees
            float outerAngle = 45.0f; // default
            // Access the spot light angles through dynamic behavior
            // Since ILight doesn't expose GetAngles, we'll use a reasonable default FOV
            // The outer cone angle is typically between 30-90 degrees
            // For shadow map, we use 2x outer angle as the FOV to cover the full cone
            float fov = outerAngle * 2.0f;

            // Push shadow render state
            ShadowMapRenderState rs;
            rs.m_viewport = m_viewport;
            rs.m_spOldCamera = GetCamera();
            m_spContext->OMGetRenderTargets(1, &rs.m_spOldRT, &rs.m_spOldStencil);
            m_shadowMapRenderState.push(rs);

            // Set up depth-only render target
            m_spContext->OMSetRenderTargets(0, nullptr, m_spSpotShadowDSV[idx]);
            m_spContext->ClearDepthStencilView(m_spSpotShadowDSV[idx], D3D11_CLEAR_DEPTH, 1.0f, 0);

            D3D11_VIEWPORT shadowVP = {};
            shadowVP.Width = (float)c_SpotShadowMapSize;
            shadowVP.Height = (float)c_SpotShadowMapSize;
            shadowVP.MaxDepth = 1.0f;
            m_viewport = shadowVP;
            m_spContext->RSSetViewports(1, &shadowVP);

            // Create shadow camera aligned to spot light
            auto spShadowCamera = CreateCamera(true);
            // Compute an up vector that isn't parallel to lightDir
            Vector3 up(0.0f, 1.0f, 0.0f);
            if (abs(lightDir.y) > 0.99f)
                up = Vector3(1.0f, 0.0f, 0.0f);
            spShadowCamera->SetPosition(lightPos, lightDir, up);
            spShadowCamera->SetParams(fov, 1.0f, 0.1f, lightRange);
            this->SetCamera(spShadowCamera);

            // Store view-projection matrix for shader
            DirectX::XMMATRIX viewMat = spShadowCamera->GetView();
            DirectX::XMMATRIX projMat = spShadowCamera->GetProjection();
            m_spotShadowViewProj[idx] = DirectX::XMMatrixMultiply(viewMat, projMat);

            // Render shadow-casting objects
            for (size_t j = 0; j < m_singleObjs.size(); j++)
            {
                if (m_singleObjs[j]->InPass(pass))
                    m_singleObjs[j]->Render(this, m_lights, m_spRenderCtx);
            }

            // Restore state
            ShadowMapRenderState rsPop = m_shadowMapRenderState.top();
            this->SetCamera(rsPop.m_spOldCamera);
            m_viewport = rsPop.m_viewport;
            m_spContext->RSSetViewports(1, &m_viewport);
            m_shadowMapRenderState.pop();
            m_spContext->OMSetRenderTargets(1, &rsPop.m_spOldRT.p, rsPop.m_spOldStencil);

            m_numSpotShadowLights++;
        }
    }

    //**********************************************************************
    // Method: GenerateIBLMaps
    // Generates irradiance and pre-filtered specular cubemaps from the
    // current environment map for image-based lighting.
    //**********************************************************************
    void CRenderer::GenerateIBLMaps()
    {
        if (m_spEnvironmentMap == nullptr)
            return;

        std::vector<CRefObj<ILight>> emptyLights;
        UINT quadVertexSize = sizeof(CQuadVertex);
        UINT quadOffset = 0;

        // Save current state
        CComPtr<ID3D11RenderTargetView> spOldRT;
        CComPtr<ID3D11DepthStencilView> spOldDS;
        m_spContext->OMGetRenderTargets(1, &spOldRT, &spOldDS);
        D3D11_VIEWPORT oldVP = m_viewport;

        //**********************************************************************
        // Generate irradiance cubemap (32x32 per face)
        //**********************************************************************
        if (m_spIrradianceShader != nullptr)
        {
            const int irradSize = 32;
            m_spIrradianceMap = CreateCubemapTexture(this, irradSize, DXGI_FORMAT_R16G16B16A16_FLOAT);

            D3D11_VIEWPORT irradVP = {};
            irradVP.Width = (float)irradSize;
            irradVP.Height = (float)irradSize;
            irradVP.MaxDepth = 1.0f;
            m_spContext->RSSetViewports(1, &irradVP);

            for (int face = 0; face < 6; face++)
            {
                ID3D11RenderTargetView* pFaceRTV = m_spIrradianceMap->GetFaceRTV(face);
                m_spContext->OMSetRenderTargets(1, &pFaceRTV, nullptr);
                FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
                m_spContext->ClearRenderTargetView(pFaceRTV, black);

                m_spIrradianceShader->SetPSParam(L"environmentMap", std::any(m_spEnvironmentMap));
                m_spIrradianceShader->SetPSParamInt(L"faceIndex", face);
                m_spIrradianceShader->SetVSParamFloat(L"minu", 0.0f);
                m_spIrradianceShader->SetVSParamFloat(L"minv", 0.0f);
                m_spIrradianceShader->SetVSParamFloat(L"maxu", 1.0f);
                m_spIrradianceShader->SetVSParamFloat(L"maxv", 1.0f);
                m_spIrradianceShader->BeginRender(this, nullptr, emptyLights, nullptr);
                m_spContext->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &quadVertexSize, &quadOffset);
                m_spContext->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT_R32_UINT, 0);
                m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                m_spContext->DrawIndexed(6, 0, 0);
                m_spIrradianceShader->EndRender(this);
            }
        }

        //**********************************************************************
        // Generate pre-filtered specular cubemap (128x128, 5 mip levels)
        //**********************************************************************
        if (m_spPrefilterShader != nullptr)
        {
            const int prefilterSize = 128;
            const int maxMipLevels = 5;
            m_spPrefilteredMap = CreateCubemapTexture(this, prefilterSize, DXGI_FORMAT_R16G16B16A16_FLOAT, maxMipLevels);

            for (int mip = 0; mip < maxMipLevels; mip++)
            {
                int mipWidth = prefilterSize >> mip;
                int mipHeight = prefilterSize >> mip;
                float roughness = (float)mip / (float)(maxMipLevels - 1);

                D3D11_VIEWPORT prefilterVP = {};
                prefilterVP.Width = (float)mipWidth;
                prefilterVP.Height = (float)mipHeight;
                prefilterVP.MaxDepth = 1.0f;
                m_spContext->RSSetViewports(1, &prefilterVP);

                for (int face = 0; face < 6; face++)
                {
                    // Create RTV for this specific mip level of this face
                    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
                    rtvDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                    rtvDesc.Texture2DArray.MipSlice = mip;
                    rtvDesc.Texture2DArray.FirstArraySlice = face;
                    rtvDesc.Texture2DArray.ArraySize = 1;

                    CComPtr<ID3D11RenderTargetView> spMipFaceRTV;
                    CT(m_spDevice->CreateRenderTargetView(m_spPrefilteredMap->GetD3DTexture(), &rtvDesc, &spMipFaceRTV));

                    m_spContext->OMSetRenderTargets(1, &spMipFaceRTV.p, nullptr);
                    FLOAT black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
                    m_spContext->ClearRenderTargetView(spMipFaceRTV, black);

                    m_spPrefilterShader->SetPSParam(L"environmentMap", std::any(m_spEnvironmentMap));
                    m_spPrefilterShader->SetPSParamInt(L"faceIndex", face);
                    m_spPrefilterShader->SetPSParamFloat(L"roughness", roughness);
                    m_spPrefilterShader->SetPSParamFloat(L"envMapSize", (float)prefilterSize);
                    m_spPrefilterShader->SetVSParamFloat(L"minu", 0.0f);
                    m_spPrefilterShader->SetVSParamFloat(L"minv", 0.0f);
                    m_spPrefilterShader->SetVSParamFloat(L"maxu", 1.0f);
                    m_spPrefilterShader->SetVSParamFloat(L"maxv", 1.0f);
                    m_spPrefilterShader->BeginRender(this, nullptr, emptyLights, nullptr);
                    m_spContext->IASetVertexBuffers(0, 1, &m_spQuadVB.p, &quadVertexSize, &quadOffset);
                    m_spContext->IASetIndexBuffer(m_spQuadIB, DXGI_FORMAT_R32_UINT, 0);
                    m_spContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                    m_spContext->DrawIndexed(6, 0, 0);
                    m_spPrefilterShader->EndRender(this);
                }
            }
        }

        // Restore state
        m_spContext->OMSetRenderTargets(1, &spOldRT.p, spOldDS);
        m_viewport = oldVP;
        m_spContext->RSSetViewports(1, &m_viewport);

        m_iblDirty = false;
    }

    //**********************************************************************
    // GPU-side light data struct (must match LightData in defs.h)
    //**********************************************************************
    struct GPULightData
    {
        float posWS[4];    // xyz = position, w = range
        float dirWS[4];    // xyz = direction, w = unused
        float color[4];    // rgb = color, a = unused
        float intensity;
        int type;
        int shadowIndex;
        float innerAngle;  // spot inner cone angle in degrees
        float outerAngle;  // spot outer cone angle in degrees
        float pad[3];
    };

    //**********************************************************************
    // Method: DispatchTileLightCull
    // Runs the tiled light culling compute shader to build per-tile light
    // index lists for the current frame's lights.
    //**********************************************************************
    void CRenderer::DispatchTileLightCull()
    {
        if (m_spTileCullShader == nullptr || !m_tiledLightingEnabled)
            return;
        if (m_lights.empty())
            return;

        const int TILE_SIZE = 16;
        const int MAX_TILE_LIGHTS = 32;
        int numLights = (int)m_lights.size();

        // Create/recreate light buffer if needed
        if (m_spLightBuffer == nullptr || numLights > 0)
        {
            m_spLightBuffer = CreateGPUBuffer(this, EBufferType::StructuredBuffer,
                numLights, sizeof(GPULightData), 0);
        }

        // Upload light data
        std::vector<GPULightData> lightData(numLights);
        for (int i = 0; i < numLights; i++)
        {
            Vector3 pos = m_lights[i]->GetPosition();
            lightData[i].posWS[0] = pos.x;
            lightData[i].posWS[1] = pos.y;
            lightData[i].posWS[2] = pos.z;
            lightData[i].posWS[3] = m_lights[i]->GetRange();

            Vector3 dir = m_lights[i]->GetDirection();
            lightData[i].dirWS[0] = dir.x;
            lightData[i].dirWS[1] = dir.y;
            lightData[i].dirWS[2] = dir.z;
            lightData[i].dirWS[3] = 0.0f;

            FRGBColor clr = m_lights[i]->GetColor();
            lightData[i].color[0] = clr.r;
            lightData[i].color[1] = clr.g;
            lightData[i].color[2] = clr.b;
            lightData[i].color[3] = 0.0f;

            lightData[i].intensity = m_lights[i]->GetIntensity();
            lightData[i].type = (int)m_lights[i]->GetType();
            lightData[i].shadowIndex = m_lights[i]->GetCastsShadows() ? i : -1;
            lightData[i].innerAngle = 0.0f;
            lightData[i].outerAngle = 0.0f;
            if (m_lights[i]->GetType() == ELightType::SpotLight)
            {
                auto angles = dynamic_cast<ISpotLight*>(m_lights[i].p)->GetAngles();
                lightData[i].innerAngle = angles.x;
                lightData[i].outerAngle = angles.y;
            }
            lightData[i].pad[0] = lightData[i].pad[1] = lightData[i].pad[2] = 0.0f;
        }
        m_spLightBuffer->CopyFromCPU(this, (uint8_t*)lightData.data());

        // Create tile output buffer
        int numTilesX = (m_BBDesc.Width + TILE_SIZE - 1) / TILE_SIZE;
        int numTilesY = (m_BBDesc.Height + TILE_SIZE - 1) / TILE_SIZE;
        int totalTiles = numTilesX * numTilesY;
        int tileBufferElems = totalTiles * (MAX_TILE_LIGHTS + 1);

        if (m_spTileLightBuffer == nullptr)
        {
            m_spTileLightBuffer = CreateGPUBuffer(this, EBufferType::RWStructuredBuffer,
                tileBufferElems, sizeof(uint32_t), 0);
        }

        // Set compute shader params
        Matrix viewMat;
        {
            DirectX::XMFLOAT4X4 f4x4;
            DirectX::XMStoreFloat4x4(&f4x4, GetCamera()->GetView());
            viewMat = Matrix(reinterpret_cast<float*>(&f4x4));
        }
        m_spTileCullShader->SetCSParam(L"viewMatrix", std::any(viewMat));
        Matrix projMat;
        {
            DirectX::XMFLOAT4X4 f4x4;
            DirectX::XMStoreFloat4x4(&f4x4, GetCamera()->GetProjection());
            projMat = Matrix(reinterpret_cast<float*>(&f4x4));
        }
        m_spTileCullShader->SetCSParam(L"projMatrix", std::any(projMat));
        m_spTileCullShader->SetCSParamInt(L"numLights", numLights);
        m_spTileCullShader->SetCSParamInt(L"screenWidth", (int)m_BBDesc.Width);
        m_spTileCullShader->SetCSParamInt(L"screenHeight", (int)m_BBDesc.Height);
        m_spTileCullShader->SetCSParam(L"lightBuffer", std::any(m_spLightBuffer));
        m_spTileCullShader->SetCSParam(L"depthTexture", std::any(m_spDepthTextureObj));
        m_spTileCullShader->SetCSParam(L"tileLightData", std::any(m_spTileLightBuffer));

        // Dispatch
        std::vector<CRefObj<ILight>> emptyLights;
        m_spTileCullShader->BeginRender(this, nullptr, emptyLights, nullptr);
        m_spContext->Dispatch(numTilesX, numTilesY, 1);
        m_spTileCullShader->EndRender(this);
    }
}
