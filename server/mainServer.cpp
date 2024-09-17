
#include <GameManager.h>
#include <LogManager.h>

#include "ServerSample.h"
#include <SharedSample.h>

int main(int argc, char** argv) {
    // Start up game manager.
    if (GM.startUp()) {
        LM.writeLog("Error starting game manager!");
        GM.shutDown();
        return 0;
    }

    LM.writeLog(ServerSample::serverString.c_str());
    LM.writeLog(SharedSample::sharedString.c_str());

    // Show splash screen.
    df::splash();

    // Shut everything down.
    GM.shutDown();
}