#include "pch.h"
#include "restaurant.h"
#include "EventHandler.h"


namespace Fiea::GameEngine
{
	void restaurant::CompleteOrder(IEvent* _event, const GameTime& time)
	{
		EventHandler::Post(_event, time);
	}
}

