//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <string>
#include <functional>
#include <d3d11.h>

export module Geometry.Mesh.IMaterialAttrib;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Imaging.Color;
import Imaging.Image.IImage;

//**********************************************************************
// File: IRenderMaterial.h
// This file defines the published interface for render materials.
//**********************************************************************
export namespace Caustic
{
	//**********************************************************************
	// Enum: EShaderAccess
	// Indicates whether a material is used by a pixel or vertex shader or both
	//
	// Module:
	// {Link:import Rendering.Caustic.IMaterialAttrib;{Rendering/Caustic/IMaterialAttrib.ixx}}
	//**********************************************************************
	enum EShaderAccess
	{
		PixelShader,
		VertexShader,
		Both
	};

	//**********************************************************************
	// Interface: IMaterialAttrib
	// Used for manipulating the materials assigned to a mesh.
	//
	// Module:
	// {Link:import Rendering.Caustic.IMaterialAttrib;{Rendering/Caustic/IMaterialAttrib.ixx}}
	//**********************************************************************
	struct IMaterialAttrib : public ISerialize
	{
		//**********************************************************************
		// Method: SetName
		// Sets the name of the material (mostly used for debugging)
		//
		// Parameters:
		// pName - name of material
		//**********************************************************************
		virtual void SetName(const char* pName) = 0;

		//**********************************************************************
		// Method: GetName
		// Returns name of material
		//**********************************************************************
		virtual std::string GetName() = 0;

		//**********************************************************************
		// Method: SetCullMode
		// Sets the culling mode
		//
		// Parameters:
		// cullMode - culling mode
		//**********************************************************************
		virtual void SetCullMode(D3D11_CULL_MODE cullMode) = 0;

		//**********************************************************************
		// Method: GetCullMode
		// Returns current culling mode
		//**********************************************************************
		virtual D3D11_CULL_MODE GetCullMode() = 0;

		//**********************************************************************
		// Method: SetFillMode
		// Sets fill mode for rendering
		//
		// Parameters:
		// renderMode - mode to use
		//**********************************************************************
		virtual void SetFillMode(D3D11_FILL_MODE renderMode) = 0;

		//**********************************************************************
		// Method: GetFillMode
		// Returns current fill mode
		//**********************************************************************
		virtual D3D11_FILL_MODE GetFillMode() = 0;

		//**********************************************************************
		// Method: GetIsTransparent
		// Indicates whether material is considered transparent
		//
		// Returns:
		// Returns true if material is considered to be transparent. False otherwise.
		//**********************************************************************
		virtual bool GetIsTransparent() = 0;

		//**********************************************************************
		// Method: SetIsTransparent
		// Allows client to override whether material is considered transparent
		//
		// Parameters:
		// isTransparent - override whether material is transparent
		//**********************************************************************
		virtual void SetIsTransparent(bool isTransparent) = 0;

		//**********************************************************************
		// Method: GetIsShadowReceiver
		// Indicates whether the surface acts as a shadow receiver
		//**********************************************************************
		virtual bool GetIsShadowReceiver() = 0;

		//**********************************************************************
		// Method: SetIsShadowReceiver
		// Allows client to set whether the surface is a shadow receiver
		//
		// Parameters:
		// receivesShadow - If true material is acts as a shadow receiver. Otherwise, it doesn't
		//**********************************************************************
		virtual void SetIsShadowReceiver(bool receivesShadow) = 0;

		//**********************************************************************
		// Method: GetColor
		// Returns the color associated with the specified name
		//
		// Parameters:
		// pName - name of property (e.g. "Diffuse")
		//
		// Returns:
		// Returns the associated color
		//**********************************************************************
		virtual FRGBColor GetColor(const wchar_t* pName) = 0;
	
		//**********************************************************************
		// Method: SetColor
		// Sets the color associated with the specified property name
		//
		// Parameters:
		// pName - name of property (e.g. "Diffuse")
		// v - color to assign
		//**********************************************************************
		virtual void SetColor(const wchar_t* pName, FRGBColor& v) = 0;

