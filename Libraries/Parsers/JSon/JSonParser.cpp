//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Parsers\Lex\Lex.h"
import Parsers.JSon.JSonParser;
import Parsers.JSon.IJSonParser;
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <any>
#include <windows.h>
#include <varargs.h>

// Namespace: Caustic
namespace Caustic
{
	CRefObj<IJSonObj> CJSonParser::LoadDOM(std::wstring& fn)
	{
		HANDLE f = CreateFile(fn.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (f == INVALID_HANDLE_VALUE)
			CT(GetLastError());
		DWORD dwSize = GetFileSize(f, nullptr);
		std::unique_ptr<char> pBuffer(new char[dwSize + 1]);
		ZeroMemory(pBuffer.get(), dwSize + 1);
		DWORD bytesRead = 0;
		if (!ReadFile(f, pBuffer.get(), dwSize, &bytesRead, nullptr))
		{
			CloseHandle(f);
			CT(GetLastError());
		}
		CloseHandle(f);
		return ReadDOM(pBuffer.get());
	}
	
	void CJSonParser::SaveDOM(CRefObj<IJSonObj>& dom, std::wstring& fn)
	{
		HANDLE f = CreateFile(fn.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, 0, nullptr);
		if (f == INVALID_HANDLE_VALUE)
			CT(GetLastError());
		uint32 bufSize = WriteDOM(dom, nullptr, 0);
		std::unique_ptr<char> pBuffer(new char[bufSize + 1]);
		ZeroMemory(pBuffer.get(), bufSize + 1);
		bufSize = WriteDOM(dom, pBuffer.get(), bufSize + 1);
		DWORD bytesWritten = 0;
		if (!WriteFile(f, pBuffer.get(), bufSize, &bytesWritten, nullptr))
		{
			CloseHandle(f);
			CT(GetLastError());
		}
		CloseHandle(f);
	}

	const int c_LexToken_True = c_LexToken_Last + 1;
	const int c_LexToken_False = c_LexToken_Last + 2;
	const int c_LexToken_Null = c_LexToken_Last + 3;

	void CJSonParser::ParseObject(ILex* pLex, std::map<std::string, CRefObj<IJSonObj>> &data)
	{
		while (true)
		{
			auto token = pLex->PeekToken();
			if (token.m_id == c_LexToken_RightBrace)
			{
				token = pLex->ReadToken();
				break;
			}
			token = pLex->ReadToken();
			if (token.m_id != c_LexToken_String)
				CT(E_UNEXPECTED);
			std::string valueName = token.m_sval;
			token = pLex->ReadToken();
			if (token.m_id != c_LexToken_Colon)
				CT(E_UNEXPECTED);
			data[valueName] = ParseValue(pLex, valueName.c_str());
			token = pLex->ReadToken();
			if (token.m_id == c_LexToken_RightBrace)
				break;
			CT((token.m_id != c_LexToken_Character || token.m_cval != ',') ? E_UNEXPECTED : S_OK);
		}
	}

	void CJSonParser::ParseArray(ILex* pLex, std::vector<CRefObj<IJSonObj>> &data)
	{
		while (true)
		{
			auto token = pLex->PeekToken();
			if (token.m_id == c_LexToken_RightBracket)
			{
				token = pLex->ReadToken();
				break;
			}
			data.push_back(ParseValue(pLex, ""));
			token = pLex->ReadToken();
			 if (token.m_id == c_LexToken_RightBracket)
				break;
			 CT((token.m_id != c_LexToken_Character || token.m_cval != ',') ? E_UNEXPECTED : S_OK);
		}
	}

	CRefObj<IJSonObj> CJSonParser::ParseValue(ILex *pLex, const char *pName)
	{
		CJSonObj* pObj = new CJSonObj();
		LexToken token = pLex->ReadToken();
		pObj->m_propertyName = std::string(pName);
		switch (token.m_id)
		{
		case c_LexToken_String:
			pObj->m_type = CJSonType::String;
			pObj->m_value = std::any(token.m_sval);
			break;
		case c_LexToken_LeftBrace: // Parse object
			{
				pObj->m_type = CJSonType::Object;
				std::map<std::string, CRefObj<IJSonObj>> data;
				ParseObject(pLex, data);
				pObj->m_value = std::any(data);
			}
			break;
		case c_LexToken_LeftBracket: // Parse array
			{
				pObj->m_type = CJSonType::Array;
				std::vector<CRefObj<IJSonObj>> data;
				ParseArray(pLex, data);
				pObj->m_value = std::any(data);
			}
			break;
		case c_LexToken_Float:
			pObj->m_type = CJSonType::Number;
			pObj->m_value = std::any(token.m_fval);
			break;
		case c_LexToken_Integer:
			pObj->m_type = CJSonType::Number;
			pObj->m_value = std::any(float(token.m_ival));
			break;
		case c_LexToken_True:
			pObj->m_type = CJSonType::Bool;
			pObj->m_value = std::any(true);
			break;
		case c_LexToken_False:
			pObj->m_type = CJSonType::Bool;
			pObj->m_value = std::any(false);
			break;
		case c_LexToken_Null:
			pObj->m_type = CJSonType::Null;
			pObj->m_value = std::any(nullptr);
			break;
		}
		return pObj;
	}

	CRefObj<IJSonObj> CJSonParser::ReadDOM(const char* pBuffer)
	{
		CRefObj<ILex> spLex = CreateLex(pBuffer, false);
		ParseTableEntry entries[] = {
			{ "true", c_LexToken_True },
			{ "false", c_LexToken_False },
			{ "null", c_LexToken_Null },
			{ nullptr, 0 },
		};
		spLex->SetParseTable(entries);
		return ParseValue(spLex, "");
	}

	void CJSonParser::ConditionalWrite(char **ppBuffer, uint32 *pTotalSize, const char *format, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, format);
		int count = vsprintf_s(buffer, format, args);
		va_end(args);
		if (ppBuffer)
		{
			memcpy(*ppBuffer, buffer, count);
			(*ppBuffer) += count;
		}
		pTotalSize += count;
	}
	
