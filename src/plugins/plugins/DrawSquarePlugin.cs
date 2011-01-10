namespace AmazingPlugins
{
    class DrawSquarePlugin : IMouseClickPlugin
    {
        private readonly DrawingBridge bridge_;
        public DrawSquarePlugin(DrawingBridge bridge) {
            bridge_ = bridge;
        }

        public string Name
        {
            get { return "Amazing drawer of squares"; }
        }

        public void ReceiveEvent(IDomain domain, object[] data)
        {
            double x = (double)data[0];
            double y = (double)data[1];
            bridge_.DrawSquare(x - 2, y - 2, 16);
        }
    }
}
