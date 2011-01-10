#include "gl_win.hpp"

OpenGLWindow::OpenGLWindow(GdkGLConfig * glconfig, GdkGLContext *context)
{
  drawn_ = false;
  const int INITIAL_WIDTH  = 800;
  const int INITIAL_HEIGHT = 600;
  this->set_title("OpenGL Window");
  Gtk::HBox *hbox = Gtk::manage(new Gtk::HBox(false, 0));
  this->add(*hbox);
  hbox->show();

  m_drawingArea = Gtk::manage(new Gtk::DrawingArea());
  m_drawingArea->set_size_request(INITIAL_WIDTH, INITIAL_HEIGHT);
  bridge_ = new Bridge(m_drawingArea);

  gtk_widget_set_gl_capability(GTK_WIDGET(m_drawingArea->gobj()),
                                glconfig, context, TRUE, GDK_GL_RGBA_TYPE);
  m_drawingArea->add_events(
    static_cast<Gdk::EventMask>
   (Gdk::POINTER_MOTION_MASK |
     Gdk::KEY_PRESS           |
     Gdk::KEY_RELEASE         |
     Gdk::BUTTON_MOTION_MASK  |
     Gdk::BUTTON_PRESS_MASK   |
     Gdk::BUTTON_RELEASE_MASK |
     Gdk::ENTER_NOTIFY_MASK   |
     Gdk::LEAVE_NOTIFY_MASK   |
     Gdk::VISIBILITY_NOTIFY_MASK |
     Gdk::SCROLL_MASK));

  // "realize"
  m_drawingArea->signal_realize().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_on_realize));

  // "configure-event"
  m_configureConnection = m_drawingArea->signal_configure_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_on_configure_event));
  
  m_configureConnection.block();

  // "expose-event"
  m_drawingArea->signal_expose_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_on_expose_event));

  // "unrealize"
  m_drawingArea->signal_unrealize().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_on_unrealize));

    // "motion-notify-event"
  m_drawingArea->signal_motion_notify_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_MotionNotifyEvent));

  // "button-press-event"
  m_drawingArea->signal_button_press_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_ButtonPressEvent));

  // "button-release-event",
  m_drawingArea->signal_button_release_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_ButtonReleaseEvent));

  // "scroll-event"
  m_drawingArea->signal_scroll_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_MouseScrollEvent));

  // "key-press-event"
  this->signal_key_press_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_KeyPressEvent));

  // "key-release-event"
  this->signal_key_release_event().connect(
    sigc::mem_fun(*this, &OpenGLWindow::da_KeyReleaseEvent));

  hbox->pack_start(*m_drawingArea, true, true, 0);
  m_drawingArea->show();
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::update_canvas()
{
  m_drawingArea->get_window()->invalidate_rect(
    m_drawingArea->get_allocation(), false);
}

void OpenGLWindow::da_on_realize()
{
  GtkWidget * widget = GTK_WIDGET(m_drawingArea->gobj());
  GdkGLContext  *glContext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable(widget);

  GLsizei Canvasw = widget->allocation.width;
  GLsizei Canvash = widget->allocation.height;
  
  printf("Configuring on realize\n");
  bridge_->configure_gl_plugin();

  if (!gdk_gl_drawable_gl_begin(glDrawable, glContext))
  {
    gtk_main_quit();
    exit(0);
  }

  gdk_gl_drawable_gl_end(glDrawable);

  m_configureConnection.unblock();
}

