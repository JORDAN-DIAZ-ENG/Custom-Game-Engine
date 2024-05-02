#include "pch.h"
#include "IEvent.h"



namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(IEvent);

	void IEvent::AddSubscriber(ISubscriber* subscriber)
	{
		//_subscribers.push_back(std::move(subscriber));
		_subscribers.push_back(subscriber);
	}

	void IEvent::RemoveSubscriber(ISubscriber* subscriber)
	{
		_subscribers.erase(std::remove(_subscribers.begin(), _subscribers.end(), subscriber), _subscribers.end());
	}

	void IEvent::NotifySubscribers()
	{

		//for (std::unique_ptr<ISubscriber>& subscriber : _subscribers)
		//{
		//	/*subscriber->Notified(this);*/
		//	subscriber->Notified(this);
		//}
		
		for (ISubscriber* subscriber : _subscribers)
		{
			/*subscriber->Notified(this);*/
			subscriber->Notified(this);
		}

	}


	IEvent::IEvent()
	{

	}

}

