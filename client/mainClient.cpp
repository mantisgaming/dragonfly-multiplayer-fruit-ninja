
#include <GameManager.h>
#include <LogManager.h>
#include <NetworkManager.h>
#include <Util.h>

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

    // Show splash screen.
    df::splash();

    // Shut everything down.
    GM.shutDown();
}