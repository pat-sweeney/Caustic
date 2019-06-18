//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "stdafx.h"
#include "Base\Core\Core.h"
#include "ShaderInfo.h"
#include <MsXml6.h>

namespace Caustic
{
	//**********************************************************************
	std::vector<ShaderParamDef> &CShaderInfo::PixelShaderParameterDefs()
	{
		return m_pixelShaderParamDefs;
	}

	std::vector<ShaderParamDef> &CShaderInfo::VertexShaderParameterDefs()
	{
		return m_vertexShaderParamDefs;
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> &CShaderInfo::VertexLayout()
	{
		return m_vertexLayout;
	}

	uint32 CShaderInfo::GetVertexSize()
	{
		return m_vertexSize;
	}

	//**********************************************************************
	// Method: StringToFormat
	// Converts a string description of a DXGI_FORMAT into an enum
	//**********************************************************************
	DXGI_FORMAT CShaderInfo::StringToFormat(BSTR bstrStr)
	{
		static struct {
			std::wstring name;
			DXGI_FORMAT format;
		} a[] =
		{
			{ std::wstring(L"DXGI_FORMAT_R32G32B32A32_FLOAT"), DXGI_FORMAT_R32G32B32A32_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R32G32B32A32_UINT"), DXGI_FORMAT_R32G32B32A32_UINT },
			{ std::wstring(L"DXGI_FORMAT_R32G32B32A32_SINT"), DXGI_FORMAT_R32G32B32A32_SINT },
			{ std::wstring(L"DXGI_FORMAT_R32G32B32_TYPELESS"), DXGI_FORMAT_R32G32B32_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R32G32B32_FLOAT"), DXGI_FORMAT_R32G32B32_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R32G32B32_UINT"), DXGI_FORMAT_R32G32B32_UINT },
			{ std::wstring(L"DXGI_FORMAT_R32G32B32_SINT"), DXGI_FORMAT_R32G32B32_SINT },
			{ std::wstring(L"DXGI_FORMAT_R16G16B16A16_TYPELESS"), DXGI_FORMAT_R16G16B16A16_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R16G16B16A16_FLOAT"), DXGI_FORMAT_R16G16B16A16_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R16G16B16A16_UNORM"), DXGI_FORMAT_R16G16B16A16_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R16G16B16A16_UINT"), DXGI_FORMAT_R16G16B16A16_UINT },
			{ std::wstring(L"DXGI_FORMAT_R16G16B16A16_SNORM"), DXGI_FORMAT_R16G16B16A16_SNORM },
			{ std::wstring(L"DXGI_FORMAT_R16G16B16A16_SINT"), DXGI_FORMAT_R16G16B16A16_SINT },
			{ std::wstring(L"DXGI_FORMAT_R32G32_TYPELESS"), DXGI_FORMAT_R32G32_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R32G32_FLOAT"), DXGI_FORMAT_R32G32_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R32G32_UINT"), DXGI_FORMAT_R32G32_UINT },
			{ std::wstring(L"DXGI_FORMAT_R32G32_SINT"), DXGI_FORMAT_R32G32_SINT },
			{ std::wstring(L"DXGI_FORMAT_R32G8X24_TYPELESS"), DXGI_FORMAT_R32G8X24_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_D32_FLOAT_S8X24_UINT"), DXGI_FORMAT_D32_FLOAT_S8X24_UINT },
			{ std::wstring(L"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS"), DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT"), DXGI_FORMAT_X32_TYPELESS_G8X24_UINT },
			{ std::wstring(L"DXGI_FORMAT_R10G10B10A2_TYPELESS"), DXGI_FORMAT_R10G10B10A2_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R10G10B10A2_UNORM"), DXGI_FORMAT_R10G10B10A2_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R10G10B10A2_UINT"), DXGI_FORMAT_R10G10B10A2_UINT },
			{ std::wstring(L"DXGI_FORMAT_R11G11B10_FLOAT"), DXGI_FORMAT_R11G11B10_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R8G8B8A8_TYPELESS"), DXGI_FORMAT_R8G8B8A8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R8G8B8A8_UNORM"), DXGI_FORMAT_R8G8B8A8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB"), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_R8G8B8A8_UINT"), DXGI_FORMAT_R8G8B8A8_UINT },
			{ std::wstring(L"DXGI_FORMAT_R8G8B8A8_SNORM"), DXGI_FORMAT_R8G8B8A8_SNORM },
			{ std::wstring(L"DXGI_FORMAT_R8G8B8A8_SINT"), DXGI_FORMAT_R8G8B8A8_SINT },
			{ std::wstring(L"DXGI_FORMAT_R16G16_TYPELESS"), DXGI_FORMAT_R16G16_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R16G16_FLOAT"), DXGI_FORMAT_R16G16_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R16G16_UNORM"), DXGI_FORMAT_R16G16_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R16G16_UINT"), DXGI_FORMAT_R16G16_UINT },
			{ std::wstring(L"DXGI_FORMAT_R16G16_SNORM"), DXGI_FORMAT_R16G16_SNORM },
			{ std::wstring(L"DXGI_FORMAT_R16G16_SINT"), DXGI_FORMAT_R16G16_SINT },
			{ std::wstring(L"DXGI_FORMAT_R32_TYPELESS"), DXGI_FORMAT_R32_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_D32_FLOAT"), DXGI_FORMAT_D32_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R32_FLOAT"), DXGI_FORMAT_R32_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_R32_UINT"), DXGI_FORMAT_R32_UINT },
			{ std::wstring(L"DXGI_FORMAT_R32_SINT"), DXGI_FORMAT_R32_SINT },
			{ std::wstring(L"DXGI_FORMAT_R24G8_TYPELESS"), DXGI_FORMAT_R24G8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_D24_UNORM_S8_UINT"), DXGI_FORMAT_D24_UNORM_S8_UINT },
			{ std::wstring(L"DXGI_FORMAT_R24_UNORM_X8_TYPELESS"), DXGI_FORMAT_R24_UNORM_X8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_X24_TYPELESS_G8_UINT"), DXGI_FORMAT_X24_TYPELESS_G8_UINT },
			{ std::wstring(L"DXGI_FORMAT_R8G8_TYPELESS"), DXGI_FORMAT_R8G8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R8G8_UNORM"), DXGI_FORMAT_R8G8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R8G8_UINT"), DXGI_FORMAT_R8G8_UINT },
			{ std::wstring(L"DXGI_FORMAT_R8G8_SNORM"), DXGI_FORMAT_R8G8_SNORM },
			{ std::wstring(L"DXGI_FORMAT_R8G8_SINT"), DXGI_FORMAT_R8G8_SINT },
			{ std::wstring(L"DXGI_FORMAT_R16_TYPELESS"), DXGI_FORMAT_R16_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R16_FLOAT"), DXGI_FORMAT_R16_FLOAT },
			{ std::wstring(L"DXGI_FORMAT_D16_UNORM"), DXGI_FORMAT_D16_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R16_UNORM"), DXGI_FORMAT_R16_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R16_UINT"), DXGI_FORMAT_R16_UINT },
			{ std::wstring(L"DXGI_FORMAT_R16_SNORM"), DXGI_FORMAT_R16_SNORM },
			{ std::wstring(L"DXGI_FORMAT_R16_SINT"), DXGI_FORMAT_R16_SINT },
			{ std::wstring(L"DXGI_FORMAT_R8_TYPELESS"), DXGI_FORMAT_R8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_R8_UNORM"), DXGI_FORMAT_R8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R8_UINT"), DXGI_FORMAT_R8_UINT },
			{ std::wstring(L"DXGI_FORMAT_R8_SNORM"), DXGI_FORMAT_R8_SNORM },
			{ std::wstring(L"DXGI_FORMAT_R8_SINT"), DXGI_FORMAT_R8_SINT },
			{ std::wstring(L"DXGI_FORMAT_A8_UNORM"), DXGI_FORMAT_A8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R1_UNORM"), DXGI_FORMAT_R1_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R9G9B9E5_SHAREDEXP"), DXGI_FORMAT_R9G9B9E5_SHAREDEXP },
			{ std::wstring(L"DXGI_FORMAT_R8G8_B8G8_UNORM"), DXGI_FORMAT_R8G8_B8G8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_G8R8_G8B8_UNORM"), DXGI_FORMAT_G8R8_G8B8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC1_TYPELESS"), DXGI_FORMAT_BC1_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC1_UNORM"), DXGI_FORMAT_BC1_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC1_UNORM_SRGB"), DXGI_FORMAT_BC1_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_BC2_TYPELESS"), DXGI_FORMAT_BC2_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC2_UNORM"), DXGI_FORMAT_BC2_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC2_UNORM_SRGB"), DXGI_FORMAT_BC2_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_BC3_TYPELESS"), DXGI_FORMAT_BC3_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC3_UNORM"), DXGI_FORMAT_BC3_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC3_UNORM_SRGB"), DXGI_FORMAT_BC3_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_BC4_TYPELESS"), DXGI_FORMAT_BC4_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC4_UNORM"), DXGI_FORMAT_BC4_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC4_SNORM"), DXGI_FORMAT_BC4_SNORM },
			{ std::wstring(L"DXGI_FORMAT_BC5_TYPELESS"), DXGI_FORMAT_BC5_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC5_UNORM"), DXGI_FORMAT_BC5_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC5_SNORM"), DXGI_FORMAT_BC5_SNORM },
			{ std::wstring(L"DXGI_FORMAT_B5G6R5_UNORM"), DXGI_FORMAT_B5G6R5_UNORM },
			{ std::wstring(L"DXGI_FORMAT_B5G5R5A1_UNORM"), DXGI_FORMAT_B5G5R5A1_UNORM },
			{ std::wstring(L"DXGI_FORMAT_B8G8R8A8_UNORM"), DXGI_FORMAT_B8G8R8A8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_B8G8R8X8_UNORM"), DXGI_FORMAT_B8G8R8X8_UNORM },
			{ std::wstring(L"DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM"), DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM },
			{ std::wstring(L"DXGI_FORMAT_B8G8R8A8_TYPELESS"), DXGI_FORMAT_B8G8R8A8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_B8G8R8A8_UNORM_SRGB"), DXGI_FORMAT_B8G8R8A8_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_B8G8R8X8_TYPELESS"), DXGI_FORMAT_B8G8R8X8_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_B8G8R8X8_UNORM_SRGB"), DXGI_FORMAT_B8G8R8X8_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_BC6H_TYPELESS"), DXGI_FORMAT_BC6H_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC6H_UF16"), DXGI_FORMAT_BC6H_UF16 },
			{ std::wstring(L"DXGI_FORMAT_BC6H_SF16"), DXGI_FORMAT_BC6H_SF16 },
			{ std::wstring(L"DXGI_FORMAT_BC7_TYPELESS"), DXGI_FORMAT_BC7_TYPELESS },
			{ std::wstring(L"DXGI_FORMAT_BC7_UNORM"), DXGI_FORMAT_BC7_UNORM },
			{ std::wstring(L"DXGI_FORMAT_BC7_UNORM_SRGB"), DXGI_FORMAT_BC7_UNORM_SRGB },
			{ std::wstring(L"DXGI_FORMAT_AYUV"), DXGI_FORMAT_AYUV },
			{ std::wstring(L"DXGI_FORMAT_Y410"), DXGI_FORMAT_Y410 },
			{ std::wstring(L"DXGI_FORMAT_Y416"), DXGI_FORMAT_Y416 },
			{ std::wstring(L"DXGI_FORMAT_NV12"), DXGI_FORMAT_NV12 },
			{ std::wstring(L"DXGI_FORMAT_P010"), DXGI_FORMAT_P010 },
			{ std::wstring(L"DXGI_FORMAT_P016"), DXGI_FORMAT_P016 },
			{ std::wstring(L"DXGI_FORMAT_420_OPAQUE"), DXGI_FORMAT_420_OPAQUE },
			{ std::wstring(L"DXGI_FORMAT_YUY2"), DXGI_FORMAT_YUY2 },
			{ std::wstring(L"DXGI_FORMAT_Y210"), DXGI_FORMAT_Y210 },
			{ std::wstring(L"DXGI_FORMAT_Y216"), DXGI_FORMAT_Y216 },
			{ std::wstring(L"DXGI_FORMAT_NV11"), DXGI_FORMAT_NV11 },
			{ std::wstring(L"DXGI_FORMAT_AI44"), DXGI_FORMAT_AI44 },
			{ std::wstring(L"DXGI_FORMAT_IA44"), DXGI_FORMAT_IA44 },
			{ std::wstring(L"DXGI_FORMAT_P8"), DXGI_FORMAT_P8 },
			{ std::wstring(L"DXGI_FORMAT_A8P8"), DXGI_FORMAT_A8P8 },
			{ std::wstring(L"DXGI_FORMAT_B4G4R4A4_UNORM"), DXGI_FORMAT_B4G4R4A4_UNORM },
			{ std::wstring(L"DXGI_FORMAT_P208"), DXGI_FORMAT_P208 },
			{ std::wstring(L"DXGI_FORMAT_V208"), DXGI_FORMAT_V208 },
			{ std::wstring(L"DXGI_FORMAT_V408"), DXGI_FORMAT_V408 },
		};
		for (int i = 0; i < _countof(a); i++)
		{
			if (a[i].name == bstrStr)
			{
				return a[i].format;
			}
		}
		return DXGI_FORMAT_UNKNOWN;
	}

	//**********************************************************************
	// Method: FormatSize
	// Given a DXGI_FORMAT type this method returns the size of that format in bytes
	//**********************************************************************
	uint32 CShaderInfo::FormatSize(DXGI_FORMAT format)
	{
		switch (format)
		{
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			return 16;
		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 12;
		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_UINT:
		case DXGI_FORMAT_R16G16B16A16_SNORM:
		case DXGI_FORMAT_R16G16B16A16_SINT:
		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 8;
		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R16G16_TYPELESS:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_UINT:
		case DXGI_FORMAT_R16G16_SNORM:
		case DXGI_FORMAT_R16G16_SINT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return 4;
		case DXGI_FORMAT_R8G8_TYPELESS:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_SINT:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
		case DXGI_FORMAT_B5G6R5_UNORM:
		case DXGI_FORMAT_B5G5R5A1_UNORM:
		case DXGI_FORMAT_A8P8:
		case DXGI_FORMAT_B4G4R4A4_UNORM:
			return 2;
		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
		case DXGI_FORMAT_A8_UNORM:
		case DXGI_FORMAT_P8:
			return 1;
		};
		return 0;
	}

	static LPCSTR s_SemanticNames[] =
	{
		"POSITION",
		"NORMAL",
		"TEXCOORD",
		"SV_POSITION"
	};

	void CShaderInfo::ParseLayout(IXMLDOMNode *pNode)
	{
		CComPtr<IXMLDOMNodeList> spChildren;
		CT(pNode->get_childNodes(&spChildren));
		long len;
		CT(spChildren->get_length(&len));
		m_vertexLayout.clear();
		m_vertexSize = 0;
		for (long i = 0; i < len; i++)
		{
			CComPtr<IXMLDOMNode> spNode;
			CT(spChildren->get_item(i, &spNode));
			CComBSTR bstrName;
			CT(spNode->get_nodeName(&bstrName));
			if (bstrName == L"Field")
			{
				// 	<Field Name = 'POSITION' Semantic = 'POSITION' SemanticIndex = '0' Format = 'DXGI_FORMAT_R32G32B32_FLOAT' / >
				D3D12_INPUT_ELEMENT_DESC desc;
				CComPtr<IXMLDOMNamedNodeMap> spAttribs;
				CT(spNode->get_attributes(&spAttribs));
				long numAttribs;
				CT(spAttribs->get_length(&numAttribs));
				for (long j = 0; j < numAttribs; j++)
				{
					CComPtr<IXMLDOMNode> spAttrib;
					CT(spAttribs->get_item(j, &spAttrib));
					CComBSTR attribName;
					CT(spAttrib->get_nodeName(&attribName));
					CComVariant attribVal;
					CT(spAttrib->get_nodeValue(&attribVal));
					if (attribName == L"Semantic")
					{
						std::wstring wstr(attribVal.bstrVal);
						std::string str(wstr.begin(), wstr.end());
						for (auto name : s_SemanticNames)
						{
							if (name == str)
							{
								desc.SemanticName = name;
								break;
							}
						}
						_ASSERT(std::string(desc.SemanticName) != "");
					}
					else if (attribName == L"SemanticIndex")
					{
						std::wstring wstr(attribVal.bstrVal);
						desc.SemanticIndex = std::stoi(std::string(wstr.begin(), wstr.end()));
					}
					else if (attribName == L"Format")
						desc.Format = StringToFormat(attribVal.bstrVal);
				}
				desc.InputSlot = 0;
				desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
				desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				m_vertexLayout.push_back(desc);
				m_vertexSize += FormatSize(desc.Format);
			}
		}
	}

	void CShaderInfo::ParseCBuffer(IXMLDOMNode *pNode, bool isPixelShader)
	{
		CComPtr<IXMLDOMNodeList> spChildren;
		CT(pNode->get_childNodes(&spChildren));
		long len;
		int offset = 0;
		CT(spChildren->get_length(&len));
		for (long i = 0; i < len; i++)
		{
			CComPtr<IXMLDOMNode> spNode;
			CT(spChildren->get_item(i, &spNode));
			CComBSTR bstrName;
			CT(spNode->get_nodeName(&bstrName));
			if (bstrName == L"Property")
			{
				// <Property Name = 'lightPos' Type = 'float4' / >
				CComPtr<IXMLDOMNamedNodeMap> spAttribs;
				CT(spNode->get_attributes(&spAttribs));
				long numAttribs;
				CT(spAttribs->get_length(&numAttribs));
				ShaderParamDef param;
				param.m_offset = offset;
                param.m_members = 0;
				for (long j = 0; j < numAttribs; j++)
				{
					CComPtr<IXMLDOMNode> spAttrib;
					CT(spAttribs->get_item(j, &spAttrib));
					CComBSTR attribName;
					CT(spAttrib->get_nodeName(&attribName));
					CComVariant attribVal;
					CT(spAttrib->get_nodeValue(&attribVal));
					if (attribName == "Name")
						param.m_name = attribVal.bstrVal;
					else if (attribName == "Type")
					{
						std::wstring wstr(attribVal.bstrVal);
						if (wstr == L"float4x4")
						{
							param.m_type = ShaderType_Matrix;
							offset += sizeof(float) * 16;
						}
						else if (wstr == L"float")
						{
							param.m_type = ShaderType_Float;
							offset += sizeof(float);
						}
                        else if (wstr.substr(0, 6) == L"float[")
                        {
                            param.m_type = ShaderType_Float_Array;
                            param.m_members = std::stoi(wstr.substr(6).c_str());
                            offset += sizeof(float) * param.m_members;
                        }
                        else if (wstr == L"float2")
                        {
                            param.m_type = ShaderType_Float2;
                            offset += sizeof(float) * 2;
                        }
                        else if (wstr.substr(0, 7) == L"float2[")
                        {
                            param.m_type = ShaderType_Float2_Array;
                            param.m_members = std::stoi(wstr.substr(7).c_str());
                            offset += sizeof(float) * 2 * param.m_members;
                        }
                        else if (wstr == L"float3")
						{
							param.m_type = ShaderType_Float3;
							offset += sizeof(float) * 3;
						}
                        else if (wstr.substr(0, 7) == L"float3[")
                        {
                            param.m_type = ShaderType_Float3_Array;
                            param.m_members = std::stoi(wstr.substr(7).c_str());
                            offset += sizeof(float) * 3 * param.m_members;
                        }
                        else if (wstr == L"float4")
						{
							param.m_type = ShaderType_Float4;
							offset += sizeof(float) * 4;
						}
                        else if (wstr.substr(0, 7) == L"float4[")
                        {
                            param.m_type = ShaderType_Float4_Array;
                            param.m_members = std::stoi(wstr.substr(7).c_str());
                            offset += sizeof(float) * 4 * param.m_members;
                        }
                        else if (wstr == L"int")
						{
							param.m_type = ShaderType_Int;
							offset += sizeof(int);
						}
                        else if (wstr.substr(0, 4) == L"int[")
                        {
                            param.m_type = ShaderType_Int_Array;
                            param.m_members = std::stoi(wstr.substr(4).c_str());
                            offset += sizeof(int) * param.m_members;
                        }
                        else
							param.m_type = ShaderType_Undefined;
					}
				}
				if (isPixelShader)
					m_pixelShaderParamDefs.push_back(param);
				else
					m_vertexShaderParamDefs.push_back(param);
			}
		}
	}

	void CShaderInfo::ParseShaderEntry(IXMLDOMNode *pNode, bool isPixelShader)
	{
		CComPtr<IXMLDOMNodeList> spChildren;
		CT(pNode->get_childNodes(&spChildren));
		long len;
		CT(spChildren->get_length(&len));
		for (long i = 0; i < len; i++)
		{
			CComPtr<IXMLDOMNode> spNode;
			CT(spChildren->get_item(i, &spNode));
			CComBSTR bstrName;
			CT(spNode->get_nodeName(&bstrName));
			if (bstrName == L"VertexLayout" && !isPixelShader)
				ParseLayout(spNode);
			else if (bstrName == L"CBuffer")
				ParseCBuffer(spNode, isPixelShader);
		}
	}

	void CShaderInfo::ParseShaderDef(IXMLDOMNode *pNode)
	{
		CComPtr<IXMLDOMNodeList> spChildren;
		CT(pNode->get_childNodes(&spChildren));
		long len;
		CT(spChildren->get_length(&len));
		for (long i = 0; i < len; i++)
		{
			CComPtr<IXMLDOMNode> spNode;
			CT(spChildren->get_item(i, &spNode));
			CComBSTR bstrName;
			CT(spNode->get_nodeName(&bstrName));
			if (bstrName == L"VertexShader")
				ParseShaderEntry(spNode, false);
			else if (bstrName == L"PixelShader")
				ParseShaderEntry(spNode, true);
		}
	}

	void CShaderInfo::LoadShaderDef(std::wstring &fn)
	{
		CComPtr<IXMLDOMDocument> spDocument;
		CT(CoCreateInstance(CLSID_DOMDocument60, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&spDocument));
		VARIANT_BOOL success;
		VARIANT xmlSrc;
		CComBSTR bstr(fn.c_str());
		xmlSrc.bstrVal = bstr;
		xmlSrc.vt = VT_BSTR;
		CT(spDocument->load(xmlSrc, &success));

		CComPtr<IXMLDOMNodeList> spChildren;
		CT(spDocument->get_childNodes(&spChildren));
		long len;
		CT(spChildren->get_length(&len));
		for (long i = 0; i < len; i++)
		{
			CComPtr<IXMLDOMNode> spNode;
			CT(spChildren->get_item(i, &spNode));
			CComBSTR bstrName;
			CT(spNode->get_nodeName(&bstrName));
			if (bstrName == L"ShaderDef")
				ParseShaderDef(spNode);
		}
	}
}
