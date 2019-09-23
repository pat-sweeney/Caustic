//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Rendering\Caustic\Caustic.h"
#include "Rendering\Caustic\IShaderInfo.h"
#include "Shader.h"

namespace Caustic
{
	//**********************************************************************
	// Class: CShaderInfo
	// Defines information parsed from a shader
	//**********************************************************************
	class CShaderInfo : public IShaderInfo, public CRefCount
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_vertexLayout;
		std::vector<ShaderParamDef> m_pixelShaderParamDefs;
		std::vector<ShaderParamDef> m_vertexShaderParamDefs;
		uint32 m_vertexSize;
        D3D10_PRIMITIVE_TOPOLOGY m_topologyType;

		void ParseShaderDef(IXMLDOMNode *pNode);
		void ParseLayout(IXMLDOMNode *pNode);
		void ParseCBuffer(IXMLDOMNode *pNode, bool isPixelShader);
		DXGI_FORMAT StringToFormat(BSTR bstrStr);
		uint32 FormatSize(DXGI_FORMAT format);
		void ParseShaderEntry(IXMLDOMNode *pNode, bool isPixelShader);
	public:
        CShaderInfo()
        {
            m_topologyType = D3D10_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        }

		void LoadShaderDef(std::wstring &fn);

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IShaderInfo
		//**********************************************************************
        virtual D3D10_PRIMITIVE_TOPOLOGY GetTopologyType() override;
        virtual std::vector<ShaderParamDef> &PixelShaderParameterDefs() override;
		virtual std::vector<ShaderParamDef> &VertexShaderParameterDefs() override;
		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> &VertexLayout() override;
		virtual uint32 GetVertexSize() override;
	};
}
