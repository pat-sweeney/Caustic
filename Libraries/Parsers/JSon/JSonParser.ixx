//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include "Parsers\Lex\Lex.h"
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <any>

export module Parsers.JSon.JSonParser;
import Parsers.JSon.IJSonParser;

// Namespace: Caustic
export namespace Caustic
{
	//**********************************************************************
	// Class: CJSonObj
	// Defines a JSON object in our parse tree
	//**********************************************************************
	class CJSonObj : public IJSonObj, public CRefCount
	{
		std::string m_propertyName;
		CJSonType m_type;
		std::any m_value;

		friend class CJSonParser;
	public:
		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::AddRef(); }

		//**********************************************************************
		// IJSonParser
		//**********************************************************************
		virtual std::string GetName() override { return m_propertyName; }
		virtual CJSonType GetType() override { return m_type; }
		virtual std::any GetValue() override { return m_value; }
	};

	//**********************************************************************
	// Class: CJSonParser
	// Implements IJSonParser which is used to parse JSON
	//**********************************************************************
	class CJSonParser : public IJSonParser, public CRefCount
	{
		CRefObj<ILex> m_spLex;

		void WriteValue(CRefObj<IJSonObj> obj, char** ppBuffer, uint32* pTotalSize);
		void ConditionalWrite(char** ppBuffer, uint32* pTotalSize, const char* format, ...);
		void ParseObject(ILex* pLex, std::map<std::string, CRefObj<IJSonObj>>& data);
		CRefObj<IJSonObj> ParseValue(ILex* pLex, const char* pName);
		void ParseArray(ILex* pLex, std::vector<CRefObj<IJSonObj>>&data);
	public:
		CJSonParser()
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::AddRef(); }

		//**********************************************************************
		// IJSonParser
		//**********************************************************************
		virtual CRefObj<IJSonObj> LoadDOM(std::wstring& fn) override;
		virtual void SaveDOM(CRefObj<IJSonObj>& dom, std::wstring& fn) override;
		virtual CRefObj<IJSonObj> ReadDOM(const char *pBuffer) override;
		virtual uint32 WriteDOM(CRefObj<IJSonObj>& dom, char *pBuffer, uint32 bufLen) override;
	};
}
