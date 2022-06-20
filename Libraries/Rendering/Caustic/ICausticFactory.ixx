//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <Windows.h>
#include <atlbase.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <any>
#include <string>

export module Rendering.Caustic.ICausticFactory;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Base.Math.BBox;
import Imaging.Color;
import Imaging.Image.IImage;
import Geometry.Mesh.Mesh;
import Rendering.Caustic.ITrackball;
import Rendering.Caustic.ICamera;
import Rendering.Caustic.ISpotLight;
import Rendering.Caustic.IPointLight;
import Rendering.Caustic.IPointCloud;
import Rendering.Caustic.IDirectionalLight;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.ISampler;

//**********************************************************************
// File: ICausticFactory.ixx
// Primary interface through which clients talk to the Caustic factory.
//**********************************************************************

export namespace Caustic
{
	//**********************************************************************
	// Interface: ICausticFactory
	// Defines the main factory for creating Caustic objects
	//
	// Module:
	// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
	//**********************************************************************
	struct ICausticFactory : public IRefCount
	{
		//**********************************************************************
		// Method: CreateRenderer 
		// Creates our basic renderer object to be used by the client
		//
		// Parameters:
		// hwnd - HWND to attach D3D renderer to
		// viewport - viewport for final render output (0..1)
		// shaderFolder - folder in which to search for shaders
		// startFrozen - should renderer start in frozen state
		// desktopIndex - index indicating which desktop should duplication service use
		//    (this is for Caustic::CreateDesktopTexture())
		//
		// Returns:
		// Returns the created renderer
		//
		// CreateRenderer creates the renderer object that the client will use to
		// talk to the renderer. This object runs on the clients thread and acts
		// only as a proxy for marshalling commands+data over to the renderer thread.
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IRenderer> CreateRenderer(HWND hwnd, BBox2 &viewport, std::wstring &shaderFolder, bool startFrozen = false, int desktopIndex = 0) = 0;

		//**********************************************************************
		// Method: CreatePointCloud
		// Creates a point cloud
		//
		// Parameters:
		// pRenderer - renderer
		// width - width of depth map that generates the point cloud
		// height - height of depth map that generates the point cloud
		// pFrontMaterial - material for front side of points
		// pBackMaterial - material for back side of points
		// mat - transformation to apply to points
		//
		// Returns:
		// Returns the created point cloud
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IPointCloud> CreatePointCloud(IRenderer* pRenderer, uint32 width, uint32 height) = 0;
		
		//**********************************************************************
		// Method: CreateRenderMesh
		// Creates a render mesh object
		//
		// Returns:
		// Returns the created render mesh
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IRenderMesh> CreateRenderMesh() = 0;

		//**********************************************************************
		// Method: CreateRenderSubMesh
		// Creates a render submesh object
		//
		// Returns:
		// Returns the created render submesh
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IRenderSubMesh> CreateRenderSubMesh() = 0;

		//**********************************************************************
		// Method: CreatePointLight
		// Creates a point light source
		//
		// Parameters:
		// pos - position of light source in world coordinates
		// color - color of light
		// intensity - light's intensity
		//
		// Returns:
		// Returns the point light source
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IPointLight> CreatePointLight(Vector3& pos, FRGBColor& color, float intensity) = 0;

		//**********************************************************************
		// Method: CreateSpotLight
		// Creates a spot light
		//
		// Parameters:
		// pos - position of light source in world coordinates
		// dir - direction light is pointing 
		// color - color of light
		// intensity - light's intensity
		// innerAngle - inner angle defining where light is full intensity
		// outerAngle - outer angle. Light falls off from full to no intensity between inner and outer angle
		// casts - does light participate in shadow mapping?
		//
		// Returns:
		// Returns the spotlight
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ISpotLight> CreateSpotLight(Vector3& pos, Vector3& dir, FRGBColor& color, float intensity = 1000.0f, float innerAngle = 30.0f, float outerAngle = 45.0f, bool casts = true) = 0;

		//**********************************************************************
		// Method: CreateDirectionalLight
		// Creates a directional light source
		//
		// Parameters:
		// pos - position of light source in world coordinates
		// dir - direction of light source in world coordinates
		// color - color of light
		// intensity - light's intensity
		//
		// Returns:
		// Returns the directional light source
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IDirectionalLight> CreateDirectionalLight(Vector3 &pos, Vector3& dir, FRGBColor& color, float intensity) = 0;

		//**********************************************************************
		// Method: CreateTrackball
		// Creates a new trackball object
		//
		// Returns:
		// Returns the created trackball
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITrackball> CreateTrackball() = 0;

		//**********************************************************************
		// Method: CreateMaterialAttrib
		// Creates a MaterialAttrib object.
		//
		// Returns:
		// Returns the created MaterialAttrib object
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IMaterialAttrib> CreateMaterialAttrib() = 0;

		//**********************************************************************
		// Method: CreateRenderMaterial
		// Creates a RenderMaterial object.
		//
		// Parameters:
		// pRenderer - Graphics device
		// pMaterialAttrib - Material definition
		// pShader - Vertex+Pixel shader
		//
		// Returns:
		// Returns the created RenderMaterial object
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<IRenderMaterial> CreateRenderMaterial(IRenderer *pRenderer, IMaterialAttrib *pMaterialAttrib, IShader *pShader) = 0;
		
