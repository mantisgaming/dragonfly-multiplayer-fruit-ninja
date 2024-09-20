#pragma once

#include <TextEntry.h>

const std::string CONNECTION_FIELD_STRING = "connection-field";

class ConnectionField : df::TextEntry {
public:
	ConnectionField();
	void callback() override;
};

