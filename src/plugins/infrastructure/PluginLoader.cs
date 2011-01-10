using System;
using System.Reflection;
using System.Collections.Generic;
using System.IO;

namespace AmazingPlugins
{
    public class PluginLoader
    {
        private readonly string path_;
        private readonly DrawingBridge bridge_;

        public PluginLoader(string path, DrawingBridge bridge)
        {
            path_ = path;
            bridge_ = bridge;
        }

        public IList<IAmazingPlugin> LoadPlugins()
        {
            IList<IAmazingPlugin> plugins = new List<IAmazingPlugin>();
            DirectoryInfo di = new DirectoryInfo(path_);
            foreach(FileInfo file in di.GetFiles("*.dll")) {
                string plugName = file.ToString();
                Assembly asm = Assembly.LoadFile(plugName);
                object[] bridges = new object[1] { bridge_ };
                foreach(Type t in asm.GetTypes()) {
                    if(t.FindInterfaces(delegate(Type o, object crit) {
                                return (o.ToString() == crit.ToString());
                            }, (object)typeof(IAmazingPlugin)).Length != 0) {
                        IAmazingPlugin plugin = Activator.CreateInstance(t, bridges) as IAmazingPlugin;
                        Console.WriteLine("Registered {0}", plugin.Name);
                        plugins.Add(plugin);
                    }
                }
            }
            return plugins;
        }
    }
}
