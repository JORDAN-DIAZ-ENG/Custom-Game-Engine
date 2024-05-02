#pragma once

namespace Fiea::GameEngine
{
	class IEvent;
	class ISubscriber
	{
	public:

		ISubscriber() = default;
		virtual ~ISubscriber() = default;

		virtual void Notified(IEvent* event) = 0;
		virtual void Subscribe(IEvent* event) = 0;
		virtual void Unsubscribe(IEvent* event) = 0;
	};


}