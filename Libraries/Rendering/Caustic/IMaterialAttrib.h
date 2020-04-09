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

//**********************************************************************
// File: IRenderMaterial.h
// This file defines the published interface for render materials.
//**********************************************************************

namespace Caustic
{
	//**********************************************************************
	// Interface: IMaterialAttrib
	// Used for manipulating the materials assigned to a mesh.
	//**********************************************************************
	struct IMaterialAttrib : public ISerialize
	{
		//**********************************************************************
		// Method: GetAmbientColor
		// Returns the ambient color for the material
		//**********************************************************************
		virtual Vector3 GetAmbientColor() = 0;

		//**********************************************************************
		// Method: GetDiffuseColor
		// Returns the diffuse color for the material
		//**********************************************************************
		virtual Vector3 GetDiffuseColor() = 0;

		//**********************************************************************
		// Method: GetSpecularColor
		// Returns the specular color for the material
		//**********************************************************************
		virtual Vector3 GetSpecularColor() = 0;

		//**********************************************************************
		// Method: GetSpecularExp
		// Returns the specular color's exponent
		//**********************************************************************
		virtual float GetSpecularExp() = 0;

		//**********************************************************************
		// Method: GetAlpha
		// Returns the alpha value for the material. 0.0 = transparent; 1.0 = opaque
		//**********************************************************************
		virtual float GetAlpha() = 0;

		//**********************************************************************
		// Method: GetMaterialID
		// Returns a unique Id associated with this material
		//**********************************************************************
		virtual uint32 GetMaterialID() = 0;

		//**********************************************************************
		// Method: SetAmbientColor
		// Sets the ambient color
		//
		// Parameters:
		// v - color value (0.0 to 1.0) to set ambient color to
		//**********************************************************************
		virtual void SetAmbientColor(Vector3& v) = 0;

		//**********************************************************************
		// Method: SetDiffuseColor
		// Sets the diffuse color
		//
		// Parameters:
		// v - color value (0.0 to 1.0) to set diffuse color to
		//**********************************************************************
		virtual void SetDiffuseColor(Vector3& v) = 0;

		//**********************************************************************
		// Method: SetSpecularColor
		// Sets the specular color
		//
		// Parameters:
		// v - color value (0.0 to 1.0) to set specular color to
		//**********************************************************************
		virtual void SetSpecularColor(Vector3& v) = 0;

		//**********************************************************************
		// Method: SetSpecularExp
		// Sets the specular exponent
		//
		// Parameters:
		// v - value for the specular component
		//**********************************************************************
		virtual void SetSpecularExp(float v) = 0;

		//**********************************************************************
		// Method: SetAlpha
		// Sets the alpha component
		//
		// Parameters:
		// v - value for the alpha component. 0.0 = transparent, 1.0 = opaque
		//**********************************************************************
		virtual void SetAlpha(float v) = 0;

		//**********************************************************************
		// Method: SetMaterialID
		// Sets the material's unique ID
		//
		// Parameters:
		// v - material identifier
		//**********************************************************************
		virtual void SetMaterialID(uint32 v) = 0;

		//**********************************************************************
		// Method: SetAmbientTexture
		// Sets the ambient texture
		//
		// Parameters:
		// pImage - image to use for texture
		//**********************************************************************
		virtual void SetAmbientTexture(IImage *pImage) = 0;

		//**********************************************************************
		// Method: SetDiffuseTexture
		// Sets the diffuse texture
		//
		// Parameters:
		// pImage - image to use for texture
		//**********************************************************************
		virtual void SetDiffuseTexture(IImage* pImage) = 0;

		//**********************************************************************
		// Method: SetSpecularTexture
		// Sets the specular texture
		//
		// Parameters:
		// pImage - image to use for texture
		//**********************************************************************
		virtual void SetSpecularTexture(IImage* pImage) = 0;

		//**********************************************************************
		// Method: SetAmbientTextureFN
		// Sets the ambient texture filename
		//
		// Parameters:
		// v - Path to ambient texture file
		//**********************************************************************
		virtual void SetAmbientTextureFN(std::string p) = 0;

		//**********************************************************************
		// Method: SetDiffuseTextureFN
		// Sets the diffuse texture filename
		//
		// Parameters:
		// v - Path to diffuse texture file
		//**********************************************************************
		virtual void SetDiffuseTextureFN(std::string p) = 0;

		//**********************************************************************
		// Method: SetSpecularTextureFN
		// Sets the specular texture filename
		//
		// Parameters:
		// v - Path to specular texture file
		//**********************************************************************
		virtual void SetSpecularTextureFN(std::string p) = 0;

		//**********************************************************************
		// Method: GetAmbientTexture
		// Returns the ambient texture
		//**********************************************************************
		virtual CRefObj<IImage> GetAmbientTexture() = 0;

		//**********************************************************************
		// Method: GetDiffuseTexture
		// Returns the diffuse texture
		//**********************************************************************
		virtual CRefObj<IImage> GetDiffuseTexture() = 0;

		//**********************************************************************
		// Method: GetSpecularTexture
		// Returns the specular texture
		//**********************************************************************
		virtual CRefObj<IImage> GetSpecularTexture() = 0;

		//**********************************************************************
		// Method: GetAmbientTextureFN
		// Returns the ambient texture filename
		//**********************************************************************
		virtual std::string GetAmbientTextureFN() = 0;

		//**********************************************************************
		// Method: GetDiffuseTextureFN
		// Returns the diffuse texture filename
		//**********************************************************************
		virtual std::string GetDiffuseTextureFN() = 0;

		//**********************************************************************
		// Method: GetSpecularTextureFN
		// Returns the specular texture filename
		//**********************************************************************
		virtual std::string GetSpecularTextureFN() = 0;
	};

	CAUSTICAPI void CreateMaterial(Vector3 ambientColor, Vector3 diffuseColor,
		Vector3 specularColor, float specularExp, float alpha, IMaterialAttrib** ppMaterial);
	CAUSTICAPI void CreateMaterial(IMaterialAttrib** ppMaterial);

}