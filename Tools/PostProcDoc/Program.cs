using System;
using System.Diagnostics;
using System.IO;

namespace PostProcDoc
{
    class Program
    {
        static void Usage()
        {
            Console.WriteLine("Usage:  PostProcDoc <path>");
            Environment.Exit(0);
        }

        static void ProcessFiles(string folder, int level)
        {
            // Recursively process each HTML file
            var fileIter = Directory.EnumerateFiles(folder);
            foreach (var fileEntry in fileIter)
            {
                string ext = Path.GetExtension(fileEntry);
                if (ext == ".html")
                {
                    Console.WriteLine($"Processing file: {fileEntry}");
                    if (fileEntry.Contains("Vector"))
                        System.Diagnostics.Debug.WriteLine("blah");

                    var data = File.ReadAllBytes(fileEntry);
                    // scan text and find {Link:#include "..."{...}} tags
                    bool changed = false;
                    string s2 = System.Text.Encoding.UTF8.GetString(data, 0, data.Length);
                    char[] convertedText = new char[s2.Length * 2];
                    int outputIndex = 0;
                    int inputIndex = 0;
                    while (inputIndex < s2.Length)
                    {
                        if (s2[inputIndex] == '{')
                        {
                            if ("{Link:" == s2.Substring(inputIndex, 6))
                            {
                                inputIndex += 6;
                                int l = inputIndex;
                                while (s2[l] != '}' && s2[l] != '{')
                                    l++;
                                string linkName = s2.Substring(inputIndex, l - inputIndex);
                                inputIndex = l;

                                string refpath = string.Empty;
                                if (s2[l] == '{')
                                {
                                    l = ++inputIndex;
                                    while (s2[l] != '}')
                                        l++;
                                    refpath = s2.Substring(inputIndex, l - inputIndex);
                                    inputIndex = l + 1;
                                }
                                inputIndex++; // skip final ']'
                                string levels = "";
                                for (int k = 0; k < level; k++)
                                    levels += "../";
                                if (refpath.Length == 0)
                                    refpath = linkName;
                                string newlink = $"<a href='{levels}index.html#File:{refpath}' target='_top'>{linkName}</a>";
                                for (int i = 0; i < newlink.Length; i++)
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
                        FileStream fs = File.Create(fileEntry);
                        byte[] convertedBytes = System.Text.Encoding.UTF8.GetBytes(convertedText, 0, outputIndex);
                        fs.Write(convertedBytes, 0, convertedBytes.Length);
                        fs.Close();
                    }
                }
            }
            var dirIter = Directory.EnumerateDirectories(folder);
            foreach (var entry in dirIter)
            {
                ProcessFiles(entry, level + 1);
            }
        }
        // This app is used to post-process our documentation so
        // that we can support features that aren't directly supported
        // by NaturalDocs.
        static void Main(string[] args)
        {
            if (args.Length == 0)
                Usage();
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-help")
                    Usage();
            }
            string folder = args[0];
            ProcessFiles(folder, 0);
        }
    }
}
