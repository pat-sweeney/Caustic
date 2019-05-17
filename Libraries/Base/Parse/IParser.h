//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base/Core/IRefCount.h"
#include <string>
#include <limits.h>

namespace Caustic
{
	const int c_Token_Char = -1;
	const int c_Token_String = -2;
	const int c_Token_Float = -3;
	const int c_Token_Int = -4;
	const int c_Token_Space = -5;
	const int c_Token_Comment = -6;
	const int c_Token_EOL = -7;
	const int c_Token_EOF = -8;
	const int c_Token_Last = 0;

	struct CParserEntry
	{
		const char *value;
		const int token;
	};

	struct CParserConfig
	{
		CParserEntry *entries;
		int numEntries;
		bool ignoreSpaces;
		bool consolidateSpaces;
		bool ignorePreprocessor;
		std::wstring path;
	};

	struct CParserToken
	{
		int m_tokenType;
		std::string m_tokenVal;
		union {
			float m_tokenFlt;
			int m_tokenInt;
			char m_tokenChar;
		} x;

		CParserToken()
		{
			m_tokenType = 0;
			x.m_tokenInt = 0;
		}
	};

	struct IParser : public IRefCount
	{
		virtual bool NextToken(CParserToken &token, bool intAsFloat = false) = 0;
		virtual bool PeekToken(CParserToken &token, bool intAsFloat = false) = 0;
		virtual void ReadEOL() = 0;
		virtual bool IsEOF() = 0;
	};
}
