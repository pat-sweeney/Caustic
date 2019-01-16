using System.Windows;
using System.Windows.Controls;
using System.Collections.Generic;
using System.Windows.Shapes;
using System.Windows.Media;

namespace PsiEditor
{
    public static partial class Operators
    {
        static string spaces = "                                                                                ";

        public static void WriteIndent(this System.IO.StreamWriter strm, string value, int indent, bool newLine = true)
        {
            strm.Write(spaces.Substring(0, indent));
            if (newLine)
                strm.WriteLine(value);
            else
                strm.Write(value);
        }
    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private DispGraph dispGraph;

        public void BuildDispGraph()
        {
            VisualNode.MasterCanvas = this.MasterCanvas;
            this.dispGraph = new DispGraph(this.MasterCanvas);
            dispGraph.Layout(0.0, 0.0);
        }

        public MainWindow()
        {
            this.DataContext = this;
            InitializeComponent();
            BuildDispGraph();
            this.MouseDown += MainWindow_MouseDown;
            this.MouseUp += MainWindow_MouseUp;
            this.MouseMove += MainWindow_MouseMove;
            SetupLinkPath();
        }

        public enum Mode
        {
            None,
            DragNode,
            LinkConnectors
        }
        private Mode mode = Mode.None;
        private DispNode curNode = null;
        private DispConnector curConnector = null;
        private double nodeOffsetX = 0.0;
        private double nodeOffsetY = 0.0;
        private Path linkPath;
        private PathGeometry linkGeom;
        private PathFigure linkFigure;
        private BezierSegment linkSegment;
        private System.Windows.Point[] linkPts;

        private void SetupLinkPath()
        {
            this.linkPath = new Path();
            this.linkPath.Stroke = new SolidColorBrush(Color.FromRgb(0, 0, 0));
            this.linkPath.StrokeThickness = 4.0;
            this.linkGeom = new PathGeometry();
            this.linkFigure = new PathFigure();
            this.linkSegment = new BezierSegment();
            this.linkPts = new Point[4];
            this.linkFigure.Segments.Add(this.linkSegment);
            this.linkGeom.Figures.Add(this.linkFigure);
            this.linkPath.Data = this.linkGeom;
            this.MasterCanvas.Children.Add(this.linkPath);
        }

        private void MainWindow_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (mode == Mode.DragNode)
            {
                Point p = e.GetPosition(this.MasterCanvas);
                curNode.X = p.X - nodeOffsetX;
                curNode.Y = p.Y - nodeOffsetY;
            }
            else if (mode == Mode.LinkConnectors)
            {
                Point p = e.GetPosition(this.MasterCanvas);
                this.linkPts[2].X = p.X + 10;
                this.linkPts[2].Y = p.Y;
                this.linkPts[3].X = p.X;
                this.linkPts[3].Y = p.Y;
                this.linkFigure.StartPoint = this.linkPts[0];
                this.linkSegment.Point1 = this.linkPts[1];
                this.linkSegment.Point2 = this.linkPts[2];
                this.linkSegment.Point3 = this.linkPts[3];
                this.linkSegment.IsStroked = true;
            }
        }

        private DispConnector InConnector(Point p)
        {
            foreach (var dispNode in dispGraph.dispNodes)
            {
                DispConnector connector = InConnectorList(p, dispNode.inputs);
                if (connector != null)
                    return connector;
                connector = InConnectorList(p, dispNode.outputs);
                if (connector != null)
                    return connector;
            }
            return null;
        }

        private DispConnector InConnectorList(Point p, List<DispConnector> connectors)
        {
            for (int i = 0; i < connectors.Count; i++)
            {
                if (connectors[i].PointInObject(p.X, p.Y))
                    return connectors[i];
            }
            return null;
        }

