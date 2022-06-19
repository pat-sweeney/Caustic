//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <any>

export module Parsers.JSon.JSonParser;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Parsers.JSon.IJSonParser;
import Parsers.Lex.Lex;
import Parsers.Lex.ILex;

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
		CJSonObj() = default;
		~CJSonObj()
		{
			switch (m_type)
			{
			case CJSonType::Array:
				{
					std::vector<CRefObj<IJSonObj>>* vec = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(GetValue());
					delete vec;
				}
				break;
			case CJSonType::Object:
				{
					std::map<std::string, CRefObj<IJSonObj>>* obj = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(GetValue());
					delete obj;
				}
				break;
			}
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::AddRef(); }

		//**********************************************************************
		// IJSonObj
		//**********************************************************************
		virtual void AddElement(IJSonObj* pValue) override
		{
			switch (GetType())
			{
			case CJSonType::Array:
				{
					std::vector<CRefObj<IJSonObj>> *vec = std::any_cast<std::vector<CRefObj<IJSonObj>>*>(GetValue());
					vec->push_back(CRefObj<IJSonObj>(pValue));
				}
				break;
			case CJSonType::Object:
				{
					std::map<std::string, CRefObj<IJSonObj>> *obj = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(GetValue());
					obj->insert(std::make_pair(CRefObj<IJSonObj>(pValue)->GetName(), CRefObj<IJSonObj>(pValue)));
				}
				break;
			}
		}
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
		int m_indentLevel;

		void WriteValue(CRefObj<IJSonObj> obj, char** ppBuffer, uint32* pTotalSize);
		void ConditionalWrite(bool indent, char** ppBuffer, uint32* pTotalSize, const char* format, ...);
		void ParseObject(ILex* pLex, std::map<std::string, CRefObj<IJSonObj>>& data);
		CRefObj<IJSonObj> ParseValue(ILex* pLex, const char* pName);
		void ParseArray(ILex* pLex, std::vector<CRefObj<IJSonObj>>&data);
	public:
		CJSonParser() : m_indentLevel(0)
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
		virtual CRefObj<IJSonObj> CreateJSon(const char* pPropertyName, float value) override;
		virtual CRefObj<IJSonObj> CreateJSon(const char* pPropertyName, int value) override;
		virtual CRefObj<IJSonObj> CreateJSon(const char *pPropertyName, std::string value) override;
		virtual CRefObj<IJSonObj> CreateJSon(const char *pPropertyName, bool value) override;
		virtual CRefObj<IJSonObj> CreateJSon(const char *pPropertyName, void* value) override;
		virtual CRefObj<IJSonObj> CreateJSonArray(const char* pPropertyName, IJSonObj* pValue0, ...) override;
		virtual CRefObj<IJSonObj> CreateJSonMap(const char *pPropertyName, IJSonObj *pValue0, ...) override;
	};
}
