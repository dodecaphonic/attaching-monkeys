#include <gtkmm.h>
#include <gtk/gtkgl.h>
#include <GL/gl.h>
#include "bridge.h"

class GLArea : public Gtk::DrawingArea
{
private:
        GdkGLConfig *glconfig_;
        Bridge *bridge_;
        bool realized_;
        virtual bool on_expose_event(GdkEventExpose *e);
        virtual bool on_configure_event(GdkEventConfigure *e);
        virtual void on_realize_event();
        virtual bool on_button_clicked(GdkEventButton *e);
        virtual bool on_move_event(GdkEventMotion *e);        
        virtual void setup_gl_capability();

public:
        GLArea();
        void set_bridge(Bridge *b);
};
