//**********************************************************************
// Copyright Patrick Sweeney 2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "Base\Core\error.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\IRefCount.h"
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <stack>

const int c_MaxFrames = 2; // Maximum number of frames being buffered

// A shader is a object that manages the vertex and pixel shader
//
namespace Caustic
{
    using namespace std;

    //**********************************************************************
    struct ISchemeInterpreter : public IRefCount
    {
        virtual void Bind(std::string& procname, std::function<void()> func) = 0;
        virtual void Eval(string& s) = 0;
        virtual void Eval(const char* s) = 0;
    };

    //**********************************************************************
    enum ESchemeASTNodeType
    {
        Unknown,
        Integer,
        Float,
        Symbol,
        Boolean,
        String,
        Character,
        ByteVector,
        Pair,
        Vector,
        ProcedureCall,
        LambdaExpression,
        IfClause,
        Assignment,
        Quote,
    };

    //**********************************************************************
    class CSchemeASTNode
    {
        ESchemeASTNodeType m_type;

        friend class CSchemeAST;
        friend class CSchemeInterpreter;
    public:
        virtual ESchemeASTNodeType GetType() = 0;
    };

    //**********************************************************************
    class CSchemeASTNode_Symbol : public CSchemeASTNode
    {
        std::string m_name;
    public:
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::Symbol; }
    };

    //**********************************************************************
    class CSchemeASTNode_Float : public CSchemeASTNode
    {
        float m_val;
    public:
        CSchemeASTNode_Float() {}
        CSchemeASTNode_Float(float f) { m_val = f; }
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::Float; }
    };

    //**********************************************************************
    class CSchemeASTNode_Integer : public CSchemeASTNode
    {
        int m_val;
    public:
        CSchemeASTNode_Integer() {}
        CSchemeASTNode_Integer(int f) { m_val = f; }
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::Integer; }
    };

    //**********************************************************************
    class CSchemeASTNode_Boolean : public CSchemeASTNode
    {
        bool m_val;
    public:
        CSchemeASTNode_Boolean() {}
        CSchemeASTNode_Boolean(bool f) { m_val = f; }
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::Boolean; }
    };

    //**********************************************************************
    class CSchemeASTNode_String : public CSchemeASTNode
    {
        std::string m_val;
    public:
        CSchemeASTNode_String() {}
        CSchemeASTNode_String(std::string f) { m_val = f; }
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::String; }
    };

    //**********************************************************************
    class CSchemeASTNode_Character : public CSchemeASTNode
    {
        char m_val;
    public:
        CSchemeASTNode_Character() {}
        CSchemeASTNode_Character(char f) { m_val = f; }
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::Character; }
    };

    //**********************************************************************
    class CSchemeASTNode_ByteVector : public CSchemeASTNode
    {
        std::vector<uint8> m_val;
    public:
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::ByteVector; }
    };

    //**********************************************************************
    class CSchemeASTNode_Pair : public CSchemeASTNode
    {
        std::shared_ptr<CSchemeASTNode> m_spCAR;
        std::shared_ptr<CSchemeASTNode_Pair> m_spCDR;

        friend class CSchemeAST;
        friend class CSchemeInterpreter;
    public:
        virtual ESchemeASTNodeType GetType() { return ESchemeASTNodeType::Pair; }
    };

    //**********************************************************************
    class CSchemeAST
    {
        std::vector<std::shared_ptr<CSchemeASTNode>> m_expressions;

        friend class CSchemeInterpreter;
    };

    //**********************************************************************
    enum ESchemeTokenType
    {
        Unknown,
        Identifier,
        Boolean,
        FloatNumber,
        IntNumber,
        Character,
        String,
        OpenParenthesis,
        CloseParenthesis,
        OpenVector,
        Quote,
        BackQuote,
        Comma,
        CommaAt,
        Pair,
        EndOfFile
    };

    //**********************************************************************
    class CSchemeToken
    {
        ESchemeTokenType m_type;
        float m_fval;
        int m_ival;
        char m_cval;
        bool m_bval;
        std::string m_sval;

        friend class CSchemeInterpreter;
        friend class CSchemeTokenizer;
    public:
        CSchemeToken() : m_type(ESchemeTokenType::Unknown)
        {
        }
        
        CSchemeToken(ESchemeTokenType t, std::string data) : m_type(t)
        {
            m_sval = data;
        }

        CSchemeToken(ESchemeTokenType t, float data) : m_type(t)
        {
            m_fval = data;
        }

        CSchemeToken(ESchemeTokenType t, int data) : m_type(t)
        {
            m_ival = data;
        }

        CSchemeToken(ESchemeTokenType t, char data) : m_type(t)
        {
            m_cval = data;
        }
    };

    //**********************************************************************
    class CSchemeTokenizer
    {
        std::string m_data; // String containing code to be evaluated
        int m_curIndex;
        CSchemeToken m_lastTokenRead;
        bool m_lastTokenValid;

#ifdef DUMP_TABLE
        void BuildCharDesignations();
#endif
        CSchemeToken ParseNumber();

        friend class CSchemeInterpreter;
    public:
        CSchemeTokenizer() :
            m_curIndex(0),
            m_lastTokenValid(false)
        {
        }
        
        CSchemeToken ReadToken();
        CSchemeToken PeekToken();
    };
    
    //**********************************************************************
    // Class: SchemeInterpreter
    //
    // Header:
    // [Link:Interpreter/Scheme/Scheme.h]
    //**********************************************************************
    class CSchemeInterpreter : public ISchemeInterpreter, public CRefCount
    {
        CSchemeTokenizer m_tokenizer;
        CSchemeAST m_ast;
        std::map<string, std::function<void()>> m_bindings;
        
        void Parse(std::string& str);
        void BuildAST(std::string& str);
        std::shared_ptr<CSchemeASTNode> ParseDatum();
        std::shared_ptr<CSchemeASTNode> ParseListOrPair();
        std::shared_ptr<CSchemeASTNode> ParseExpression();
    public:
        CSchemeInterpreter();

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISchemeInterpreter
        //**********************************************************************
        virtual void Bind(std::string& procname, std::function<void()> func) override;
        virtual void Eval(string& s) override;
        virtual void Eval(const char *s) override;
    };
}
