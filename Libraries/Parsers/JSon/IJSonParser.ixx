//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <memory>
#include <any>
#include <vector>
#include <map>

export module Parsers.JSon.IJSonParser;
import Base.Core.Core;
import Base.Core.IRefCount;
import Base.Core.RefCount;

// Namespace: Caustic
export namespace Caustic
{
    enum class CJSonType
    {
        Object, // GetValue() returns std::any<map<std::string, CRefObj<IJSonObj>>>
        Array,  // GetValue() returns std::any<vector<CRefObj<IJSonObj>>>
        Number, // GetValue() returns std::any<float>
        Integer,// GetValue() returns std::any<int>. Technically not part of JSON but useful
        String, // GetValue() returns std::any<std::string>
        Bool,   // GetValue() returns std::any<bool>
        Null    // GetValue() returns nullptr
    };

    //**********************************************************************
    // Interface: IJSonObj 
    // Defines an interface to our basic JSON object returned by the parser.
    //
    // Module:
    // {Link:import Parsers.JSon.IJsonParser{Parsers/JSon/IJsonParser.ixx}}
    //**********************************************************************
    struct IJSonObj : public IRefCount
    {
        //**********************************************************************
        // Method: AddElement
        // Adds a child element to this element (assumes it is either a map or
        // array object)
        // 
        // Parameters:
        // pValue - value to add
        //**********************************************************************
        virtual void AddElement(IJSonObj* pValue) = 0;

        //**********************************************************************
        // Method: GetName
        // Returns the name of the object
        //**********************************************************************
        virtual std::string GetName() = 0;

        //**********************************************************************
        // Method: GetType
        // Returns type of the JSON object
        //**********************************************************************
        virtual CJSonType GetType() = 0;

        //**********************************************************************
        // Method: GetValue
        // Returns the value of the JSON object
        //**********************************************************************
        virtual std::any GetValue() = 0;

        //**********************************************************************
        // Method: FindObj
        // Walks the JSon document starting at the specified root in search of
        // the node specified by 'nameToFind'. 'nameToFind' should use the path
        // format "name/subname/...".
        // 
        // Parameters:
        // pRootObj - root of source to search
        // nameToFind - path of object to find
        //**********************************************************************
        virtual CRefObj<IJSonObj> FindObj(IJSonObj* pRootObj, std::string nameToFind) = 0;

        //**********************************************************************
        // Method: FindValue_String
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be a string
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual std::string FindValue_String(std::string name) = 0;

        //**********************************************************************
        // Method: FindValue_Float
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be a float
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual float FindValue_Float(std::string name) = 0;

        //**********************************************************************
        // Method: FindValue_Int
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be a int
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual int FindValue_Int(std::string name) = 0;

        //**********************************************************************
        // Method: FindValue_StringArray
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be an array of strings
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual std::vector<std::string> FindValue_StringArray(std::string name) = 0;

        //**********************************************************************
        // Method: FindValue_IntArray
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be an array of ints
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual std::vector<int> FindValue_IntArray(std::string name) = 0;

        //**********************************************************************
        // Method: FindValue_FloatArray
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be an array of floats
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual std::vector<float> FindValue_FloatArray(std::string name) = 0;

        //**********************************************************************
        // Method: FindValue_Map
        // Searches the JSON DOM in search of the specified object and returns
        // that object's value, which is expected to be a map
        // 
        // Parameters:
        // name - name of object path to find
        //**********************************************************************
        virtual std::map<std::string, CRefObj<IJSonObj>> FindValue_Map(std::string name) = 0;
    };

    //**********************************************************************
    // Interface: IJSonParser 
    // Defines an interface for parsing JSON files.
    //
    // Module:
    // {Link:import Parsers.JSon.IJsonParser{Parsers/JSon/IJsonParser.ixx}}
    //**********************************************************************
    struct IJSonParser : public IRefCount
    {
        //**********************************************************************
        // Method: LoadDOM
        // Loads the Document object model from the specified JSON file
        //
        // Parameters:
        // fn - name of file from which to read
        //**********************************************************************
        virtual CRefObj<IJSonObj> LoadDOM(std::wstring& fn) = 0;

        //**********************************************************************
        // Method: SaveDOM
        // Saves the Document object model (DOM) to the specified JSON file
        //
        // Parameters:
        // dom - our JSON tree
        // fn - name of file to write to
        //**********************************************************************
        virtual void SaveDOM(CRefObj<IJSonObj>& dom, std::wstring& fn) = 0;

        //**********************************************************************
        // Method: ReadDOM
        // Reads a JSON tree from the specified null terminated string buffer
        //
        // Parameters:
        // pBuffer - buffer that contains JSON data
        //
        // Returns:
        // A JSON Document object model (DOM)
        //**********************************************************************
        virtual CRefObj<IJSonObj> ReadDOM(const char* pBuffer) = 0;

        //**********************************************************************
        // Method: WriteDOM
        // Writes a JSON tree to the specified buffer. To determine how big the buffer
        // must be to contain the JSON text, you may call this method with a nullptr
        // for 'pBuffer' and it will return the number of characters the buffer requires.
        //
        // Parameters:
        // dom - JSON tree to write
        // pBuffer - buffer to write to. Maybe nullptr, in which case the method returns
        // the required size of the buffer in characters.
        // bufLen - length of buffer
        //
        // Returns:
        // Number of characters written
        //**********************************************************************
        virtual uint32_t WriteDOM(CRefObj<IJSonObj>& dom, char* pBuffer, uint32_t bufLen) = 0;

        //**********************************************************************
        // Method: CreateJSon
        // Creates a JSON number object
        //
        // Parameters:
        // pPropertyName - name of property
        // value - float value for new IJSonObject object
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSon(const char* pPropertyName, float value) = 0;

        //**********************************************************************
        // Method: CreateJSon
        // Creates a JSON integer object
        //
        // Parameters:
        // pPropertyName - name of property
        // value - integer value for new IJSonObject object
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSon(const char* pPropertyName, int value) = 0;

        //**********************************************************************
        // Method: CreateJSon
        // Creates a JSON string object
        //
        // Parameters:
        // pPropertyName - name of property
        // value - string value for new IJSonObject object
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSon(const char *pPropertyName, std::string value) = 0;

        //**********************************************************************
        // Method: CreateJSon
        // Creates a JSON boolean object
        //
        // Parameters:
        // pPropertyName - name of property
        // value - boolean value for new IJSonObject object
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSon(const char *pPropertyName, bool value) = 0;

        //**********************************************************************
        // Method: CreateJSon
        // Creates a JSON null object
        //
        // Parameters:
        // pPropertyName - name of property
        // value - this value is ignored
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSon(const char *pPropertyName, void* value) = 0;

        //**********************************************************************
        // Method: CreateJSonArray
        // Creates a JSON array object
        //
        // Parameters:
        // pPropertyName - name of property
        // pValue0 - vararg list of objects to add to the array
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSonArray(const char* pPropertyName, IJSonObj* pValue0, ...) = 0;

        //**********************************************************************
        // Method: CreateJSon
        // Creates a JSON object
        //
        // Parameters:
        // pPropertyName - name of property
        // pValue0 - vararg list of objects to add to the object's map
        //
        // Returns:
        // Returns the newly created IJSonObj
        //**********************************************************************
        virtual CRefObj<IJSonObj> CreateJSonMap(const char* pPropertyName, IJSonObj* value0, ...) = 0;
    };
    
    CRefObj<IJSonParser> CreateJSonParser();
    void JSONTree(IJSonObj* pObj, const char* pTreeName);
}
