using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PsiEditor
{
    public class InputConnector : Connector
    {
        public OutputConnector source;

        public InputConnector(Node parent, string name) : base(parent, name)
        {
            Parent = parent;
            Name = name;
        }

        public void ConnectTo(OutputConnector oc)
        {
            source = oc;
        }
    }
}
