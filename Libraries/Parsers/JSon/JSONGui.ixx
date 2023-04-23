//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
export module Parsers.JSon.JSonTree;
import Parsers.JSon.IJSonParser;

export namespace Caustic
{
    void JSONTree(IJSonObj* pObj, const char* pTreeName);
}