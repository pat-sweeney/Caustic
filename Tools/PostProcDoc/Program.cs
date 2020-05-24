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
                    MemoryStream ms = new MemoryStream();
                    var data = File.ReadAllBytes(fileEntry);
                    // scan text and find [Link:...] tags
                    bool changed = false;
                    for (int j = 0; j < data.Length; j++)
                    {
                        if (data[j] == '[')
                        {
                            string s = System.Text.Encoding.UTF8.GetString(data, j, 10);
                            if ("[Link:" == s.Substring(0, 6))
                            {
                                j += 6;
                                int l = j;
                                while (data[l] != ']')
                                    l++;
                                string refpath = System.Text.Encoding.UTF8.GetString(data, j, l - j);
                                j = l;
                                string levels = "";
                                for (int k = 0; k < level; k++)
                                    levels += "../";
                                string newlink = $"<a href='{levels}index.html#File:{refpath}' target='_top'>{refpath}</a>";
                                ms.Write(System.Text.Encoding.UTF8.GetBytes(newlink));
                                changed = true;
                            }
                        }
                        else
                        {
                            ms.Write(data, j, 1);
                        }
                    }
                    if (changed)
                    {
                        FileStream fs = File.Create(fileEntry);
                        fs.Write(ms.ToArray());
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
