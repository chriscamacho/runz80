#include <glib.h>
#include <gmodule.h>

typedef unsigned char byte;

typedef void (* initialiseFunc) (void* pInst, int portStart, int addressStart);
typedef int (* getPortSizeFunc) ();
typedef int (* getAddressSizeFunc) ();
typedef byte (* getPortFunc) (void* pInst, int port);
typedef void (* setPortFunc) (void* pInst, int port, byte val);
typedef byte (* getAddressFunc) (void* pInst, int address);
typedef void (* setAddressFunc) (void* pInst, int address, byte data);

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
} pluginStruct;

typedef struct {
    pluginStruct plug;
    char name[80];
    void** data;
} plugInstStruct;




void integratePlugin(pluginStruct *plugin);
void namePluginInstance(plugInstStruct *plugInst, char* str);
