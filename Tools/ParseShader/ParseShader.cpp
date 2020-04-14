//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
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
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <atlbase.h>
#include "Base/Core/Core.h"
#include "Base/Core/error.h"
#include "Base/Core/convertStr.h"
#include <MsXml6.h>
#include <stdlib.h>
#include <map>

enum EShaderType
{
    PixelShader,
    VertexShader,
    ComputeShader
};

std::map<std::string, std::string> variables;
void Usage()
{
    fprintf(stderr, "Usage: ParseShader -i <shaderFN> -o <paramFN>\n");
    fprintf(stderr, "where:\n");
    fprintf(stderr, "		<shaderFN> : name of vertex, pixel or compute shader filename\n");
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

void GetDXGIFormat(D3D11_SIGNATURE_PARAMETER_DESC &pd, std::string &format)
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

void ParseLoop(ID3D11ShaderReflection *pReflection, HANDLE oh, EShaderType shaderType)
{
    D3D11_SHADER_DESC shaderDesc;
    CT(pReflection->GetDesc(&shaderDesc));

    // Determine our vertex format
    if (shaderType == PixelShader || shaderType == VertexShader)
    {
        WriteStr(oh, "        <VertexLayout>\n");
        for (UINT i = 0; i < shaderDesc.InputParameters; i++)
        {
            D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
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
            D3D11_SHADER_INPUT_BIND_DESC bindDesc;
            pReflection->GetResourceBindingDesc(i, &bindDesc);
            if (bindDesc.Type == D3D_SIT_TEXTURE)
                WriteStr(oh, "            <Texture Name='%s' Register='%d'/>\n", bindDesc.Name, (int)bindDesc.BindPoint);
        }
        WriteStr(oh, "        </Textures>\n");

        WriteStr(oh, "        <Samplers>\n");
        for (UINT i = 0; i < shaderDesc.BoundResources; i++)
        {
            D3D11_SHADER_INPUT_BIND_DESC bindDesc;
            pReflection->GetResourceBindingDesc(i, &bindDesc);
            if (bindDesc.Type == D3D_SIT_SAMPLER)
                WriteStr(oh, "            <SamplerState Name='%s' Register='%d'/>\n", bindDesc.Name, (int)bindDesc.BindPoint);
        }
        WriteStr(oh, "        </Samplers>\n");
    }
    
    if (shaderType == ComputeShader)
        WriteStr(oh, "        <Buffers>\n");
    
    // Get constant buffer names
    for (UINT i = 0; i < shaderDesc.ConstantBuffers; i++)
    {
        ID3D11ShaderReflectionConstantBuffer* pCB = pReflection->GetConstantBufferByIndex(i);
        D3D11_SHADER_BUFFER_DESC cbDesc;
        pCB->GetDesc(&cbDesc);
        if (shaderType == ComputeShader)
        {
            D3D11_SHADER_INPUT_BIND_DESC bind;
            pReflection->GetResourceBindingDesc(i, &bind);
            if (bind.Type == D3D_SIT_STRUCTURED)
                WriteStr(oh, "             <StructuredBuffer Name='%s' Slot='%d'/>\n", cbDesc.Name, bind.BindPoint);
            else if (bind.Type == D3D_SIT_UAV_RWSTRUCTURED)
                WriteStr(oh, "             <RWStructuredBuffer Name='%s' Slot='%d'/>\n", cbDesc.Name, bind.BindPoint);
            else if (bind.Type == D3D_SIT_UAV_APPEND_STRUCTURED)
                WriteStr(oh, "             <AppendStructuredBuffer Name='%s' Slot='%d'/>\n", cbDesc.Name, bind.BindPoint);
        }
        else
        {
            WriteStr(oh, "        <CBuffer>\n");
            for (UINT j = 0; j < cbDesc.Variables; j++)
            {
                ID3D11ShaderReflectionVariable* pVar = pCB->GetVariableByIndex(j);
                ID3D11ShaderReflectionType* pType = pVar->GetType();
                D3D11_SHADER_TYPE_DESC typeDesc;
                pType->GetDesc(&typeDesc);
                D3D11_SHADER_VARIABLE_DESC varDesc;
                pVar->GetDesc(&varDesc);
                if (typeDesc.Class == D3D_SHADER_VARIABLE_CLASS::D3D10_SVC_VECTOR && typeDesc.Elements > 1)
                    WriteStr(oh, "             <Property Name='%s' Type='%s[%d]'/>\n", varDesc.Name, typeDesc.Name, typeDesc.Elements);
                else
                    WriteStr(oh, "             <Property Name='%s' Type='%s'/>\n", varDesc.Name, typeDesc.Name);
            }
            WriteStr(oh, "        </CBuffer>\n");
        }
    }
    if (shaderType == ComputeShader)
        WriteStr(oh, "        </Buffers>\n");
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

void CompileShader(IXMLDOMNode *pNode, EShaderType shaderType, std::string &shaderFn, std::string &outputFn)
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
            shaderFn = Caustic::wstr2str(attribVal.bstrVal);
        else if (attribName == L"Output")
            outputFn = Caustic::wstr2str(attribVal.bstrVal);
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
        char fxcpath[_MAX_PATH];
        DWORD dwResult = GetEnvironmentVariable("CausticFXCPath", fxcpath, sizeof(fxcpath));
        if (dwResult == 0)
        {
            ::MessageBox(nullptr, "Path to fxc not found. Define path to fxc.exe in $(CausticFXCPath)", "Undefined path", MB_OK);
            exit(1);
        }
        char* entryPoint = "";
        char* profile = "";
        switch (shaderType)
        {
        case EShaderType::PixelShader:
            entryPoint = "PS";
            profile = "ps_5_0";
            break;
        case EShaderType::VertexShader:
            entryPoint = "VS";
            profile = "vs_5_0";
            break;
        case EShaderType::ComputeShader:
            entryPoint = "CS";
            profile = "cs_5_0";
            break;
        }
        sprintf_s(buffer, "\"%s\" /Zi /E\"%s\" /Od /Fd\"%s\" /Fo\"%s\" /T\"%s\" /nologo %s", fxcpath,
            entryPoint, pdbFn.c_str(), outputFn.c_str(), profile, shaderFn.c_str());
        if (CreateProcess(nullptr, buffer, nullptr, nullptr, TRUE, 0, nullptr, nullptr, &info, &processInfo))
        {
            WaitForSingleObject(processInfo.hProcess, INFINITE);
            CloseHandle(processInfo.hProcess);
            CloseHandle(processInfo.hThread);
        }
    }
}

