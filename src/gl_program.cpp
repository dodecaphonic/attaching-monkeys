#include "gl_win.hpp"
#define RC_FILENAME "./gtkrc"

GdkGLConfig * createGLConfig()
{
  GdkGLConfig *glconfig;
  GdkGLConfigMode mode;

  mode = static_cast <GdkGLConfigMode>
    (GDK_GL_MODE_RGBA | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE);
  glconfig = gdk_gl_config_new_by_mode(mode);
  if (glconfig == NULL)
  {
    //fprintf(stderr, "Não foi possível iniciar o modo Double Buffer.");
    exit(1);
  }
  return glconfig;
}

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  gtk_rc_add_default_file(RC_FILENAME);
  gtk_rc_reparse_all();
  gtk_gl_init(&argc, &argv);

  GdkGLConfig *glconfig = createGLConfig();
  OpenGLWindow win(glconfig, NULL);
  Gtk::Main::run(win);
  
  return EXIT_SUCCESS;
}

#ifdef _MSC_VER
int _tmain(int argc, char* argv[])
{
  return main(argc, argv);
}
#endif

