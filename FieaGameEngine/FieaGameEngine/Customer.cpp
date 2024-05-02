#include "pch.h"
#include "Customer.h"


namespace Fiea::GameEngine
{

	void Customer::Notified(IEvent* _event)
	{	 
		_isFed = true;
		++_count;
	}	 
		 
	void Customer::Subscribe(IEvent* _event)
	{	 
		_event->AddSubscriber(this);


		//std::unique_ptr<Customer> customer(this);
		//_event->AddSubscriber(std::move(customer));

		//_event->AddSubscriber(std::move(std::make_unique<Customer>(this)));
	}	 
		 
	void Customer::Unsubscribe(IEvent* _event)
	{	 
		_event->RemoveSubscriber(this);

		//std::unique_ptr<Customer> customer(this);
		//_event->RemoveSubscriber(std::move(customer));
		//_event->RemoveSubscriber(std::move(std::make_unique<Customer>(this)));
	}
}

