#include "pch.h"
#include "EventHandler.h"



namespace Fiea::GameEngine
{
	void EventHandler::Init()
	{
		_events = new std::vector<IEvent*>();
	}
	void EventHandler::Post(IEvent* _event, const GameTime& time)
	{
		_events->push_back(_event);
		_event->setStartTime(time.Game());
	}

	void EventHandler::Update(const GameTime& time)
	{
		std::vector<IEvent*>* eventsToKeep = new std::vector<IEvent*>();

		for (IEvent*& event : *_events)
		{
			long long currentTime = time.Game();
			if (event->getDelay() >= currentTime - event->getStartTime())
			{
				event->NotifySubscribers();
				delete event;
				event = nullptr;
			}
			else
			{
				eventsToKeep->push_back(event);
			}
		}

		_events->clear();
		delete _events;
		_events = eventsToKeep;

		//_events.clear(); 
	}

}

