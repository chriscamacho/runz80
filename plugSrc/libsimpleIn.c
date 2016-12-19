
#include "../plugin.h"
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct {
    GtkWidget* parent;
    GtkWidget* frame;
    GtkWidget* label;
    GtkWidget* switch8;
    GtkWidget* switch7;
    GtkWidget* switch6;
    GtkWidget* switch5;
    GtkWidget* switch4;
    GtkWidget* switch3;
    GtkWidget* switch2;
    GtkWidget* switch1;
} simpleInVars;


G_MODULE_EXPORT void initialise(void* inst, GtkWidget* parent) {

    plugInstStruct* pl = (plugInstStruct*)inst;

    pl->data = malloc(sizeof(simpleInVars));

// the plugins GUI
    simpleInVars* vars = ((simpleInVars*)pl->data);

    GtkBuilder* gtkBuilder = gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder,"plugins/simpleIN.glade",NULL);
    vars->frame = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"mainFrame"));
    vars->parent = parent;
    vars->label = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"portLabel"));
    vars->switch8 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch8")); // hi bit
    vars->switch7 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch7"));
    vars->switch6 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch6"));
    vars->switch5 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch5"));
    vars->switch4 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch4"));
    vars->switch3 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch3"));
    vars->switch2 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch2"));
    vars->switch1 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"switch1"));


    // seems I can't load a fragment of a UI unless its in a top level container ??
    GtkWidget* bogus = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"arrgh"));
    gtk_container_remove (GTK_CONTAINER(bogus),vars->frame);

    gchar* l = g_strdup_printf ("Port %s @ 0x%02x",(const gchar *)&pl->name,pl->portStart); 
    gtk_label_set_text ((GtkLabel*)vars->label,l);
    g_free(l);

    gtk_container_add (GTK_CONTAINER (vars->parent), vars->frame);

    // can't unref until frame is reparented...
    g_object_unref ( G_OBJECT(gtkBuilder) );
}

G_MODULE_EXPORT int getPortSize() { return 1; }
G_MODULE_EXPORT int getAddressSize() { return 0; }

G_MODULE_EXPORT byte getPort(void* inst, int port) {
    // ignore port as there is only one
    int v=0;
    plugInstStruct* pl = (plugInstStruct*)inst;
    simpleInVars* vars = ((simpleInVars*)pl->data);
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch8)) v=v+128;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch7)) v=v+64;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch6)) v=v+32;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch5)) v=v+16;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch4)) v=v+8;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch3)) v=v+4;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch2)) v=v+2;
    if (gtk_switch_get_state ((GtkSwitch*)vars->switch1)) v=v+1;
    return v;
}
G_MODULE_EXPORT void setPort(void* inst, int port, byte val) {
    g_print("Warning: port 0x%02x (%s) is input only!\n", port, ((plugInstStruct*)inst)->name);
}

G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {  }



