//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Parsers.Lex.ILex;
import Parsers.Lex.Lex;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;

// Namespace: Caustic
namespace Caustic
{
	CRefObj<ILex> CreateLex(const char* buffer, bool returnWhitespace /* = false */)
	{
		return CRefObj<ILex>(new CLex(buffer, returnWhitespace));
	}
}

