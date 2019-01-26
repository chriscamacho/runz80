
#include "../plugin.h"
#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct {
    GtkWidget* parent;
    GtkLabel* label;
    int keyVal;
} keyjoyVars;

int joyVal(GdkEventKey* event) {
    int v=0;
    switch (event->keyval)
    {
        case GDK_KEY_Left:
            v = 1;
            break;
        case GDK_KEY_Right:
            v = 2;
            break;
        case GDK_KEY_Up:
            v = 4;
            break;            
        case GDK_KEY_Down:
            v = 8;
            break;
        case GDK_KEY_Shift_L:
            v = 16;
            break;
        case GDK_KEY_Control_L:
            v = 32;
            break; 
    }
    return v;
}

gboolean keyPress(GtkWidget* widget, GdkEventKey* event, gpointer vars)
{
    int v = joyVal(event);
    ((keyjoyVars*)vars)->keyVal = ((keyjoyVars*)vars)->keyVal | v;
    //g_print("key press 0x%02x 0x%02x\n",v,((keyjoyVars*)vars)->keyVal);
}

gboolean keyRelease(GtkWidget* widget, GdkEventKey* event, gpointer vars)
{
    int v = joyVal(event);
    v=v^0xff;
    ((keyjoyVars*)vars)->keyVal = ((keyjoyVars*)vars)->keyVal & v;
    //g_print("key release 0x%02x 0x%02x\n",v,((keyjoyVars*)vars)->keyVal);
}

G_MODULE_EXPORT void initialise(void* inst, GtkWidget *parent)
{
    plugInstStruct* pl = (plugInstStruct*)inst;

    pl->data = malloc(sizeof(keyjoyVars));

    keyjoyVars* vars = ((keyjoyVars*)pl->data);
    vars->parent = parent;
    vars->label = (GtkLabel*)gtk_label_new ("");
    vars->keyVal = 0;

    gchar* l = g_strdup_printf ("%s (cursor keys and left shift/ctrl)",(const gchar *)&pl->name); 
    gtk_label_set_text ((GtkLabel*)vars->label,l);
    g_free(l);
    
    gtk_container_add (GTK_CONTAINER (vars->parent), (GtkWidget*)vars->label);

    g_signal_connect (G_OBJECT (vars->parent), "key-press-event", G_CALLBACK (keyPress), vars);
    g_signal_connect (G_OBJECT (vars->parent), "key-release-event", G_CALLBACK (keyRelease), vars);
}

G_MODULE_EXPORT int getPortSize() { return 1; }
G_MODULE_EXPORT int getAddressSize() { return 0; }

G_MODULE_EXPORT byte getPort(void* inst, int port)
{
    plugInstStruct* pl = (plugInstStruct*)inst;
    keyjoyVars* vars = ((keyjoyVars*)pl->data);
    return vars->keyVal;
}

G_MODULE_EXPORT void setPort(void* inst, int port, byte val)
{
    g_print("Warning: port 0x%02x (%s) is input only!\n", port, ((plugInstStruct*)inst)->name);
}

G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {  }