void ParseShader(IXMLDOMNode* pNode,
    std::string& vertexShader, std::string& vertexOuput,
    std::string& pixelShader, std::string& pixelOuput,
    std::string& computeShader, std::string& computeOuput,
    std::string& topology)
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
            CompileShader(spNode, EShaderType::VertexShader, vertexShader, vertexOuput);
        }
        else if (bstrName == L"PixelShader")
        {
            // <VertexShader Filename="Default.vs" Output="$(CausticRoot)\$(Configuration)\ColorNormal_VS.cso"/>
            CompileShader(spNode, EShaderType::PixelShader, pixelShader, pixelOuput);
        }
        else if (bstrName == L"ComputeShader")
        {
            // <ComputeShader Filename="Default.cs" Output="$(CausticRoot)\$(Configuration)\Default_CS.cso"/>
            CompileShader(spNode, EShaderType::ComputeShader, computeShader, computeOuput);
        }
        else if (bstrName == L"Topology")
        {
            CComBSTR var;
            CT(spNode->get_text(&var));
            topology = (var == L"Line") ? "Line" : "Triangle";
        }
    }
}

void LoadShaderDefinition(std::string &fn,
    std::string &vertexShader, std::string &vertexOutput,
    std::string& pixelShader, std::string& pixelOutput,
    std::string& computeShader, std::string& computeOutput,
    std::string &topology)
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
            ParseShader(spNode, 
                vertexShader, vertexOutput, 
                pixelShader, pixelOutput,
                computeShader, computeOutput,
                topology);
    }
}

void ExportShader(std::string &compiledFN, HANDLE oh, EShaderType shaderType)
{
    if (compiledFN.empty())
        return;
    // Load the shader
    HANDLE h = ::CreateFile(compiledFN.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
    if (h == INVALID_HANDLE_VALUE)
        CT(E_FAIL);
    DWORD bytesRead;
    DWORD dwBytesToRead = ::GetFileSize(h, nullptr);
    byte *buffer = new byte[dwBytesToRead];
    CT(::ReadFile(h, buffer, dwBytesToRead, &bytesRead, nullptr) ? S_OK : E_FAIL);
    CloseHandle(h);

    CComPtr<ID3D11ShaderReflection> spReflection;
    CT(D3DReflect(buffer, bytesRead, __uuidof(ID3D11ShaderReflection), (void**)&spReflection));

    int numParams = 0;
    ParseLoop(spReflection, oh, shaderType);
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
            switch (argv[i][1])
            {
            case '?': Usage(); break;
            case 'd': DebugBreak(); break;
            case 'i': infn = argv[i + 1]; i++; break;
            case 'o': outfn = argv[i + 1]; i++; break;
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
    if (infn.empty() || outfn.empty())
        Usage();

    CoInitialize(nullptr);
    std::string vertexShader;
    std::string vertexOuput;
    std::string pixelShader;
    std::string pixelOuput;
    std::string computeShader;
    std::string computeOuput;
    std::string topology;
    LoadShaderDefinition(infn, 
        vertexShader, vertexOuput, 
        pixelShader, pixelOuput,
        computeShader, computeOuput,
        topology);

    HANDLE oh = CreateFile(outfn.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
    if (oh == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Unable to write parameter file\n");
        exit(1);
    }

    WriteStr(oh, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    WriteStr(oh, "<ShaderDef>\n");
    WriteStr(oh, "    <Topology>%s</Topology>\n", topology.c_str());
    if (!vertexOuput.empty())
    {
        WriteStr(oh, "    <VertexShader>\n");
        ExportShader(vertexOuput, oh, EShaderType::VertexShader);
        WriteStr(oh, "    </VertexShader>\n");
    }
    if (!pixelOuput.empty())
    {
        WriteStr(oh, "    <PixelShader>\n");
        ExportShader(pixelOuput, oh, EShaderType::PixelShader);
        WriteStr(oh, "    </PixelShader>\n");
    }
    if (!computeOuput.empty())
    {
        WriteStr(oh, "    <ComputeShader>\n");
        ExportShader(computeOuput, oh, EShaderType::ComputeShader);
        WriteStr(oh, "    </ComputeShader>\n");
    }
    WriteStr(oh, "</ShaderDef>\n");
    CloseHandle(oh);

    CoUninitialize();
    exit(0);
    return 0;
}
