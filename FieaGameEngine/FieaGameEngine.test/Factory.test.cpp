#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "empty.h"
#include "emptyChild.h"
#include "Foo.h"

#include "Factory.h"
#include "Monster.h"
#include <memory>

#include "ScopeHelper.h"
#include "ScopeWrapper.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine::Test;

using namespace std::string_literals;


using std::unique_ptr;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Datum>(const Datum& t)
	{
		//RETURN_WIDE_STRING(t.getAsString); //use ToString when you define it
		RETURN_WIDE_STRING("Mario");
	}

	template<>
	std::wstring ToString<Datum::DatumType>(const Datum::DatumType& t)
	{
		switch (t)
		{
		case Datum::DatumType::Integer:
			return L"Integer";
		case Datum::DatumType::Float:
			return L"Float";
		case Datum::DatumType::String:
			return L"String";
		case Datum::DatumType::Vector4:
			return L"Vector4";
		case Datum::DatumType::Matrix4x4:
			return L"Matrix4x4";
		default:
			return L"Unknown";
		}
	}

	template<>
	std::wstring ToString<glm::vec4>(const glm::vec4& v)
	{
		//RETURN_WIDE_STRING(t.getAsString); //use ToString when you define it
		RETURN_WIDE_STRING("Mario");
	}

	template<>
	std::wstring ToString<glm::mat4x4>(const glm::mat4x4& v)
	{
		//RETURN_WIDE_STRING(t.getAsString); //use ToString when you define it
		RETURN_WIDE_STRING("Mario");
	}


	template<>
	std::wstring ToString<Scope>(const Scope& s)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString<Scope>(const Scope* s)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString<Scope>(Scope* s)
	{
		RETURN_WIDE_STRING("");
	}

}

namespace Fiea::GameEngine::Test
{
	TEST_CLASS(FactoryTests)
	{

	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);

#endif

			//write code under here

			FactoryManager<Scope>::init();
			FactoryManager<Scope>::Add(std::move(std::make_unique<MonsterFactory>()));
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			//Write code up here
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


		TEST_METHOD(TestSimpleFactory)
		{

			//Init

			IFactory<Scope>* monsterFactory = FactoryManager<Scope>::Find("Monster");

			Monster* newMonster1 = monsterFactory->Create()->As<Monster>();

			Assert::IsTrue(newMonster1->intValue == 0);

			delete(newMonster1);
		}

		TEST_METHOD(TestScopeGrammar)
		{
			//non const
			//std::string jsonString = R"-({
			//	"intValue": 42,
			//	"floatValue": 3.14,
			//	"stringValue": "Hello, world!",
			//	"intArray": [1, 2, 3],
			//	"floatArray": [4.5, 6.7, 8.9],
			//	"stringArray": ["foo", "bar", "baz"],
			//	"vec4Value": "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
			//	"mat4Value": "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
				//"{Scope}nestedObject": {
				//	"nestedIntValue": 10,
				//	"nestedFloatValue": 2.718,
				//	"nestedStringValue": "Nested string",
					//"nestedVec4Varr": [
					//		"vec4(1.000000, 2.000000, 3.000000, 4.000000)",
					//		"vec4(1.000000, 2.000000, 3.000000, 4.000000)"
					//	],
					//"nestedMat4Varr": [
					//			"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
					//			"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"
					//			]
					//	}
				//	})-"s;

			/*
			"{Scope}GenericScope" : {
						"genericInt" : 5
					}
			*/


			/*
			* ,
					"floatArr": [4.5, 6.7],
					"stringArr": ["foo", "bar"]
			*/
			std::string jsonString = R"-({
				"{Monster}Mike" : {
					"intValue": 42,
					"floatValue": 3.14,
					"stringValue": "Hello, world!",
					"vec4Value": "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
					"mat4Value": "mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
					"intArr": [1, 2],
					"floatArr": [4.5, 7.0],
					"stringArr": ["foo", "bar"],
					"vec4Arr": [
							"vec4(1.000000, 2.000000, 3.000000, 4.000000)",
							"vec4(1.000000, 2.000000, 3.000000, 4.000000)"
						],
					"mat4x4Arr": [
								"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))",
								"mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"
								]
						}
				}

			})-"s;

			Scope* s = new Scope();
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(jsonString); //Parse the string

			Monster* m = s->Find("Mike")->at<Scope*>(0)->As<Monster>();

			int intVal = m->intValue;
			float floatVal = m->floatValue;
			std::string stringVal = m->stringValue;
			glm::vec4 vec4Value = m->vec4Value;
			glm::mat4x4 mat4Value = m->mat4Value;
			//Scope* scopeVal = m->genericScope/*.Find("GenericScope")->at<Scope*>(0)*/;
			//Scope scopeVal = m->genericScope;



			Assert::AreEqual(intVal, 42);
			Assert::AreEqual(floatVal, 3.14f);
			Assert::AreEqual(stringVal, std::string("Hello, world!"));
			Assert::AreEqual(vec4Value, glm::vec4(1.0, 2.0, 3.0, 4.0));
			Assert::AreEqual(mat4Value, glm::mat4x4(1.0));

			Assert::AreEqual(m->intArr[0], 1);
			Assert::AreEqual(m->intArr[1], 2);

			Assert::AreEqual(m->floatArr[0], 4.5f);
			Assert::AreEqual(m->floatArr[1], 7.0f);
			
			Assert::AreEqual(m->stringArr[0], std::string("foo"));
			Assert::AreEqual(m->stringArr[1], std::string("bar"));

			Assert::AreEqual(m->vec4Arr[0], glm::vec4(1.0, 2.0, 3.0, 4.0));
			Assert::AreEqual(m->vec4Arr[1], glm::vec4(1.0, 2.0, 3.0, 4.0));
			
			Assert::AreEqual(m->mat4x4Arr[0], glm::mat4x4(1.0));
			Assert::AreEqual(m->mat4x4Arr[1], glm::mat4x4(1.0));

			

			delete s;

		}




	private:
		inline static _CrtMemState _startMemState;
	};
}


