using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PsiEditor
{
    public class WebCamOperator : Node
    {
        public WebCamOperator()
        {
            var oc = new OutputConnector(this, "Output");
            outputs.Add(oc);
        }
    }

    public class DoOperator : Node
    {
        public DoOperator()
        {
            var ic = new InputConnector(this, "Input");
            inputs.Add(ic);
            var oc = new OutputConnector(this, "Output");
            outputs.Add(oc);
        }
    }
}
