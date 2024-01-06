// PostProcDoc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
import Base.Core.ConvertStr;

static void Usage()
{
    std::cout << "Usage:  PostProcDoc [-help] [-debug] <path>";
    exit(0);
}

std::wstring getExtension(const std::wstring& filename)
{
    size_t dot = filename.find_last_of(L".");
    if (dot == std::string::npos) return L"";
    return filename.substr(dot + 1);
}

std::unique_ptr<byte> ReadBytesInFile(std::wstring fn)
{
    HANDLE hFile = CreateFile(fn.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        std::string sfn = Caustic::wstr2str(fn.c_str());
        std::cout << "Failed to open file: '" << sfn.c_str() << "'" << std::endl;
        return std::unique_ptr<byte>(nullptr);
    }

    DWORD dwFileSize = GetFileSize(hFile, NULL);
    byte* lpBuffer = new byte[dwFileSize];

    DWORD dwBytesRead;
    if (!ReadFile(hFile, lpBuffer, dwFileSize, &dwBytesRead, NULL))
    {
        std::string sfn = Caustic::wstr2str(fn.c_str());
        std::cout << "Failed to read file: '" << sfn.c_str() << "'" << std::endl;
        CloseHandle(hFile);
        delete[] lpBuffer;
        return std::unique_ptr<byte>(nullptr);
    }

    CloseHandle(hFile);
    return std::unique_ptr<byte>(lpBuffer);
}

static void ProcessFiles(std::wstring folder, int level)
{
    // Recursively process each HTML file
    WIN32_FIND_DATA fd;
    std::wstring wildcardFolder = folder + L"\\*";
    HANDLE h = ::FindFirstFile(wildcardFolder.c_str(), &fd);
    if (h != INVALID_HANDLE_VALUE)
    {
        do {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (fd.cFileName[0] == '.')
                    continue;
                std::wstring dirname(fd.cFileName);
                ProcessFiles(folder + L"\\" + dirname, level + 1);
            }
            else
            {
                std::wstring ext = getExtension(fd.cFileName);
                if (ext == L"html")
                {
                    std::wstring fn(folder + L"\\" + fd.cFileName);
                    std::string sfn = Caustic::wstr2str(fn);
                    std::cout << "Processing file:" << sfn.c_str();
                    std::unique_ptr<byte> spData = ReadBytesInFile(fn);
                    if (spData != nullptr)
                    {
                        // scan text and find {Link:#include "..."{...}} or
                        // {Link:import ...;{...}} tags
                        bool changed = false;
                        std::string s2((char*)spData.get());
                        char* convertedText = new char[s2.length() * 2];
                        int outputIndex = 0;
                        int inputIndex = 0;
                        while (inputIndex < s2.length())
                        {
                            if (s2[inputIndex] == '{')
                            {
                                if ("{Link:" == s2.substr(inputIndex, 6))
                                {
                                    inputIndex += 6;
                                    int l = inputIndex;
                                    while (s2[l] != '}' && s2[l] != '{')
                                        l++;
                                    std::string linkName = s2.substr(inputIndex, l - inputIndex);
                                    inputIndex = l;

                                    std::string refpath;
                                    if (s2[l] == '{')
                                    {
                                        l = ++inputIndex;
                                        while (s2[l] != '}')
                                            l++;
                                        refpath = s2.substr(inputIndex, l - inputIndex);
                                        inputIndex = l + 1;
                                    }
                                    inputIndex++; // skip final ']'
                                    std::string levels;
                                    for (int k = 0; k < level; k++)
                                        levels += "../";
                                    if (refpath.length() == 0)
                                        refpath = linkName;

                                    std::string newlink = "<a href='" + levels + "index.html#File:" + refpath + "' target='_top'>" + linkName + "</a>";
                                    for (int i = 0; i < newlink.length(); i++)
                                        convertedText[outputIndex++] = newlink[i];
                                    changed = true;
                                }
                                else
                                {
                                    convertedText[outputIndex++] = s2[inputIndex++];
                                }
                            }
                            else
                            {
                                convertedText[outputIndex++] = s2[inputIndex++];
                            }
                        }
                        if (changed)
                        {
                            std::ofstream fs(fn, std::ios::binary);
                            fs.write(convertedText, outputIndex);
                            fs.close();
                        }
                    }
                }
            }
        } while (::FindNextFile(h, &fd));
        ::FindClose(h);
    }
}

// This app is used to post-process our documentation so
// that we can support features that aren't directly supported
// by NaturalDocs.
int main(int argc, const char** argv)
{
    if (argc == 0)
        Usage();
    for (int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if (arg == std::string("-debug"))
            while (true); // Give debugger chance to connect
        else if (arg == std::string("-help"))
            Usage();
    }
    std::string folder = std::string(argv[argc - 1]);
    std::wstring f = Caustic::str2wstr(folder);
    ProcessFiles(f, 0);
}
