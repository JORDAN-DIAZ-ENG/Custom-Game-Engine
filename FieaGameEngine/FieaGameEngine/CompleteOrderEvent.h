#pragma once
#include "IEvent.h"
#include <memory>

class ISubscriber;


namespace Fiea::GameEngine
{

	class completeOrderEvent : public IEvent
	{
		RTTI_DECLARATIONS(completeOrderEvent, IEvent);

	public:
		

		completeOrderEvent() = default;

		// Inherited via IEvent
		//void AddSubscriber(std::shared_ptr<ISubscriber> subscriber) override;
		//void RemoveSubscriber(std::shared_ptr<ISubscriber> subscriber) override;
		//void NotifySubscribers() override;

		virtual ~completeOrderEvent() override = default;

	};
}