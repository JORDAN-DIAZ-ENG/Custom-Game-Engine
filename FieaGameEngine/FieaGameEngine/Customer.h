#pragma once
#include "ISubscriber.h"
#include "IEvent.h"

namespace Fiea::GameEngine
{
	class Customer : public ISubscriber
	{
	public:
			Customer() = default;
			~Customer() = default;

			// Inherited via ISubscriber
			void Notified(IEvent* _event) override;

			void Subscribe(IEvent* _event) override;

			void Unsubscribe(IEvent* _event) override;

			int getCount() { return _count; };

			bool isFed() { return _isFed; };

	private:
		bool _isFed{ false };
		int _count{ 0 };

	};
}