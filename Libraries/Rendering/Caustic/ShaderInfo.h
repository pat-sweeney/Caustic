//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
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
		std::vector<ShaderParamDef> m_computeShaderParamDefs;
		uint32 m_shaderTypeFlags;
		uint32 m_vertexSize;
		int m_xThreads, m_yThreads, m_zThreads;
        D3D10_PRIMITIVE_TOPOLOGY m_topologyType;

		void ParseShaderDef(IXMLDOMNode *pNode);
        void ParseSamplersOrTextures(IXMLDOMNode *pNode, EShaderType shaderType);
        void ParseLayout(IXMLDOMNode *pNode);
		void ParseCBuffer(IXMLDOMNode* pNode, EShaderType shaderType);
		void ParseBuffers(IXMLDOMNode* pNode);
		DXGI_FORMAT StringToFormat(BSTR bstrStr);
		uint32 FormatSize(DXGI_FORMAT format);
		void ParseShaderEntry(IXMLDOMNode *pNode, EShaderType shaderType);
	public:
        CShaderInfo() :
			m_shaderTypeFlags(0),
			m_vertexSize(0),
			m_xThreads(1), 
			m_yThreads(1),
			m_zThreads(1)
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
		virtual bool HasShader(EShaderType type) override
		{
			return (type & m_shaderTypeFlags);
		}
		virtual D3D10_PRIMITIVE_TOPOLOGY GetTopologyType() override;
        virtual std::vector<ShaderParamDef> &PixelShaderParameterDefs() override;
		virtual std::vector<ShaderParamDef>& VertexShaderParameterDefs() override;
		virtual std::vector<ShaderParamDef>& ComputeShaderParameterDefs() override;
		virtual std::vector<D3D11_INPUT_ELEMENT_DESC> &VertexLayout() override;
		virtual uint32 GetVertexSize() override;
		virtual void GetThreadGroupSize(uint32* pXThreads, uint32* pYThreads, uint32* pZThreads) override
		{
			if (pXThreads)
				*pXThreads = m_xThreads;
			if (pYThreads)
				*pYThreads = m_yThreads;
			if (pZThreads)
				*pZThreads = m_zThreads;
		}
	};
}
