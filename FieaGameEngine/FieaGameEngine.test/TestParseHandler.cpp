#include "pch.h"
#include "TestParseHandler.h"

namespace Fiea::GameEngine::test
{
#pragma region TestParseHandler::Wrapper
	RTTI_DEFINITIONS(TestParseHandler::Wrapper);

	/**
	 * @brief RTTI override for Equals
	 * @param rhs
	 * @return true if equal, false otherwise
	*/
	bool TestParseHandler::Wrapper::Equals(const RTTI* rhs) const {
		if (rhs == nullptr) return false;

		const TestParseHandler::Wrapper* other = rhs->As<TestParseHandler::Wrapper>();
		return maxDepth == other->maxDepth;
	}

	/**
	 * @brief RTTI override for ToString
	 * @return string representation of this Wrapper
	*/
	std::string TestParseHandler::Wrapper::ToString() const {
		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
	}
#pragma endregion TestParseHandler::Wrapper

#pragma region TestParseHandler
	/**
	 * @brief Override for IParseHandler::Initialize
	*/
	void TestParseHandler::Initialize() {
		initializeCalled = true;
	}

	/**
	 * @brief Override for IParseHandler::Cleanup
	*/
	void TestParseHandler::Cleanup() {
		cleanupCalled = true;
	}

	//JORDAN METHODS
	bool TestParseHandler::Start(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, const std::string& jsonKey, Json::Value object, bool isArrayElement)
	{
		TestParseHandler::Wrapper* testWrapper = incomingWrapper->As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) return false;

		if (testWrapper->Depth() >= testWrapper->maxDepth) {
			testWrapper->maxDepth = testWrapper->Depth();
		}
		++startCount;
		return true;
		////shared_ptr<TestParseHandler::Wrapper> testWrapper(incomingWrapper.get()->As<TestParseHandler::Wrapper>());

		//Wrapper* testWrapper = incomingWrapper->As<TestParseHandler::Wrapper>();

		////shared_ptr<TestParseHandler::Wrapper> testWrapper = std::make_shared<TestParseHandler::Wrapper>(incomingWrapper->As<TestParseHandler::Wrapper>());

		//if (testWrapper == nullptr) return false;

		//if (testWrapper->Depth() >= testWrapper->maxDepth) {
		//	testWrapper->maxDepth = testWrapper->Depth();
		//}
		//++startCount;
		//return true;
	}

	bool TestParseHandler::End(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, Json::Value object, const std::string& jsonKey)
	{

		Wrapper* testWrapper = incomingWrapper->As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) return false;

		++endCount;
		return true;


		//Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();

		//shared_ptr<TestParseHandler::Wrapper> testWrapper = std::make_shared<TestParseHandler::Wrapper>(incomingWrapper->As<TestParseHandler::Wrapper>());

		//if (!testWrapper) return false;

		//++endCount;
		//return true;
	}

	//JP METHODS
	///**
	// * @brief Override for IParseHandler::Start
	// * @param unused
	// * @param unused
	// * @param wrapper The wrapper to populate
	// * @return True, if handled, false otherwise
	//*/
	//bool TestParseHandler::Start(const std::string&, const Json::Value&, ParseCoordinator::Wrapper* wrapper) {
		//TestParseHandler::Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();
		//if (testWrapper == nullptr) return false;

		//if (testWrapper->Depth() >= testWrapper->maxDepth) {
		//	testWrapper->maxDepth = testWrapper->Depth();
		//}
		//++startCount;
		//return true;
	//}

	///**
	// * @brief Override for IParseHandler::End
	// * @param unused
	// * @param wrapper The wrapper to populate
	// * @return True, if handled, false otherwise
	//*/
	//bool TestParseHandler::End(const std::string&, ParseCoordinator::Wrapper* wrapper) {
		//Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();
		//if (testWrapper == nullptr) return false;

		//++endCount;
		//return true;
	//}
#pragma endregion TestParseHandler
}