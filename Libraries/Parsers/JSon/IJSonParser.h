//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Base\Core\IRefCount.h"
#include <string>
#include <memory>
#include <any>

// Namespace: Caustic
namespace Caustic
{
	enum class CJSonType
	{
		Object, // GetValue() returns std::any<map<std::string, CRefObj<IJSonObj>>>
		Array,  // GetValue() returns std::any<vector<CRefObj<IJSonObj>>>
		Number, // GetValue() returns std::any<float>
		String, // GetValue() returns std::any<std::string>
		Bool,   // GetValue() returns std::any<bool>
		Null    // GetValue() returns nullptr
	};

	struct IJSonObj : public IRefCount
	{
		virtual std::string GetName() = 0;
		virtual CJSonType GetType() = 0;
		virtual std::any GetValue() = 0;
	};

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
		virtual uint32 WriteDOM(CRefObj<IJSonObj>& dom, char* pBuffer, uint32 bufLen) = 0;
	};
	
	CAUSTICAPI CRefObj<IJSonParser> CreateJSonParser();
	extern void JSONTree(IJSonObj* pObj, const char* pTreeName);
}
