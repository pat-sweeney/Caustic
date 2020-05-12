//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once

#include "Base\Core\Core.h"
#include "Base\Core\IRefCount.h"
#include "Base\Math\Vector.h"
#include "Base\Core\ISerialize.h"
#include "Imaging\Image\Image.h"
#include <vector>
#include <string>
#include <functional>

//**********************************************************************
// File: IRenderMaterial.h
// This file defines the published interface for render materials.
//**********************************************************************

namespace Caustic
{
	enum EShaderAccess
	{
		PixelShader,
		VertexShader,
		Both
	};

	//**********************************************************************
	// Interface: IMaterialAttrib
	// Used for manipulating the materials assigned to a mesh.
	//**********************************************************************
	struct IMaterialAttrib : public ISerialize
	{
		virtual Vector3 GetColor(const wchar_t* pName) = 0;
		virtual void SetColor(const wchar_t* pName, Vector3& v) = 0;
		virtual float GetScalar(const wchar_t *pName) = 0;
		virtual void SetScalar(const wchar_t *pName, float s) = 0;
		virtual CRefObj<IImage> GetTexture(const wchar_t *pName) = 0;
		virtual void SetTexture(const wchar_t *pName, IImage* pImage, EShaderAccess access) = 0;
		virtual void SetTextureViaFilename(const wchar_t* pName, std::wstring& filename, EShaderAccess access) = 0;
		virtual void EnumerateColors(std::function<void(const wchar_t* pName, Vector3 & v)> func) = 0;
		virtual void EnumerateScalars(std::function<void(const wchar_t* pName, float s)> func) = 0;
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

	CAUSTICAPI CRefObj<IMaterialAttrib> CreateStandardMaterial(Vector3 ambientColor, Vector3 diffuseColor,
		Vector3 specularColor, float specularExp, float alpha);
	CAUSTICAPI CRefObj<IMaterialAttrib> CreateMaterial();

}