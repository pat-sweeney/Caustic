using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media.Effects;
using System.Windows.Media;

namespace PsiEditor
{
    public class DispNode : VisualNode
    {
        private Canvas canvas;
        private Node node;
        private Rectangle rect;
        private Rectangle selectRect;
        public double NodeWidth { get; set; }
        public double NodeHeight { get; set; }
        public List<DispConnector> inputs;
        public List<DispConnector> outputs;

        public DispNode(Node node, Canvas parentCanvas) : base(parentCanvas)
        {
            this.node = node;
            this.inputs = new List<DispConnector>();
            this.outputs = new List<DispConnector>();

            // Create main canvas that will act as group for our node
            this.canvas = new Canvas();
            this.ParentCanvas.Children.Add(this.canvas);

            this.NodeWidth = 100.0;
            this.NodeHeight = 100.0;

            // Create main body rectangle
            this.rect = new Rectangle();
            this.rect.Width = 100.0;
            this.rect.Height = 100.0;
            this.rect.Fill = new SolidColorBrush(Color.FromRgb(0x5a, 0x9a, 0xe0));
            this.rect.RadiusX = 10.0;
            this.rect.RadiusY = 10.0;
            this.rect.Stretch = Stretch.Uniform;
            this.rect.Effect = new DropShadowEffect();
            this.rect.Effect.SetCurrentValue(DropShadowEffect.ShadowDepthProperty, 10.0);
            this.rect.Effect.SetCurrentValue(DropShadowEffect.DirectionProperty, 315.0);
            this.rect.Effect.SetCurrentValue(DropShadowEffect.ColorProperty, Color.FromRgb(0, 0, 0));
            this.rect.Effect.SetCurrentValue(DropShadowEffect.OpacityProperty, 0.5);
            this.rect.Effect.SetCurrentValue(DropShadowEffect.BlurRadiusProperty, 10.0);
            this.canvas.ClipToBounds = false;
            this.canvas.Children.Add(this.rect);

            // Define the rectangle for outlining node when it is selected
            this.selectRect = new Rectangle();
            this.canvas.Children.Add(this.selectRect);
            this.selectRect.Visibility = Visibility.Hidden;
            this.selectRect.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
            this.selectRect.SetValue(Canvas.LeftProperty, -5.0);
            this.selectRect.SetValue(Canvas.TopProperty, -5.0);
            this.selectRect.Width = 110.0;
            this.selectRect.Height = 110.0; // Depends on connector count
            this.selectRect.RadiusX = 10.0;
            this.selectRect.RadiusY = 10.0;
            this.selectRect.Stretch = Stretch.UniformToFill;

            // Define delete rectangle in upper right corner of node (when clicked on will delete node)
            var rect = new Rectangle();
            canvas.Children.Add(rect);
            rect.SetValue(Canvas.TopProperty, 5.0);
            rect.SetValue(Canvas.LeftProperty, 80.0);
            rect.Width = 10.0;
            rect.Height = 10.0;
            rect.Fill = new SolidColorBrush(Color.FromRgb(0, 0, 0));

            // Draw 'X' for deleting node
            var line = new Line();
            canvas.Children.Add(line);
            line.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
            line.StrokeThickness = 3.0;
            line.X1 = 81.0;
            line.Y1 = 6.0;
            line.X2 = 89.0;
            line.Y2 = 14.0;

            line = new Line();
            canvas.Children.Add(line);
            line.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
            line.StrokeThickness = 3.0;
            line.X1 = 81.0;
            line.Y1 = 14.0;
            line.X2 = 89.0;
            line.Y2 = 6.0;

            // Add name of connector
            var tb = new TextBlock();
            canvas.Children.Add(tb);
            tb.SetValue(Canvas.TopProperty, 0.0);
            tb.SetValue(Canvas.LeftProperty, 20.0);
            tb.Text = node.Name;

            // Add separator line between node name and list of connectors
            line = new Line();
            canvas.Children.Add(line);
            line.X1 = 0.0;
            line.Y1 = 20.0;
            line.X2 = 100.0;
            line.Y2 = 20.0;
            line.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            line.StrokeThickness = 3.0;

            // Define line separating first input connector
            line = new Line();
            canvas.Children.Add(line);
            line.X1 = 0.0;
            line.Y1 = 27.0;
            line.X2 = 100.0;
            line.Y2 = 27.0;
            line.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            line.StrokeThickness = 1.0;

            this.NodeHeight = 33.0;
            for (int i = 0; i < node.inputs.Count; i++)
            {
                var dc = new DispConnector(node.inputs[i], node.inputs[i].Name, this.canvas, true);
                dc.Y = this.NodeHeight;
                this.NodeHeight += dc.Height;
                this.inputs.Add(dc);
            }
            for (int i = 0; i < node.outputs.Count; i++)
            {
                var dc = new DispConnector(node.outputs[i], node.outputs[i].Name, this.canvas, false);
                dc.Y = this.NodeHeight;
                this.NodeHeight += dc.Height;
                this.outputs.Add(dc);
            }
        }

        public override void Layout(double x, double y)
        {
            this.ParentCanvas.SetValue(Canvas.LeftProperty, x);
            this.ParentCanvas.SetValue(Canvas.TopProperty, y);
            this.rect.Width = 100.0;
            this.rect.Height = this.NodeHeight;
            this.selectRect.Width = 100.0;
            this.selectRect.Height = this.NodeHeight + 10;
            double offset = 33.0;
            for (int i = 0; i < this.inputs.Count; i++)
            {
                this.inputs[i].Layout(x, offset);
                offset += this.inputs[i].Height;
            }
            for (int i = 0; i < this.outputs.Count; i++)
            {
                this.outputs[i].Layout(x, offset);
                offset += this.inputs[i].Height;
            }
        }
    }
}
