using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media;
using System.Collections.Generic;

namespace PsiEditor
{
    public class DispConnector : VisualNode
    {
        // Define attributes of connector
        public List<DispConnector> Connections;
        public DispNode Parent { get; set; }
        public bool IsInput { get; set; }
        public string Name { get; set; }

        // Define visual attributes of connector
        private Line line;
        private Ellipse ellipse;
        private Line separator;
        private TextBlock tb;
        private Path link; // Connects an input connector to an output connector
        private System.Windows.Point[] pts = new System.Windows.Point[4];

        public DispConnector(DispNode parent, string name, Canvas parentCanvas, bool isInput) : base(parentCanvas)
        {
            this.Parent = parent;
            this.Connections = new List<DispConnector>();
            this.Name = name;
            this.IsInput = isInput;

            this.Width = 140.0;
            this.Height = 15.0;

            // Draw connector line
            this.line = new Line();
            this.ParentCanvas.Children.Add(this.line);
            this.line.SetValue(Canvas.ZIndexProperty, 4);
            this.line.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.line.StrokeThickness = 3.0;

            // Draw connector circle
            this.ellipse = new Ellipse();
            this.ellipse.SetValue(Canvas.ZIndexProperty, 4);
            this.ParentCanvas.Children.Add(ellipse);
            this.ellipse.Fill = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.ellipse.Width = 10.0;
            this.ellipse.Height = 10.0;
            this.ellipse.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.ellipse.StrokeThickness = 2.0;

            // Add name of input node
            this.tb = new TextBlock();
            this.tb.SetValue(Canvas.ZIndexProperty, 4);
            this.ParentCanvas.Children.Add(tb);
            this.tb.Text = name;

            // line delimiting input nodes
            this.separator = new Line();
            this.separator.SetValue(Canvas.ZIndexProperty, 4);
            this.ParentCanvas.Children.Add(this.separator);
            this.separator.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.separator.StrokeThickness = 1.0;

            if (isInput)
            {
                this.link = new Path();
                this.link.SetValue(Canvas.ZIndexProperty, 5);
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

        public override bool PointInObject(double x, double y)
        {
            double ex = (double)this.ellipse.GetValue(Canvas.LeftProperty);
            double ey = (double)this.ellipse.GetValue(Canvas.TopProperty);
            if (x >= ex && x < ex + this.ellipse.Width && y >= ey && y < ey + this.ellipse.Height)
                return true;
            return false;
        }

        public void ConnectTo(DispConnector connector)
        {
            DispConnector inputConnector = (IsInput) ? this : connector;
            DispConnector outputConnector = (IsInput) ? connector : this;
            if (inputConnector == outputConnector)
                return;
            if (inputConnector.Connections.Count == 0 || inputConnector.Connections[0] != outputConnector)
            {
                inputConnector.Connections.Clear();
                inputConnector.Connections.Add(outputConnector);
                outputConnector.Connections.Add(inputConnector);
            }
        }

        public override void Layout(double x, double y)
        {
            this.line.X1 = x + ((this.IsInput) ? -20.0 : 90.0);
            this.line.Y1 = y + 8;
            this.line.X2 = x + ((this.IsInput) ? 10.0 : 120.0);
            this.line.Y2 = y + 8;
            this.ellipse.SetValue(Canvas.TopProperty, y + 3.0);
            this.ellipse.SetValue(Canvas.LeftProperty, x + ((this.IsInput) ? -20.0 : 110.0));
            this.tb.SetValue(Canvas.TopProperty, y + 0.0);
            this.tb.SetValue(Canvas.LeftProperty, x + 20.0);
            this.separator.X1 = x + 0.0;
            this.separator.Y1 = y + 17.0;
            this.separator.X2 = x + 100.0;
            this.separator.Y2 = y + 17.0;
            for (int i = 0; i < this.Connections.Count;i++)
            {
                if (this.IsInput)
                    UpdateConnectionLink(this, this.line, this.ParentCanvas, this.Connections[i].line, this.Connections[i].ParentCanvas);
                else
                    UpdateConnectionLink(this.Connections[i], this.Connections[i].line, this.Connections[i].ParentCanvas, this.line, this.ParentCanvas);
            }
        }

        void UpdateConnectionLink(DispConnector owner, Line inputLine, Canvas inputCanvas, Line outputLine, Canvas outputCanvas)
        {
            this.pts[0] = inputCanvas.TranslatePoint(new System.Windows.Point(inputLine.X1, inputLine.Y1), VisualNode.MasterCanvas);
            this.pts[1] = inputCanvas.TranslatePoint(new System.Windows.Point(inputLine.X1 - 50.0, inputLine.Y2), VisualNode.MasterCanvas);
            this.pts[2] = outputCanvas.TranslatePoint(new System.Windows.Point(outputLine.X1 + 50.0, outputLine.Y1), VisualNode.MasterCanvas);
            this.pts[3] = outputCanvas.TranslatePoint(new System.Windows.Point(outputLine.X1, outputLine.Y2), VisualNode.MasterCanvas);
            owner.UpdateConnection();
        }
    }
}
