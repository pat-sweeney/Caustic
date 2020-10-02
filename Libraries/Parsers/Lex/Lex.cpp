//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "pch.h"
#include "Lex.h"

// Namespace: Caustic
namespace Caustic
{
	CAUSTICAPI CRefObj<ILex> CreateLex(const char* buffer, bool returnWhitespace /* = false */)
	{
		return CRefObj<ILex>(new CLex(buffer, returnWhitespace));
	}

	void CLex::SetParseTable(ParseTableEntry* pTable)
	{
		m_pUserTokens = pTable;
	}
	
	LexToken CLex::ParseString(char terminator)
	{
		m_lastToken.m_id = c_LexToken_String;
		m_lastToken.m_sval = "";
		m_curIndex++; // skip quote
		while (m_pBuffer[m_curIndex] != '\0')
		{
			if (m_pBuffer[m_curIndex] == terminator)
				break;
			if (m_pBuffer[m_curIndex] == '\\' && m_pBuffer[m_curIndex + 1] != '\0')
				m_curIndex++;
			m_lastToken.m_sval += m_pBuffer[m_curIndex];
			m_curIndex++;
		}
		m_curIndex++; // skip quote
		return m_lastToken;
	}

	LexToken CLex::ParseNumber()
	{
		bool isNeg = false;
		if (m_pBuffer[m_curIndex] == '-')
		{
			m_lastToken.m_sval += m_pBuffer[m_curIndex];
			m_curIndex++;
			isNeg = true;
		}
		else if (m_pBuffer[m_curIndex] == '+')
		{
			m_lastToken.m_sval += m_pBuffer[m_curIndex];
			m_curIndex++;
		}
		bool isFloat = false;
		int num = 0;
		while (isdigit(m_pBuffer[m_curIndex]))
		{
			num = num * 10 + m_pBuffer[m_curIndex] - '0';
			m_lastToken.m_sval += m_pBuffer[m_curIndex];
			m_curIndex++;
		}
		m_lastToken.m_id = c_LexToken_Integer;
		if (m_pBuffer[m_curIndex] == '.')
		{
			float fraction = 0.0f;
			float divisor = 10.0f;
			m_lastToken.m_sval += m_pBuffer[m_curIndex];
			m_curIndex++;
			while (isdigit(m_pBuffer[m_curIndex]))
			{
				fraction = fraction + (m_pBuffer[m_curIndex] - '0') / divisor;
				m_lastToken.m_sval += m_pBuffer[m_curIndex];
				divisor *= 10.0f;
				m_curIndex++;
			}
			m_lastToken.m_id = c_LexToken_Float;
			m_lastToken.m_fval = ((isNeg) ? -1 : +1) * (float(num) + fraction);
		}
		else
		{
			m_lastToken.m_ival = (isNeg) ? -num : num;
		}
		return m_lastToken;
	}

