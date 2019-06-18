//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <varargs.h>
#include <map>
#include <dxgi1_6.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include "Base/Core/Core.h"
#include "Base/Core/error.h"
#include <MsXml6.h>
#include <stdlib.h>
#include <map>

std::map<std::string, std::string> variables;
void Usage()
{
	fprintf(stderr, "Usage: ParseShader -i <shaderFN> -o <paramFN>\n");
	fprintf(stderr, "where:\n");
	fprintf(stderr, "		<shaderFN> : name of vertex or pixel shader filename\n");
	fprintf(stderr, "		<paramFN>  : name of output filename to write parameter usage to\n");
	exit(0);
}

void WriteStr(HANDLE oh, const char *pFormat, ...)
{
	DWORD dw;
	char buffer[1024];
	va_list arglist;
	va_start(arglist, pFormat);
	vsprintf_s(buffer, pFormat, arglist);
	va_end(arglist);
	if (!WriteFile(oh, buffer, (DWORD)strlen(buffer), &dw, nullptr))
	{
		fprintf(stderr, "Unable to write parameter file\n");
		exit(1);
	}
};

void GetDXGIFormat(D3D12_SIGNATURE_PARAMETER_DESC &pd, std::string &format)
{
	int numFields = 0;
	while (pd.Mask)
	{
		if (pd.Mask & 0x01)
			numFields++;
		pd.Mask = pd.Mask >> 1;
	}
	if (pd.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
	{
		switch (numFields)
		{
		case 1: format = "DXGI_FORMAT_R32_FLOAT"; break;
		case 2: format = "DXGI_FORMAT_R32G32_FLOAT"; break;
		case 3: format = "DXGI_FORMAT_R32G32B32_FLOAT"; break;
		case 4: format = "DXGI_FORMAT_R32G32B32A32_FLOAT"; break;
		}
	}
	else if (pd.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
	{
		switch (numFields)
		{
		case 1: format = "DXGI_FORMAT_R32_SINT"; break;
		case 2: format = "DXGI_FORMAT_R32G32_SINT"; break;
		case 3: format = "DXGI_FORMAT_R32G32B32_SINT"; break;
		case 4: format = "DXGI_FORMAT_R32G32B32A32_SINT"; break;
		}
	}
	else if (pd.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
	{
		switch (numFields)
		{
		case 1: format = "DXGI_FORMAT_R32_UINT"; break;
		case 2: format = "DXGI_FORMAT_R32G32_UINT"; break;
		case 3: format = "DXGI_FORMAT_R32G32B32_UINT"; break;
		case 4: format = "DXGI_FORMAT_R32G32B32A32_UINT"; break;
		}
	}
}

void ParseLoop(ID3D12ShaderReflection *pReflection, HANDLE oh)
{
	D3D12_SHADER_DESC shaderDesc;
	CT(pReflection->GetDesc(&shaderDesc));

	// Determine our vertex format
	WriteStr(oh, "        <VertexLayout>\n");
	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		CT(pReflection->GetInputParameterDesc(i, &paramDesc));
		std::string format;
		GetDXGIFormat(paramDesc, format);
		WriteStr(oh, "            <Field Name='%s' Semantic='%s' SemanticIndex='%d' Format='%s'/>\n",
			paramDesc.SemanticName, paramDesc.SemanticName, (int)paramDesc.SemanticIndex, format.c_str());
	}
	WriteStr(oh, "        </VertexLayout>\n");

	// Parse samplers
	WriteStr(oh, "        <Textures>\n");
	for (UINT i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC bindDesc;
		pReflection->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_TEXTURE)
			WriteStr(oh, "            <Texture Name='%s' Register='%d'/>\n", bindDesc.Name, (int)bindDesc.BindPoint);
	}
	WriteStr(oh, "        </Textures>\n");
	
	WriteStr(oh, "        <Samplers>\n");
	for (UINT i = 0; i < shaderDesc.BoundResources; i++)
	{
		D3D12_SHADER_INPUT_BIND_DESC bindDesc;
		pReflection->GetResourceBindingDesc(i, &bindDesc);
		if (bindDesc.Type == D3D_SIT_SAMPLER)
			WriteStr(oh, "            <SamplerState Name='%s' Register='%d'/>\n", bindDesc.Name, (int)bindDesc.BindPoint);
	}
	WriteStr(oh, "        </Samplers>\n");

	// Get constant buffer names
	for (UINT i = 0; i < shaderDesc.ConstantBuffers; i++)
	{
		ID3D12ShaderReflectionConstantBuffer *pCB = pReflection->GetConstantBufferByIndex(i);
		D3D12_SHADER_BUFFER_DESC cbDesc;
		pCB->GetDesc(&cbDesc);
		WriteStr(oh, "        <CBuffer>\n");
		for (UINT j = 0; j < cbDesc.Variables; j++)
		{
			ID3D12ShaderReflectionVariable *pVar = pCB->GetVariableByIndex(j);
			ID3D12ShaderReflectionType *pType = pVar->GetType();
			D3D12_SHADER_TYPE_DESC typeDesc;
			pType->GetDesc(&typeDesc);
			D3D12_SHADER_VARIABLE_DESC varDesc;
			pVar->GetDesc(&varDesc);
            if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D10_SVC_VECTOR && typeDesc.Elements > 1)
                WriteStr(oh, "             <Property Name='%s' Type='%s[%d]'/>\n", varDesc.Name, typeDesc.Name, typeDesc.Elements);
            else
                WriteStr(oh, "             <Property Name='%s' Type='%s'/>\n", varDesc.Name, typeDesc.Name);
		}
		WriteStr(oh, "        </CBuffer>\n");
	}
}

