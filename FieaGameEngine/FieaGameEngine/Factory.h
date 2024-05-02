#pragma once
#include <string>
#include <unordered_map>
#include <memory>

using std::unique_ptr;

namespace Fiea::GameEngine
{
	template<typename Type>
	class IFactory
	{
	public:
		/**
		 * @brief Base class override to create an instance of a class inheriting from IFactory
		 * @return new type of this type
		*/
		virtual Type* Create() const = 0;

		/**
		 * @brief The name of the class
		 * @return std::string of class name
		*/
		virtual std::string ClassName() const = 0;
	}; 

	template<typename BaseType>
	class FactoryManager
	{
	public:

		/**
		 * @brief Called on initialization
		*/
		static void init();

		/**
		 * @brief Called on cleanup
		*/
		static void cleanup();

		/**
		 * @brief Finds the IFactory that has the given name
		 * @param className name to look for
		 * @return the IFactory pointer for the given key, if not found returns nullptr
		*/
		static IFactory<BaseType>* Find(const std::string& className);

		/**
		 * @brief The base create, returns a base class 
		 * @param className the key
		 * @return the pointer to a base type
		*/
		[[nodiscard]] static BaseType* Create(const std::string& className);

		/**
		 * @brief Adds to the map of factories
		 * @param concreteFactory specific concrete factory
		*/
		static void Add(unique_ptr<IFactory<BaseType>>&& concreteFactory);

		/**
		 * @brief Removes from the map of factories
		 * @param productName the name of the product
		*/
		static void Remove(const std::string& productName);

		inline static std::unordered_map<std::string, unique_ptr<IFactory<BaseType>>>* factories;

	};



}

#define MAKE_FACTORY(_Base, _Concrete) \
	class _Concrete ## Factory : public IFactory<_Base> { \
	public: \
		_Base* Create() const override { \
			return new _Concrete(); \
		} \
		\
		std::string ClassName() const override { \
			return #_Concrete; \
		} \
	};

////Base = Scope
////Concrete = Monster
//class MonsterFactory : public IFactory<Monster>
//{
//public:
//	Monster* Create() const override { return new Monster(); };
//
//	std::string ClassName() const override { return "Monster"; };
//};
#include "Factory.inl"
