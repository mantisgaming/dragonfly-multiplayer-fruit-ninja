
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include <Util.h>
#include "ConnectionField.h"

int main(int argc, char** argv) {
    // Start up game manager.
    if (GM.startUp()) {
        LM.writeLog("Error starting game manager!");
        GM.shutDown();
        return 0;
    }

    if (NM.startUp()) {
        LM.writeLog("Error starting network manager!");
        NM.shutDown();
        GM.shutDown();
        return 0;
    }

    Util::loadResources();

    new ConnectionField();

    // Show splash screen.
    df::splash();

    // Shut everything down.
    GM.shutDown();
}