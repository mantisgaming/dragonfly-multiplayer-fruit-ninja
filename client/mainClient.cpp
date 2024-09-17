
#include <GameManager.h>
#include <LogManager.h>

#include "ClientSample.h"

int main(int argc, char** argv) {
    // Start up game manager.
    if (GM.startUp()) {
        LM.writeLog("Error starting game manager!");
        GM.shutDown();
        return 0;
    }

    LM.writeLog(ClientSample::clientString.c_str());

    // Show splash screen.
    df::splash();

    // Shut everything down.
    GM.shutDown();
}