/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea::GameEngine::test
{
	class TestParseHandler final : public IParseHandler {
	public:
		class Wrapper : public ParseCoordinator::Wrapper {
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

		};
	public:

		void Initialize() override;
		void Cleanup() override;

		//Jordan Methods

		//JP Methods
		//bool Start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;
		//bool End(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount{ 0 };
		size_t endCount{ 0 };

		// Inherited via IParseHandler
		bool Start(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, const std::string& jsonKey, Json::Value object, bool isArrayElement) override;
		bool End(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, Json::Value object, const std::string& jsonKey) override;
	};
}
