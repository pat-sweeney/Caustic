//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <vector>
#include <string>

export module Rendering.SceneGraph.SceneCtx;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Matrix;
import Rendering.Caustic.ILight;
import Rendering.Caustic.IShader;
import Rendering.Caustic.RendererFlags;
import Geometry.Mesh.IMaterialAttrib;

export namespace Caustic
{
	//**********************************************************************
	// Struct: SceneCtx
	// Used to maintain state while rendering the scene graph
	//**********************************************************************
	struct SceneCtx
	{
		SceneCtx()
		{
			m_Transform = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
			m_CurrentPass = c_PassFirst;
			m_inShadowLightGroup = false;
		}

		//**********************************************************************
		// Property: m_spDevice
		// Defines our current rendering device
		//**********************************************************************
		CComPtr<ID3D11Device> m_spDevice;

		//**********************************************************************
		// Property: m_Transform
		// Defines the current transform
		//**********************************************************************
		Matrix4x4 m_Transform;

		//**********************************************************************
		// Property: m_lights
		// Defines list of current lights effecting this portion of the scene graph
		//**********************************************************************
		std::vector<CRefObj<ILight>> m_lights;

		//**********************************************************************
		// Property: m_CurrentPass
		// Defines which render pass we are in
		//**********************************************************************
		int m_CurrentPass;

		//**********************************************************************
		// Property: m_inShadowLightGroup
		// Indicates whether we are currently rendering a group of lights+children
		// that cast shadows. Otherwise, during the shadow pass we will skip rendering.
		//**********************************************************************
		bool m_inShadowLightGroup;

		//**********************************************************************
		// Property: m_spCurrentMaterial
		// Defines the current material
		//**********************************************************************
		CRefObj<IMaterialAttrib> m_spCurrentMaterial;

		//**********************************************************************
		// Property: m_spCurrentShader
		// Defines the current shader
		//**********************************************************************
		CRefObj<IShader> m_spCurrentShader;

		//**********************************************************************
		// Property: m_ShowProxyObjects
		// If true renderer will draw a proxy object. This is useful for rendering
		// hidden objects (for instance, place a lightbulb at a light's position)
		//**********************************************************************
		bool m_ShowProxyObjects;
	};
}
