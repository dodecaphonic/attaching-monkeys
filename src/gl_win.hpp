#ifndef _OPENGL_WINDOW_H_
#define _OPENGL_WINDOW_H_

//#include "disable_warns.hpp"
#include <gtkmm.h>
#include <gtk/gtkgl.h>
#ifdef G_OS_WIN32
#include <windows.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "bridge.h"

class OpenGLWindow : public Gtk::Window
{
private:

public:
  OpenGLWindow(GdkGLConfig * glconfig, GdkGLContext *context);
  ~OpenGLWindow();
protected:
  void da_on_realize();
  bool da_on_configure_event(GdkEventConfigure * event);
  bool da_on_expose_event(GdkEventExpose * event);
  void da_on_unrealize();

  bool da_on_visibility_notify_event(GdkEventVisibility * event);

  bool da_MotionNotifyEvent(GdkEventMotion * event);
  bool da_ButtonPressEvent(GdkEventButton * event);
  bool da_ButtonReleaseEvent(GdkEventButton * event);
  bool da_MouseScrollEvent(GdkEventScroll * event);
  bool da_KeyPressEvent(GdkEventKey * event);
  bool da_KeyReleaseEvent(GdkEventKey * event);

private:
  void update_canvas();
  Gtk::DrawingArea  *m_drawingArea;
  sigc::connection   m_configureConnection;
  Bridge *bridge_;
        bool drawn_;
};

#endif//_OPENGL_WINDOW_H_

