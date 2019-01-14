using System.Collections.Generic;
using System.Windows.Controls;

namespace PsiEditor
{
    public class DispGraph : VisualNode
    {
        public List<DispNode> dispNodes;

        public DispGraph(Canvas parentCanvas) : base(parentCanvas)
        {
            dispNodes = new List<DispNode>();
        }

        public override void Layout(double x, double y)
        {
            for (int i = 0; i < dispNodes.Count; i++)
            {
                dispNodes[i].Layout(x, y);
            }
        }
    }
}