	LexToken CLex::PeekToken()
	{
		if (m_lastTokenValid)
			return m_lastToken;
		
		// By default we will assume the token is EOF
		m_lastToken.m_id = c_LexToken_EOF;
		m_lastTokenValid = true;
		m_lastToken.m_sval = "";
		
		// Ignore whitespace
		if (isspace(m_pBuffer[m_curIndex]))
		{
			while (isspace(m_pBuffer[m_curIndex]))
			{
				m_lastToken.m_sval += m_pBuffer[m_curIndex];
				m_curIndex++;
			}
			if (m_returnWhitespace)
			{
				m_lastToken.m_id = c_LexToken_Whitespace;
				return m_lastToken;
			}
		}
		m_lastToken.m_sval = "";

		if (m_pBuffer[m_curIndex] == '\0')
			return m_lastToken;
		
		if (m_pBuffer[m_curIndex] == '/' &&
			m_pBuffer[m_curIndex + 1] == '/')
		{
			m_lastToken.m_id = c_LexToken_Comment;
			while (m_pBuffer[m_curIndex] != '\0' &&
				m_pBuffer[m_curIndex] != '\r' && m_pBuffer[m_curIndex] != '\n')
			{
				m_lastToken.m_sval += m_pBuffer[m_curIndex];
				m_curIndex++;
			}
			return m_lastToken;
		}

		// Next check if token is a user defined token
		if (m_pUserTokens)
		{
			int i = 0;
			while (true)
			{
				if (m_pUserTokens[i].token == nullptr)
					break;
				int index = m_curIndex;
				int tokenIndex = 0;
				while (m_pBuffer[index] &&
					m_pUserTokens[i].token[tokenIndex] &&
					m_pBuffer[index] == m_pUserTokens[i].token[tokenIndex])
				{
					index++;
					tokenIndex++;
				}
				if (m_pUserTokens[i].token[tokenIndex] == '\0')
				{
					m_curIndex = index;
					m_lastToken.m_id = m_pUserTokens[i].id;
					m_lastToken.m_sval = std::string(m_pUserTokens[i].token);
					return m_lastToken;
				}
				i++;
			}
		}

		// Not a user defined token. Check if its a string
		if (m_pBuffer[m_curIndex] == '\'')
			return ParseString('\'');
		else if (m_pBuffer[m_curIndex] == '\"')
			return ParseString('\"');
		if (isdigit(m_pBuffer[m_curIndex]) ||
			(m_pBuffer[m_curIndex] == '+' && isdigit(m_pBuffer[m_curIndex + 1])) ||
			(m_pBuffer[m_curIndex] == '-' && isdigit(m_pBuffer[m_curIndex + 1])))
			return ParseNumber();

		if (isalpha(m_pBuffer[m_curIndex]))
		{
			m_lastToken.m_sval = "";
			while (isalnum(m_pBuffer[m_curIndex]))
			{
				m_lastToken.m_sval += m_pBuffer[m_curIndex];
				m_curIndex++;
			}
			m_lastToken.m_id = c_LexToken_Identifier;
			return m_lastToken;
		}
		
		// Check if current character is a single character token
		struct {
			char ch;
			char ch2;
			int id;
		} CharMap[] = {
			{ '{', '\0', c_LexToken_LeftBrace },
			{ '}', '\0', c_LexToken_RightBrace },
			{ '(', '\0', c_LexToken_LeftParen },
			{ ')', '\0', c_LexToken_RightParen },
			{ ':', '\0', c_LexToken_Colon },
			{ ';', '\0', c_LexToken_Semicolon },
			{ '.', '\0', c_LexToken_Period },
			{ '+', '\0', c_LexToken_Add },
			{ '-', '\0', c_LexToken_Subtract },
			{ '*', '\0', c_LexToken_Multiply },
			{ '/', '\0', c_LexToken_Divide },
			{ '=', '=', c_LexToken_Equal },
			{ '=', '>', c_LexToken_LeadsTo },
			{ '=', '\0', c_LexToken_Assignment },
			{ '>', '=', c_LexToken_GreaterThanEqual },
			{ '<', '=', c_LexToken_LessThanEqual },
			{ '>', '\0', c_LexToken_GreaterThan },
			{ '<', '\0', c_LexToken_LessThan },
		};
		for (int i = 0; i < _countof(CharMap); i++)
		{
			if (m_pBuffer[m_curIndex] == CharMap[i].ch)
			{
				if (CharMap[i].ch2 == '\0' ||
					m_pBuffer[m_curIndex + 1] == CharMap[i].ch2)
				{
					m_lastToken.m_sval += m_pBuffer[m_curIndex];
					m_curIndex++;
					if (CharMap[i].ch2 != '\0')
					{
						m_lastToken.m_sval += m_pBuffer[m_curIndex];
						m_curIndex++;
					}
					m_lastToken.m_id = CharMap[i].id;
					return m_lastToken;
				}
			}
		}
		m_lastToken.m_id = c_LexToken_Character;
		m_lastToken.m_cval = m_pBuffer[m_curIndex];
		m_lastToken.m_sval += m_pBuffer[m_curIndex++];
		return m_lastToken;
	}

	LexToken CLex::ReadToken()
	{
		LexToken token = PeekToken();
		m_lastTokenValid = false;
		return token;
	}
}
