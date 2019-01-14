using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media;

namespace PsiEditor
{
    public class DispConnector : VisualNode
    {
        private Connector connector;
        private Line line;
        private Ellipse ellipse;
        private Line separator;
        private TextBlock tb;
        private bool isInput;

        public DispConnector(Connector connector, string name, Canvas parentCanvas, bool isInput) : base(parentCanvas)
        {
            this.connector = connector;
            this.isInput = isInput;
            this.Width = 140.0;
            this.Height = 15.0;

            // Draw connector line
            this.line = new Line();
            this.ParentCanvas.Children.Add(this.line);
            this.line.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.line.StrokeThickness = 3.0;

            // Draw connector circle
            this.ellipse = new Ellipse();
            this.ParentCanvas.Children.Add(ellipse);
            this.ellipse.Fill = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.ellipse.Width = 10.0;
            this.ellipse.Height = 10.0;
            this.ellipse.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.ellipse.StrokeThickness = 2.0;

            // Add name of input node
            this.tb = new TextBlock();
            this.ParentCanvas.Children.Add(tb);
            this.tb.Text = name;

            // line delimiting input nodes
            this.separator = new Line();
            this.ParentCanvas.Children.Add(this.separator);
            this.separator.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.separator.StrokeThickness = 1.0;

            Layout(0.0, 0.0);
        }

        public override void Layout(double x, double y)
        {
            this.line.X1 = x + ((this.isInput) ? -20.0 : 90.0);
            this.line.Y1 = y + 8;
            this.line.X2 = x + ((this.isInput) ? 10.0 : 120.0);
            this.line.Y2 = y + 8;
            this.ellipse.SetValue(Canvas.TopProperty, y + 3.0);
            this.ellipse.SetValue(Canvas.LeftProperty, x + ((this.isInput) ? -20.0 : 110.0));
            this.tb.SetValue(Canvas.TopProperty, y + 0.0);
            this.tb.SetValue(Canvas.LeftProperty, x + 20.0);
            this.separator.X1 = x + 0.0;
            this.separator.Y1 = y + 9.0;
            this.separator.X2 = x + 100.0;
            this.separator.Y2 = y + 9.0;
        }
    }
}