        private void MainWindow_MouseUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (mode == Mode.DragNode)
            {
                Point p = e.GetPosition(this.MasterCanvas);
                curNode.X = p.X - nodeOffsetX;
                curNode.Y = p.Y - nodeOffsetY;
                curNode = null;
                mode = Mode.None;
            }
            else if (mode == Mode.LinkConnectors)
            {
                Point p = e.GetPosition(this.MasterCanvas);
                this.linkPts[2].X = p.X + 10;
                this.linkPts[2].Y = p.Y;
                this.linkPts[3].X = p.X;
                this.linkPts[3].Y = p.Y;
                this.linkFigure.StartPoint = this.linkPts[0];
                this.linkSegment.Point1 = this.linkPts[1];
                this.linkSegment.Point2 = this.linkPts[2];
                this.linkSegment.Point3 = this.linkPts[3];
                this.linkSegment.IsStroked = true;
                this.linkPath.Visibility = Visibility.Hidden;
                DispConnector connector = InConnector(p);
                if (connector != null)
                {
                    curConnector.ConnectTo(connector);
                    curConnector.Parent.Layout(curConnector.Parent.X, curConnector.Parent.Y);
                    connector.Parent.Layout(connector.Parent.X, connector.Parent.Y);
                }
                mode = Mode.None;
            }
        }

        private void UpdateProperties(DispNode node)
        {
            this.dispGraph.SelectNode(node);
            PropertyGrid.Children.Clear();
            PropertyGrid.RowDefinitions.Clear();
            if (node != null && node.properties != null)
            {
                int row = 0;
                foreach (var item in node.properties)
                {
                    PropertyGrid.RowDefinitions.Add(new RowDefinition());
                    Label l = new Label();
                    l.Content = item.Key;
                    l.SetValue(Grid.RowProperty, row);
                    l.SetValue(Grid.ColumnProperty, 0);
                    PropertyGrid.Children.Add(l);
                    TextBox tb = new TextBox();
                    tb.Text = item.Value.ToString();
                    tb.SetValue(Grid.RowProperty, row++);
                    tb.SetValue(Grid.ColumnProperty, 1);
                    PropertyGrid.Children.Add(tb);
                }
            }
        }

        private void MainWindow_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            Point p = e.GetPosition(this.MasterCanvas);
            if (mode == Mode.None)
            {
                // Check if point is in node
                foreach (var dispNode in dispGraph.dispNodes)
                {
                    if (dispNode.PointInObject(p.X, p.Y))
                    {
                        curNode = dispNode;
                        nodeOffsetX = p.X - dispNode.X;
                        nodeOffsetY = p.Y - dispNode.Y;
                        mode = Mode.DragNode;
                        UpdateProperties(curNode);
                        break;
                    }
                    else
                    {
                        UpdateProperties(null);
                        // Check if in connector ring
                        curConnector = InConnectorList(p, dispNode.inputs);
                        if (curConnector == null)
                            curConnector = InConnectorList(p, dispNode.outputs);
                        if (curConnector != null)
                        {
                            curNode = curConnector.Parent;
                            mode = Mode.LinkConnectors;
                            this.linkPts[0].X = p.X;
                            this.linkPts[0].Y = p.Y;
                            this.linkPts[1].X = p.X - 10.0;
                            this.linkPts[1].Y = p.Y;
                            this.linkPts[2].X = p.X - 10.0;
                            this.linkPts[2].Y = p.Y;
                            this.linkPts[3].X = p.X - 10.0;
                            this.linkPts[3].Y = p.Y;
                            this.linkPath.Visibility = Visibility.Visible;
                            break;
                        }
                    }
                }
            }
        }

        private void CropClick(object sender, RoutedEventArgs e)
        {
            Node_Crop cropnode = new Node_Crop(this.MasterCanvas);
            dispGraph.dispNodes.Add(cropnode);
        }

        private void WebCamClick(object sender, RoutedEventArgs e)
        {
            Node_WebCam webnode = new Node_WebCam(this.MasterCanvas);
            dispGraph.dispNodes.Add(webnode);
        }

        private void DisplayImageClick(object sender, RoutedEventArgs e)
        {
            Node_DisplayImage dispImage = new Node_DisplayImage(this.MasterCanvas);
            dispGraph.dispNodes.Add(dispImage);
        }

        private void GenerateCodeClick(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.SaveFileDialog dlg = new Microsoft.Win32.SaveFileDialog();
            if (dlg.ShowDialog() == true)
            {
                System.IO.FileStream fs = new System.IO.FileStream(dlg.FileName, System.IO.FileMode.Create);
                System.IO.StreamWriter strm = new System.IO.StreamWriter(fs);
                strm.WriteIndent("// Copyright (c) Microsoft Corporation. All rights reserved.", 0);
                strm.WriteIndent("// Licensed under the MIT license.", 0);
                strm.WriteIndent("using System;", 0);
                strm.WriteIndent("using System.Collections.Generic;", 0);
                strm.WriteIndent("using System.ComponentModel;", 0);
                strm.WriteIndent("using System.IO;", 0);
                strm.WriteIndent("using Microsoft.Psi;", 0);
                strm.WriteIndent("using Microsoft.Psi.Imaging;", 0);
                strm.WriteIndent("using Microsoft.Psi.Visualization.Common;", 0);
                strm.WriteIndent("public partial class MainWindow : Window, INotifyPropertyChanged", 0);
                strm.WriteIndent("{", 0);
                strm.WriteIndent("private Pipeline pipeline;", 4);
                for (int i = 0; i < this.dispGraph.dispNodes.Count; i++)
                {
                    this.dispGraph.dispNodes[i].GenerateVariableDefinitions(strm, 4);
                }
                strm.WriteIndent("", 0);
                strm.WriteIndent("public MainWindow()", 4);
                strm.WriteIndent("{", 4);
                strm.WriteIndent("    this.DataContext = this;", 4);
                strm.WriteIndent("    InitializeComponent();", 4);
                strm.WriteIndent("    SetupPsi();", 4);
                strm.WriteIndent("}", 4);
                strm.WriteIndent("", 0);
                strm.WriteIndent("public event PropertyChangedEventHandler PropertyChanged;", 4);
                strm.WriteIndent("public void RaisePropertyChanged(string propertyName)", 4);
                strm.WriteIndent("{", 4);
                strm.WriteIndent("    if (PropertyChanged != null)", 4);
                strm.WriteIndent("        PropertyChanged(this, new PropertyChangedEventArgs(propertyName));", 4);
                strm.WriteIndent("}", 4);
                strm.WriteIndent("", 0);
                strm.WriteIndent("private void SetupPsi()", 4);
                strm.WriteIndent("{", 4);
                strm.WriteIndent("    this.pipeline = Microsoft.Psi.Pipeline.Create(\"PsiPipeline\", DeliveryPolicy.LatestMessage);", 4);
                for (int i = 0; i < this.dispGraph.dispNodes.Count; i++)
                {
                    this.dispGraph.dispNodes[i].GenerateSetup(strm, 8);
                }
                strm.WriteIndent("    this.pipeline.RunAsync();", 4);
                strm.WriteIndent("}", 4);
                strm.WriteIndent("}", 0);
                strm.Flush();
                fs.Close();
            }
        }
    }
}
