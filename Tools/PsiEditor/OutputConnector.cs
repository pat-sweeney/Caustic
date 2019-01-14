using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PsiEditor
{
    public class OutputConnector : Connector
    {
        public List<InputConnector> destinations;

        public OutputConnector(Node parent, string name) : base(parent, name)
        {
            destinations = new List<InputConnector>();
            Parent = parent;
            Name = name;
        }

        void ConnectTo(InputConnector ic)
        {
            destinations.Add(ic);
        }
    }
}
