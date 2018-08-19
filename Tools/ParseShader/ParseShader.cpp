//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstring>

void Usage()
{
	fprintf(stderr, "Usage: ParseShader -i <shaderFN> -o <paramFN> -v <varName>\n");
	fprintf(stderr, "where:\n");
	fprintf(stderr, "		<shaderFN> : name of vertex or pixel shader filename\n");
	fprintf(stderr, "		<paramFN>  : name of output filename to write parameter usage to\n");
	fprintf(stderr, "		<varName>  : name of variable table\n");
	exit(0);
}

class CReader
{
	std::vector<char> buffer;
	int offset;
public:
	CReader(const char *pFn);
	std::string GetLine();
};

CReader::CReader(const char *pFN)
{
	offset = 0;
	HANDLE h = CreateFile(pFN, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (h == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Failed to read shader\n");
		exit(1);
	}
	buffer.resize(GetFileSize(h, nullptr) + 1);
	DWORD bytesRead;
	if (!ReadFile(h, buffer.data(), (DWORD)buffer.size()-1, &bytesRead, nullptr))
	{
		fprintf(stderr, "Failed to read shader\n");
		exit(1);
	}
	buffer[bytesRead]='\0';
	CloseHandle(h);
}

std::string CReader::GetLine()
{
	std::string str;
	while (buffer[offset] != '\0' && buffer[offset] != '\r' && buffer[offset] != '\n')
		str += buffer[offset++];
	while(buffer[offset] != '\0' && (buffer[offset] == '\r' || buffer[offset] == '\n'))
		offset++;
	return str;
}

// Parses a shader (vertex or pixel shader) to determine the name of the parameters
// referenced by a shader so that the rendering engine can build the proper constant buffer
int _tmain(int argc, _TCHAR* argv[])
{
	std::string infn;
	std::string outfn;
	std::string var;
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
			case 'v': var = argv[i+1]; i++; break; 
			}
		}
	}
	CReader reader(infn.c_str());
	HANDLE oh = CreateFile(outfn.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (oh == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Unable to write parameter file\n");
		exit(1);
	}
	auto WriteStr = [&](const char *p)
	{
		DWORD dw;
		if (!WriteFile(oh, p, (DWORD)strlen(p), &dw, nullptr))
		{
			fprintf(stderr, "Unable to write parameter file\n");
			exit(1);
		}
	};
	auto WritePropertyType = [&](const char *proptype, const char *propname)
	{
		WriteStr("    { ");
		WriteStr(proptype);
		WriteStr(", L\"");
		WriteStr(propname);
		WriteStr("\", 0 },\n");
	};

	WriteStr("ShaderDefs ");
	WriteStr(var.c_str());
	WriteStr("[] = {\n");
	int numParams = 0;
	bool in_cbuffer = false;
	while (true)
	{
		std::string str = reader.GetLine();
		if (str == "")
			break;
		if (str == "};")
		{
			in_cbuffer = false;
			continue;
		}

        // Really wish boost::tokenizer was part of std::
        // (originally that is how I wrote this, but since removing
        // the boost dependency I had to do this the old school way. Yuck).
        const char *p = str.c_str();
        auto NextToken = [&]() -> std::string {
            std::string token;
            while (*p && !isalnum(*p))
            {
                if (*p == '[' || *p == ']' || *p == ';')
                {
                    token.push_back(*p);
                    p++;
                    return token;
                }
                p++;
            }
            while (*p && isalnum(*p))
            {
                token.push_back(*p);
                p++;
            }
            return token;
        };
        auto token = NextToken();
        if (token.length() == 0)
            continue;
		if (token == "Texture2D")
		{
            token = NextToken();
			WriteStr("    { ShaderType_Texture, L\"");
			WriteStr(token.c_str()); // Name
			WriteStr("\", ");
            token = NextToken();
            if (token == "register")
			{
                token = NextToken();
                WriteStr(&(token.c_str())[1]);
				WriteStr(" },\n");
			}
			else
			{
				WriteStr("0 },\n");
			}
		}
		else if (token == "SamplerState")
		{
            token = NextToken();
            WriteStr("    { ShaderType_Sampler, L\"");
			WriteStr(token.c_str()); // Name
			WriteStr("\", ");
            token = NextToken();
            if (token == "register")
			{
                token = NextToken();
                WriteStr(&(token.c_str())[1]);
				WriteStr(" },\n");
			}
			else
			{
				WriteStr("0 },\n");
			}
		}
		else if (token == "cbuffer")
		{
			in_cbuffer = true;
		}
		else if (in_cbuffer && (token == "matrix" || token == "float4x4"))
		{
            token = NextToken(); // skip type
			std::string propName = token;
            token = NextToken(); // Skip name
			if (token == "[")
			{
                token = NextToken(); // Skip '['
				WritePropertyType("ShaderType_Matrix_Array", propName.c_str());
			}
			else
				WritePropertyType("ShaderType_Matrix", propName.c_str());
		}
		else if (in_cbuffer && token == "float4")
		{
            token = NextToken(); // skip type
            std::string propName = token;
            token = NextToken(); // Skip name
            if (token == "[")
			{
                token = NextToken(); // Skip '['
                WritePropertyType("ShaderType_Float4_Array", propName.c_str());
			}
			else
				WritePropertyType("ShaderType_Float4", propName.c_str());
		}
		else if (in_cbuffer && token == "float3")
		{
            token = NextToken(); // skip type
            std::string propName = token;
            token = NextToken(); // Skip name
            if (token == "[")
			{
                token = NextToken(); // Skip '['
                WritePropertyType("ShaderType_Float3_Array", propName.c_str());
			}
			else
				WritePropertyType("ShaderType_Float3", propName.c_str());
		}
		else if (in_cbuffer && token == "float2")
		{
            token = NextToken(); // skip type
            std::string propName = token;
            token = NextToken(); // Skip name
            if (token == "[")
			{
                token = NextToken(); // Skip '['
                WritePropertyType("ShaderType_Float2_Array", propName.c_str());
			}
			else
				WritePropertyType("ShaderType_Float2", propName.c_str());
		}
		else if (in_cbuffer && (token == "float1" || token == "float"))
		{
            token = NextToken(); // skip type
            std::string propName = token;
            token = NextToken(); // Skip name
            if (token == "[")
			{
                token = NextToken(); // Skip '['
                WritePropertyType("ShaderType_Float_Array", propName.c_str());
			}
			else
				WritePropertyType("ShaderType_Float", propName.c_str());
		}
		else if (in_cbuffer && token == "int")
		{
            token = NextToken(); // skip type
            std::string propName = token;
            token = NextToken(); // Skip name
            if (token == "[")
			{
                token = NextToken(); // Skip '['
                WritePropertyType("ShaderType_Int_Array", propName.c_str());
			}
			else
				WritePropertyType("ShaderType_Int", propName.c_str());
		}
	}
	WriteStr("    { ShaderType_Undefined, L\"\", 0 }\n");
	WriteStr("};\n");
	CloseHandle(oh);
	return 0;
}

