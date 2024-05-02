#pragma once
#include "IEvent.h"
#include <vector>
#include "GameClock.h"

namespace Fiea::GameEngine 
{
	class EventHandler
	{
	public:
		static void Init();

		static void Post(IEvent* _event, const GameTime& time);
		/**
		 * @brief 
		*/
		static void Update(const GameTime& time);

		static void CleanUp()
		{
			for (IEvent* event : *_events)
			{
				if (event == nullptr)
				{
					continue;
				}
				delete event;
				//event = nullptr;
			}
			_events->clear();
			delete _events;
		}
		inline static std::vector<IEvent*>* _events;

	private:
	};
	
}