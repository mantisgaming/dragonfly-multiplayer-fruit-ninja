
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include <Util.h>
#include "ConnectionField.h"

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

    Util::loadResources();

    // Show splash screen.
    df::splash();

    new ConnectionField();
    
    GM.run();

    // Shut everything down.
    GM.shutDown();
}

BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType) {
    GM.setGameOver(true);
    return true;
}