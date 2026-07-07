#include "..\headers\controller.h"

/*******************************************************************************/

Controller::Controller()
{
	for (int i = 0; i < irr::KEY_KEY_CODES_COUNT; i++)
	{
		keys[i] = 0;
	}
}

/*******************************************************************************/

Controller::~Controller()
{
}

/*******************************************************************************/

bool Controller::OnEvent(const SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if (event.KeyInput.PressedDown)
		{
			int x = 1;
			if (event.KeyInput.Shift) x += 2;
			if (event.KeyInput.Control) x += 4;
			keys[event.KeyInput.Key] = x;
		}
		else keys[event.KeyInput.Key] = 0;
		return true;
	}
	return false;
}

/*******************************************************************************/
