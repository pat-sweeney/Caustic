//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
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
import Parsers.Lex.Lex;
import Parsers.Lex.ILex;
import Parsers.JSon.IJSonParser;

// Namespace: Caustic
export namespace Caustic
{
    //**********************************************************************
    // Class: JSonEnumerator
    // Enumerates the JSON objects in a DOM
    //**********************************************************************
    class JSonEnumerator
    {
    public:
        int m_index;
        CJSonType m_type;
        CRefObj<IJSonObj> m_spObjBeingEnumerated;
        CRefObj<IJSonObj> m_spCurObj;
        std::map<std::string, CRefObj<IJSonObj>>::iterator m_mapIter;
    public:
        JSonEnumerator(IJSonObj* pObj)
        {
            m_spObjBeingEnumerated = pObj;
            m_type = pObj->GetType();
            m_spCurObj = nullptr;
            switch (m_type)
            {
            case Caustic::CJSonType::Bool:
            case Caustic::CJSonType::Integer:
            case Caustic::CJSonType::Null:
            case Caustic::CJSonType::Number:
            case Caustic::CJSonType::String:
                break;
            case Caustic::CJSonType::Array:
                {
                    m_index = 0;
                    std::vector<CRefObj<IJSonObj>> data = std::any_cast<std::vector<CRefObj<IJSonObj>>>(m_spObjBeingEnumerated->GetValue());
                    if (data.size() > 0)
                        m_spCurObj = data[m_index];
                }
                break;
            case Caustic::CJSonType::Object:
                {
                    std::any val = m_spObjBeingEnumerated->GetValue();
                    std::map<std::string, CRefObj<IJSonObj>>* obj = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(val);
                    m_mapIter = obj->begin();
                    m_spCurObj = m_mapIter->second;
                }
                break;
            }
        }

        CRefObj<IJSonObj> CurrentObj()
        {
            return m_spCurObj;
        }
        
        CRefObj<IJSonObj> NextObj()
        {
            switch (m_type)
            {
            case Caustic::CJSonType::Array:
                {
                    std::vector<CRefObj<IJSonObj>> data = std::any_cast<std::vector<CRefObj<IJSonObj>>>(m_spObjBeingEnumerated->GetValue());
                    if (m_index == data.size())
                        return nullptr;
                    m_spCurObj = data[m_index++];
                    return m_spCurObj;
                }
            case Caustic::CJSonType::Object:
                {
                    std::any val = m_spObjBeingEnumerated->GetValue();
                    std::map<std::string, CRefObj<IJSonObj>>* obj = std::any_cast<std::map<std::string, CRefObj<IJSonObj>>*>(val);
                    m_mapIter++;
                    if (m_mapIter == obj->end())
                        return nullptr;
                    m_spCurObj = m_mapIter->second;
                    return m_spCurObj;
                }
            }
            return nullptr;
        }
    };
    
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
        CRefObj<IJSonObj> FindObj(IJSonObj* pRootObj, std::string name);
        CRefObj<IJSonObj> FindObjInternal(IJSonObj* pRootObj, std::string name, std::string curPath);
    public:
        CJSonObj() = default;
        ~CJSonObj();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // IJSonObj
        //**********************************************************************
        virtual void AddElement(IJSonObj* pValue) override;
        virtual std::string GetName() override { return m_propertyName; }
        virtual CJSonType GetType() override { return m_type; }
        virtual std::any GetValue() override { return m_value; }
        virtual std::string FindValue_String(std::string name) override;
        virtual float FindValue_Float(std::string name) override;
        virtual int FindValue_Int(std::string name) override;
        virtual std::vector<std::string> FindValue_StringArray(std::string name) override;
        virtual std::vector<float> FindValue_FloatArray(std::string name) override;
        virtual std::vector<int> FindValue_IntArray(std::string name) override;
        virtual std::map<std::string, CRefObj<IJSonObj>> FindValue_Map(std::string name) override;
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
