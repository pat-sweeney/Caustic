//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "stdafx.h"
#include "Base/Core/Core.h"
#include "Base/Core/RefCount.h"
#include "Parser.h"
#include <locale>

namespace Caustic
{
	CParser::CParser(const wchar_t *pFilename, CParserConfig &config) : m_config(config)
	{
		OpenFile(pFilename);
	}
	
	void CParser::OpenFile(const wchar_t *pFilename)
	{
		HANDLE h = CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (h == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Failed to read shader\n");
			exit(1);
		}
		CParserBuffer buffer;
		m_bufferStack.push(buffer);
		m_pCurBuffer = &m_bufferStack.top();
		DWORD dwSize = GetFileSize(h, nullptr);
		m_pCurBuffer->m_buffer.resize(dwSize + 1);
		DWORD bytesRead;
		if (!ReadFile(h, m_pCurBuffer->m_buffer.data(), (DWORD)m_pCurBuffer->m_buffer.size() - 1, &bytesRead, nullptr))
		{
			fprintf(stderr, "Failed to read shader\n");
			exit(1);
		}
		m_pCurBuffer->m_buffer[bytesRead] = '\0';
		CloseHandle(h);
		m_pCurBuffer->m_pBuffer = &m_pCurBuffer->m_buffer[0];
		m_pCurBuffer->m_offset = 0;
	}

	CParser::CParser(char *pBuffer, CParserConfig &config) : m_config(config)
	{
		CParserBuffer buffer;
		buffer.m_pBuffer = pBuffer;
		buffer.m_offset = 0;
		m_bufferStack.push(buffer);
	}
	
	bool CParser::ReadString(CParserToken &token, bool isQuoted, char endQuote)
	{
		token.m_tokenType = c_Token_String;
		token.m_tokenVal.clear();
		if (isQuoted)
			m_pCurBuffer->m_offset++; // skip '"'
		while (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] &&
			((isQuoted && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] != endQuote) ||
			   (!isQuoted && (isalnum(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset]) || m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] == '_'))))
		{
			if (isQuoted && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] == '\\')
			{
				token.m_tokenVal.push_back(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset]);
				m_pCurBuffer->m_offset++;
			}
			token.m_tokenVal.push_back(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset]);
			m_pCurBuffer->m_offset++;
		}
		if (isQuoted)
			m_pCurBuffer->m_offset++; // skip trailing quote
		return true;
	}

	bool CParser::ReadLineComment(CParserToken &token)
	{
		token.m_tokenType = c_Token_Comment;
		token.m_tokenVal.push_back(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset++]);
		token.m_tokenVal.push_back(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset++]);
		while (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] != '\r' && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] != '\n')
			token.m_tokenVal.push_back(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset++]);
		return (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset]) ? true : false;
	}

	bool CParser::ReadBlockComment(CParserToken &token)
	{
		token.m_tokenType = c_Token_Comment;
		m_pCurBuffer->m_offset += 2; // Skip '/*'
		while (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] != '*' && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset + 1] && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset + 1] != '/')
		{
			token.m_tokenVal.push_back(m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset]);
			m_pCurBuffer->m_offset++;
		}
		m_pCurBuffer->m_offset += 2; // Skip '*/'
		return true;
	}

	bool CParser::ParseInclude(std::string &inc)
	{
		std::wstring wfn(inc.begin(), inc.end());
		wchar_t drive[_MAX_DRIVE];
		wchar_t dir[_MAX_DIR];
		wchar_t fn[_MAX_FNAME];
		wchar_t ext[_MAX_EXT];
		_wsplitpath_s(wfn.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, fn, _MAX_FNAME, ext, _MAX_EXT);
		if (dir[0] == '\0')
			wfn = m_config.path + wfn;
		OpenFile(wfn.c_str());
		return true;
	}

	bool CParser::ParseDefine()
	{
		return true;
	}

	bool CParser::NextToken(CParserToken &token, bool intAsFloat /* = false*/)
	{
		return NextTokenInternal(token, intAsFloat, m_config.ignoreSpaces, m_config.ignorePreprocessor);
	}

	bool CParser::NextTokenInternal(CParserToken &token, bool intAsFloat, bool ignoreSpaces, bool ignorePreprocessor)
	{
		char *pBuffer = m_pCurBuffer->m_pBuffer;
		int &offset = m_pCurBuffer->m_offset;
		if (ignoreSpaces)
		{
			while (pBuffer[offset] && (pBuffer[offset] == ' ' || pBuffer[offset] == '\t'))
				offset++;
		}
		if (pBuffer[offset] == '"' || pBuffer[offset] == '\'')
			ReadString(token, true, pBuffer[offset]);
		else if (pBuffer[offset] == '/' && pBuffer[offset + 1] && pBuffer[offset + 1] == '/')
			ReadLineComment(token);
		else if (pBuffer[offset] == '/' && pBuffer[offset + 1] && pBuffer[offset + 1] == '*')
			ReadBlockComment(token);
		else if (!ignorePreprocessor && pBuffer[offset] == '#' && (offset == 0 || pBuffer[offset-1] == '\n' || pBuffer[offset-1] == '\r'))
		{
			CParserToken t;
			offset++; // skip '#'
			PeekToken(t);
			if (t.m_tokenVal == "include")
			{
				CParserToken t2;
				NextTokenInternal(t2, intAsFloat, true, false); // skip "include"
				NextTokenInternal(t2, intAsFloat, true, false); // get filename
				ParseInclude(t2.m_tokenVal);
			}
			else if (t.m_tokenVal == "define")
				ParseDefine();
		}
		else if (isalnum(pBuffer[offset]) || pBuffer[offset] == '_')
		{
			ReadString(token, false, '\0');
			for (int i = 0; i < m_config.numEntries; i++)
			{
				if (token.m_tokenVal == m_config.entries[i].value)
				{
					token.m_tokenType = m_config.entries[i].token;
					break;
				}
			}
		}
		else
		{
			token.m_tokenVal.push_back(pBuffer[offset]);
			offset++;
		}
		return true;
	}

	bool CParser::PeekToken(CParserToken &token, bool intAsFloat /*= false*/)
	{
		int o = m_pCurBuffer->m_offset;
		bool r = NextToken(token, intAsFloat);
		m_pCurBuffer->m_offset = o;
		return r;
	}

	void CParser::ReadEOL()
	{
		while (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] != '\r' && m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] != '\n')
			m_pCurBuffer->m_offset++;
		while (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] && (m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] == '\r' || m_pCurBuffer->m_pBuffer[m_pCurBuffer->m_offset] == '\n'))
			m_pCurBuffer->m_offset++;
	}
}
