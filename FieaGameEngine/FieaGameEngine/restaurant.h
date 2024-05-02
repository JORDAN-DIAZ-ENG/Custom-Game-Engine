#pragma once
#include "IEvent.h"
#include <vector>

namespace Fiea::GameEngine
{
	class restaurant
	{
	public:
		restaurant() = default;

		void CompleteOrder(IEvent* _event, const GameTime& time);

	};
}