		//**********************************************************************
		// Method: SetColor
		// Sets the color associated with the specified property name
		//
		// Parameters:
		// pName - name of property (e.g. "Diffuse")
		// v - color to assign
		//**********************************************************************
		virtual void SetColor(const wchar_t* pName, FRGBAColor& v) = 0;

		//**********************************************************************
		// Method: GetScalar
		// Gets a float value associated with the specified property name
		//
		// Parameters:
		// pName - name of property (e.g. "SpecularPower")
		//
		// Returns:
		// Value associated with specified property
		//**********************************************************************
		virtual float GetScalar(const wchar_t *pName) = 0;

		//**********************************************************************
		// Method: SetScalar
		// Sets a float value associated with the specified property name
		//
		// Parameters:
		// pName - name of property (e.g. "SpecularPower")
		// v - value
		//**********************************************************************
		virtual void SetScalar(const wchar_t *pName, float s) = 0;

		//**********************************************************************
		// Method: GetTexture
		// Returns the texture with the specified property name
		//
		// Parameters:
		// pName - name of property (e.g. "DiffuseTexture")
		//
		// Returns:
		// Texture associated with specified property
		//**********************************************************************
		virtual CRefObj<IImage> GetTexture(const wchar_t *pName) = 0;

		//**********************************************************************
		// Method: SetTexture
		// Sets a texture with the specified property name
		//
		// Parameters:
		// pName - name of property (e.g. "DiffuseTexture")
		// pImage - Image to use as texture
		// access - Indicates whether texture is accessible from pixel, vertex, or both shaders
		//**********************************************************************
		virtual void SetTexture(const wchar_t *pName, IImage* pImage, EShaderAccess access) = 0;

		//**********************************************************************
		// Method: SetTextureViaFilename
		// Loads a texture from the spcified file and sets it to the specified property name
		//
		// Parameters:
		// pName - name of property being assigned (e.g. "DiffuseTexture")
		// filename - name of file to load texture from
		// access - Indicates whether texture is accessible from pixel, vertex, or both shaders
		//**********************************************************************
		virtual void SetTextureViaFilename(const wchar_t* pName, std::wstring& filename, EShaderAccess access) = 0;

		//**********************************************************************
		// Method: EnumerateColors
		// Enumerates each color currently assigned in this material
		//
		// Parameters:
		// func - Function to call for each enumerated color
		//
		// Callback Parameters:
		// pName - name of color property
		// v - value of color property
		//**********************************************************************
		virtual void EnumerateColors(std::function<void(const wchar_t* pName, FRGBAColor & v)> func) = 0;

		//**********************************************************************
		// Method: EnumerateScalars
		// Enumerates each scalar currently assigned in this material
		//
		// Parameters:
		// func - Function to call for each enumerated scalar
		//
		// Callback Parameters:
		// pName - name of scalar property
		// s - value of scalar property
		//**********************************************************************
		virtual void EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func) = 0;

		//**********************************************************************
		// Method: EnumerateTextures
		// Enumerates each texture currently assigned in this material
		//
		// Parameters:
		// func - Function to call for each enumerated texture
		//
		// Callback Parameters:
		// pName - name of texture property
		// pTexture - value of texture property
		// access - indicates whether texture is accessed via pixel, vertex, or both shaders
		//**********************************************************************
		virtual void EnumerateTextures(std::function<void(const wchar_t* pName, IImage * pTexture, EShaderAccess access)> func) = 0;

		//**********************************************************************
		// Method: GetMaterialID
		// Returns a unique Id associated with this material
		//**********************************************************************
		virtual uint32 GetMaterialID() = 0;

		//**********************************************************************
		// Method: SetMaterialID
		// Sets the material's unique ID
		//
		// Parameters:
		// v - material identifier
		//**********************************************************************
		virtual void SetMaterialID(uint32 v) = 0;
	};
}