﻿using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media.Effects;
using System.Windows.Media;

namespace PsiEditor
{
    public class DispNode : VisualNode
    {
        public System.Collections.Generic.Dictionary<string, object> properties;
        // Define attributes of a node
        public List<DispConnector> inputs;
        public List<DispConnector> outputs;
        public string Name { get; set; }

        // Define visual attributes associated with node
        private Rectangle rect;
        private Rectangle selectRect;
        public double NodeWidth { get; set; }
        public double NodeHeight { get; set; }

        public bool Selected
        {
            get { return selectRect.Visibility == Visibility.Visible; }
            set { selectRect.Visibility = (value) ? Visibility.Visible : Visibility.Hidden; }
        }

        public static void Connect(DispNode n1, string name1, DispNode n2, string name2)
        {
            n1.FindConnector(name1).Connections.Add(n2.FindConnector(name2));
            n2.FindConnector(name2).Connections.Add(n1.FindConnector(name1));
        }

        public DispConnector FindConnector(string name)
        {
            for (int i = 0; i < inputs.Count; i++)
                if (inputs[i].Name == name)
                    return inputs[i];
            for (int i = 0; i < outputs.Count; i++)
                if (outputs[i].Name == name)
                    return outputs[i];
            return null;
        }

        public DispNode(string name, Canvas parentCanvas) : base(parentCanvas)
        {
            this.properties = new Dictionary<string, object>();
            this.inputs = new List<DispConnector>();
            this.outputs = new List<DispConnector>();
            this.Name = name;
        }

        public void CreateVisuals()
        {
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
            this.rect.Stretch = Stretch.Fill;
            this.rect.Effect = new DropShadowEffect();
            this.rect.Effect.SetCurrentValue(DropShadowEffect.ShadowDepthProperty, 10.0);
            this.rect.Effect.SetCurrentValue(DropShadowEffect.DirectionProperty, 315.0);
            this.rect.Effect.SetCurrentValue(DropShadowEffect.ColorProperty, Color.FromRgb(0, 0, 0));
            this.rect.Effect.SetCurrentValue(DropShadowEffect.OpacityProperty, 0.5);
            this.rect.Effect.SetCurrentValue(DropShadowEffect.BlurRadiusProperty, 10.0);
            this.rect.SetValue(Canvas.ZIndexProperty, 0);
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
            this.selectRect.Stretch = Stretch.Fill;
            this.selectRect.SetValue(Canvas.ZIndexProperty, 0);

            // Define delete rectangle in upper right corner of node (when clicked on will delete node)
            var rect = new Rectangle();
            canvas.Children.Add(rect);
            rect.SetValue(Canvas.ZIndexProperty, 1);
            rect.SetValue(Canvas.TopProperty, 5.0);
            rect.SetValue(Canvas.LeftProperty, 80.0);
            rect.Width = 10.0;
            rect.Height = 10.0;
            rect.Fill = new SolidColorBrush(Color.FromRgb(0, 0, 0));

            // Draw 'X' for deleting node
            var line = new Line();
            canvas.Children.Add(line);
            line.SetValue(Canvas.ZIndexProperty, 2);
            line.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
            line.StrokeThickness = 3.0;
            line.X1 = 81.0;
            line.Y1 = 6.0;
            line.X2 = 89.0;
            line.Y2 = 14.0;

            line = new Line();
            canvas.Children.Add(line);
            line.SetValue(Canvas.ZIndexProperty, 2);
            line.Stroke = new SolidColorBrush(Color.FromRgb(255, 0, 0));
            line.StrokeThickness = 3.0;
            line.X1 = 81.0;
            line.Y1 = 14.0;
            line.X2 = 89.0;
            line.Y2 = 6.0;

            // Add name of connector
            var tb = new TextBlock();
            canvas.Children.Add(tb);
            tb.SetValue(Canvas.ZIndexProperty, 3);
            tb.SetValue(Canvas.TopProperty, 0.0);
            tb.SetValue(Canvas.LeftProperty, 20.0);
            tb.Text = Name;

            // Add separator line between node name and list of connectors
            line = new Line();
            canvas.Children.Add(line);
            line.SetValue(Canvas.ZIndexProperty, 2);
            line.X1 = 0.0;
            line.Y1 = 20.0;
            line.X2 = 100.0;
            line.Y2 = 20.0;
            line.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            line.StrokeThickness = 3.0;

            // Define line separating first input connector
            line = new Line();
            canvas.Children.Add(line);
            line.SetValue(Canvas.ZIndexProperty, 2);
            line.X1 = 0.0;
            line.Y1 = 27.0;
            line.X2 = 100.0;
            line.Y2 = 27.0;
            line.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            line.StrokeThickness = 1.0;
            /*
            this.NodeHeight = 33.0;
            for (int i = 0; i < inputs.Count; i++)
            {
                inputs[i].Y = this.NodeHeight;
                this.NodeHeight += inputs[i].Height;
            }
            for (int i = 0; i < outputs.Count; i++)
            {
                outputs[i].Y = this.NodeHeight;
                this.NodeHeight += outputs[i].Height;
            }
            */
            Layout(0.0, 0.0);
        }

        public override bool PointInObject(double x, double y)
        {
            if (x >= X && x < X + NodeWidth && y >= Y && y < Y + NodeHeight)
                return true;
            return false;
        }

        public override void Layout(double x, double y)
        {
            this.canvas.SetValue(Canvas.LeftProperty, x);
            this.canvas.SetValue(Canvas.TopProperty, y);
            this.rect.Height = this.NodeHeight;
            this.selectRect.Width = 110.0;
            this.selectRect.Height = this.NodeHeight + 10;
            double offset = 25.0;
            for (int i = 0; i < this.inputs.Count; i++)
            {
                this.inputs[i].Layout(x, y + offset);
                offset += this.inputs[i].Height;
            }
            for (int i = 0; i < this.outputs.Count; i++)
            {
                this.outputs[i].Layout(x, y + offset);
                offset += this.outputs[i].Height;
            }
        }
    }
}
