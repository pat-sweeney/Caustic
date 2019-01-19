using System.Collections.Generic;
using System.IO;


namespace UPCInput
{
    class Program
    {
        static void Main(string[] args)
        {
            Dictionary<string, int> items = new Dictionary<string, int>();
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
            // write data to csv
            FileStream fs = new FileStream(@"d:\inventory.csv", FileMode.Create);
            StreamWriter strm = new StreamWriter(fs);
            foreach (var data in items)
            {
                strm.WriteLine($"{data.Key},{data.Value.ToString()}");
            }
            strm.Flush();
            fs.Close();
        }
    }
}
