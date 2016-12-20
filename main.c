

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "libz80/z80.h"
#include "main.h"

#include <gmodule.h>
#include "plugin.h"

#include <gtk/gtk.h>
#include <expat.h>

GtkWidget *mainwin, *runButton;
GtkWidget *AFtext,*BCtext,*DEtext,*HLtext;
GtkWidget *IXtext,*IYtext,*SPtext,*PCtext;
GtkWidget *addressInput,*memCont1,*memCont2;
GtkWidget *pluginPanel;

/* 64Kb of RAM */
static byte memory[ 0x10000 ];
static Z80Context context;
char tmpstr[80];
gboolean running = FALSE;

// list of plugin instances
GList *plugins;
guint idle_id;      // thread idle id

gboolean inhibitFocus=FALSE;

gboolean on_mainWindow_focus(GtkWidget *widget, GdkEvent  *event, gpointer   user_data)
{
    /* lot easier now all in one window...*/
    return TRUE;
}



void close_app(GtkWidget* widget,gpointer user_data)
{
    gtk_main_quit();
}

void on_reset(GtkWidget* widget,gpointer user_data)
{
    Z80RESET(&context);
    dump_z80_state();
}
void on_addressChange(GtkWidget* widget,gpointer user_data)
{
    int m = (int)strtol(gtk_entry_get_text((GtkEntry*)addressInput), NULL, 16);
    sprintf(tmpstr,"%02x %02x %02x %02x %02x %02x %02x %02x",
            memory[m   ],memory[m+ 1],memory[m+ 2],memory[m+ 3],
            memory[m+ 4],memory[m+ 5],memory[m+ 6],memory[m+ 7]);
    gtk_entry_set_text ((GtkEntry*)memCont1,tmpstr);
    sprintf(tmpstr,"%02x %02x %02x %02x %02x %02x %02x %02x",
            memory[m+ 8],memory[m+ 9],memory[m+10],memory[m+11],
            memory[m+12],memory[m+13],memory[m+14],memory[m+15]);
    gtk_entry_set_text ((GtkEntry*)memCont2,tmpstr);

}



gboolean on_idle( gpointer data );

void on_run(GtkWidget* widget,gpointer user_data)
{

    if (running==FALSE) {
        if (!context.halted) {
            running = TRUE;
            idle_id = gdk_threads_add_idle ( on_idle, NULL );
        }
    } else {
        running = FALSE;
        g_source_remove (idle_id);
    }

    if (running) {
        gtk_button_set_label ((GtkButton*)runButton,"Stop");
    } else {
        gtk_button_set_label ((GtkButton*)runButton,"Run");
    }
}


gboolean on_idle( gpointer data )
{
//    gdk_threads_enter();  // TODO ...and I'm supposed to do what instead
    if(!context.halted && running==TRUE) {
        Z80Execute(&context);
        //dump_z80_state();
    } else {
        running = TRUE;
        on_run(NULL, NULL);
    }


//    gdk_threads_leave();
    return( TRUE );
}

void on_step(GtkWidget* widget,gpointer user_data)
{
    if(!context.halted) {
        Z80Execute(&context);
        dump_z80_state();
    }
}

void on_load(GtkWidget* widget,gpointer user_data)
{
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new ("Open File",
                                          GTK_WINDOW(mainwin),
                                          action,
                                          "_Cancel",
                                          GTK_RESPONSE_CANCEL,
                                          "_Open",
                                          GTK_RESPONSE_ACCEPT,
                                          NULL);


    GtkFileFilter *filter = gtk_file_filter_new ();
    gtk_file_filter_add_pattern (filter, "*.bin");
    gtk_file_chooser_add_filter ((GtkFileChooser*)dialog, filter);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        FILE *file = fopen( filename, "r" );
        if ( file == 0 ) {
            printf( "Could not open file\n" );
        } else {
            int x;
            long p=0;
            while  ( ( x = fgetc( file ) ) != EOF ) {
                memory[p++]=x;
            }
            fclose( file );
        }
        g_free (filename);
    }

    gtk_widget_destroy (dialog);

}

