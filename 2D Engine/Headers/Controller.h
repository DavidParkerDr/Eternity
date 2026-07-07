#pragma once
#include <irrlicht.h>
using namespace irr;

class Controller
	: public IEventReceiver
{
	/////////////////
	// Public Members
public:
	unsigned char keys[irr::KEY_KEY_CODES_COUNT];

	////////////
	// Structors
	Controller();
	virtual ~Controller();

	// Override for the Event Listener
	virtual bool OnEvent(const SEvent& event);
};
