//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once

#include "Base/Math/Vector.h"
#include "Base/Core/ISerialize.h"
#include <string>

namespace Caustic
{
	//**********************************************************************
	// Interface: IMaterialAttrib
	// Defines the attributes of a material.
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
		virtual void SetAmbientColor(Vector3 &v) = 0;

		//**********************************************************************
		// Method: SetDiffuseColor
		// Sets the diffuse color
		//
		// Parameters:
		// v - color value (0.0 to 1.0) to set diffuse color to
		//**********************************************************************
		virtual void SetDiffuseColor(Vector3 &v) = 0;

		//**********************************************************************
		// Method: SetSpecularColor
		// Sets the specular color
		//
		// Parameters:
		// v - color value (0.0 to 1.0) to set specular color to
		//**********************************************************************
		virtual void SetSpecularColor(Vector3 &v) = 0;

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
		// v - Path to ambient texture file
		//**********************************************************************
		virtual void SetAmbientTexture(std::string p) = 0;

		//**********************************************************************
		// Method: SetDiffuseTexture
		// Sets the diffuse texture
		//
		// Parameters:
		// v - Path to diffuse texture file
		//**********************************************************************
		virtual void SetDiffuseTexture(std::string p) = 0;

		//**********************************************************************
		// Method: SetSpecularTexture
		// Sets the specular texture
		//
		// Parameters:
		// v - Path to specular texture file
		//**********************************************************************
		virtual void SetSpecularTexture(std::string p) = 0;

		//**********************************************************************
		// Method: GetAmbientTexture
		// Returns the ambient texture filename
		//**********************************************************************
		virtual std::string GetAmbientTexture() = 0;

		//**********************************************************************
		// Method: GetDiffuseTexture
		// Returns the diffuse texture filename
		//**********************************************************************
		virtual std::string GetDiffuseTexture() = 0;

		//**********************************************************************
		// Method: GetSpecularTexture
		// Returns the specular texture filename
		//**********************************************************************
		virtual std::string GetSpecularTexture() = 0;
	};
}
