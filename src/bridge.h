#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#include <gtkmm.h>
#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/appdomain.h>
#include <string>
#include <gtk/gtkgl.h>
#include <GL/gl.h>

class Bridge
{
private:
        MonoDomain *domain_;
        MonoAssembly *assembly_;
        MonoObject *manager_;
        MonoMethod *notifier;
        MonoMethod *config_;
        guint32    gchManager_;
        void create_plugin_manager(MonoDomain *domain,
                                   MonoAssembly *assembly,
                                   std::string plugin_path);
public:
        Gtk::DrawingArea *canvas_;
        Bridge(Gtk::DrawingArea *dw);
        ~Bridge();
        void notify_click_event(double x, double y);
        void notify_bounding_box_changed_event(double x0, double y0,
                                               double x1, double y1);
        void notify_mouse_move_event(double x, double y);
        void start_drawing_operation();
        void end_drawing_operation();
        void configure_gl_plugin();
};

extern "C" {
        void bridge_callback_handler(Bridge *b);
        void draw_square(void* ptr_bridge, double x, double y, int size);
        void start_drawing_operation(void *p);
        void end_drawing_operation(void *p);
}



#endif //__BRIDGE_H__
