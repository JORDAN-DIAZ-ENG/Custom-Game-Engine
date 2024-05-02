#pragma once

#include "pch.h"
#include <vector>
#include <memory>
#include "json/json.h"
#include "RTTI.h"

class IHandler {};
using std::shared_ptr;
using std::weak_ptr;
using std::unique_ptr;
//Order is important

//MostSpecificHandler
//LessSpecificHandler
//DefaultHandler


namespace Fiea::GameEngine
{
	class IParseHandler;

	class ParseCoordinator {
	public:
		/**
		 * @brief This class will represent data that all helpers share with each other and with the coordinator.
		*/
		class Wrapper : public RTTI
		{

			RTTI_DECLARATIONS(Wrapper, RTTI);


			//You should use the lightweight homebrew RTTI schemes covered in lecture, to verify the object type on each access.
		public:
			/**
			 * @brief increment a “nesting depth” counter. This gets incremented upon the start of each element.
			*/
			void IncrementDepth();

			/**
			 * @brief decrement a “nesting depth” counter. This gets decremented upon the end of each element.
			*/
			void DecrementDepth();

			/**
			 * @brief return the current nesting depth.
			*/
			size_t Depth() const;

			//Hint: What qualifier should the destructor have?

			//RTTI Overrides
			virtual bool Equals(const RTTI* rhs) const override;

			
		private:

			size_t _depth{ 0 };
		};

	public:

		/**
		 * @brief given a reference to a Wrapper object, initialize this object.
		*/
		ParseCoordinator(std::shared_ptr<Wrapper> incomingWrapper);

		/**
		 * @brief Destructor
		*/
		~ParseCoordinator();

		/**
		 * @brief No Copy yet explicitly delete, for clarity of intent
		 * @param other
		*/
		ParseCoordinator(const ParseCoordinator& other) = delete;

		/**
		* @brief No Assignment yet explicitly delete, for clarity of intent
		* @param other
		*/
		ParseCoordinator& operator=(const ParseCoordinator& rhs) = delete;

		/**
		* @brief No Move Semantics yet explicitly delete, for clarity of intent
		* @param other
		*/
		ParseCoordinator(ParseCoordinator&& other) = delete;

		/**
		* @brief No Move yet explicitly delete, for clarity of intent
		* @param other
		*/
		ParseCoordinator& operator=(ParseCoordinator&& rhs) = delete;

		/**
		 * @brief given a reference to an IParseHelper object, add it to the list.
		 * @param helper 
		*/
		void AddHelper(std::shared_ptr<IParseHandler> handler);

		/**
		 * @brief  given a reference to an IParseHelper object, remove it from the list.
		*/
		void RemoveHelper(std::shared_ptr<IParseHandler> handler);

		/**
		 * @brief a string of Json data.
		*/
		void DeserializeObject(const std::string& jsonString);

		void DeserializeObject(std::istream& jsonInputStreamData);

		/**
		 * @brief given a filename, read in the file and parse it.
		 * @param filename 
		*/
		void DeserializeObjectFromFile(const std::string& filename);

		/**
		 * @brief return the address of the Wrapper associated with this object.
		 * @return 
		*/
		std::shared_ptr<Wrapper>& GetWrapper();

		const std::shared_ptr<Wrapper>& GetWrapper() const;

		/**
		 * @brief create wrapper, if needed
		*/
		void SetWrapper();

		/**
 * @brief Retrieves the members from a Json::Value object and Parse's each of them.
*/
		void ParseMembers(Json::Value object);

		/**
		 * @brief Given a key/value pair (string/Json::Value) and a bool indicating if the value is an element of an array, trigger the chain of responsibility.
		 * In other words, walk through the list of helpers and invoke the StartHander methods until one responds that it has handled the pair.
		 * The handler that responded true to a StartHandler call should be the same handler that accepts the EndHandler call.
		 * This method is where most of the work resides.
		 * You'll need to give careful thought to how this method should handle values that are objects, arrays of objects, or normal (non-object, non-array-of-object) key/value pairs.
		 * If the value is an object or an array of objects you'll need to "descend" (i.e. recursively parse) into the child name/value pairs.
		*/
		void Parse(std::string key, Json::Value object, bool isElementOfArray);

	private:

		std::shared_ptr<Wrapper> _wrapper;
		std::vector<std::shared_ptr<IParseHandler>> _handlers;





		//	HandlerRef AddHandler(unique_ptr<IHandler>&& handler)
		//	{
		//		_handlers.push_back(std::move(std::move(handler)));
		//	}
		//
		//	void RemoveHandler(HandlerRef ref)
		//	{
		//
		//	}
		//
		//private:
		//	std::vector<shared_ptr<IHandler>> _handlers;
		//	//PriorityList<IHandler*> _ph;

	};

	//EXAMPLE OF PROBLEM
	//THIS WOULD CAUSE A PROBLEM WHERE THE SHARED POINTERS ARE LOOKING AT EACHOTHER AND CANNOT BE DELETED
	//class HandlerA : public IHandler
	//{
	//public:
	//	shared_ptr<ParseCoordinator> _coordinator;
	//};

	////END OF EXAMPLE OF PROBLEM
	//
	//main()
	//{
	//	ParseCoordinator p;
	//	//HandlerA* a = new HandlerA();
	//	//shared_ptr<IHandler> ptr(a);
	//	//p.AddHandler(ptr);
	//	p.AddHandler(std::make_unique<HandlerA>(/*ctor parms for Handler A*/));
	//
	//	//delete a;
	//
	//	//p.ParseStuff();
	//}
}