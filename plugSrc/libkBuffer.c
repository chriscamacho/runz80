
#include "../plugin.h"
#include <stdlib.h>
#include <gtk/gtk.h>

#include <gdk/gdk.h>


// a 1KB framebuffer graphics device (monochrome! oh the humanity :o ) 128x64px

typedef struct {
    GtkWidget* parent;
    GtkWidget *drawing_area;
    byte* buffer;
} kBufferVars;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

gboolean
draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    guint width, height;
    GdkRGBA color;


    for (int y=0;y<64;y++) {
        for (int x=0;x<128;x++) {
            int a=y*16; // 128/8
            a=a+((int)(x/8));
            int b=x % 8;

            if (CHECK_BIT(((kBufferVars*)data)->buffer[a],b))
                            cairo_rectangle (cr, x*4, y*4, 3, 3);
        }
    }
    //gtk_style_context_get_color (gtk_widget_get_style_context (widget),
    //           0, &color);
    color.red = 0.2;
    color.green = 1.0;
    color.blue = 0.2;
    color.alpha = 1;
    
    gdk_cairo_set_source_rgba (cr, &color);

    cairo_fill (cr);

    for (int y=0;y<64;y++) {
        for (int x=0;x<128;x++) {
            int a=y*16; // 128/8
            a=a+((int)(x/8));
            int b=x % 8;

            if (!CHECK_BIT(((kBufferVars*)data)->buffer[a],b))
                            cairo_rectangle (cr, x*4, y*4, 3, 3);
        }
    }
    color.red = 0.2;
    color.green = 0.6;
    color.blue = 0.2;
    color.alpha = 1;

    gdk_cairo_set_source_rgba (cr, &color);

    cairo_fill (cr);


    return FALSE;
}

G_MODULE_EXPORT void initialise(void* inst, GtkWidget* parent) {

    plugInstStruct* pl = (plugInstStruct*)inst;

    pl->data = malloc(sizeof(kBufferVars));
// the plugins GUI
    kBufferVars* vars = ((kBufferVars*)pl->data);
    vars->buffer = malloc(1024);

    char* p = vars->buffer;
    for (int i=0;i<1024;i++) p[i]=0;
    
    vars->parent = parent;

    vars->drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (vars->drawing_area, 512, 256);

    gtk_container_add ( (GtkContainer*)vars->parent, vars->drawing_area);
                     
    g_signal_connect (G_OBJECT (vars->drawing_area), "draw",
                    G_CALLBACK (draw_callback), vars);
                    
}

G_MODULE_EXPORT int getPortSize() { return 1; }
G_MODULE_EXPORT int getAddressSize() { return 1024; }

G_MODULE_EXPORT byte getPort(void* inst, int port) { return 0xff; }
G_MODULE_EXPORT void setPort(void* inst, int port, byte val) {
    plugInstStruct* pl = (plugInstStruct*)inst;
    kBufferVars* vars = ((kBufferVars*)pl->data);
    gtk_widget_queue_draw_area (vars->drawing_area, 0, 0, 512, 256);
}


G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {
    plugInstStruct* pl = (plugInstStruct*)inst;
    kBufferVars* vars = ((kBufferVars*)pl->data);
    int a = address - pl->addressStart;
    a = a & 1023;
    vars->buffer[a]=data;
    
}