static byte context_mem_read_callback(int param, ushort address)
{
    return memory[address];
}

static void context_mem_write_callback(int param, ushort address, byte data)
{
    
    memory[address] = data;

    for (GList* l = plugins; l != NULL; l = l->next) {
        plugInstStruct* i = (plugInstStruct*)l->data;
        int a = i->addressStart;
        int r = i->plug.getAddressSize();
        if (address>=a && address<(a+r)) {
            // address in range
            i->plug.setAddress(i, address, data);

        }
    }
}

static byte context_io_read_callback(int param, ushort address)
{
    int data=0xff;
    address = address & 0xff;
    gboolean handled = FALSE;
    for (GList* l = plugins; l != NULL; l = l->next) {
        plugInstStruct* i = (plugInstStruct*)l->data;
        int a = i->portStart;
        int r = i->plug.getPortSize();
        if (address>=a && address<a+r) {
            // IO port in range
            data = i->plug.getPort(i, address);
            handled = TRUE;
        }
    }
    if (!handled) {
        g_print("warning port write not handled by a plugin (0x%02x)=0x%02x\n",address,data);
    }

    return data;
}

static void context_io_write_callback(int param, ushort address, byte data)
{
    address = address & 0xff; // port address
    gboolean handled = FALSE;
    for (GList* l = plugins; l != NULL; l = l->next) {
        plugInstStruct* i = (plugInstStruct*)l->data;
        int a = i->portStart;
        int r = i->plug.getPortSize();
        if (address>=a && address<a+r) {
            // IO port in range
            i->plug.setPort(i, address, data);
            handled = TRUE;
        }
    }
    if (!handled) {
        g_print("warning port write not handled by a plugin (0x%02x)=0x%02x\n",address,data);
    }
}

void init_emulator()
{
    context.memRead = context_mem_read_callback;
    context.memWrite = context_mem_write_callback;
    context.ioRead = context_io_read_callback;
    context.ioWrite = context_io_write_callback;
}

static void setValText(GtkEntry* ent, ushort val)
{
    sprintf(tmpstr,"%04x",val);
    gtk_entry_set_text (ent,tmpstr);
}

static void dump_z80_state( void )
{
    /*
        printf( "AF %04x BC %04x DE %04x HL %04x\nAF'%04x BC'%04x DE'%04x HL'%04x IX%04x IY%04x SP%04x PC%04x\n",
                AF1, BC1, DE1, HL1, AF2, BC2, DE2, HL2, IX, IY, SP, PC );
        printf( "I%02x R%02x IFF1%d IFF2%d IM%d halted %d tstates %d\n", I, R,
                IFF1, IFF2, IM, context.halted, context.tstates );
    */
    setValText((GtkEntry*)AFtext, AF1);
    setValText((GtkEntry*)BCtext, BC1);
    setValText((GtkEntry*)DEtext, DE1);
    setValText((GtkEntry*)HLtext, HL1);

    setValText((GtkEntry*)IXtext, IX);
    setValText((GtkEntry*)IYtext, IY);
    setValText((GtkEntry*)SPtext, SP);
    setValText((GtkEntry*)PCtext, PC);

    on_addressChange(NULL,NULL);
}

// cache loaded plugin libraries
GHashTable* pluglookup;

// during xml attribute iteration each of these are filled in
// ready for the end of the element callback
char label[1024];
pluginStruct* plug;
int portStart,addressStart;

