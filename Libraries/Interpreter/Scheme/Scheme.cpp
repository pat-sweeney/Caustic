//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#include "Scheme.h"

namespace Caustic
{
    using namespace std;

    void CSchemeInterpreter::Bind(std::string& procname, std::function<void()> func)
    {
    }

    void CSchemeInterpreter::Eval(string& s)
    {
        return Parse(s);
    }
    
    void CSchemeInterpreter::Eval(const char* s)
    {
        std::string str(s);
        Eval(str);
    }
    
    CSchemeInterpreter::CSchemeInterpreter()
    {
    }
    
    void CSchemeInterpreter::ParseExpression()
    {
    }

    std::shared_ptr<CSchemeASTNode> CSchemeInterpreter::ParseListOrPair()
    {
        std::shared_ptr<CSchemeASTNode> spDatum = ParseDatum();
        auto token = m_tokenizer.PeekToken();
        if (token.m_type == ESchemeTokenType::Pair)
        {
            CSchemeASTNode_Pair* pPairNode = new CSchemeASTNode_Pair();
            pPairNode->m_spCAR = spDatum;
            token = m_tokenizer.ReadToken(); // skip '.'
            pPairNode->m_spCDR = ParseListOrPair();
            return std::make_shared<CSchemeASTNode>(pPairNode);
        }
        else
        {
            CSchemeASTNode_Pair* pHead = new CSchemeASTNode_Pair();
            CSchemeASTNode_Pair* pPrev = nullptr;
            CSchemeASTNode_Pair* pCur = pHead;
            pHead->m_spCAR = spDatum;
            pHead->m_spCDR = nullptr;
            while (true)
            {
                auto token = m_tokenizer.PeekToken();
                if (token.m_type == ESchemeTokenType::CloseParenthesis)
                    break;
                CSchemeASTNode_Pair* pCur = new CSchemeASTNode_Pair();
                auto spCur = std::make_shared<CSchemeASTNode_Pair>(pCur);
                pCur->m_spCAR = ParseDatum();
                pCur->m_spCDR = nullptr;
                if (pPrev)
                    pPrev->m_spCDR = spCur;
                pPrev = pCur;
            }
            return std::make_shared<CSchemeASTNode>(pHead);
        }
    }

    // ; (define sum (lambda (a b) (+ a b)))
    std::shared_ptr<CSchemeASTNode> CSchemeInterpreter::ParseDatum()
    {
        auto token = m_tokenizer.PeekToken();
        if (token.m_type == ESchemeTokenType::OpenParenthesis)
        {
            // Start of list or pair
            token = m_tokenizer.ReadToken();
            return ParseListOrPair();
        }
        else if (token.m_type == ESchemeTokenType::FloatNumber)
        {
            token = m_tokenizer.ReadToken();
            auto pFloat = new CSchemeASTNode_Float(token.m_fval);
            return std::make_shared<CSchemeASTNode>(pFloat);
        }
        else if (token.m_type == ESchemeTokenType::IntNumber)
        {
            token = m_tokenizer.ReadToken();
            auto pInt = new CSchemeASTNode_Integer(token.m_ival);
            return std::make_shared<CSchemeASTNode>(pInt);
        }
        else if (token.m_type == ESchemeTokenType::String)
        {
            token = m_tokenizer.ReadToken();
            auto pString = new CSchemeASTNode_String(token.m_sval);
            return std::make_shared<CSchemeASTNode>(pString);
        }
        else if (token.m_type == ESchemeTokenType::Character)
        {
            token = m_tokenizer.ReadToken();
            auto pString = new CSchemeASTNode_Character(token.m_cval);
            return std::make_shared<CSchemeASTNode>(pString);
        }
        else if (token.m_type == ESchemeTokenType::Boolean)
        {
            token = m_tokenizer.ReadToken();
            auto pString = new CSchemeASTNode_Boolean(token.m_bval);
            return std::make_shared<CSchemeASTNode>(pString);
        }
    }
    
    std::shared_ptr<CSchemeASTNode> CSchemeInterpreter::ParseExpression()
    {
        auto token = m_tokenizer.ReadToken();
        if (token.m_type == ESchemeTokenType::Quote)
        {
        }
        else if (token.m_type == ESchemeTokenType::OpenParenthesis)
        {
            token = m_tokenizer.ReadToken();
            if (token.m_type == ESchemeTokenType::Identifier)
            {
                if (token.m_sval == "cond")
                    ParseConditional();
                else if (token.m_sval == "case")
                    ParseCaseClause();
                else if (token.m_sval == "and" || token.m_sval == "or")
                    ParseTestClause();
                else if (token.m_sval == "when")
                    ParseWhenClause();
                else if (token.m_sval == "unless")
                    ParseWhenClause();
                else if (token.m_sval == "let")
                    ParseWhenClause();
                else if (token.m_sval == "let*")
                    ParseWhenClause();
                else if (token.m_sval == "letrec")
                    ParseWhenClause();
                else if (token.m_sval == "letrec*")
                    ParseWhenClause();
                else if (token.m_sval == "let-values")
                    ParseWhenClause();
                else if (token.m_sval == "let*-values")
                    ParseWhenClause();
                else if (token.m_sval == "begin")
                    ParseWhenClause();
                else if (token.m_sval == "do")
                    ParseWhenClause();
                else if (token.m_sval == "delay")
                    ParseWhenClause();
                else if (token.m_sval == "delay-force")
                    ParseWhenClause();
                else if (token.m_sval == "parameterize")
                    ParseWhenClause();
                else if (token.m_sval == "guard")
                    ParseWhenClause();
                else if (token.m_sval == "case-lambda")
                    ParseWhenClause();
                (void)m_tokenizer.ReadToken(); // Read closeParenthesis
            }
        }
    }

    void CSchemeInterpreter::BuildAST(std::string& str)
    {
        m_tokenizer.m_data = str;
        while (true)
        {
            auto token = m_tokenizer.PeekToken();
            if (token.m_type == ESchemeTokenType::EndOfFile)
                break;
            auto node = ParseExpression();
            m_ast.m_expressions.push_back(node);
        }
    }

    void CSchemeInterpreter::Parse(std::string& str)
    {
        BuildAST(str);
    }
}
