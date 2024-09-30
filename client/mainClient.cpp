
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include <Util.h>
#include <NetworkSocket.h>
#include <PingManager.h>

#include "ConnectionField.h"
#include "Splash.h"
#include "Client.h"

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

    Util::loadResources();

    // Show splash screen.
    df::splash();

    // Show fruit ninja splash screen
    new Splash();

    // Start up network manager
    if (NM.startUp()) {
        LM.writeLog("ERROR: failed to start network manager!");
        NM.shutDown();
        GM.shutDown();
        return 2;
    }

    new ConnectionField();
    new Client();
    new PingManager();
    
    GM.run();

    df::NetworkSocket** sockets;
    int socketCount = NM.getConnections(sockets);

    char data = NM.getClientID();
    NetworkMessage msg = { NetworkMessage::DISCONNECT, &data, 1 };

    NM.sendToAll(msg);

    NM.setAllowSending(false);
    NM.recieve();
    NM.recieve();

    NM.shutDown();
    // Shut everything down.
    GM.shutDown();
}

BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
    GM.setGameOver(true);
    return true;
}