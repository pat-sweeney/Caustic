using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PsiEditor
{
    public class Connector
    {
        public Node Parent { get; set; }
        public string Name { get; set; }

        public Connector(Node parent, string name)
        {
            Parent = parent;
            Name = name;
        }
    }
}