	void CJSonParser::WriteValue(CRefObj<IJSonObj> obj, char** ppBuffer, uint32 *pTotalSize)
	{
		switch (obj->GetType())
		{
		case CJSonType::Array:
			{
				std::vector<CRefObj<IJSonObj>> vec = std::any_cast<std::vector<CRefObj<IJSonObj>>>(obj->GetValue());
				for (size_t i = 0; i < vec.size(); i++)
					WriteValue(vec[i], ppBuffer, pTotalSize);
			}
			break;
		case CJSonType::Bool:
			ConditionalWrite(ppBuffer, pTotalSize, "%s\n", (std::any_cast<bool>(obj->GetValue())) ? "true" : "false");
			break;
		case CJSonType::Null:
			ConditionalWrite(ppBuffer, pTotalSize, "null\n");
			break;
		case CJSonType::Number:
			ConditionalWrite(ppBuffer, pTotalSize, "%f\n", std::any_cast<float>(obj->GetValue()));
			break;
		case CJSonType::Object:
			{
				std::map<std::string, CRefObj<IJSonObj>> vec = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>>(obj->GetValue());
				for (auto it : vec)
				{
					ConditionalWrite(ppBuffer, pTotalSize, "%s : ", it.first.c_str());
					WriteValue(it.second, ppBuffer, pTotalSize);
				}
			}
			break;
		case CJSonType::String:
			ConditionalWrite(ppBuffer, pTotalSize, "%s\n", std::any_cast<std::string>(obj->GetValue()));
			break;
		}
	}

	uint32 CJSonParser::WriteDOM(CRefObj<IJSonObj>& dom, char* pBuffer, uint32 bufLen)
	{
		uint32 totalSize = 0;
		if (dom->GetName().length() > 0)
			ConditionalWrite((pBuffer) ? &pBuffer : nullptr, &totalSize, "%s : ", dom->GetName().c_str());
		WriteValue(dom, (pBuffer) ? &pBuffer : nullptr, &totalSize);
		return totalSize;
	}
}
