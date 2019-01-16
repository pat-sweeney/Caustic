using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;
using System.Windows.Media.Effects;
using System.Windows.Media;
using System.IO;
using PsiEditor;

namespace PsiEditor
{
    public class Node_DisplayImage : DispNode
    {
        public Node_DisplayImage(Canvas parentCanvas) : base("DisplayImage", parentCanvas)
        {
            inputs.Add(new DispConnector(this, "Input", parentCanvas, true));
            base.CreateVisuals();
        }

        public override void GenerateVariableDefinitions(System.IO.StreamWriter strm, int indent)
        {
            strm.WriteIndent("private DisplayImage dispImage = new DisplayImage();", indent);
            strm.WriteIndent("public WriteableBitmap DispImage { get { return this.dispImage?.Image;}}", indent);
        }

        public override void GenerateCode(System.IO.StreamWriter strm, int indent)
        {
            strm.WriteLine("Do((img)=>{this.DispImage.UpdateImage(img);});");
        }
    }

    public class Node_WebCam : DispNode
    {
        public Node_WebCam(Canvas parentCanvas) : base("WebCam", parentCanvas)
        {
            outputs.Add(new DispConnector(this, "Output", parentCanvas, false));
            base.CreateVisuals();
            properties.Add("ResolutionWidth", 1920);
            properties.Add("ResolutionHeight", 1080);
            properties.Add("ResolutionFrameRateNumerator", 30);
            properties.Add("ResolutionFrameRateDenominator", 1);
            properties.Add("DeviceId", 0);
        }

        public override void GenerateVariableDefinitions(System.IO.StreamWriter strm, int indent)
        {
            strm.WriteIndent("private Microsoft.Psi.Media.MediaCaptureConfiguration psiWebCamConfig;", indent);
            strm.WriteIndent("private Microsoft.Psi.Media.MediaCapture psiWebCam;", indent);
        }

        public override void GenerateSetup(StreamWriter strm, int indent)
        {
            base.GenerateSetup(strm, indent);
            strm.WriteIndent("this.psiWebCamConfig = new Microsoft.Psi.Media.MediaCaptureConfiguration();", indent);
            strm.WriteIndent($"this.psiWebCamConfig.Width = {this.properties["ResolutionWidth"]};", indent);
            strm.WriteIndent($"this.psiWebCamConfig.Height = {this.properties["ResolutionHeight"]};", indent);
            int numerator = (int)this.properties["ResolutionFrameRateNumerator"];
            int denominator = (int)this.properties["ResolutionFrameRateDenominator"];
            double frameRate = (double)numerator / denominator;
            strm.WriteIndent($"this.psiWebCamConfig.Framerate = {frameRate};", indent);
            strm.WriteIndent($"this.psiWebCamConfig.DeviceId = {this.properties["DeviceId"]};", indent);
            strm.WriteIndent("this.psiWebCam = new Microsoft.Psi.Media.MediaCapture(this.pipeline, this.psiWebCamConfig);", indent);
            for (int i= 0; i < this.outputs.Count; i++)
            {
                for (int j = 0; j < this.outputs.Count; j++)
                {
                    strm.WriteIndent($"this.psiWebCam.{this.outputs[i].Name}.", indent, false);
                    this.outputs[i].Connections[j].Parent.GenerateCode(strm, indent);
                }
            }
        }
    }

    public class Node_Do : DispNode
    {
        public Node_Do(Canvas parentCanvas) : base("Do", parentCanvas)
        {
            inputs.Add(new DispConnector(this, "Input", parentCanvas, true));
            outputs.Add(new DispConnector(this, "Output", parentCanvas, false));
        }

        public override void GenerateVariableDefinitions(System.IO.StreamWriter strm, int indent)
        {
        }

        public override void GenerateSetup(System.IO.StreamWriter strm, int indent)
        {
        }

        public override void GenerateCode(System.IO.StreamWriter strm, int indent)
        {
            strm.Write("Do((x)=>{Console.WriteLine(\"Executing Do!\")});");
        }
    }
    public class Node_Crop : DispNode
    {
        public Node_Crop(Canvas parentCanvas) : base("Crop", parentCanvas)
        {
            this.properties.Add("X", 0.0);
            this.properties.Add("Y", 0.0);
            this.properties.Add("Width", 100.0);
            this.properties.Add("Height", 100.0);
            inputs.Add(new DispConnector(this, "Input", parentCanvas, true));
            outputs.Add(new DispConnector(this, "Output", parentCanvas, false));
            base.CreateVisuals();
        }

        public override void GenerateVariableDefinitions(System.IO.StreamWriter strm, int indent)
        {
        }

        public override void GenerateSetup(System.IO.StreamWriter strm, int indent)
        {
        }

        public override void GenerateCode(System.IO.StreamWriter strm, int indent)
        {
            strm.WriteLine("Process<Shared<Image>, (Shared<Image>, System.Drawing.Rectangle)>(");
            indent += 4;
            strm.WriteIndent("(d, e, s) =>", indent);
            strm.WriteIndent("{", indent);
            strm.WriteIndent("    System.Drawing.Rectangle rect = default(System.Drawing.Rectangle);", indent);
            strm.WriteIndent($"   rect.X = {properties["X"]}", indent);
            strm.WriteIndent($"   rect.Y = {properties["Y"]}", indent);
            strm.WriteIndent($"   rect.Width = {properties["Width"]}", indent);
            strm.WriteIndent($"   rect.Height = {properties["Height"]}", indent);
            strm.WriteIndent("    if (rect.Width > 0 && rect.Height > 0)", indent);
            strm.WriteIndent("    {", indent);
            strm.WriteIndent("        s.Post((d, rect), e.OriginatingTime);", indent);
            strm.WriteIndent("    }", indent);
            strm.WriteIndent("}", indent);
            strm.WriteIndent(").Crop()", indent);
        }
    }
}
