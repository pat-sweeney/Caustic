// ModuleBrowser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <windows.h>
import Base.Core.Core;
import Base.Core.IRefCount;
import Parsers.JSon.JSonParser;
import Parsers.JSon.IJSonParser;

using namespace Caustic;

struct ModuleRefs
{
    std::string m_moduleName;
    std::vector<std::string> m_referencedModules;
};

bool ProcessFolder(const wchar_t *pFolder)
{
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(pFolder, &data);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;
    do {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if ((data.cFileName[0] == L'.' && data.cFileName[1] == L'\0') ||
                (data.cFileName[0] == L'.' && data.cFileName[1] == L'.' && data.cFileName[2] == L'\0'))
                continue;
            ProcessFolder(data.cFileName);
        }
        else
        {
            std::wstring fn(data.cFileName);
            if (fn.ends_with(L".module.json"))
            {
            }
        }
    } while (FindNextFile(hFind, &data));
    FindClose(hFind);
    return true;
}

int main()
{
    CRefObj<IJSonParser> spParser = CreateJSonParser();
    CRefObj<IJSonObj> spJsonObj = spParser->CreateJSon("", nullptr);
    std::map<std::string, ModuleRefs*> modules;
    ProcessFolder(L"d:\\github\\Caustic\\Libraries\\*");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file





#if false
using Newtonsoft.Json;
using System;

namespace MyApp // Note: actual namespace depends on the project name.
{
    public class ModuleInfoData
    {
        public string Source{ get; set; }
        public string ProvidedModule{ get; set; }
        public List<string> ImportedModules{ get; set; }
    }

    public class ModuleInfo
    {
        [NonSerialized]
        public string Filename;
        public string Version{ get; set; }
        public ModuleInfoData data{ get; set; }
        public List<string> Includes{ get; set; }

    }

    internal class Program
    {
        public static List<ModuleInfo> modules{ get; set; }
            public static void WalkTree(string path)
        {
            var files = Directory.EnumerateFiles(path, "*ifc.dt.module.json", SearchOption.AllDirectories);
            foreach(string file in files)
            {
                var json = File.ReadAllText(file);
                var modInfo = JsonConvert.DeserializeObject<ModuleInfo>(json);
                modInfo.Filename = file;
                modules.Add(modInfo);
                System.Console.WriteLine(file);
            }
        }

        static Dictionary<string, bool> visited;

        public static void ShowModules(int indent, Dictionary<string, bool> visited, ModuleInfo module)
        {
            for (int i = 0; i < indent; i++)
                Console.Write(" ");
            Console.WriteLine($"{module.data.ProvidedModule}");
            foreach(var f in module.data.ImportedModules)
            {
                if (!visited.ContainsKey(f))
                {
                    visited[f] = true;
                    foreach(var m in modules)
                    {
                        if (m.data.ProvidedModule == f)
                        {
                            ShowModules(indent + 4, visited, m);
                            break;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < indent + 4; i++)
                        Console.Write(" ");
                    Console.WriteLine($"*{f}");
                }
            }
        }

        public static void DumpReferences(string modulename)
        {
            foreach(var module in modules)
            {
                if (module.data.ProvidedModule == modulename)
                {
                    visited = new Dictionary<string, bool>();
                    ShowModules(0, visited, module);
                    break;
                }
            }
        }

        static void Main(string[] args)
        {
            if (args[0] == "-?" || args[0] == "--help" || args[0] == "/?")
            {
                Console.WriteLine("Usage: ParseModules [-?] <moduleName> <rootDir>");
                Console.WriteLine("Where:");
                Console.WriteLine("<moduleName> - name of module. e.g. Parsers.JSon.JSonParser");
                Console.WriteLine("<rootDir> - folder to search. e.g. d:\\github\\Caustic\\Libraries");
            }
            else
            {
                modules = new List<ModuleInfo>();
                WalkTree(args[1]);
                DumpReferences(args[0]);
            }
        }
    }
}#endif