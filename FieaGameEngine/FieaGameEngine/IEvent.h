#pragma once
#include "RTTI.h"
#include <vector>
#include "ISubscriber.h"
#include "GameClock.h"
#include <memory>


namespace Fiea::GameEngine
{
	using std::unique_ptr;


	class IEvent : public RTTI
	{

	RTTI_DECLARATIONS(IEvent, RTTI);

	public:

		
		//virtual void AddSubscriber(std::unique_ptr<ISubscriber>&& subscriber);
		//virtual void RemoveSubscriber(std::unique_ptr<ISubscriber>&& subscriber);
		//virtual void NotifySubscribers();
				
		virtual void AddSubscriber(ISubscriber* subscriber);
		virtual void RemoveSubscriber(ISubscriber* subscriber);
		virtual void NotifySubscribers();

		long long getDelay() const { return delay; };
		void setStartTime(long long time) { startTime = time; };
		long long getStartTime() const { return startTime; };
		bool getIsLooping() const { return isLooping; };
		void setIsLooping(bool loop) { isLooping = loop; };

		IEvent();

		virtual ~IEvent() = default;

	protected:
		//std::vector<std::unique_ptr<ISubscriber>> _subscribers;
		std::vector<ISubscriber*> _subscribers;


	private:
		long long startTime = 0;
		long long delay = 0;
		bool isLooping = false;

	};


}