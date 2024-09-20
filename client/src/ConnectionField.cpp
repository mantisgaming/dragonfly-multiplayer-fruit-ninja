#include "ConnectionField.h"

#include <LogManager.h>
#include <NetworkManager.h>
#include <WorldManager.h>

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

    LM.writeLog("ConnectionBox::callback(): address: %s", getText().c_str());
    setColor(df::WHITE);

    if (NM.connect(getText(), DEFAULT_PORT) < 0) {
        setColor(df::RED);
    }
    else {
        WM.markForDelete(this);
    }
}