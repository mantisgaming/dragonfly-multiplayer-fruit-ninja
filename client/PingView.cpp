#include "PingView.h"

#include <EventView.h>

PingView::PingView() {
	setViewString("Ping: ");
	setLocation(df::TOP_RIGHT);
	registerInterest(df::VIEW_EVENT);
}

int PingView::eventHandler(const df::Event* p_e) {

	ViewObject::eventHandler(p_e);

	if (p_e->getType() != df::VIEW_EVENT)
		return 0;

	auto ev = (df::EventView*) p_e;

	if (ev->getTag() != getViewString())
		return 0;

	if (getValue() <= 100) {
		setColor(df::GREEN);
	}
	else if (getValue() <= 300) {
		setColor(df::YELLOW);
	}
	else {
		setColor(df::RED);
	}

	return 1;
}
