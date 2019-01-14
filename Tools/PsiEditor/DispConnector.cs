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
        private Path link; // Connects an input connector to an output connector
        private DispConnector connectedTo;
        private System.Windows.Point[] pts = new System.Windows.Point[4];

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

            if (isInput)
            {
                this.link = new Path();
                this.link.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
                this.link.Opacity = 0.7;
                this.link.StrokeThickness = 4.0;
                var geom = new PathGeometry();
                var figure = new PathFigure();
                this.pts[0] = new System.Windows.Point(-20.0, 0.0);
                this.pts[1] = new System.Windows.Point(-30.0, 0.0);
                this.pts[2] = new System.Windows.Point(-30.0, 10.0);
                this.pts[3] = new System.Windows.Point(-20.0, 10.0);
                figure.StartPoint = this.pts[0];
                var segment = new BezierSegment(this.pts[1], this.pts[2], this.pts[3], true);
                figure.Segments.Add(segment);
                geom.Figures.Add(figure);
                this.link.Data = geom;
                VisualNode.MasterCanvas.Children.Add(this.link);
            }

            Layout(0.0, 0.0);
        }

        private void UpdateConnection()
        {
            var geom = new PathGeometry();
            var figure = new PathFigure();
            figure.StartPoint = this.pts[0];
            var segment = new BezierSegment(this.pts[1], this.pts[2], this.pts[3], true);
            figure.Segments.Add(segment);
            geom.Figures.Add(figure);
            this.link.Data = geom;
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
            this.separator.Y1 = y + 17.0;
            this.separator.X2 = x + 100.0;
            this.separator.Y2 = y + 17.0;
            if (this.isInput)
            {
                this.pts[0] = this.ParentCanvas.TranslatePoint(new System.Windows.Point(this.line.X1, this.line.Y1), VisualNode.MasterCanvas);
                this.pts[1] = this.ParentCanvas.TranslatePoint(new System.Windows.Point(this.line.X1 - 10.0, this.line.Y2), VisualNode.MasterCanvas);
                if (this.connectedTo != null)
                {
                    this.pts[2] = this.connectedTo.ParentCanvas.TranslatePoint(new System.Windows.Point(this.connectedTo.line.X1 + 10.0, this.connectedTo.line.Y1), VisualNode.MasterCanvas);
                    this.pts[3] = this.connectedTo.ParentCanvas.TranslatePoint(new System.Windows.Point(this.connectedTo.line.X1, this.connectedTo.line.Y2), VisualNode.MasterCanvas);
                }
                UpdateConnection();
            }
        }
    }
}
