#include "Factory.h"


namespace Fiea::GameEngine
{
	template<typename BaseType>
	void FactoryManager<BaseType>::Add(unique_ptr<IFactory<BaseType>>&& concreteFactory)
	{

		//factories->insert({ concreteFactory->ClassName(), concreteFactory });



		//_ptr = std::move(concreteFactory); //unique_ptr uses move semantics so you can do this

		//factories[className] = std::move(std::move(concreteFactory));
		factories->insert({concreteFactory->ClassName(), std::move(concreteFactory)});


		//Unique ptr
		/*
		Add(unique_ptr<IFactory>&& ptr)
		{
			_ptr = std::move(ptr); //unique_ptr uses move semantics so you can do this
			factories[className] = std::move(ptr);
		}
		
		
		*/
	}

	template<typename BaseType>
	inline void FactoryManager<BaseType>::Remove(const std::string& productName)
	{
		//factories->removeByKey(product name)
		factories->erase(productName);
	}



	template<typename BaseType>
	inline void FactoryManager<BaseType>::init()
	{
		factories = new std::unordered_map<std::string, unique_ptr<IFactory<BaseType>>>();
	}

	template<typename BaseType>
	inline void FactoryManager<BaseType>::cleanup()
	{
		factories->clear();
		delete(factories);
	}

	template<typename BaseType>
	inline IFactory<BaseType>* Fiea::GameEngine::FactoryManager<BaseType>::Find(const std::string& className)
	{
		assert(factories != nullptr);
		auto it = factories->find(className);
		if (it != factories->end())
		{
			return it->second.get();
		}

		return nullptr;
	}

	template<typename BaseType>
	inline BaseType* FactoryManager<BaseType>::Create(const std::string& className)
	{
		return factories->at(className)->Create();
	}
}
