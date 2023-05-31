//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module Parsers.Phonemes.IPhonemes;
import Parsers.Phonemes.Phonemes;

namespace Caustic
{
    CRefObj<IPhonemes> CreatePhonemes()
    {
        extern CRefObj<IPhonemes> CreatePhonemesInternal();
        return CreatePhonemesInternal();
    }
}
