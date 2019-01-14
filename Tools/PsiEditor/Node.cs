using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PsiEditor
{
    public class Node
    {
        public List<InputConnector> inputs;
        public List<OutputConnector> outputs;
        public string Name { get; set; }

        public Node()
        {
            inputs = new List<InputConnector>();
            outputs = new List<OutputConnector>();
        }

        public Node(string name)
        {
            Name = name;
            inputs = new List<InputConnector>();
            outputs = new List<OutputConnector>();
        }
    }
}