void ExpandEnvironmentVariables(std::string &outputFn)
{
	std::string expanded;
	int len = (int)outputFn.length();
	for (int i = 0; i < len; i++)
	{
		if (outputFn[i] == '$')
		{
			i += 2; // skip $(
			std::string envName;
			while (outputFn[i] != ')')
				envName += outputFn[i++];
			std::string envVal = variables[envName.c_str()];
			int j = 0;
			while (envVal[j])
				expanded += envVal[j++];
		}
		else
			expanded += outputFn[i];
	}
	outputFn = expanded;
}

void CompileShader(IXMLDOMNode *pNode, bool pixelShader, std::string &shaderFn, std::string &outputFn)
{
	CComPtr<IXMLDOMNamedNodeMap> spAttribs;
	CT(pNode->get_attributes(&spAttribs));
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
		if (attribName == L"Filename")
		{
			std::wstring fn(attribVal.bstrVal);
			shaderFn = std::string(fn.begin(), fn.end());
		}
		else if (attribName == L"Output")
		{
			std::wstring fn(attribVal.bstrVal);
			outputFn = std::string(fn.begin(), fn.end());
		}
	}

	ExpandEnvironmentVariables(shaderFn);
	ExpandEnvironmentVariables(outputFn);

	// Compile our shader
	if (shaderFn.length() > 0 && outputFn.length() > 0)
	{
		STARTUPINFO info = { sizeof(info), 0 };
		PROCESS_INFORMATION processInfo;
        int index = (int)outputFn.rfind(".cso");
        std::string pdbFn = outputFn;
        pdbFn.replace(index, 4, ".pdb");
        char buffer[10240];
		sprintf_s(buffer, "\"C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.17763.0\\x86\\fxc.exe\" /Zi /E\"%s\" /Od /Fd\"%s\" /Fo\"%s\" /T\"%s\" /nologo %s",
			(pixelShader) ? "PS" : "VS", pdbFn.c_str(), outputFn.c_str(), (pixelShader) ? "ps_5_1" : "vs_5_0", shaderFn.c_str());
		if (CreateProcess(nullptr, buffer, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &info, &processInfo))
		{
			WaitForSingleObject(processInfo.hProcess, INFINITE);
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
		}
	}
}

void ParseShader(IXMLDOMNode *pNode, std::string &vertexShader, std::string &vertexOuput,
	std::string &pixelShader, std::string &pixelOuput)
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
		{
			// <PixelShader Filename="ColorNormal.ps" Output="$(CausticRoot)\$(Configuration)\ColorNormal_PS.cso"/>
			CompileShader(spNode, false, vertexShader, vertexOuput);
		}
		else if (bstrName == L"PixelShader")
		{
			// <VertexShader Filename="Default.vs" Output="$(CausticRoot)\$(Configuration)\ColorNormal_VS.cso"/>
			CompileShader(spNode, true, pixelShader, pixelOuput);
		}
	}
}

void LoadShaderDefinition(std::string &fn, std::string &vertexShader, std::string &vertexOuput,
	std::string &pixelShader, std::string &pixelOuput)
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
		if (bstrName == L"Shader")
			ParseShader(spNode, vertexShader, vertexOuput, pixelShader, pixelOuput);
	}
}

void ExportShader(std::string &compiledVS, HANDLE oh)
{
	// Load the shader
	HANDLE h = ::CreateFile(compiledVS.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (h == INVALID_HANDLE_VALUE)
		CT(E_FAIL);
	DWORD bytesRead;
	DWORD dwBytesToRead = ::GetFileSize(h, nullptr);
	byte *buffer = new byte[dwBytesToRead];
	CT(::ReadFile(h, buffer, dwBytesToRead, &bytesRead, nullptr) ? S_OK : E_FAIL);
	CloseHandle(h);

	CComPtr<ID3D12ShaderReflection> spReflection;
	CT(D3DReflect(buffer, bytesRead, __uuidof(ID3D12ShaderReflection), (void**)&spReflection));

	int numParams = 0;
	ParseLoop(spReflection, oh);
}

// Parses a shader (vertex or pixel shader) to determine the name of the parameters
// referenced by a shader so that the rendering engine can build the proper constant buffer
int _tmain(int argc, _TCHAR* argv[])
{
	std::string infn;
	std::string outfn;
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch(argv[i][1])
			{
			case '?': Usage(); break;
			case 'd': DebugBreak(); break;
			case 'i': infn = argv[i+1]; i++; break; 
			case 'o': outfn = argv[i+1]; i++; break; 
			case 'v':
				{
					std::string varName = argv[i + 1];
					std::string varValue = argv[i + 2];
					variables.insert(std::make_pair(varName, varValue));
				}
				break;
			}
		}
	}

	CoInitialize(nullptr);
	std::string vertexShader;
	std::string vertexOuput;
	std::string pixelShader;
	std::string pixelOuput;
	LoadShaderDefinition(infn, vertexShader, vertexOuput, pixelShader, pixelOuput);

	HANDLE oh = CreateFile(outfn.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (oh == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Unable to write parameter file\n");
		exit(1);
	}

	WriteStr(oh, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	WriteStr(oh, "<ShaderDef>\n");
	WriteStr(oh, "    <VertexShader>\n");
	ExportShader(vertexOuput, oh);
	WriteStr(oh, "    </VertexShader>\n");
	WriteStr(oh, "    <PixelShader>\n");
	ExportShader(pixelOuput, oh);
	WriteStr(oh, "    </PixelShader>\n");
	WriteStr(oh, "</ShaderDef>\n");
	CloseHandle(oh);

	CoUninitialize();
	exit(0);
	return 0;
}
