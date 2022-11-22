//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Parsers.Lex.ILex;
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
			CT(HRESULT_FROM_WIN32(GetLastError()));
		DWORD dwSize = GetFileSize(f, nullptr);
		std::unique_ptr<char> pBuffer(new char[dwSize + 1]);
		ZeroMemory(pBuffer.get(), dwSize + 1);
		DWORD bytesRead = 0;
		if (!ReadFile(f, pBuffer.get(), dwSize, &bytesRead, nullptr))
		{
			CloseHandle(f);
			CT(HRESULT_FROM_WIN32(GetLastError()));
		}
		CloseHandle(f);
		return ReadDOM(pBuffer.get());
	}
	
	void CJSonParser::SaveDOM(CRefObj<IJSonObj>& dom, std::wstring& fn)
	{
		HANDLE f = CreateFile(fn.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, 0, nullptr);
		if (f == INVALID_HANDLE_VALUE)
			CT(HRESULT_FROM_WIN32(GetLastError()));
		uint32 bufSize = WriteDOM(dom, nullptr, 0);
		std::unique_ptr<char> pBuffer(new char[bufSize + 1]);
		ZeroMemory(pBuffer.get(), bufSize + 1);
		bufSize = WriteDOM(dom, pBuffer.get(), bufSize + 1);
		DWORD bytesWritten = 0;
		if (!WriteFile(f, pBuffer.get(), bufSize, &bytesWritten, nullptr))
		{
			CloseHandle(f);
			CT(HRESULT_FROM_WIN32(GetLastError()));
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
			if (token.m_id != c_LexToken_String && token.m_id != c_LexToken_Identifier)
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
		case c_LexToken_Identifier:
			pObj->m_type = CJSonType::String;
			pObj->m_value = std::any(token.m_sval);
			break;
		case c_LexToken_LeftBrace: // Parse object
			{
				pObj->m_type = CJSonType::Object;
				std::map<std::string, CRefObj<IJSonObj>> *data = new std::map<std::string, CRefObj<IJSonObj>>();
				ParseObject(pLex, *data);
				pObj->m_value = std::any(data);
			}
			break;
		case c_LexToken_LeftBracket: // Parse array
			{
				pObj->m_type = CJSonType::Array;
				std::vector<CRefObj<IJSonObj>> *data = new std::vector<CRefObj<IJSonObj>>();
				ParseArray(pLex, *data);
				pObj->m_value = std::any(data);
			}
			break;
		case c_LexToken_Float:
			pObj->m_type = CJSonType::Number;
			pObj->m_value = std::any(token.m_fval);
			break;
		case c_LexToken_Integer:
			pObj->m_type = CJSonType::Integer;
			pObj->m_value = std::any(token.m_ival);
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

	void CJSonParser::ConditionalWrite(bool indent, char **ppBuffer, uint32 *pTotalSize, const char *format, ...)
	{
		if (indent)
		{
			if (ppBuffer && m_indentLevel > 0)
			{
				for (int i = 0; i < m_indentLevel; i++)
					(*ppBuffer)[i] = ' ';
				(*ppBuffer) += m_indentLevel;
			}
			*pTotalSize += m_indentLevel;
		}
		
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
		*pTotalSize += count;
	}
	
	void CJSonParser::WriteValue(CRefObj<IJSonObj> obj, char** ppBuffer, uint32 *pTotalSize)
	{
		switch (obj->GetType())
		{
		case CJSonType::Array:
			{
				std::vector<CRefObj<IJSonObj>> *vec = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(obj->GetValue());
				ConditionalWrite(false, ppBuffer, pTotalSize, "[\n");
				m_indentLevel += 4;
				bool writeComma = false;
				for (size_t i = 0; i < vec->size(); i++)
				{
					if (writeComma)
						ConditionalWrite(false, ppBuffer, pTotalSize, ",\n");
					WriteValue((*vec)[i], ppBuffer, pTotalSize);
					writeComma = true;
				}
				ConditionalWrite(false, ppBuffer, pTotalSize, "\n");
				m_indentLevel += 4;
				ConditionalWrite(true, ppBuffer, pTotalSize, "]");
			}
			break;
		case CJSonType::Bool:
			ConditionalWrite(false, ppBuffer, pTotalSize, "%s", (std::any_cast<bool>(obj->GetValue())) ? "true" : "false");
			break;
		case CJSonType::Null:
			ConditionalWrite(false, ppBuffer, pTotalSize, "null");
			break;
		case CJSonType::Number:
			ConditionalWrite(false, ppBuffer, pTotalSize, "%f", std::any_cast<float>(obj->GetValue()));
			break;
		case CJSonType::Integer:
			ConditionalWrite(false, ppBuffer, pTotalSize, "%d", std::any_cast<int>(obj->GetValue()));
			break;
		case CJSonType::Object:
			{
				std::map<std::string, CRefObj<IJSonObj>> *vec = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(obj->GetValue());
				ConditionalWrite(false, ppBuffer, pTotalSize, "{\n");
				m_indentLevel += 4;
				bool writeComma = false;
				for (auto it : *vec)
				{
					if (writeComma)
						ConditionalWrite(false, ppBuffer, pTotalSize, ",\n");
					ConditionalWrite(true, ppBuffer, pTotalSize, "%s : ", it.first.c_str());
					WriteValue(it.second, ppBuffer, pTotalSize);
					writeComma = true;
				}
				ConditionalWrite(false, ppBuffer, pTotalSize, "\n");
				m_indentLevel -= 4;
				ConditionalWrite(true, ppBuffer, pTotalSize, "}");
			}
			break;
		case CJSonType::String:
			ConditionalWrite(false, ppBuffer, pTotalSize, "\"%s\"", std::any_cast<std::string>(obj->GetValue()));
			break;
		}
	}

	uint32 CJSonParser::WriteDOM(CRefObj<IJSonObj>& dom, char* pBuffer, uint32 bufLen)
	{
		uint32 totalSize = 0;
		if (dom->GetName().length() > 0)
			ConditionalWrite(true, (pBuffer) ? &pBuffer : nullptr, &totalSize, "%s : ", dom->GetName().c_str());
		WriteValue(dom, (pBuffer) ? &pBuffer : nullptr, &totalSize);
		return totalSize;
	}

	static std::string BuildRandomName()
	{
		std::string x;
		for (int i = 0; i < 10; i++)
		{
			int index = rand() % 52;
			if (index < 26)
				x += 'a' + index;
			else
				x += 'A' + index;
		}
		return x;
	}
	
	CRefObj<IJSonObj> CJSonParser::CreateJSon(const char* pPropertyName, float value)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::Number;
		pObj->m_value = std::any(value);
		pObj->m_propertyName = (pPropertyName) ? pPropertyName : BuildRandomName();
		return CRefObj<IJSonObj>(pObj);
	}

	CRefObj<IJSonObj> CJSonParser::CreateJSon(const char* pPropertyName, int value)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::Integer;
		pObj->m_value = std::any(value);
		pObj->m_propertyName = (pPropertyName) ? pPropertyName : BuildRandomName();
		return CRefObj<IJSonObj>(pObj);
	}

	CRefObj<IJSonObj> CJSonParser::CreateJSon(const char* pPropertyName, std::string value)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::String;
		pObj->m_value = std::any(value);
		pObj->m_propertyName = (pPropertyName) ? pPropertyName : BuildRandomName();
		return CRefObj<IJSonObj>(pObj);
	}
	
	CRefObj<IJSonObj> CJSonParser::CreateJSon(const char* pPropertyName, bool value)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::Bool;
		pObj->m_value = std::any(value);
		pObj->m_propertyName = (pPropertyName) ? pPropertyName : BuildRandomName();
		return CRefObj<IJSonObj>(pObj);
	}
	
	CRefObj<IJSonObj> CJSonParser::CreateJSon(const char* pPropertyName, void* value)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::Null;
		pObj->m_value = std::any(value);
		pObj->m_propertyName = (pPropertyName) ? pPropertyName : BuildRandomName();
		return CRefObj<IJSonObj>(pObj);
	}
	
	CRefObj<IJSonObj> CJSonParser::CreateJSonArray(const char* pPropertyName, IJSonObj* pValue0, ...)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::Array;
		std::vector<CRefObj<IJSonObj>> *data = new std::vector<CRefObj<IJSonObj>>();
		auto pNextObj = pValue0;
		va_list vl;
		va_start(vl, pValue0);
		while (pNextObj != nullptr)
		{
			data->push_back(CRefObj<IJSonObj>(pNextObj));
			pNextObj = va_arg(vl, IJSonObj*);
		}
		va_end(vl);
		pObj->m_value = std::any(data);
		return CRefObj<IJSonObj>(pObj);
	}

	CRefObj<IJSonObj> CJSonParser::CreateJSonMap(const char* pPropertyName, IJSonObj* pValue0, ...)
	{
		CJSonObj* pObj = new CJSonObj();
		pObj->m_type = CJSonType::Object;
		pObj->m_propertyName = std::string(pPropertyName);
		std::map<std::string, CRefObj<IJSonObj>> *data = new std::map<std::string, CRefObj<IJSonObj>>();
		auto pNextObj = pValue0;
		va_list vl;
		va_start(vl, pValue0);
		while (pNextObj != nullptr)
		{
			data->insert(std::make_pair<const char*, CRefObj<IJSonObj>>(pNextObj->GetName().c_str(), CRefObj<IJSonObj>(pNextObj)));
			pNextObj = va_arg(vl, IJSonObj*);
		}
		va_end(vl);
		pObj->m_value = std::any(data);
		return CRefObj<IJSonObj>(pObj);
	}
}
