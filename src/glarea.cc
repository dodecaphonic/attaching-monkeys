#include "glarea.h"

GLArea::GLArea() : Gtk::DrawingArea(), realized_(false)
{
        setup_gl_capability();
        add_events(Gdk::BUTTON_PRESS_MASK | Gdk::POINTER_MOTION_MASK);
        signal_button_press_event().connect(sigc::mem_fun(*this, &GLArea::on_button_clicked));
        signal_motion_notify_event().connect(sigc::mem_fun(*this, &GLArea::on_move_event));
}

bool
GLArea::on_expose_event(GdkEventExpose *e)
{
        printf("Exposing\n");
        GdkGLContext *glcontext = gtk_widget_get_gl_context(GTK_WIDGET(this->gobj()));
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(GTK_WIDGET(this->gobj()));
        if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	{
                printf("Merda *fodida*\n");
	}
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gdk_gl_drawable_gl_end(gldrawable);

        Cairo::RefPtr<Cairo::Context> ctx = get_window()->create_cairo_context();
        ctx->set_source_rgb(0.2, 1.0, 0.73);
        ctx->rectangle(0, 0, 640, 480);
        ctx->fill();
        return true;
}

bool
GLArea::on_configure_event(GdkEventConfigure *e)
{
        printf("Configuring. Realized? %d\n", realized_);
        if(!realized_) {
                on_realize_event();
        }
        GdkGLContext *glcontext = gtk_widget_get_gl_context(GTK_WIDGET(this->gobj()));
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(GTK_WIDGET(this->gobj()));
        if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	{
                printf("Merda *fodida*\n");
	}
        glClearColor(0.0, 0.0, 1.0, 1.0);
	glViewport (0, 0,
                    GTK_WIDGET(this->gobj())->allocation.width,
                    GTK_WIDGET(this->gobj())->allocation.height);
        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (-10,10,-10,10,-1,1);
        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity();
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glScalef (10., 10., 10.);
        
        gdk_gl_drawable_gl_end(gldrawable);
        return true;
}

void
GLArea::on_realize_event()
{
        printf("Realizing\n");
        GdkGLContext *glcontext = gtk_widget_get_gl_context(GTK_WIDGET(this->gobj()));
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(GTK_WIDGET(this->gobj()));
        if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	{
                printf("Merda *fodida*\n");
	}
        // glEnable(GL_CULL_FACE);
        // glEnable(GL_DEPTH_TEST);
        // glEnable(GL_NORMALIZE);        
        int width, height;
        width = GTK_WIDGET(this->gobj())->allocation.width;
        height = GTK_WIDGET(this->gobj())->allocation.height;
        printf("width: %d, height: %d\n", width, height);
        printf("GL_RENDERER = %s\n", (char*) glGetString(GL_RENDERER));
        glClearColor(0.0, 0.0, 1.0, 1.0);        
	glViewport (0, 0,
                    width, height);
        glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (-10,10,-10,10,-1,1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gdk_gl_drawable_gl_end(gldrawable);        
        realized_ = true;
}

void
GLArea::setup_gl_capability()
{
        GdkGLConfigMode mode;
        mode = static_cast<GdkGLConfigMode>(GDK_GL_MODE_RGBA |
                                            GDK_GL_MODE_DEPTH |
                                            GDK_GL_MODE_DOUBLE);
        glconfig_ = gdk_gl_config_new_by_mode(mode);
        if(!glconfig_) {
                printf("Deu merda\n");
        }
        gtk_widget_set_gl_capability(GTK_WIDGET(this->gobj()), glconfig_,
                                     NULL, TRUE, GDK_GL_RGBA_TYPE);
        // glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT |
        //              GL_POINT_BIT | GL_LINE_BIT);
        // glEnable(GL_POINT_SMOOTH);

}

bool
GLArea::on_button_clicked(GdkEventButton *e)
{
        if(bridge_) {
                bridge_->notify_click_event(e->x, e->y);
        }
        return true;
}

bool
GLArea::on_move_event(GdkEventMotion *e)
{
        if(bridge_) {
                bridge_->notify_mouse_move_event(e->x, e->y);
        }
        return true;
}

void
GLArea::set_bridge(Bridge *b)
{
        bridge_ = b;
}
