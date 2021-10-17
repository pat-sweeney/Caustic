//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
import Base.Core.Core;
import Base.Core.IRefCount;
#include <string>

// Namespace: Caustic
namespace Caustic
{
	//**********************************************************************
	// List of predefined tokens. User defined token should start with id
	// starting at c_LexToken_Last.
	//
	// Header:
	// {Link:#include "Parsers/Lex/ILex.h"{Parsers/Lex/ILex.h}}
	//**********************************************************************
	const int c_LexToken_EOF = 0;
	const int c_LexToken_Whitespace = 1;
	const int c_LexToken_Character = 2;
	const int c_LexToken_String = 3;
	const int c_LexToken_Float = 4;
	const int c_LexToken_Integer = 5;
	const int c_LexToken_LeftParen = 6;
	const int c_LexToken_RightParen = 7;
	const int c_LexToken_LeftBrace = 8;
	const int c_LexToken_RightBrace = 9;
	const int c_LexToken_Colon = 10;
	const int c_LexToken_Semicolon = 11;
	const int c_LexToken_Period = 12;
	const int c_LexToken_Add = 13;
	const int c_LexToken_Subtract = 14;
	const int c_LexToken_Multiply = 15;
	const int c_LexToken_Divide = 16;
	const int c_LexToken_Assignment = 17;
	const int c_LexToken_Equal = 18;
	const int c_LexToken_GreaterThan = 19;
	const int c_LexToken_GreaterThanEqual = 20;
	const int c_LexToken_LessThan = 21;
	const int c_LexToken_LessThanEqual = 22;
	const int c_LexToken_LeadsTo = 23;
	const int c_LexToken_Comment = 24;
	const int c_LexToken_Identifier = 25;
	const int c_LexToken_LeftBracket = 26;
	const int c_LexToken_RightBracket = 27;
	const int c_LexToken_Last = 28;

	//**********************************************************************
	// Struct: LexToken
	// Defines a token returned by the parser
	//
	// Header:
	// {Link:#include "Parsers/Lex/ILex.h"{Parsers/Lex/ILex.h}}
	//**********************************************************************
	struct LexToken
	{
	public:
		int m_id;
		std::string m_sval;
		char m_cval;
		float m_fval;
		int m_ival;

		bool IsCharacter()
		{
			return m_id == c_LexToken_Character || (m_id >= c_LexToken_LeftParen && m_id <= c_LexToken_Semicolon);
		}
	};

	//**********************************************************************
	// Struct: ParseTableEntry
	// Defines a string to be treated as a token along with its associated Id
	//
	// Header:
	// {Link:#include "Parsers/Lex/ILex.h"{Parsers/Lex/ILex.h}}
	//**********************************************************************
	struct ParseTableEntry
	{
		const char* token; // token's string
		int id;      // Id to associate with that string
	};

	//**********************************************************************
	// Interface: IImageBase
	// Base interface shared across all image types
	//
	// Header:
	// {Link:#include "Parsers/Lex/ILex.h"{Parsers/Lex/ILex.h}}
	//**********************************************************************
	struct ILex : public IRefCount
	{
		//**********************************************************************
		// Method: PeekToken
		// Returns the next token without advancing the reader
		//**********************************************************************
		virtual LexToken PeekToken() = 0;
		
		//**********************************************************************
		// Method: ReadToken
		// Returns the next token.
		//**********************************************************************
		virtual LexToken ReadToken() = 0;
		
		//**********************************************************************
		// Method: SetParseTable
		// Set the list of keywords that the parser will isolate as individual
		// tokens.
		//
		// Parameters:
		// pTable - list of token strings with associated ids.
		//**********************************************************************
		virtual void SetParseTable(ParseTableEntry* pTable) = 0;
	};

	CRefObj<ILex> CreateLex(const char *buffer, bool returnWhitespace = false);
}
