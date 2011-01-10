require 'rake'

task :all => ["base", "plugins"]

task :base do
  cc = "g++"
  cflags = "-rdynamic -ggdb -o app -Isrc"
  src = "src/gl_program.cpp src/bridge.cc src/gl_win.cpp"
  libs = "`pkg-config --libs --cflags mono gtkglext-1.0 gtkmm-2.4`"
  puts "Compiling C infrastructure"
  `#{cc} #{cflags} #{src} #{libs}`
end

task :plugins => ["plugin:infrastructure", "plugin:plugins"]

namespace :plugin do
  gmcs = "gmcs"
  options = "-t:library"
  plugin_dir = "plugins"
  plugin_options = "-t:library `pkg-config --libs gtk-sharp-2.0` -r:bin/Tao.OpenGl.dll -r:Plugins.dll"

  task :infrastructure do
    files = Dir.glob("src/plugins/infrastructure/*.cs").join " "
    puts "Compiling Plugin infrastructure"
    `#{gmcs} #{options} -out:Plugins.dll #{files}`
  end

  task :plugins do
    Dir.glob("src/plugins/plugins/*.cs").each do |pl|
      filename = File.split(pl).last
      out = filename.sub ".cs", ".dll"
      puts "Compiling #{pl}"
      `#{gmcs} #{plugin_options} -out:#{File.join plugin_dir, out} #{pl}`
    end
  end
end
