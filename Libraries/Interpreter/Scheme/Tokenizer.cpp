//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Scheme.h"
#include <string>

namespace Caustic
{
    const int c_SpecialInitialMask = 0x1;
    const int c_SpecialSubsequentMask = 0x2;
    const int c_LetterMask = 0x4;
    const int c_DigitMask = 0x8;

    //#define DUMP_TABLE
#ifdef DUMP_TABLE
    int s_charDesignations[256] = { 0 };

    void CSchemeTokenizer::BuildCharDesignations()
    {
        s_charDesignations['!'] |= c_SpecialInitialMask;
        s_charDesignations['$'] |= c_SpecialInitialMask;
        s_charDesignations['%'] |= c_SpecialInitialMask;
        s_charDesignations['&'] |= c_SpecialInitialMask;
        s_charDesignations['*'] |= c_SpecialInitialMask;
        s_charDesignations['/'] |= c_SpecialInitialMask;
        s_charDesignations[':'] |= c_SpecialInitialMask;
        s_charDesignations['<'] |= c_SpecialInitialMask;
        s_charDesignations['='] |= c_SpecialInitialMask;
        s_charDesignations['>'] |= c_SpecialInitialMask;
        s_charDesignations['?'] |= c_SpecialInitialMask;
        s_charDesignations['^'] |= c_SpecialInitialMask;
        s_charDesignations['_'] |= c_SpecialInitialMask;
        s_charDesignations['~'] |= c_SpecialInitialMask;
        s_charDesignations['a'] |= c_LetterMask;
        s_charDesignations['b'] |= c_LetterMask;
        s_charDesignations['c'] |= c_LetterMask;
        s_charDesignations['d'] |= c_LetterMask;
        s_charDesignations['e'] |= c_LetterMask;
        s_charDesignations['f'] |= c_LetterMask;
        s_charDesignations['g'] |= c_LetterMask;
        s_charDesignations['h'] |= c_LetterMask;
        s_charDesignations['i'] |= c_LetterMask;
        s_charDesignations['j'] |= c_LetterMask;
        s_charDesignations['k'] |= c_LetterMask;
        s_charDesignations['l'] |= c_LetterMask;
        s_charDesignations['m'] |= c_LetterMask;
        s_charDesignations['n'] |= c_LetterMask;
        s_charDesignations['o'] |= c_LetterMask;
        s_charDesignations['p'] |= c_LetterMask;
        s_charDesignations['q'] |= c_LetterMask;
        s_charDesignations['r'] |= c_LetterMask;
        s_charDesignations['s'] |= c_LetterMask;
        s_charDesignations['t'] |= c_LetterMask;
        s_charDesignations['u'] |= c_LetterMask;
        s_charDesignations['v'] |= c_LetterMask;
        s_charDesignations['w'] |= c_LetterMask;
        s_charDesignations['x'] |= c_LetterMask;
        s_charDesignations['y'] |= c_LetterMask;
        s_charDesignations['z'] |= c_LetterMask;
        s_charDesignations['A'] |= c_LetterMask;
        s_charDesignations['B'] |= c_LetterMask;
        s_charDesignations['C'] |= c_LetterMask;
        s_charDesignations['D'] |= c_LetterMask;
        s_charDesignations['E'] |= c_LetterMask;
        s_charDesignations['F'] |= c_LetterMask;
        s_charDesignations['G'] |= c_LetterMask;
        s_charDesignations['H'] |= c_LetterMask;
        s_charDesignations['I'] |= c_LetterMask;
        s_charDesignations['J'] |= c_LetterMask;
        s_charDesignations['K'] |= c_LetterMask;
        s_charDesignations['L'] |= c_LetterMask;
        s_charDesignations['M'] |= c_LetterMask;
        s_charDesignations['N'] |= c_LetterMask;
        s_charDesignations['O'] |= c_LetterMask;
        s_charDesignations['P'] |= c_LetterMask;
        s_charDesignations['Q'] |= c_LetterMask;
        s_charDesignations['R'] |= c_LetterMask;
        s_charDesignations['S'] |= c_LetterMask;
        s_charDesignations['T'] |= c_LetterMask;
        s_charDesignations['U'] |= c_LetterMask;
        s_charDesignations['V'] |= c_LetterMask;
        s_charDesignations['W'] |= c_LetterMask;
        s_charDesignations['X'] |= c_LetterMask;
        s_charDesignations['Y'] |= c_LetterMask;
        s_charDesignations['Z'] |= c_LetterMask;
        s_charDesignations['0'] |= c_DigitMask;
        s_charDesignations['1'] |= c_DigitMask;
        s_charDesignations['2'] |= c_DigitMask;
        s_charDesignations['3'] |= c_DigitMask;
        s_charDesignations['4'] |= c_DigitMask;
        s_charDesignations['5'] |= c_DigitMask;
        s_charDesignations['6'] |= c_DigitMask;
        s_charDesignations['7'] |= c_DigitMask;
        s_charDesignations['8'] |= c_DigitMask;
        s_charDesignations['9'] |= c_DigitMask;
        s_charDesignations['+'] |= c_SpecialSubsequentMask;
        s_charDesignations['-'] |= c_SpecialSubsequentMask;
        s_charDesignations['.'] |= c_SpecialSubsequentMask;
        s_charDesignations['@'] |= c_SpecialSubsequentMask;
        OutputDebugString(L"int s_charDesignations[256] = { \n");
        for (int i = 0; i < 256; i++)
        {
            wchar_t buf[1024];
            if ((i + 1) % 16 == 0)
                OutputDebugString(L"\n");
            swprintf_s(buf, L"%d, ", s_charDesignations[i]);
            OutputDebugString(buf);
        }
        OutputDebugString(L"\n");
        OutputDebugString(L"}\n");
    }
#else // DUMP_TABLE
    static int s_charDesignations[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 2, 0, 2, 2,
        1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1, 0, 1, 1, 1,
        1, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 1,
        1, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0
    };
#endif // DUMP_TABLE

