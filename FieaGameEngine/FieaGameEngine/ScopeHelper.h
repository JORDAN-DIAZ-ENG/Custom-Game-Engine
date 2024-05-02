#pragma once
#include "IParseHandler.h"

namespace Fiea::GameEngine
{
	class ScopeHelper : public IParseHandler
	{

		// Inherited via IParseHandler

		void Initialize() override;

		void Cleanup() override;

		// Inherited via IParseHandler
		bool Start(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, const std::string& jsonKey, Json::Value object, bool isArrayElement) override;
		bool End(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, Json::Value object, const std::string& jsonKey) override;

	};

}