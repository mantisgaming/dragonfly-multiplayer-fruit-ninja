#pragma once

#include <Object.h>
#include <EventStep.h>
#include "EventNetwork.h"

namespace df {

	class NetworkSentry : public Object
	{
	private:
		int stepHandler(const EventStep* p_event);
		int networkHandler(const EventNetwork* p_event);
	public:
		NetworkSentry();
		int eventHandler(const Event* p_event) override;
	};

}