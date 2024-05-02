#pragma once
#include "ParseCoordinator.h"

namespace Fiea::GameEngine
{
	class IParseHandler
	{
	public:
		/**
		 * @brief  (virtual) Initialize this helper. 
		 * This will get called just before each file is parsed... 
		 * allows the helper to initialize state, if necessary
		*/
		virtual void Initialize(); //This is set to virtual so that the children can inherit base functinoality, expedted to reset

		/**
		 * @brief (pure virtual) Given a wrapper reference, a string for the Json key, a reference to the Json::Value object, and a bool indicating if the value is an array element, attempt to handle the key/value pair.
		 * If this routine does indeed handle the pair, return true, otherwise return false.
		*/
		virtual bool Start(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, const std::string& jsonKey, Json::Value object, bool isArrayElement) = 0;

		/**
		 * @brief (pure virtual) Given a wrapper reference, a string for the Json key, attempt to complete the handling of the element pair. 
		 * If this routine does indeed handle the pair, return true, otherwise return false. 
		*/
		virtual bool End(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, Json::Value object, const std::string& jsonKey) = 0;

		/**
		 * @brief Hint: What qualifier should the destructor have?
		*/
		virtual void Cleanup();
	};
}