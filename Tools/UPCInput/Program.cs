using System.Collections.Generic;
using System.IO;


namespace UPCInput
{
    class Program
    {
        static void InputProc(string fn, string outfn)
        {
            Dictionary<string, int> items = new Dictionary<string, int>();
            LoadFile(ref items, fn);
            while (true)
            {
                System.Console.WriteLine("UPC:");
                string s = System.Console.ReadLine();
                if (s == "762274813938")
                    break;
                if (items.ContainsKey(s))
                    items[s]++;
                else
                    items.Add(s, 1);
            }
            SaveFile(ref items, outfn);
        }

        static void OutputProc(string fn, string outfn)
        {
            Dictionary<string, int> items = new Dictionary<string, int>();
            LoadFile(ref items, fn);
            while (true)
            {
                System.Console.WriteLine("UPC:");
                string s = System.Console.ReadLine();
                if (s == "762274813938")
                    break;
                if (items.ContainsKey(s))
                {
                    if (items[s] > 0)
                        items[s]--;
                    else
                        System.Console.WriteLine("Inventory empty!");
                }
            }
            SaveFile(ref items, outfn);
        }

        static void LoadFile(ref Dictionary<string, int> items, string fn)
        {
            FileStream fs1 = new FileStream(fn, FileMode.Open);
            StreamReader strm1 = new StreamReader(fs1);
            while (!strm1.EndOfStream)
            {
                string s = strm1.ReadLine();
                string[] sa = s.Split(new char[] { ',' });
                if (items.ContainsKey(sa[0]))
                    items[sa[0]] += System.Int32.Parse(sa[1]);
                else
                {
                    items.Add(sa[0], System.Int32.Parse(sa[1]));
                }
            }
            fs1.Close();
        }

        static void SaveFile(ref Dictionary<string, int> items, string fn)
        {
            FileStream fs1 = new FileStream(fn, FileMode.Create);
            StreamWriter strm1 = new StreamWriter(fs1);
            foreach (var k in items)
            {
                strm1.WriteLine(k.Key + "," + k.Value);
            }
            strm1.Flush();
            fs1.Close();
        }

        static void Merge(string fn1, string fn2, string outputfn)
        {
            Dictionary<string, int> items = new Dictionary<string, int>();
            LoadFile(ref items, fn1);
            LoadFile(ref items, fn2);
            SaveFile(ref items, outputfn);
        }

        public static void Main(string[] args)
        {
            int mode = 1;
            string fn1 = "", fn2 = "", outfn = "";
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i] == "-input")
                {
                    fn1 = args[i + 1];
                    fn2 = args[i + 2];
                    mode = 1;
                    i += 2;
                }
                else if (args[i] == "-output")
                {
                    fn1 = args[i + 1];
                    fn2 = args[i + 2];
                    mode = 2;
                    i += 2;
                }
                else if (args[i] == "-merge")
                {
                    fn1 = args[i + 1];
                    fn2 = args[i + 2];
                    outfn = args[i + 3];
                    mode = 3;
                    i += 3;
                }
            }
            switch(mode)
            {
                case 1: InputProc(fn1, fn2); break;
                case 2: OutputProc(fn1, fn2); break;
                case 3: Merge(fn1, fn2, outfn); break;
                default:
                    break;
            }
        }
    }
}
