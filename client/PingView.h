#pragma once

#include <ViewObject.h>

class PingView : df::ViewObject {
public:
	PingView();
	int eventHandler(const df::Event* p_e) override;
};

