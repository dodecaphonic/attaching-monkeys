using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace AmazingPlugins
{
    public class PluginManager
    {
        private readonly IList<IAmazingPlugin> plugins_;
        private readonly IDomain domain_;
        private readonly DrawingBridge  bridge_;

        public PluginManager(string loadPath, IntPtr bridge)
        {
            domain_  = new Domain();
            bridge_  = new DrawingBridge(bridge);
            plugins_ = new PluginLoader(loadPath, bridge_).LoadPlugins();
        }

        public void NotifyPlugins()
        {
        }

        public void ConfigureGLPlugins()
        {
            Console.WriteLine("Plugging GLutties");
            foreach(IAmazingPlugin plugin in plugins_)
            {
                if(plugin is IGLPlugin) {
                    IGLPlugin gl = (IGLPlugin)plugin;
                    gl.Configure();
                }
            }
        }

        public void NotifyPlugins(string eventType, params object[] data)
        {
            // We can use eventType to determine stuff, such as
            // "BoundingBoxChangedEvent", "MethodCallEvent", 
            // et cetera.
            // Re: method calls, *maybe* we can return those
            // as executable thunks directly for consumption
            // in the C++ side.
            foreach(IAmazingPlugin plugin in plugins_) {
                if(eventType == "MouseMoveEvent" &&
                   plugin is IMouseMovePlugin) {
                    plugin.ReceiveEvent(domain_, data);
                } else if(eventType == "MouseClickEvent" &&
                          plugin is IMouseClickPlugin) {
                    plugin.ReceiveEvent(domain_, data);
                }
            }
        }
    }
}
