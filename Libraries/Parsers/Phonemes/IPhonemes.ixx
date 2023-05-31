//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <vector>

export module Parsers.Phonemes.IPhonemes;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;

export namespace Caustic
{
    struct IPhonemes : public IRefCount
    {
        virtual void GetPhonemes(std::string word, std::vector<std::string> & vec) = 0;
    };

    CRefObj<IPhonemes> CreatePhonemes();
}
