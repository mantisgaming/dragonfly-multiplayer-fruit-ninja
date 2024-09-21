
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include "Server.h"

int main(int argc, char** argv) {

#ifdef _DEBUG
    LM.setFlush(true);
#endif

    // Start up game manager.
    if (GM.startUp()) {
        LM.writeLog("ERROR: failed to start game manager!");
        GM.shutDown();
        return 1;
    }

    // Start up network manager
    if (NM.startUp()) {
        LM.writeLog("ERROR: failed to start network manager!");
        NM.shutDown();
        GM.shutDown();
        return 2;
    }

    // load the default port
    uint16_t port = DEFAULT_PORT;

    // check for a port in the command line arguments
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    // set the network manager to server mode
    if (NM.startupServer(port)) {
        LM.writeLog("ERROR: Failed to start server");
        NM.shutDown();
        GM.shutDown();
        return 3;
    }

    LM.writeLog("INFO: Server started successfully on port %hu", port);

    // create server
    new Server();

    // run server
    GM.run();

    // shutdown everything
    NM.shutDown();
    GM.shutDown();
}