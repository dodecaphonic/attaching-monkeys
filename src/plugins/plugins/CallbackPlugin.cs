namespace AmazingPlugins
{
    class CallbackPlugin : ICallbackPlugin
    {
        readonly DrawingBridge bridge_;

        public CallbackPlugin(DrawingBridge bridge)
        {
            bridge_ = bridge;
        }
        
        public string Name
        {
            get { return "Callback Plugin"; }
        }
    }
}
