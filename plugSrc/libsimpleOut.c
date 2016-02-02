
#include "../plugin.h"
#include <stdlib.h>
#include <gtk/gtk.h>


G_MODULE_EXPORT void initialise(void* inst, int portStart, int addressStart) {
    // first 2 ints of plugin instances MUST be portStart and addressStart
    plugInstStruct* pl = (plugInstStruct*)inst;

    // an array of pointers...
    pl->data = malloc(sizeof(void*)*4);
    int** p= (int**)pl->data;
    p[0] = malloc(sizeof(int));
    *p[0] = portStart;
    p[1] = malloc(sizeof(int));
    *p[1] = addressStart;

// the plugins GUI
    p[2] = malloc(sizeof(GtkWidget*));
    p[2] = (void*)gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_deletable ((GtkWindow*)p[2], FALSE);

    p[3] = malloc(sizeof(GtkWidget*));
    p[3] = (void*)gtk_label_new ("");


    gchar* l = g_strdup_printf ("Port %s = 0x%02x",(const gchar *)&pl->name,0); 
    gtk_label_set_text ((GtkLabel*)p[3],l);
    g_free(l);
    
    gtk_container_add (GTK_CONTAINER ((GtkWidget*)p[2]), (GtkWidget*)p[3]);
    gtk_widget_show_all ( (GtkWidget*)p[2] );

}

G_MODULE_EXPORT int getPortSize() { return 1; }
G_MODULE_EXPORT int getAddressSize() { return 0; }

G_MODULE_EXPORT byte getPort(void* inst, int port) { return 0xff; }
G_MODULE_EXPORT void setPort(void* inst, int port, byte val) {
    g_print("port 0x%02x (%s) set to 0x%02x\n", port, ((plugInstStruct*)inst)->name, val);
    gchar* l = g_strdup_printf ("Port %s = 0x%02x",((plugInstStruct*)inst)->name, val);
    void** p= (void**)((plugInstStruct*)inst)->data;
    gtk_label_set_text ((GtkLabel*)p[3],l);
    g_free(l);    
}

G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {  }