		//**********************************************************************
		// Method: CreateRenderable
		// Creates a renderable object. A Renderable is a mesh+material+shader.
		//
		// Parameters:
		// pSubMesh - mesh object
        // pFrontMaterial - Material for front faces
        // pBackMaterial - Material for back faces
        // mat - Matrix to apply
		//
		// Returns:
        // Returns the created Renderable object
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
        virtual CRefObj<IRenderable> CreateRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat) = 0;

		//**********************************************************************
		// Method: CreateSampler
		// Creates a new sampler. Samplers defined how shaders read from textures.
		//
		// Parameters:
		// pRenderer - graphics device
		// pTexture - texture to associate with sampler
		//
		// Returns:
		// Returns the newly create sampler
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ISampler> CreateSampler(IRenderer* pRenderer, ITexture *pTexture) = 0;

		//**********************************************************************
		// Function: CreateCamera
		// Creates a new camera object. This camera can then
		// be passed to IRenderer to be used for rendering.
		//
		// Parameters:
		// leftHanded - Defines whether we have a left or right handed coordinate system
		//
		// Returns:
		// Returns the newly created camera object
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ICamera> CreateCamera(bool leftHanded) = 0;

		//**********************************************************************
		// Method: CreateTexture
		// Creates a new texture
		//
		// Parameters:
		// pRenderer - graphics device
		// width - width of texture in pixels
		// height - height of texture in pixels
		// format - pixel format for the texture
		// cpuFlags - DirectX cpu flags
		// bindFlags - DirectX bind flags
		//
		// Returns:
		// Returns the newly created texture
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, uint32 width, uint32 height, DXGI_FORMAT format, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) = 0;

		//**********************************************************************
		// Method: CreateTexture
		// Creates a new texture
		//
		// Parameters:
		// pRenderer - graphics device
		// pImage - image data to set texture to
		// cpuFlags - DirectX cpu flags
		// bindFlags - DirectX bind flags
		//
		// Returns:
		// Returns the newly created texture
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> CreateTexture(IRenderer* pRenderer, IImage *pImage, D3D11_CPU_ACCESS_FLAG cpuFlags, D3D11_BIND_FLAG bindFlags) = 0;

		//**********************************************************************
		// Method: CheckerboardTexture
		// Creates a texture with a checkerboard pattern
		//
		// Parameters:
		// pRenderer - graphics device
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> CheckerboardTexture(IRenderer* pRenderer) = 0;

		//**********************************************************************
		// Method: CreateDesktopTexture
		// Creates a texture displaying the windows desktop
		//
		// Parameters:
		// pRenderer - graphics device
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> CreateDesktopTexture(IRenderer* pRenderer) = 0;

		//**********************************************************************
		// Method: LoadTexture
		// Loads a texture from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pRenderer - graphics device
		//
		// Returns:
		// Returns the newly created texture
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> LoadTexture(const wchar_t *pFilename, IRenderer* pRenderer) = 0;

		//**********************************************************************
		// Method: LoadVideoTexture
		// Loads a video from disk
		//
		// Parameters:
		// pFilename - filename of the image file
		// pRenderer - graphics device
		//
		// Returns:
		// Returns the newly created texture
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> LoadVideoTexture(const wchar_t* pFilename, IRenderer* pRenderer) = 0;

		//**********************************************************************
		// Method: VideoTextureFromWebcam
		// Loads a video texture using a webcam as the source
		//
		// Parameters:
		// pRenderer - graphics device
		//
		// Returns:
		// Returns the newly created texture
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
		virtual CRefObj<ITexture> VideoTextureFromWebcam(IRenderer* pRenderer) = 0;

		//**********************************************************************
        // Method: CreateShader
        // Creates a shader
        //
        // Parameters:
        // pRenderer - renderer
        // pShaderName - name of shader
        // pVertexShaderBlob - blob containing the compiled vertex shader
        // pPixelShaderBlob - blob containing the compiled pixel shader
        // pShaderInfo - info about the shader (from the .shi file produced by ParseShader)
		//
		// Returns:
        // Returns the created shader
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
        virtual CRefObj<IShader> CreateShader(IRenderer *pRenderer, const wchar_t *pShaderName,
            ID3DBlob *pVertexShaderBlob, ID3DBlob* pPixelShaderBlob, ID3DBlob* pComputeShaderBlob,
			IShaderInfo *pShaderInfo) = 0;

        //**********************************************************************
        // Method: CreateShaderInfo
        // Loads the shader info associated with a shader
        //
        // Parameters:
        // pFilename - path to .shi file
		//
		// Returns:
        // Returns the loaded shader info
		// 
		// Module:
		// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
		//**********************************************************************
        virtual CRefObj<IShaderInfo> CreateShaderInfo(const wchar_t *pFilename) = 0;
    };

    //**********************************************************************
    // Function: CreateCausticFactory
    // Function used by clients to create a Caustic factory
	//
	// Module:
	// {Link:import Rendering/Caustic/ICausticFactory;{Rendering/Caustic/ICausticFactory.ixx}}
	//**********************************************************************
    CRefObj<ICausticFactory> CreateCausticFactory();
}
