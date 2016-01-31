

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "libz80/z80.h"
#include "main.h"

#include <gtk/gtk.h>

GtkWidget *mainwin, *runButton;
GtkWidget *AFtext,*BCtext,*DEtext,*HLtext;
GtkWidget *IXtext,*IYtext,*SPtext,*PCtext;
GtkWidget *addressInput,*memCont1,*memCont2;

/* 64Kb of RAM */
static byte memory[ 0x10000 ];
static Z80Context context;
char tmpstr[80];
gboolean running = FALSE;

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

void on_run(GtkWidget* widget,gpointer user_data)
{

    if (running==FALSE) {
        if (!context.halted) running = TRUE;
    } else {
        running = FALSE;
    }

    if (running) {
        gtk_button_set_label ((GtkButton*)runButton,"Stop");
    } else {
        gtk_button_set_label ((GtkButton*)runButton,"Run");
    }
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
        //open_file (filename);
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

gboolean on_idle( gpointer data )
{
    if(!context.halted && running==TRUE) {
        Z80Execute(&context);
//        gdk_threads_enter();
        dump_z80_state();
//        gdk_threads_leave();
    } else {
        running = FALSE;
    }

    return( TRUE );
}

static byte context_mem_read_callback(int param, ushort address)
{
    return memory[address];
}

static void context_mem_write_callback(int param, ushort address, byte data)
{
    memory[address] = data;
}

static byte context_io_read_callback(int param, ushort address)
{
    byte data = address >> 8;
    printf("PR %04x %02x\n", address, data);
    return data;
}

static void context_io_write_callback(int param, ushort address, byte data)
{
    printf("PW %04x %02x\n", address, data);
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

int main( int argc, char **argv ) {

    for (int i=0; i<0x10000; i++) {
        memory[i] = 0;
    }
    init_emulator();

    GtkBuilder *gtkBuilder;

    //if( ! g_thread_supported() ) g_thread_init( NULL );
    //gdk_threads_init();
    //gdk_threads_enter();
    
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

    g_object_unref ( G_OBJECT(gtkBuilder) );
    gdk_threads_add_idle ( on_idle, NULL );
    gtk_widget_show_all ( mainwin );
    gtk_main ();
    //gdk_threads_leave();
    
    return 0;
}
