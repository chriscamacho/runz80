#include <glib.h>
#include <gmodule.h>
#include <gtk/gtk.h>

typedef unsigned char byte;

typedef void (* initialiseFunc) (void* pInst, GtkWidget* parent);
typedef int (* getPortSizeFunc) ();
typedef int (* getAddressSizeFunc) ();
typedef byte (* getPortFunc) (void* pInst, int port);
typedef void (* setPortFunc) (void* pInst, int port, byte val);
typedef byte (* getAddressFunc) (void* pInst, int address);
typedef void (* setAddressFunc) (void* pInst, int address, byte data);
//typedef void (* focusUiFunc) (void* pInst);

typedef struct {
    GModule *module;
    char libName[1024];
    initialiseFunc initialise;
    getPortSizeFunc getPortSize;
    getAddressSizeFunc getAddressSize;
    getPortFunc getPort;
    setPortFunc setPort;
    getAddressFunc getAddress;
    setAddressFunc setAddress;
//    focusUiFunc focusUI;
} pluginStruct;

typedef struct {
    pluginStruct plug;
    char name[80];
    int portStart;      // the first port in the block of ports used by the plugin
    int addressStart;   // the first address in the block of memory mapped to the plugin
    void* data; 
} plugInstStruct;




void integratePlugin(pluginStruct *plugin);
void namePluginInstance(plugInstStruct *plugInst, char* str);
void setPluginInstanceStartPort(plugInstStruct *plugInst, int p);
void setPluginInstanceStartAddress(plugInstStruct *plugInst, int a);
