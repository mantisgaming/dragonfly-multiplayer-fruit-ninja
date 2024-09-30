
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include "Server.h"
#include "Grocer.h"
#include <Util.h>

#include <PingManager.h>

BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType);

int main(int argc, char** argv) {

    SetConsoleCtrlHandler(HandlerRoutine, true);

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

    Util::loadResources();

    LM.writeLog("INFO: Server started successfully on port %hu", port);

    // create server
    new Server();
    new PingManager();

    NM.setDelay(5);

    // run server
    GM.run();

    df::NetworkSocket** sockets;
    int socketCount = NM.getConnections(sockets);

    char data = NM.getClientID();
    NetworkMessage msg = { NetworkMessage::DISCONNECT, &data, 1 };

    NM.sendToAll(msg);

    NM.setAllowSending(false);
    NM.recieve();

    // shutdown everything
    NM.shutDown();

    LM.writeLog("INFO: Server closed");

    GM.shutDown();
}

BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
    GM.setGameOver(true);
    return true;
}