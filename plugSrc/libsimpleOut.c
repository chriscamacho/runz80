
#include "../plugin.h"
#include <stdlib.h>


G_MODULE_EXPORT void initialise(void* inst, int portStart, int addressStart) {
    // first 2 ints of plugin instances MUST be portStart and addressStart
    plugInstStruct* pl = (plugInstStruct*)inst;

    // an array of pointers...
    pl->data = malloc(sizeof(void*)*2);
    int** p= (int**)pl->data;
    p[0] = malloc(sizeof(int));
    *p[0] = portStart;
    p[1] = malloc(sizeof(int));
    *p[1] = addressStart;
}

G_MODULE_EXPORT int getPortSize() { return 1; }
G_MODULE_EXPORT int getAddressSize() { return 0; }

G_MODULE_EXPORT byte getPort(void* inst, int port) { return 0xff; }
G_MODULE_EXPORT void setPort(void* inst, int port, byte val) {
    g_print("port 0x%02x (%s) set to 0x%02x\n", port, ((plugInstStruct*)inst)->name, val);
}

G_MODULE_EXPORT byte getAddress(void* inst, int address) { return 0xff; }
G_MODULE_EXPORT void setAddress(void* inst, int address, byte data) {  }