void startElement(void *data, const char *el, const char **attr) {
    int i;

    for (i = 0; attr[i]; i += 2) {
        if (g_strcmp0(attr[i],"libName")==0) {
            plug = (pluginStruct*)g_hash_table_lookup (pluglookup, attr[i + 1]);
            //g_print("\nplug %s - %lu\n",attr[i+1],plug);
            if (plug==NULL) {
                plug = malloc(sizeof(pluginStruct));
                sprintf(plug->libName,(const char*)attr[i + 1]);
                integratePlugin(plug);
                //g_print("integrated %s\n",attr[i + 1]);
                g_hash_table_insert (pluglookup, (gpointer)attr[i + 1],plug);
            }
        }
        if (g_strcmp0(attr[i],"label")==0) {
            sprintf(label,(const char*)attr[i + 1]);
        }
        if (g_strcmp0(attr[i],"portStart")==0) {
            portStart = (int)strtol((const char*)attr[i + 1], NULL, 16);
        }
        if (g_strcmp0(attr[i],"addressStart")==0) {
            addressStart = (int)strtol((const char*)attr[i + 1], NULL, 16);
        }
    }
}



void endElement(void *data, const char *el)
{
    if (plug!=NULL) {
        plugInstStruct*  pInst = malloc(sizeof(plugInstStruct));
        pInst->plug=*plug;
        namePluginInstance(pInst,label);
        setPluginInstanceStartPort(pInst,portStart);
        setPluginInstanceStartAddress(pInst,addressStart);
        pInst->plug.initialise(pInst,pluginPanel);

        plugins=g_list_append (plugins, pInst);
        sprintf(label,"\0");
        plug=NULL;
        portStart=0;
        addressStart=0;
    }

}


void centreWidget (GtkWidget *child, gpointer data) {
    // “halign”                   GtkAlign
    // GTK_ALIGN_CENTER
    gtk_widget_set_halign (child, GTK_ALIGN_CENTER);
}


int main( int argc, char **argv )
{
    for (int i=0; i<0x10000; i++) {
        memory[i] = 0;
    }
    init_emulator();

    GtkBuilder *gtkBuilder;

    gtk_init (&argc, &argv);

    gtkBuilder= gtk_builder_new();
    gtk_builder_add_from_file(gtkBuilder,"ui.glade",NULL);
    gtk_builder_connect_signals ( gtkBuilder, NULL );
    mainwin = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"mainWindow"));
    runButton = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"runButton"));

// is there an automagical way to do this...?
    AFtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"AFtext"));
    BCtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"BCtext"));
    DEtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"DEtext"));
    HLtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"HLtext"));

    IXtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"IXtext"));
    IYtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"IYtext"));
    SPtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"SPtext"));
    PCtext = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"PCtext"));

    addressInput = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"addressInput"));
    memCont1 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"memCont1"));
    memCont2 = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"memCont2"));

    pluginPanel = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"pluginPanel"));
    
    g_object_unref ( G_OBJECT(gtkBuilder) );
    gtk_widget_show_all ( mainwin );


    pluglookup = g_hash_table_new ( g_str_hash , g_str_equal);

    g_print("requested machine config=%s\n",argv[1]);
    char buf[BUFSIZ];
    FILE *file = fopen(argv[1], "r" );
    if (file!=NULL) {
        XML_Parser parser = XML_ParserCreate(NULL);
        int done;
        int depth = 0;
        XML_SetUserData(parser, &depth);
        XML_SetElementHandler(parser, startElement, endElement);
        do {
            size_t len = fread(buf, 1, sizeof(buf), file);
            done = len < sizeof(buf);
            if (!XML_Parse(parser, buf, len, done)) {
                fprintf(stderr,
                        "%s at line %d\n",
                        XML_ErrorString(XML_GetErrorCode(parser)),
                        XML_GetCurrentLineNumber(parser));
                return 1;
            }
        } while (!done);
        XML_ParserFree(parser);
    }

    gtk_container_foreach (GTK_CONTAINER (pluginPanel), (GtkCallback) centreWidget, NULL);
    gtk_widget_show_all ( (GtkWidget*)mainwin );
    gtk_window_set_position(GTK_WINDOW(mainwin), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_main ();


    return 0;
}

