//#pragma once
//#include <functional>
//
//namespace Fiea::GameEngine
//{
//	class IEvent {};
//
//	class ThrowBoneEvent : public IEvent
//	{
//	};
//
//	class IEventListener
//	{
//	public:
//		void ReactTo(/*event*/) = 0;
//	};
//
//	class Dog : public IEventListener
//	{
//	public:
//			void ReactTo(/*event*/) override
//			{
//				// do something
//			}
//	};
//
//	class EventQueue
//	{
//		template <class EventType>
//		void AddListener(EventListener* listener)
//		{
//
//		}
//
//		void Send(const IEvent& ev)
//		{
//			// send to all listeners
//			foreach(auto listener : _listeners[])
//			{
//				listener(ev);
//			}
//		})
//
//		void Enqueue(const IEvent& ev, GameTime time, delay = 0)
//		{
//			_events.push_back(ev);
//		}
//
//		void ProcessQueuedEvents(GameTime time)
//		{
//			foreach(IEvent * event in _events)
//			{
//				Send(*event);
//			})
//		}
//
//	private:
//		std::unordered_map<eventtype ? , EventCallback> _listeners;
//
//		using EventCallback = std::function<void>(IEvent& ev)
//
//	};
//
//	TEST_CLASS(EventStuff)
//	{
//		TEST_METHOD(Basic)
//		{
//			Person p;
//			Dog d;
//
//			EventQueue eq;
//
//			// declare a throws bone event
//			ThrowBoneEvent event;
//
//			eq.AddListener<ThrowBoneEvent>(d);
//
//			eq.AddListener<ThrowBoneEvent>(HearBone);
//
//			eq.AddListener<ThrowBoneEvent>(std::bind(Dog::TryFetch, d, std::placeholder::_1))
//
//
//			eq.AddListener<ThrowBoneEvent>([&d](const IEvent& ev)
//			{
//				d.TryFetch(throwEv);
//			});
//
//			// dog listens for event
//			d.ListenFor<ThrowBoneEvent>();
//
//			// enqueue the event
//			p.Post<ThrowBoneEvent>();
//
//			// dispatch enqueued events when safe
//			eq.Dispatch();
//		}
//
//	};
//}