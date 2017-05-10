// emulates a character display in a very simplistic way
// any write to the port apears in the text buffer
// ansi code ignored.

// very rudimentry but would be nice to eventually make
// more realistic ansi terminal (both ways) with
// realistic sized fifo and delay etc

#include "../plugin.h"
#include <gtk/gtk.h>
#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#define __USE_BSD
#include <termios.h>
#include <sys/ioctl.h>

// missing include ?
extern char *ptsname();
extern int grantpt(int);
extern int unlockpt(int);

typedef struct {
    GtkWidget* parent;
    GtkLabel* text;
    int fdm,fds;	// master and slave file handles.

} terminalVars;



G_MODULE_EXPORT void initialise(void* inst, GtkWidget *parent) {

    plugInstStruct* pl = (plugInstStruct*)inst;

    pl->data = malloc(sizeof(terminalVars));

// the plugins GUI
    terminalVars* vars = ((terminalVars*)pl->data);
    vars->parent = parent;

	vars->fdm = open("/dev/ptmx", O_RDWR);	// open master 
	grantpt(vars->fdm);						// change permission of slave 
	unlockpt(vars->fdm);						// unlock slave 
	char *slavename;
	slavename = ptsname(vars->fdm);			// get name of slave 
	vars->fds = open(slavename, O_RDWR);		// open slave 
	fcntl(vars->fdm, F_SETFL, 0);				// don't block read

    vars->text = (GtkLabel*)gtk_label_new(slavename);
    char cmd[1024];
    snprintf(cmd,1023,"xterm -e \"miniterm %s\" &\0",slavename);
    system(cmd);
    
    gtk_container_add (GTK_CONTAINER (vars->parent), (GtkWidget*)vars->text);
}

G_MODULE_EXPORT int getPortSize() { return 2; }
G_MODULE_EXPORT int getAddressSize() { return 0; }

G_MODULE_EXPORT byte getPort(void* inst, int port) {

	//printf("getPort port=%i\n",port);
	//return 0xff;
	int ps = ((plugInstStruct*)inst)->portStart;
	terminalVars* p= ((plugInstStruct*)inst)->data;
	int avail=0;
	ioctl(p->fdm, FIONREAD, &avail);
	
	if (ps != port) {	// second port is available bytes
		return avail & 0xff;
	} else {	// first port is data
		if (avail==0) return 0;
		char c;
		read(p->fdm, &c, 1);
		return c;
	}
}

G_MODULE_EXPORT void setPort(void* inst, int port, byte val) {

	int ps = ((plugInstStruct*)inst)->portStart;
	terminalVars* p= ((plugInstStruct*)inst)->data;
	//printf("setPort port=%i\n",port);
	if (ps == port) {
		write(p->fdm, &val, 1);
	}

}

G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {  }


