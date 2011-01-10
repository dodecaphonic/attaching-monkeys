using System;
using Gtk;

namespace AmazingPlugins
{
    public class GtkPlugin : IMouseMovePlugin
    {
        private readonly Gtk.Window win_;
        private readonly Gtk.Label  xCoord_;
        private readonly Gtk.Label  yCoord_;
        private readonly DrawingBridge bridge_;

        public GtkPlugin(DrawingBridge bridge)
        {
            win_ = new Window("Amazing Gtk Plugin");
            win_.Resize(200, 200);
            xCoord_ = new Label();
            yCoord_ = new Label();
            VBox box = new VBox();
            box.PackStart(xCoord_);
            box.PackStart(yCoord_);
            box.ShowAll();
            win_.Add(box);
        }

        public void ReceiveEvent(IDomain domain, object[] data)
        {
            xCoord_.Text = data[0].ToString();
            yCoord_.Text = data[1].ToString();
            win_.Show();
        }
        
        public string Name
        {
            get { return "Watch out man, she'll use you up!"; }
        }
    }
}
