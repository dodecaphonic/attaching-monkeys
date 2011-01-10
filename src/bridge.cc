#include "bridge.h"
#include <iostream>

Bridge::Bridge(Gtk::DrawingArea *dw) : canvas_(dw)
{
        domain_ = mono_jit_init("Plugins.dll");
        assembly_ = mono_domain_assembly_open(domain_, "Plugins.dll");
        if(!assembly_) {
                std::cout << "Fucked" << std::endl;
        }
        //mono_jit_exec(domain_, assembly_, 0, NULL);
        create_plugin_manager(domain_, assembly_, "plugins");
}

Bridge::~Bridge()
{
        mono_gchandle_free(gchManager_);
        mono_jit_cleanup(domain_);
}

void
Bridge::create_plugin_manager(MonoDomain *domain, MonoAssembly *assembly,
                              std::string plugin_path)
{
        MonoClass *klass;
        MonoImage *image;
        MonoMethod *ctor;
        MonoMethodDesc *ctor_desc, *notify_desc, *config_desc;
        MonoString *path;
        gpointer args[2], handle;
        handle = this;
        path = mono_string_new(domain, plugin_path.c_str());
        //        this_ptr = mono_value_box(domain_, mono_get_intptr_class(), handle);
        args[0] = path;
        args[1] = &handle;
        image = mono_assembly_get_image(assembly);
        klass = mono_class_from_name(image, "AmazingPlugins", "PluginManager");
        manager_ = mono_object_new(domain, klass);
        ctor_desc  =  mono_method_desc_new(":.ctor(string,intptr)", FALSE);
        ctor = mono_method_desc_search_in_class(ctor_desc, klass);

        mono_runtime_invoke(ctor, manager_, args, NULL);
        gchManager_ = mono_gchandle_new(manager_, TRUE);
        // notifier
        notify_desc = mono_method_desc_new(":NotifyPlugins(string,object[])",
                                           FALSE);
        notifier = mono_method_desc_search_in_class(notify_desc, klass);
        //notify = (void (*)(MonoString*, MonoArray*))mono_method_get_unmanaged_thunk(notifier);
        // configure
        config_desc = mono_method_desc_new(":ConfigureGLPlugins()", FALSE);
        config_ = mono_method_desc_search_in_class(config_desc, klass);
        mono_method_desc_free(ctor_desc);
        mono_method_desc_free(notify_desc);
        mono_method_desc_free(config_desc);
}

void
Bridge::configure_gl_plugin()
{
        mono_runtime_invoke(config_, manager_, NULL, NULL);
}

void
Bridge::notify_click_event(double x, double y)
{
        MonoObject *boxed_x, *boxed_y;
        MonoClass  *klass;
        gpointer args[2];
        MonoArray *params;
        params = mono_array_new(domain_, mono_get_object_class(), 2);
        klass = mono_get_double_class();
        boxed_x = mono_value_box(domain_, klass, &x);
        boxed_y = mono_value_box(domain_, klass, &y);
        mono_array_set(params, MonoObject*, 0, boxed_x);
        mono_array_set(params, MonoObject*, 1, boxed_y);
        args[0] = mono_string_new(domain_, "MouseClickEvent");
        args[1] = params;
        mono_runtime_invoke(notifier, manager_, args, NULL);
}

void
Bridge::notify_bounding_box_changed_event(double x0, double y0,
                                          double x1, double y1)
{
}

void
Bridge::notify_mouse_move_event(double x, double y)
{
        MonoObject *boxed_x, *boxed_y;
        MonoClass  *klass;
        gpointer args[2];
        MonoArray *params;
        params = mono_array_new(domain_, mono_get_object_class(), 2);
        klass = mono_get_double_class();
        boxed_x = mono_value_box(domain_, klass, &x);
        boxed_y = mono_value_box(domain_, klass, &y);
        mono_array_set(params, MonoObject*, 0, boxed_x);
        mono_array_set(params, MonoObject*, 1, boxed_y);
        args[0] = mono_string_new(domain_, "MouseMoveEvent");
        args[1] = params;
        mono_runtime_invoke(notifier, manager_, args, NULL);        
}

void Bridge::start_drawing_operation()
{
        // GdkGLContext *glcontext = gtk_widget_get_gl_context(GTK_WIDGET(canvas_->gobj()));
	// GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(GTK_WIDGET(canvas_->gobj()));
        // if(!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	// {
        //         printf("Fodeu, fodeu\n");
 	// }                
}

void Bridge::end_drawing_operation()
{
        canvas_->get_window()->invalidate_rect(canvas_->get_allocation(), false); 
	// GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(GTK_WIDGET(canvas_->gobj()));
        // gdk_gl_drawable_swap_buffers(gldrawable);
        // gdk_gl_drawable_gl_end(gldrawable);
}

void
bridge_callback_handler(Bridge *b, double x, double y, int size)
{
        Gtk::DrawingArea *canvas = b->canvas_;
        Cairo::RefPtr<Cairo::Context> ctx = canvas->get_window()->create_cairo_context();
        ctx->rectangle(x, y, size, size);
        ctx->set_source_rgb(0, 0, 1);
        ctx->fill();
        //canvas->get_window()->invalidate_rect(canvas->get_allocation(), false); 
}

void
draw_square(void *ptr_bridge, double x, double y, int size)
{
        Bridge *bridge = reinterpret_cast<Bridge*>(ptr_bridge);
        bridge_callback_handler(bridge, x, y, size);
}

void start_drawing_operation(void *p)
{
        Bridge *bridge = static_cast<Bridge*>(p);
        bridge->start_drawing_operation();
}

void end_drawing_operation(void *p)
{
        Bridge *bridge = static_cast<Bridge*>(p);
        bridge->end_drawing_operation();
}        
