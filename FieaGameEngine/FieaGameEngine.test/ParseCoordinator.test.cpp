#include "pch.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "ParseCoordinator.h"
#include "IntWrapper.h"
#include "IntegerHelper.h"
#include "Datum.h"
#include "Scope.h"
#include "ScopeWrapper.h"
#include "ScopeHelper.h"
#include "TestParseHandler.h"
#include <iostream>
#include "Factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;
using namespace std::string_literals;
using namespace Fiea::GameEngine::test;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<glm::vec4>(const glm::vec4& v)
	{
		//RETURN_WIDE_STRING(t.getAsString); //use ToString when you define it
		std::string vecString = glm::to_string(v);
		RETURN_WIDE_STRING(std::wstring(vecString.begin(), vecString.end()));
	}

	template<>
	std::wstring ToString<glm::mat4x4>(const glm::mat4x4& v)
	{
		std::string matString = glm::to_string(v);

		RETURN_WIDE_STRING(std::wstring(matString.begin(), matString.end()));
	}

}

namespace Fiea::GameEngine::Test
{
	TEST_CLASS(ParseCoordinatorTests)
	{

	public:


		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			FactoryManager<Scope>::init();

		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			FactoryManager<Scope>::cleanup();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}


		TEST_METHOD(ParseIntTest)
		{
			std::string simpleInt = R"({
                    "int":1
            })"s;

			int health = 0;
			std::shared_ptr<IntWrapper> iWrapper = std::make_shared<IntWrapper>(IntWrapper(&health)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<IntegerHelper> intHandler = std::make_shared<IntegerHelper>(IntegerHelper()); //Create int handler
			pc.AddHelper(intHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(simpleInt); //Parse the string
			Assert::IsTrue(health == 1);
		}

		TEST_METHOD(ParseIntArrayTest)
		{
			std::string IntArray = R"({"intArray": [1, 2, 3, 4, 5]})"s;

			std::vector<int> intList;
			std::shared_ptr<IntWrapper> iWrapper = std::make_shared<IntWrapper>(IntWrapper(&intList)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<IntegerHelper> intHandler = std::make_shared<IntegerHelper>(IntegerHelper()); //Create int handler
			pc.AddHelper(intHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(IntArray); //Parse the string
			Assert::IsTrue(intList.size() == 5);
			Assert::IsTrue(intList[0] == 1);
			Assert::IsTrue(intList[1] == 2);
			Assert::IsTrue(intList[2] == 3);
			Assert::IsTrue(intList[3] == 4);
			Assert::IsTrue(intList[4] == 5);
		}

		TEST_METHOD(ParseIntTable)
		{
			//std::string nested = R"({
//                 "int":1,
//                 "obj":{
//                     "int":1,
//                     "obj":{
//                         "int":1,
//                         "str":"abc"
//                     },
//                     "str":"abc"
//                 },
//                 "obj2":{
//                     "int":1,
//                     "obj":{
//                         "int":1,
//                         "str":"abc"
//                     },
//                     "str":"abc"
//                 },
//             "str":"abc"
//         })"s;


			std::string nested = R"({
				  "intValue": 42,
				  "intValue2": 85,
				  "intArray": [1, 2, 3],
				  "{Scope}nestedObject": {
					"nestedIntValue": 10,
					"nestedIntArray": [4, 5, 6]
				  }
				})"s;

			Scope s(1);
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(&s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(nested); //Parse the string

			Assert::IsTrue(s.Find("intValue")->at<int>(0) == 42);
			Assert::IsTrue(s.Find("intValue2")->at<int>(0) == 85);
			Assert::IsTrue(s.Find("intArray") != nullptr);

			Assert::IsTrue(s.Find("intArray")->at<int>(0) == 1);
			Assert::IsTrue(s.Find("intArray")->at<int>(1) == 2);
			Assert::IsTrue(s.Find("intArray")->at<int>(2) == 3);

			Assert::IsTrue(s.Find("nestedObject") != nullptr);

			Datum* d = s.Find("nestedObject");
		
			Scope* b = d->at<Scope*>(0);

			Assert::IsTrue(b->Find("nestedIntValue") != nullptr);
			Assert::AreEqual(b->Find("nestedIntValue")->at<int>(0) , 10);

			Assert::AreEqual(b->Find("nestedIntArray")->at<int>(0), 4);
			Assert::AreEqual(b->Find("nestedIntArray")->at<int>(1), 5);
			Assert::AreEqual(b->Find("nestedIntArray")->at<int>(2), 6);			
		}

		TEST_METHOD(TestTableWithNesting)
		{
			std::string nested = R"({
				  "{Scope}nestedObject": {
					"nestedIntValue": 10
				  }
				})"s;

			Scope s(1);
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(&s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(nested); //Parse the string

			Assert::IsTrue(s.Find("nestedObject") != nullptr);
		}

		TEST_METHOD(ParseTableAllTypes)
		{
			std::string nested = R"-({
				"intValue": 42,
				"floatValue": 3.14,
				"stringValue": "Hello, world!",
				"intArray": [1, 2, 3],
				"floatArray": [4.5, 6.7, 8.9],
				"stringArray": ["foo", "bar", "baz"],
				"vec4Value": "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
				"mat4Value": "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
				"{Scope}nestedObject": {
					"nestedIntValue": 10,
					"nestedFloatValue": 2.718,
					"nestedStringValue": "Nested string",
					"nestedVec4Varr": [
							"vec4(1.000000, 2.000000, 3.000000, 4.000000)",
							"vec4(1.000000, 2.000000, 3.000000, 4.000000)"
						],
					"nestedMat4Varr": [
								"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
								"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"
								]
						}
					})-"s;

			Scope s(1);
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(&s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			Assert::ExpectException<std::runtime_error>([&] { pc.DeserializeObject("INVALID PATH"); });

			pc.DeserializeObject(nested); //Parse the string

			//Singular values
			Assert::AreEqual(s.Find("intValue")->at<int>(0), 42);
			Assert::AreEqual(s.Find("floatValue")->at<float>(0), 3.14f);
			Assert::AreEqual(s.Find("stringValue")->at<std::string>(0), std::string("Hello, world!"));

			//Int array
			Assert::AreEqual(s.Find("intArray")->at<int>(0), 1);
			Assert::AreEqual(s.Find("intArray")->at<int>(1), 2);
			Assert::AreEqual(s.Find("intArray")->at<int>(2), 3);

			//Float array
			Assert::AreEqual(s.Find("floatArray")->at<float>(0), 4.5f);
			Assert::AreEqual(s.Find("floatArray")->at<float>(1), 6.7f);
			Assert::AreEqual(s.Find("floatArray")->at<float>(2), 8.9f);

			//String array
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(0), std::string("foo"));
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(1), std::string("bar"));
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(2), std::string("baz"));

			//Vec4
			Assert::AreEqual(s.Find("vec4Value")->at<glm::vec4>(0), glm::vec4(1.0, 2.0, 3.0, 4.0));

			//Mat4x4
			Assert::AreEqual(s.Find("mat4Value")->at<glm::mat4x4>(0), glm::mat4x4(1.0));

			//Inner Scope
			Assert::IsTrue(s.Find("nestedObject") != nullptr);
			Datum* d = s.Find("nestedObject");
			Scope* b = d->at<Scope*>(0);

			Assert::AreEqual(b->Find("nestedIntValue")->at<int>(0), 10);
			Assert::AreEqual(b->Find("nestedFloatValue")->at<float>(0), 2.718f);
			Assert::AreEqual(b->Find("nestedStringValue")->at<std::string>(0), std::string("Nested string"));
			Assert::AreEqual(b->Find("nestedVec4Varr")->at<glm::vec4>(0), glm::vec4(1.0, 2.0, 3.0, 4.0));
			Assert::AreEqual(b->Find("nestedVec4Varr")->at<glm::vec4>(1), glm::vec4(1.0, 2.0, 3.0, 4.0));

			Assert::AreEqual(b->Find("nestedMat4Varr")->at<glm::mat4x4>(0), glm::mat4x4(1.0));
			Assert::AreEqual(b->Find("nestedMat4Varr")->at<glm::mat4x4>(1), glm::mat4x4(1.0));

		}

		//TEST_METHOD(TestEquals)
		//{
		//	std::string simpleInt = R"({
  //                  "int":1
  //          })"s;

		//	int health = 0;
		//	std::shared_ptr<IntWrapper> iWrapper = std::make_shared<IntWrapper>(IntWrapper(&health)); //Place int into wrapper
		//	ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

		//	std::shared_ptr<IntegerHelper> intHandler = std::make_shared<IntegerHelper>(IntegerHelper()); //Create int handler
		//	pc.AddHelper(intHandler);//Add int handler into parsecoordinator

		//	pc.DeserializeObject(simpleInt); //Parse the string
		//	Assert::IsTrue(health == 1);

		//	std::shared_ptr<IntWrapper> iWrapper2 = std::make_shared<IntWrapper>(IntWrapper(&health));

		//	Assert::IsTrue(pc.GetWrapper()->Equals(iWrapper.get()));

		//}

		TEST_METHOD(JPTestCode)
		{
			std::string simple = R"({
            "int":5,
            "str":"abc"
            })"s;

			std::shared_ptr<TestParseHandler::Wrapper> wrapper = std::make_shared<TestParseHandler::Wrapper>();
			ParseCoordinator parser(wrapper);

			std::shared_ptr<TestParseHandler> parseHandler = std::make_shared<TestParseHandler>();
			parser.AddHelper(parseHandler);

			parser.DeserializeObject(simple);

			Assert::IsTrue(parseHandler.get()->initializeCalled);
			Assert::IsTrue(parseHandler.get()->cleanupCalled);
			Assert::AreEqual((size_t)2, parseHandler.get()->endCount);
			Assert::AreEqual((size_t)2, parseHandler.get()->startCount);
			Assert::AreEqual((size_t)1, wrapper->maxDepth);
		}

		TEST_METHOD(TestGetWrapper)
		{
			//non const
			std::string simpleInt = R"({
                    "int":1
            })"s;

			int health = 0;
			std::shared_ptr<IntWrapper> iWrapper = std::make_shared<IntWrapper>(IntWrapper(&health)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<IntegerHelper> intHandler = std::make_shared<IntegerHelper>(IntegerHelper()); //Create int handler
			pc.AddHelper(intHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(simpleInt); //Parse the string
			Assert::IsTrue(health == 1);

			Assert::IsTrue(pc.GetWrapper() == iWrapper);

			//const 
			const ParseCoordinator& pcRef = pc;
			Assert::IsTrue(pcRef.GetWrapper() == iWrapper);
		}

		TEST_METHOD(TestDeserializeFromFile)
		{
			Scope s(1);
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(&s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			 //Parse the string
			Assert::ExpectException<std::runtime_error>([&] { pc.DeserializeObjectFromFile("INVALID PATH"); });

			pc.DeserializeObjectFromFile("FullJson.json");

			//Singular values
			Assert::AreEqual(s.Find("intValue")->at<int>(0), 42);
			Assert::AreEqual(s.Find("floatValue")->at<float>(0), 3.14f);
			Assert::AreEqual(s.Find("stringValue")->at<std::string>(0), std::string("Hello, world!"));

			//Int array
			Assert::AreEqual(s.Find("intArray")->at<int>(0), 1);
			Assert::AreEqual(s.Find("intArray")->at<int>(1), 2);
			Assert::AreEqual(s.Find("intArray")->at<int>(2), 3);

			//Float array
			Assert::AreEqual(s.Find("floatArray")->at<float>(0), 4.5f);
			Assert::AreEqual(s.Find("floatArray")->at<float>(1), 6.7f);
			Assert::AreEqual(s.Find("floatArray")->at<float>(2), 8.9f);

			//String array
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(0), std::string("foo"));
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(1), std::string("bar"));
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(2), std::string("baz"));

			//Vec4
			Assert::AreEqual(s.Find("vec4Value")->at<glm::vec4>(0), glm::vec4(1.0, 2.0, 3.0, 4.0));

			//Mat4x4
			Assert::AreEqual(s.Find("mat4Value")->at<glm::mat4x4>(0), glm::mat4x4(1.0));

			//Inner Scope
			Assert::IsTrue(s.Find("nestedObject") != nullptr);
			Datum* d = s.Find("nestedObject");
			Scope* b = d->at<Scope*>(0);

			Assert::AreEqual(b->Find("nestedIntValue")->at<int>(0), 10);
			Assert::AreEqual(b->Find("nestedFloatValue")->at<float>(0), 2.718f);
			Assert::AreEqual(b->Find("nestedStringValue")->at<std::string>(0), std::string("Nested string"));
			Assert::AreEqual(b->Find("nestedVec4Varr")->at<glm::vec4>(0), glm::vec4(1.0, 2.0, 3.0, 4.0));
			Assert::AreEqual(b->Find("nestedVec4Varr")->at<glm::vec4>(1), glm::vec4(1.0, 2.0, 3.0, 4.0));

			Assert::AreEqual(b->Find("nestedMat4Varr")->at<glm::mat4x4>(0), glm::mat4x4(1.0));
			Assert::AreEqual(b->Find("nestedMat4Varr")->at<glm::mat4x4>(1), glm::mat4x4(1.0));
		}
		
		TEST_METHOD(TestDeserializeFromStream)
		{
			std::stringstream jsonStream;
			jsonStream << R"-({
				"intValue": 42,
				"floatValue": 3.14,
				"stringValue": "Hello, world!",
				"intArray": [1, 2, 3],
				"floatArray": [4.5, 6.7, 8.9],
				"stringArray": ["foo", "bar", "baz"],
				"vec4Value": "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
				"mat4Value": "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
				"{Scope}nestedObject": {
					"nestedIntValue": 10,
					"nestedFloatValue": 2.718,
					"nestedStringValue": "Nested string",
					"nestedVec4Varr": [
							"vec4(1.000000, 2.000000, 3.000000, 4.000000)",
							"vec4(1.000000, 2.000000, 3.000000, 4.000000)"
						],
					"nestedMat4Varr": [
								"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
								"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"
								]
						}
					})-"s;

			Scope s(1);
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(&s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(jsonStream); //Parse the string

			//Singular values
			Assert::AreEqual(s.Find("intValue")->at<int>(0), 42);
			Assert::AreEqual(s.Find("floatValue")->at<float>(0), 3.14f);
			Assert::AreEqual(s.Find("stringValue")->at<std::string>(0), std::string("Hello, world!"));

			//Int array
			Assert::AreEqual(s.Find("intArray")->at<int>(0), 1);
			Assert::AreEqual(s.Find("intArray")->at<int>(1), 2);
			Assert::AreEqual(s.Find("intArray")->at<int>(2), 3);

			//Float array
			Assert::AreEqual(s.Find("floatArray")->at<float>(0), 4.5f);
			Assert::AreEqual(s.Find("floatArray")->at<float>(1), 6.7f);
			Assert::AreEqual(s.Find("floatArray")->at<float>(2), 8.9f);

			//String array
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(0), std::string("foo"));
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(1), std::string("bar"));
			Assert::AreEqual(s.Find("stringArray")->at<std::string>(2), std::string("baz"));

			//Vec4
			Assert::AreEqual(s.Find("vec4Value")->at<glm::vec4>(0), glm::vec4(1.0, 2.0, 3.0, 4.0));

			//Mat4x4
			Assert::AreEqual(s.Find("mat4Value")->at<glm::mat4x4>(0), glm::mat4x4(1.0));

			//Inner Scope
			Assert::IsTrue(s.Find("nestedObject") != nullptr);
			Datum* d = s.Find("nestedObject");
			Scope* b = d->at<Scope*>(0);

			Assert::AreEqual(b->Find("nestedIntValue")->at<int>(0), 10);
			Assert::AreEqual(b->Find("nestedFloatValue")->at<float>(0), 2.718f);
			Assert::AreEqual(b->Find("nestedStringValue")->at<std::string>(0), std::string("Nested string"));
			Assert::AreEqual(b->Find("nestedVec4Varr")->at<glm::vec4>(0), glm::vec4(1.0, 2.0, 3.0, 4.0));
			Assert::AreEqual(b->Find("nestedVec4Varr")->at<glm::vec4>(1), glm::vec4(1.0, 2.0, 3.0, 4.0));

			Assert::AreEqual(b->Find("nestedMat4Varr")->at<glm::mat4x4>(0), glm::mat4x4(1.0));
			Assert::AreEqual(b->Find("nestedMat4Varr")->at<glm::mat4x4>(1), glm::mat4x4(1.0));

		}
				
		TEST_METHOD(TestRemoveHelpers)
		{
			//non const
			std::string simpleInt = R"({
                    "int":1
            })"s;

			int health = 0;
			std::shared_ptr<IntWrapper> iWrapper = std::make_shared<IntWrapper>(IntWrapper(&health)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<IntegerHelper> intHandler = std::make_shared<IntegerHelper>(IntegerHelper()); //Create int handler
			pc.AddHelper(intHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(simpleInt); //Parse the string
			Assert::IsTrue(health == 1);

			pc.RemoveHelper(intHandler);
		}



	private:
		inline static _CrtMemState _startMemState;
	};
}


