//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <Windows.h>

export module Parsers.Phonemes.Phonemes;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Parsers.Phonemes.IPhonemes;

export namespace Caustic
{
    class CPhonemes : public IPhonemes, public CRefCount
    {
        std::map<std::string, std::vector<std::string>> m_words;
        std::unique_ptr<char> m_spBuffer;
        uint32_t m_bufferIndex;
        uint32_t m_bufferLen;
        const int c_BufferSize = 10240;

        bool ReadBuffer(HANDLE f);
        std::string ReadLine(HANDLE f);
    public:
        CPhonemes() :
            m_bufferIndex(0),
            m_bufferLen(0)
        {
            m_spBuffer.reset(new char[c_BufferSize]);
        }

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::AddRef(); }

        //**********************************************************************
        // IPhonemes
        //**********************************************************************
        virtual void LoadDatabase() override;
        virtual void GetPhonemes(std::string word, std::vector<std::string> &vec) override;
    };

    CRefObj<IPhonemes> CreatePhonemesInternal();
}
