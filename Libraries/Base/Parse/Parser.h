//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base/Core/Core.h"
#include "Base/Core/RefCount.h"
#include "IParser.h"
#include <vector>
#include <map>
#include <stack>

namespace Caustic
{
	struct CParserBuffer
	{
		std::vector<char> m_buffer;
		char *m_pBuffer;
		int m_offset;
	};

	class CParser : public IParser, public CRefCount
	{
		CParserConfig m_config;
		std::stack<CParserBuffer> m_bufferStack;
		std::map<std::string, std::string> m_defines;
		CParserBuffer *m_pCurBuffer;

		bool ReadString(CParserToken &token, bool isQuoted, char endQuote);
		bool ReadLineComment(CParserToken &token);
		bool ReadBlockComment(CParserToken &token);
		bool ParseInclude(std::string &fn);
		bool ParseDefine();
		void OpenFile(const wchar_t *pFilename);
		bool NextTokenInternal(CParserToken &token, bool intAsFloat, bool ignoreSpaces, bool ignorePreprocessor);
	public:
		CParser(const wchar_t *pFilename, CParserConfig &config);
		CParser(char *pBuffer, CParserConfig &config);

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }
		
		//**********************************************************************
		// IParser
		//**********************************************************************
		virtual bool NextToken(CParserToken &token, bool intAsFloat = false) override;
		virtual bool PeekToken(CParserToken &token, bool intAsFloat = false) override;
		virtual void ReadEOL() override;
		virtual bool IsEOF() override { return m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] == '\0'; }
	};
}
