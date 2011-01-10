#include <gtkmm.h>
#include "bridge.h"
#include <gtk/gtkgl.h>
#include <gtkmm/gl/init.h>
#include <GL/gl.h>
#include "glarea.h"

Gtk::DrawingArea *dw;

int
main()
{
        Gtk::Main morga(0, NULL);
        gtk_gl_init(0, NULL);        
        Gtk::Window win;
        Gtk::VBox box;
        GLArea gullie;
        Bridge b(&gullie);
        box.pack_start(gullie);
        gullie.set_bridge(&b);
        gullie.set_size_request(640, 480);
        win.set_reallocate_redraws(true);
        win.set_title("Morga");
        win.resize(640, 480);
        win.set_size_request(640, 480);
        win.add(box);
        win.show_all();
        
        Gtk::Main::run(win);
        return 0;
}

