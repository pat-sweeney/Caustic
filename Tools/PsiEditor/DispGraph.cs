using System.Collections.Generic;
using System.Windows.Controls;

namespace PsiEditor
{
    public class DispGraph : VisualNode
    {
        public List<DispNode> dispNodes;
        DispNode curSelection;

        public DispGraph(Canvas parentCanvas) : base(parentCanvas)
        {
            dispNodes = new List<DispNode>();
        }

        public void SelectNode(DispNode node)
        {
            if (curSelection != null)
                curSelection.Selected = false;
            curSelection = node;
            if (curSelection != null)
                curSelection.Selected = true;
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
