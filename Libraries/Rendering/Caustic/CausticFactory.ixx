//**********************************************************************
// Copyright Patrick Sweeney 2019-2022
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

export module Rendering.Caustic.CausticFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.ICausticFactory;

//**********************************************************************
// File: CausticFactory.ixx
// Contains the declaration for our <CCausticFactory> class.
//**********************************************************************

export namespace Caustic
{
    //**********************************************************************
    // Class: CCausticFactory
    // Implements <ICausticFactory>
    //
    // Members:
    // factory - The CausticFactory singleton
    //
    // Module:
    // {Link:import Rendering.Caustic.CausticFactory;{Rendering/Caustic/CausticFactory.ixx}}
    //**********************************************************************
    class CCausticFactory : public ICausticFactory, public CRefCount
    {
        static CRefObj<ICausticFactory> factory;
    public:
        CCausticFactory();
        ~CCausticFactory();

        //**********************************************************************
        // Method: Instance
        // Returns:
        // Returns the CCausticFactory singleton.
        //**********************************************************************
        static CRefObj<ICausticFactory> Instance()
        {
            if (factory == nullptr)
            {
                factory = CRefObj<ICausticFactory>(new CCausticFactory);
            }
            return factory;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ICausticFactory
        //**********************************************************************
        virtual CRefObj<IRenderer> CreateRenderer(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool startFrozen = false, int desktopIndex = 0) override;
        virtual CRefObj<IPointCloud> CreatePointCloud(IRenderer* pRenderer, uint32_t width, uint32_t height) override;
        virtual CRefObj<IRenderMesh> CreateRenderMesh() override;
        virtual CRefObj<IRenderSubMesh> CreateRenderSubMesh() override;
        virtual CRefObj<IPointLight> CreatePointLight(Vector3& pos, FRGBColor& color, float intensity, bool castShadows = true, bool castsLight = true) override;
        virtual CRefObj<ISpotLight> CreateSpotLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity = 1000.0f, float innerAngle = 30.0f, float outerAngle = 45.0f, bool castsShadows = true, bool castsLight = true) override;
        virtual CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity, bool castShadows = true, bool castsLight = true) override;
        virtual CRefObj<ITrackball> CreateTrackball() override;
        virtual CRefObj<IMaterialAttrib> CreateMaterialAttrib() override;
        virtual CRefObj<IRenderMaterial> CreateRenderMaterial(IRenderer *pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader) override;
        virtual CRefObj<IRenderable> CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat) override;
        virtual CRefObj<ISampler> CreateSampler(IRenderer *pRenderer, ITexture* pTexture) override;
        virtual CRefObj<ICamera> CreateCamera(bool leftHanded) override;
        virtual CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, uint32_t width, uint32_t height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) override;
        virtual CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, IImage *pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) override;
        virtual CRefObj<ITexture> CheckerboardTexture(IRenderer *pRenderer) override;
        virtual CRefObj<ITexture> LoadTexture(const wchar_t *pFilename, IRenderer *pRenderer) override;
        virtual CRefObj<IVideoTexture> LoadVideoTexture(const wchar_t* pFilename, IRenderer* pRenderer) override;
        virtual CRefObj<ITexture> VideoTextureFromWebcam(IRenderer* pRenderer) override;
        virtual CRefObj<ITexture> CreateDesktopTexture(IRenderer* pRenderer) override;
        virtual CRefObj<IShader> CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob* pPixelShaderBlob, ID3DBlob* pComputeShaderBlob,
            IShaderInfo *pShaderInfo) override;
        virtual CRefObj<IShaderInfo> CreateShaderInfo(const wchar_t *pFilename) override;
    };
};
