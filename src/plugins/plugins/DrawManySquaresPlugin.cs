using System;

namespace AmazingPlugins
{
    class DrawManySquaresPlugin : IMouseClickPlugin
    {
        public readonly DrawingBridge bridge_;

        public DrawManySquaresPlugin(DrawingBridge bridge)
        {
            bridge_ = bridge;
        }

        public void ReceiveEvent(IDomain domain, object[] data)
        {
            Random r = new Random();
            double x, y;
            int nSqs = r.Next(100, 10000);
            for(int i = 0; i < nSqs; i++)
            {
                x = r.Next(0, 640);
                y = r.Next(0, 480);
                int sz = r.Next(0, 10);
                bridge_.DrawSquare(x, y, sz);
            }
        }


        public string Name
        {
            get { return "Draw *many* squares plugin"; }
        }
    }
}
