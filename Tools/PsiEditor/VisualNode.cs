using System.Windows.Controls;

namespace PsiEditor
{
    public class VisualNode
    {
        private double x;
        private double y;

        public Canvas ParentCanvas { get; set; }
        public static Canvas MasterCanvas { get; set; }
        public double X { get { return x; } set { x = value; Layout(x, y); } }
        public double Y { get { return y; } set { y = value; Layout(x, y); } }
        public double Width { get; set; }
        public double Height { get; set; }

        public VisualNode(Canvas parent)
        {
            this.ParentCanvas = parent;
        }

        public virtual void Layout(double x, double y) { }
        public virtual bool PointInObject(double x, double y) { return false; }
        public virtual void GenerateVariableDefinitions(System.IO.StreamWriter strm, int indent) { }
        public virtual void GenerateSetup(System.IO.StreamWriter strm, int indent) { }
        public virtual void GenerateCode(System.IO.StreamWriter strm, int indent) { }
    }
}