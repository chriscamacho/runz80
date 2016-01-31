
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libz80/z80.h"


#include <gtk/gtk.h>

GtkWidget *mainwin;
GtkWidget *AFtext,*BCtext,*DEtext,*HLtext;
GtkWidget *IXtext,*IYtext,*SPtext,*PCtext;
GtkWidget *addressInput,*memCont1,*memCont2;

/* 64Kb of RAM */
static byte memory[ 0x10000 ];
static Z80Context context;
char tmpstr[80];

static void dump_z80_state( void );
 
void close_app(GtkWidget* widget,gpointer user_data) {
 gtk_main_quit();
}

void on_reset(GtkWidget* widget,gpointer user_data) {
  Z80RESET(&context);
  dump_z80_state();
}
void on_addressChange(GtkWidget* widget,gpointer user_data) {
  
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

void on_run(GtkWidget* widget,gpointer user_data) {

    while(!context.halted) {
        Z80Execute(&context);
        dump_z80_state();

    } 
}

void on_step(GtkWidget* widget,gpointer user_data) {
    if(!context.halted) {
        Z80Execute(&context);
        dump_z80_state();
    }
}

void on_load(GtkWidget* widget,gpointer user_data) {

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

#define AF1 context.R1.wr.AF
#define BC1 context.R1.wr.BC
#define DE1 context.R1.wr.DE
#define HL1 context.R1.wr.HL
#define AF2 context.R2.wr.AF
#define BC2 context.R2.wr.BC
#define DE2 context.R2.wr.DE
#define HL2 context.R2.wr.HL
#define IX context.R1.wr.IX
#define IY context.R1.wr.IY
#define SP context.R1.wr.SP
#define PC context.PC
#define I context.I
#define R context.R
#define IFF1 context.IFF1
#define IFF2 context.IFF2
#define IM context.IM

static void setValText(GtkEntry* ent, ushort val) {
  sprintf(tmpstr,"%04x",val);
  gtk_entry_set_text (ent,tmpstr);
}

static void dump_z80_state( void ) {
  printf( "AF %04x BC %04x DE %04x HL %04x\nAF'%04x BC'%04x DE'%04x HL'%04x IX%04x IY%04x SP%04x PC%04x\n",
	  AF1, BC1, DE1, HL1, AF2, BC2, DE2, HL2, IX, IY, SP, PC );
  printf( "I%02x R%02x IFF1%d IFF2%d IM%d halted %d tstates %d\n", I, R,
	  IFF1, IFF2, IM, context.halted, context.tstates );

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

int main( int argc, char **argv )
{
/*
0000			    org     0x0 
0000 3e 12		    ld      a,0x12 
0002 06 23		    ld      b,0x23 
0004 80			    add     a,b 
0005 32 00 02		ld      (0x200),a 
0008 76			    halt 
0009
* /
    memory[ 0] = 0x3e; memory[ 1] = 0x12;
    memory[ 2] = 0x06; memory[ 3] = 0x23;
    memory[ 4] = 0x80; 
    memory[ 5] = 0x32; memory[ 6] = 0x00; memory[ 7] = 0x02;
    memory[ 8] = 0x76; 
*/
/*    if ( argc != 2 ) {
        printf( "usage: %s filename\n", argv[0] );
        exit(-1);
    } else {
        FILE *file = fopen( argv[1], "r" );
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
    }
*/
    for (int i=0;i<0x10000;i++) {
        memory[i] = 0;
    }
    init_emulator();
    
//    while(!context.halted)
//    {
//        Z80Execute(&context);
//        dump_z80_state();
//    }

//    printf("(0x8000)=%x\n",memory[0x8000]);

 GtkBuilder *gtkBuilder;

 //gtk_set_locale();
 /* Initialize the widget set */
 gtk_init (&argc, &argv);
 /* Create the main window */
 gtkBuilder= gtk_builder_new();
 gtk_builder_add_from_file(gtkBuilder,"ui.glade",NULL); 
 gtk_builder_connect_signals ( gtkBuilder, NULL );
 mainwin = GTK_WIDGET(gtk_builder_get_object(gtkBuilder,"mainWindow"));

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
 /* Show the application window */
 gtk_widget_show_all ( mainwin );
 /* Enter the main event loop, and wait for user interaction */
 gtk_main ();
 /* The user lost interest */
 return 0;
}
