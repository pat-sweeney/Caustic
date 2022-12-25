//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <any>
#include <windows.h>
#include <varargs.h>

module Parsers.JSon.IJSonParser;
import Parsers.JSon.JSonParser;
import Parsers.Lex.Lex;
import Parsers.Lex.ILex;

// Namespace: Caustic
namespace Caustic
{
    CRefObj<IJSonParser> CreateJSonParser()
    {
        return CRefObj<IJSonParser>(new CJSonParser());
    }
}