bool OpenGLWindow::da_on_configure_event(GdkEventConfigure * event)
{
  GtkWidget * widget = GTK_WIDGET(m_drawingArea->gobj());
  GdkGLContext *glContext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable(widget);
  
  GLsizei w = (GLsizei)widget->allocation.width;
  GLsizei h = (GLsizei)widget->allocation.height;

  if (!gdk_gl_drawable_gl_begin(glDrawable, glContext))
    return false;

  //m_camera.Resize(w, h);
  bridge_->configure_gl_plugin();
  printf("Configuring on the C side\n");
  
  ////////////////////////////
  glClearColor(0.0f, 0.0f, 0.0f, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1, 1, -1, 1, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glViewport(0, 0, w, h);
  ////////////////////////////

  gdk_gl_drawable_gl_end(glDrawable);

  return true;
}

bool OpenGLWindow::da_on_expose_event(GdkEventExpose * event)
{
  GtkWidget * widget = GTK_WIDGET(m_drawingArea->gobj());
  GdkGLContext *glContext = gtk_widget_get_gl_context(widget);
  GdkGLDrawable *glDrawable = gtk_widget_get_gl_drawable(widget);

  if (!gdk_gl_drawable_gl_begin(glDrawable, glContext))
    return false;

  if(!drawn_) {
          glClearColor(0.46f, 0.62f, 0.95f, 0.0);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          glEnable(GL_DEPTH_TEST);
          drawn_ = true;
  }

  glColor3d(0, 0, 0.4);
  gdk_gl_draw_teapot(FALSE, 0.5);
  glColor3d(1, 0, 0);
  glLineWidth(2);
  glBegin(GL_LINE_LOOP);
  glVertex2f(-0.5, -0.5);
  glVertex2f(-0.5, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, -0.5);
  glEnd();
   
  gdk_gl_drawable_swap_buffers(glDrawable);
  gdk_gl_drawable_gl_end(glDrawable);

  //this->update_canvas();

  return true;
}

void OpenGLWindow::da_on_unrealize()
{
  // Pode-se usar para fazer a limpeza de dados
  // que tenham sido alocados em OpenGLWindow
}

bool OpenGLWindow::da_on_visibility_notify_event(GdkEventVisibility * event)
{
  return true;
}

bool
OpenGLWindow::da_MotionNotifyEvent(GdkEventMotion * event)
{
  int redraw = 0;

  if (event->x < 0.0 || event->y < 0.0)
    return false;

  if (event->state == GDK_BUTTON1_MASK)
  {
  }
  else 
  if (event->state == GDK_BUTTON2_MASK)
  {
  }
  else 
  if (event->state == GDK_BUTTON3_MASK)
  {
  }
  else 
  if (event->state ==(GDK_BUTTON1_MASK | GDK_SHIFT_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON2_MASK | GDK_SHIFT_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON3_MASK | GDK_SHIFT_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON1_MASK | GDK_CONTROL_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON2_MASK | GDK_CONTROL_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON3_MASK | GDK_CONTROL_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON1_MASK | GDK_MOD1_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON2_MASK | GDK_MOD1_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON3_MASK | GDK_MOD1_MASK))
  {
  }
  else 
  if (event->state == GDK_SHIFT_MASK)
  {
  }
  else 
  if (event->state == GDK_CONTROL_MASK)
  {
  }
  else 
  if (event->state == GDK_MOD1_MASK)
  {
  }
  else
  {
  }

  if (redraw)
    this->update_canvas();

  bridge_->notify_mouse_move_event(event->x, event->y);

  return true;
}

bool
OpenGLWindow::da_ButtonPressEvent(GdkEventButton * event)
{
  int redraw = 0;
  
  if (event->x < 0.0 || event->y < 0.0)
    return false;

  if (event->button == 1 && !event->state)
  {
  }
  else 
  if (event->button == 2 && !event->state)
  {
  }
  else 
  if (event->button == 3 && !event->state)
  {
  }
  else 
  if ((event->button == 1) &&(event->state == GDK_SHIFT_MASK))
  {
  }
  else 
  if ((event->button == 2) &&(event->state == GDK_SHIFT_MASK))
  {
  }
  else 
  if ((event->button == 3) &&(event->state == GDK_SHIFT_MASK))
  {
  }
  else 
  if ((event->button == 1) &&(event->state == GDK_CONTROL_MASK))
  {
  }
  else 
  if ((event->button == 2) &&(event->state == GDK_CONTROL_MASK))
  {
  }
  else 
  if ((event->button == 3) &&(event->state == GDK_CONTROL_MASK))
  {
  }
  else 
  if ((event->button == 1) &&(event->state == GDK_MOD1_MASK))
  {
  }
  else 
  if ((event->button == 2) &&(event->state == GDK_MOD1_MASK))
  {
  }
  else 
  if ((event->button == 3) &&(event->state == GDK_MOD1_MASK))
  {
  }
  else
    return false;

  bridge_->notify_click_event(event->x, event->y);
  if (redraw)
    this->update_canvas();
  return false;
}

bool
OpenGLWindow::da_ButtonReleaseEvent(GdkEventButton * event)
{
  int redraw = 0;

  if (event->x < 0.0 || event->y < 0.0)
    return false;

  if (event->state == GDK_BUTTON1_MASK)
  {
  }
  else 
  if (event->state == GDK_BUTTON2_MASK)
  {
  }
  else 
  if (event->state == GDK_BUTTON3_MASK)
  {
  }
  else 
  if (event->state ==(GDK_BUTTON1_MASK | GDK_SHIFT_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON2_MASK | GDK_SHIFT_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON3_MASK | GDK_SHIFT_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON1_MASK | GDK_CONTROL_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON2_MASK | GDK_CONTROL_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON3_MASK | GDK_CONTROL_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON1_MASK | GDK_MOD1_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON2_MASK | GDK_MOD1_MASK))
  {
  }
  else 
  if (event->state ==(GDK_BUTTON3_MASK | GDK_MOD1_MASK))
  {
  }
  else
    return false;

  if (redraw)
    this->update_canvas();
  return false;
}

bool
OpenGLWindow::da_MouseScrollEvent(GdkEventScroll * event)
{
  int redraw = 0;
  
  if (event->direction == GDK_SCROLL_UP)
  {
  }
  else 
  if (event->direction == GDK_SCROLL_DOWN)
  {
  }
  else
    return false;
  
  if (redraw)
    this->update_canvas();
  return true;
}

bool
OpenGLWindow::da_KeyPressEvent(GdkEventKey * event)
{
  int redraw = 0;

  switch(event->keyval)
  {
  case GDK_Control_L:
  case GDK_Control_R:
    
    break;
  case GDK_Shift_L:
  case GDK_Shift_R:

    break;
  case GDK_KP_Space:
    
    break;
  case GDK_Return:
    
    break;
  case GDK_Insert:
    
    break;
  case GDK_Delete:
    
    break;
  case GDK_Home:
    
    break;
  case GDK_End:
    
    break;
  case GDK_Page_Up:
    
    break;
  case GDK_Page_Down:
    
    break;
  case GDK_Left:
    
    break;
  case GDK_Down:
    
    break;
  case GDK_Up:
    
    break;
  case GDK_Right:
    
    break;
  case GDK_F1:
    break;
  case GDK_F2:
    break;
  case GDK_F4:
    
    break;
  case GDK_F5:
    
    break;
  case GDK_F6:
    
    break;
  case GDK_F7:
    
    break;
  case GDK_F8:
    
    break;
  case GDK_F9:
    
    break;
  case GDK_F10:
    
    break;
  case GDK_F11:
    
    break;
  case GDK_F12:
    
    break;
  default:
    return false;
  }

  if (redraw)
    this->update_canvas();

  return true;
}

bool
OpenGLWindow::da_KeyReleaseEvent(GdkEventKey * event)
{
  int redraw = 0;

  switch(event->keyval)
  {
  case GDK_Control_L:
  case GDK_Control_R:
    
    break;
  case GDK_Shift_L:
  case GDK_Shift_R:
    
    break;
  case GDK_KP_Space:
    
    break;
  case GDK_Return:
    
    break;
  case GDK_Insert:
    
    break;
  case GDK_Delete:
    
    break;
  case GDK_Home:
    
    break;
  case GDK_End:
    
    break;
  case GDK_Page_Up:
    
    break;
  case GDK_Page_Down:
    
    break;
  case GDK_Left:
    
    break;
  case GDK_Down:
    
    break;
  case GDK_Up:
    
    break;
  case GDK_Right:
    
    break;
  case GDK_F1:
    
    break;
  case GDK_F2:
    
    break;
  case GDK_F4:
    
    break;
  case GDK_F5:
    
    break;
  case GDK_F6:
    
    break;
  case GDK_F7:
    
    break;
  case GDK_F8:
    
    break;
  case GDK_F9:
    
    break;
  case GDK_F10:
    
    break;
  case GDK_F11:
    
    break;
  case GDK_F12:
    
    break;
  default:
    return false;
  }

  if (redraw)
    this->update_canvas();
  return true;
}

