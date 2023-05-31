//**********************************************************************
// Copyright Patrick Sweeney 2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <string>
#include <cstring>
#include <map>
#include <Windows.h>
#include <vector>

module Parsers.Phonemes.Phonemes;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Error;
import Parsers.Phonemes.IPhonemes;

namespace Caustic
{
	CRefObj<IPhonemes> CreatePhonemesInternal()
	{
		CRefObj<IPhonemes> spPhonemes(new CPhonemes());
		return spPhonemes;
	}
	const char* words[] = {
		"!EXCLAMATION-POINT",  "EH2 K S K L AH0 M EY1 SH AH0 N P OY2 N T", nullptr,
		"SHARP",  "SH AA1 R P", nullptr,
		"ABNORMALITIES",  "AE2 B N AO0 R M AE1 L AH0 T IY0 Z", nullptr,
		"AARDVARK",  "AA1 R D V AA2 R K", nullptr,
		"FISHERMAN",  "F IH1 SH ER0 M AE2 N", nullptr,
		"WIDGET",  "W IH1 JH IH0 T", nullptr,
		"BOOKCELLAR",  "B UH1 K S EH1 L ER0", nullptr,
		"AERODYNAMIC",  "EH2 R OW0 D AY0 N AE1 M IH0 K", nullptr,
		"THANKYOU",  "TH AE1 NG K Y UW1", nullptr,
		"ABOUT",  "AH0 B AW1 T", nullptr,
		"GASOLINE",  "G AE1 S AH0 L IY2 N", nullptr,
		"HEALTH",  "HH EH1 L TH", nullptr,
		"CHAIR",  "CH EH1 R", nullptr,
		"ABRASION",  "AH0 B R EY1 ZH AH0 N", nullptr,
		"ANOTHER",  "AH0 N AH1 DH ER0", nullptr
	};

	bool CPhonemes::ReadBuffer(HANDLE f)
	{
		DWORD dwBytesRead;
		if (!ReadFile(f, m_spBuffer.get(), c_BufferSize, &dwBytesRead, nullptr))
			return false;
		if (dwBytesRead == 0)
			return false;
		m_bufferLen = dwBytesRead;
		m_bufferIndex = 0;
		return true;
	}

	std::string CPhonemes::ReadLine(HANDLE f)
	{
		const char* pBuf = m_spBuffer.get();
		std::string str;
		while (true)
		{
			while (pBuf[m_bufferIndex] != '\n' && pBuf[m_bufferIndex] != '\r' && m_bufferIndex != m_bufferLen)
			{
				str += pBuf[m_bufferIndex++];
			}
			while ((pBuf[m_bufferIndex] == '\n' || pBuf[m_bufferIndex] == '\r') && m_bufferIndex != m_bufferLen)
				m_bufferIndex++;
			if (m_bufferIndex == m_bufferLen)
			{
				if (!ReadBuffer(f))
					break;
				continue;
			}
			break;
		}
		return str;
	}

	void CPhonemes::LoadDatabase()
	{
		HANDLE f = CreateFile(L"d:\\data\\cmudict-0.7b", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		CT(f == INVALID_HANDLE_VALUE ? E_FAIL : S_OK);
		char* nextToken;
		while (true)
		{
			std::string s = ReadLine(f);
			if (s[0] == ';' && s[1] == ';')
				continue;
			const char* pWord = strtok_s((char*)(s.c_str()), " ", &nextToken);
			if (pWord == nullptr)
				break;
			const char* pPhoneme = strtok_s(nullptr, " ", &nextToken);
			std::vector<std::string> phonemes;
			while (pPhoneme != nullptr)
			{
				phonemes.push_back(std::string(pPhoneme));
				pPhoneme = strtok_s(nullptr, " ", &nextToken);
			}
			m_words.insert(std::make_pair(std::string(pWord), phonemes));
		}
		CloseHandle(f);
	}

	void CPhonemes::GetPhonemes(std::string word, std::vector<std::string>& vec)
	{
		vec = m_words[word];
		return;
	}
}
