#include "ConnectionField.h"

#include <LogManager.h>
#include <NetworkManager.h>
#include <WorldManager.h>
#include <GameManager.h>

#include "Client.h"
#include "PingView.h"

int parseAddress(const std::string& input, std::string& address, uint16_t& port) {
    
    size_t firstInd = input.find_first_of(':');
    size_t lastInd = input.find_last_of(':');

    if (firstInd != lastInd || firstInd == input.length() - 1) {
        return -1;
    }

    if (firstInd == std::string::npos) {
        address = input;
        port = DEFAULT_PORT;
    }
    else {
        address = input.substr(0, firstInd);
        port = stoi(input.substr(firstInd+1, input.length() - firstInd - 1));
    }
    
}

ConnectionField::ConnectionField() {
	setType(CONNECTION_FIELD_STRING);
    setText("");
    setViewString("Server Address: ");
    setBorder(true);
    setColor(df::WHITE);
    setLocation(df::CENTER_CENTER);
    setLimit(20);
}

void ConnectionField::callback() {
    
    LM.writeLog("INFO: ConnectionBox::callback(): attempting connection to address: %s", getText().c_str());
    setColor(df::WHITE);

    std::string address;
    uint16_t port;

    int err = parseAddress(getText(), address, port);

    if (err < 0) {
        LM.writeLog("WARNING: ConnectionBox::callback(): Invalid address");

        setColor(df::RED);
        return;
    }

    err = NM.connect(address, port);

    if (err < 0) {
        LM.writeLog("WARNING: ConnectionBox::callback(): Connection failed");

        setColor(df::RED);
        return;
    }

    LM.writeLog("INFO: ConnectionBox::callback(): Connection succeeded");

    new PingView();
    WM.markForDelete(this);
}