
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include "Server.h"

int main(int argc, char** argv) {
    // Start up game manager.
    if (GM.startUp()) {
        LM.writeLog("Error starting game manager!");
        GM.shutDown();
        return 0;
    }

    // Start up network manager
    if (NM.startUp()) {
        LM.writeLog("Error starting network manager!");
        NM.shutDown();
        GM.shutDown();
        return 0;
    }

    // load the default port
    uint16_t port = DEFAULT_PORT;

    // check for a port in the command line arguments
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    // set the network manager to server mode
    NM.startupServer(port);

    // create server
    new Server();

    // run server
    GM.run();

    // shutdown everything
    NM.shutDown();
    GM.shutDown();
}