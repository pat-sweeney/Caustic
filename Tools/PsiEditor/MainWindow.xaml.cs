using System.Windows;
using System.Windows.Controls;
using System.Collections.Generic;

namespace PsiEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Graph graph;
        private DispGraph dispGraph;

        public void BuildGraph()
        {
            graph = new Graph();
            graph.nodes = new List<Node>();
            Node node = new Node("Join");
            InputConnector ic = new InputConnector(node, "Input");
            node.inputs.Add(ic);
            OutputConnector oc = new OutputConnector(node, "Output");
            node.outputs.Add(oc);
            graph.nodes.Add(node);

            node = new Node("Do");
            ic = new InputConnector(node, "Input");
            node.inputs.Add(ic);
            ic = new InputConnector(node, "Opacity");
            node.inputs.Add(ic);
            oc = new OutputConnector(node, "Output");
            node.outputs.Add(oc);
            graph.nodes.Add(node);
        }

        public void BuildDispGraph()
        {
            this.dispGraph = new DispGraph(this.ParentCanvas);
            for (int i = 0; i < graph.nodes.Count; i++)
            {
                var dispNode = new DispNode(graph.nodes[i], this.ParentCanvas);
                dispGraph.dispNodes.Add(dispNode);
            }
            dispGraph.Layout(0.0, 0.0);
        }

        public MainWindow()
        {
            this.DataContext = this;
            InitializeComponent();
            BuildGraph();
            BuildDispGraph();
            this.MouseDown += MainWindow_MouseDown;
            this.MouseUp += MainWindow_MouseUp;
            this.MouseMove += MainWindow_MouseMove;
        }

        public enum Mode
        {
            None,
            DragNode,
            LinkConnectors
        }
        private Mode mode = Mode.None;
        private DispNode curNode = null;
        private double nodeOffsetX = 0.0;
        private double nodeOffsetY = 0.0;

        private void MainWindow_MouseMove(object sender, System.Windows.Input.MouseEventArgs e)
        {
            if (mode == Mode.DragNode)
            {
                Point p = e.GetPosition(this.ParentCanvas);
                curNode.X = p.X - nodeOffsetX;
                curNode.Y = p.Y - nodeOffsetY;
            }
        }

        private void MainWindow_MouseUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (mode == Mode.DragNode)
            {
                Point p = e.GetPosition(this.ParentCanvas);
                curNode.X = p.X - nodeOffsetX;
                curNode.Y = p.Y - nodeOffsetY;
                curNode = null;
                mode = Mode.None;
            }
        }

        private void MainWindow_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            Point p = e.GetPosition(this.ParentCanvas);
            if (mode == Mode.None)
            {
                // Check if point is in node
                foreach (var dispNode in dispGraph.dispNodes)
                {
                    if (p.X >= dispNode.X && p.X < dispNode.X + dispNode.NodeWidth &&
                        p.Y >= dispNode.Y && p.Y < dispNode.Y + dispNode.NodeHeight)
                    {
                        // Check if in connector ring
                        for (int i = 0; i < dispNode.inputs.Count; i++)
                        {
                        }
                        curNode = dispNode;
                        nodeOffsetX = p.X - dispNode.X;
                        nodeOffsetY = p.Y - dispNode.Y;
                        mode = Mode.DragNode;
                    }
                }
            }
        }
    }
}