    CSchemeToken CSchemeTokenizer::ParseNumber()
    {
        // NOTE: We don't support full number set (only integers and floats)
        bool isNeg = false;
        if (m_data[m_curIndex] == '-' || m_data[m_curIndex] == '+')
        {
            if (m_data[m_curIndex] == '-')
                isNeg = true;
            m_curIndex++;
        }
        int val = 0;
        while (isdigit(m_data[m_curIndex]))
        {
            val = val * 10 + m_data[m_curIndex] - '0';
            m_curIndex++;
        }
        float fraction = 0.0f;
        float divisor = 10.0f;
        if (m_data[m_curIndex] == '.')
        {
            m_curIndex++;
            while (isdigit(m_data[m_curIndex]))
            {
                fraction += m_data[m_curIndex] / divisor;
                divisor *= 10.0f;
                m_curIndex++;
            }
            return CSchemeToken(ESchemeTokenType::FloatNumber, (float)val + fraction);
        }
        else
        {
            return CSchemeToken(ESchemeTokenType::IntNumber, val);
        }
    }

    CSchemeToken CSchemeTokenizer::ReadToken()
    {
        if (m_curIndex >= m_data.length())
            return CSchemeToken(ESchemeTokenType::EndOfFile, std::string(""));
#ifdef DUMP_TABLE
        static bool buildTable = true;
        if (buildTable)
        {
            BuildCharDesignations();
            buildTable = false;
        }
#endif //DUMP_TABLE
        if (m_lastTokenValid)
        {
            m_lastTokenValid = false;
            return m_lastTokenRead;
        }
        while (std::isspace(m_data[m_curIndex]))
            m_curIndex++;
        if (m_data[m_curIndex] == '(')
        {
            m_curIndex++;
            return CSchemeToken(ESchemeTokenType::OpenParenthesis, '(');
        }
        else if (m_data[m_curIndex] == ')')
        {
            m_curIndex++;
            return CSchemeToken(ESchemeTokenType::CloseParenthesis, ')');
        }
        else if (m_data[m_curIndex] == '#' && m_data[m_curIndex + 1] == '(')
        {
            m_curIndex += 2;
            return CSchemeToken(ESchemeTokenType::OpenVector, "#(");
        }
        else if (m_data[m_curIndex] == '\'')
        {
            m_curIndex++;
            return CSchemeToken(ESchemeTokenType::Quote, '\'');
        }
        else if (m_data[m_curIndex] == '`')
        {
            m_curIndex++;
            return CSchemeToken(ESchemeTokenType::BackQuote, '`');
        }
        else if (m_data[m_curIndex] == ',' && m_data[m_curIndex + 1] == '@')
        {
            m_curIndex += 2;
            return CSchemeToken(ESchemeTokenType::Comma, ",@");
        }
        else if (m_data[m_curIndex] == ',')
        {
            m_curIndex++;
            return CSchemeToken(ESchemeTokenType::Comma, ',');
        }
        else if (m_data[m_curIndex] == '#' &&
            (m_data[m_curIndex + 1] == 't' || m_data[m_curIndex + 1] == 'f'))
        {
            auto token = CSchemeToken(ESchemeTokenType::Boolean, (m_data[m_curIndex + 1] == 't') ? "#t" : "#f");
            m_curIndex += 2;
            return token;
        }
        else if (s_charDesignations[m_data[m_curIndex]] & c_DigitMask)
        {
            return CSchemeTokenizer::ParseNumber();
        }
        else if ((s_charDesignations[m_data[m_curIndex]] & c_LetterMask) ||
            (s_charDesignations[m_data[m_curIndex]] & c_SpecialInitialMask))
        {
            std::string val;
            val += m_data[m_curIndex++];
            while ((s_charDesignations[m_data[m_curIndex]] & c_LetterMask) ||
                (s_charDesignations[m_data[m_curIndex]] & c_SpecialInitialMask) ||
                (s_charDesignations[m_data[m_curIndex]] & c_DigitMask))
            {
                val += m_data[m_curIndex++];
            }
            return CSchemeToken(ESchemeTokenType::Identifier, val);
        }
        else if (m_data[m_curIndex] == '+' ||
            m_data[m_curIndex] == '-' ||
            (m_data[m_curIndex] == '.' && m_data[m_curIndex + 1] == '.' && m_data[m_curIndex + 2] == '.'))
        {
            bool isDot = (m_data[m_curIndex] == '.') ? true : false;
            CSchemeToken token;
            if (isDot)
                token = CSchemeToken(ESchemeTokenType::Identifier, "...");
            else
                token = CSchemeToken(ESchemeTokenType::Identifier, m_data[m_curIndex]);
            m_curIndex++;
            if (isDot)
                m_curIndex += 2;
            return token;
        }
        else if (m_data[m_curIndex] == '.')
        {
            m_curIndex++;
            return CSchemeToken(ESchemeTokenType::Pair, '.');
        }
        else
        {
            auto token = CSchemeToken(ESchemeTokenType::Character, m_data[m_curIndex]);
            m_curIndex++;
            return token;
        }
    }

    CSchemeToken CSchemeTokenizer::PeekToken()
    {
        if (m_lastTokenValid)
            return m_lastTokenRead;
        m_lastTokenRead = ReadToken();
        m_lastTokenValid = true;
        return m_lastTokenRead;
    }
}