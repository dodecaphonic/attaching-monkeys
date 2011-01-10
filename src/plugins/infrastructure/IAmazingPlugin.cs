namespace AmazingPlugins
{
    public interface IAmazingPlugin
    {
        string Name { get; }
        void ReceiveEvent(IDomain domain, object[] data);
    }

    public interface IMouseMovePlugin : IAmazingPlugin
    {
    }

    public interface IMouseClickPlugin : IAmazingPlugin
    {
    }

    public interface IGLPlugin : IAmazingPlugin
    {
        void Configure();
    }
}
