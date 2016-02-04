
#include "../plugin.h"
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct {
    GtkWindow* window;
    GtkLabel* label;
} simpleOutVars;


G_MODULE_EXPORT void initialise(void* inst) {

    plugInstStruct* pl = (plugInstStruct*)inst;

    pl->data = malloc(sizeof(simpleOutVars));

// the plugins GUI
    simpleOutVars* vars = ((simpleOutVars*)pl->data);
    vars->window = (GtkWindow*)gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_deletable (vars->window, FALSE);


    vars->label = (GtkLabel*)gtk_label_new ("");


    gchar* l = g_strdup_printf ("Port %s = 0x%02x",(const gchar *)&pl->name,0); 
    gtk_label_set_text (vars->label,l);
    g_free(l);
    
    gtk_container_add (GTK_CONTAINER (vars->window), (GtkWidget*)vars->label);
    gtk_widget_show_all ( (GtkWidget*)vars->window );

}

G_MODULE_EXPORT int getPortSize() { return 1; }
G_MODULE_EXPORT int getAddressSize() { return 0; }

G_MODULE_EXPORT byte getPort(void* inst, int port) { return 0xff; }
G_MODULE_EXPORT void setPort(void* inst, int port, byte val) {
    //g_print("port 0x%02x (%s) set to 0x%02x\n", port, ((plugInstStruct*)inst)->name, val);
    gchar* l = g_strdup_printf ("Port %s = 0x%02x",((plugInstStruct*)inst)->name, val);
    simpleOutVars* p= ((plugInstStruct*)inst)->data;
    gtk_label_set_text (p->label,l);
    g_free(l);    
}

G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {  }

G_MODULE_EXPORT void focusUI(void* inst) {
    plugInstStruct* pl = (plugInstStruct*)inst;
    simpleOutVars* vars = ((simpleOutVars*)pl->data);
    gtk_window_present (vars->window);
}
