//**********************************************************************
// Copyright Patrick Sweeney 2015-2018
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>

export module Parsers.Lex.Lex;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Parsers.Lex.ILex;

export namespace Caustic
{
    class CLex : public ILex, public CRefCount
    {
        const char* m_pBuffer;
        int m_curIndex;
        ParseTableEntry* m_pUserTokens;
        LexToken m_lastToken;
        bool m_lastTokenValid;
        bool m_returnWhitespace;

        LexToken ParseString(char terminator);
        LexToken ParseNumber();
    public:
        CLex(const char *buffer, bool returnWhitespace = false) :
            m_lastTokenValid(false),
            m_pUserTokens(nullptr),
            m_returnWhitespace(returnWhitespace)
        {
            m_pBuffer = buffer;
            m_curIndex = 0;
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // ILex
        //**********************************************************************
        virtual LexToken PeekToken() override;
        virtual LexToken ReadToken() override;
        virtual void SetParseTable(ParseTableEntry* pTable) override;
    };
}
