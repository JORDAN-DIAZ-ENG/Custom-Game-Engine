#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "empty.h"
#include "emptyChild.h"
#include "Foo.h"
#include "TypeManager.h"
#include"Factory.h"
#include "GameObject.h"
#include "Hero.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include"ScopeWrapper.h"
#include"ScopeHelper.h"
#include "GameClock.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine::Test;

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
	TEST_CLASS(ActionTests)
	{

	public:


		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif

			FactoryManager<Scope>::init();
			FactoryManager<Scope>::Add(std::move(std::make_unique<GameObjectFactory>()));
			FactoryManager<Scope>::Add(std::move(std::make_unique<HeroFactory>()));
			FactoryManager<Scope>::Add(std::move(std::make_unique<ActionListFactory>()));
			FactoryManager<Scope>::Add(std::move(std::make_unique<ActionListWhileFactory>()));
			FactoryManager<Scope>::Add(std::move(std::make_unique<ActionIncrementFactory>()));
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

		TEST_METHOD(ActionTest)
		{
			std::string jsonString = R"-({
				"{Hero}BaseHero" : {
						"_name" : "Mario",
						"_pos" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
						"_rot" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
						"_scale" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
						"_heroHealth" : 10,
						"{Hero}_children" : [
							{
								"_name" : "Bowser",
								"_pos" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
								"_rot" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
								"_scale" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
								"_heroHealth" : 100
							},
							{
								"_name" : "Luigi",
								"_pos" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
								"_rot" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
								"_scale" : "vec4(1.000000, 2.000000, 3.000000, 4.000000)",
								"_heroHealth" : 3
							}

						]
					}
				})-"s;

			Scope* s = new Scope();
			std::shared_ptr<ScopeWrapper> iWrapper = std::make_shared<ScopeWrapper>(ScopeWrapper(s)); //Place int into wrapper
			ParseCoordinator pc(iWrapper); //Place wrapper into parseCoordinator

			std::shared_ptr<ScopeHelper> scopeHandler = std::make_shared<ScopeHelper>(ScopeHelper()); //Create int handler
			pc.AddHelper(scopeHandler);//Add int handler into parsecoordinator

			pc.DeserializeObject(jsonString); //Parse the string

			Hero* h = s->Find("BaseHero")->at<Scope*>(0)->As<Hero>();

			Assert::AreEqual(h->Find("_actions")->size(), (size_t)0);


			//h->CreateAction("ActionIncrement", "action0");


			////Test Action Increment
			//h->Find("_actions")->at<Scope*>(0)->As<ActionIncrement>()->_datumKey = "_heroHealth";
			//h->Find("_actions")->at<Scope*>(0)->As<ActionIncrement>()->_incrementBy = 2.5f;

			////update the actions
			//h->Update(0);

			//Assert::AreEqual(h->Find("_heroHealth")->at<int>(0), 12);

			////Test ActionList
			//h->CreateAction("ActionList", "action1");
			//h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->AppendAction("ActionIncrement", "action1.1");
			//h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->AppendAction("ActionIncrement", "action1.2");

			//Assert::IsTrue(h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->Find("_Actions") != nullptr);
			//Assert::IsTrue(h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->Find("_Actions")->size() == (size_t)2);


			//h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->Find("_Actions")->at<Scope*>(0)->As<ActionIncrement>()->_datumKey = "_heroHealth";
			//h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->Find("_Actions")->at<Scope*>(0)->As<ActionIncrement>()->_incrementBy = 1.0f;

			//h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->Find("_Actions")->at<Scope*>(1)->As<ActionIncrement>()->_datumKey = "_heroHealth";
			//h->Find("_actions")->at<Scope*>(0)->As<ActionList>()->Find("_Actions")->at<Scope*>(1)->As<ActionIncrement>()->_incrementBy = 1.0f;

			//h->Update(0);
	
			//Assert::AreEqual(h->Find("_heroHealth")->at<int>(0), 12);

			h->CreateAction("ActionListWhile", "action2");

			ActionIncrement* preamble = new ActionIncrement();
			preamble->_datumKey = "_heroHealth";
			preamble->_incrementBy = 1.0f;
			
			ActionIncrement* increment = new ActionIncrement();
			increment->_datumKey = "_heroHealth";
			increment->_incrementBy = 1.0f;

			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->setPreamble(preamble);
			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->setIncrement(increment);
			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->setCondition(10);
			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->AppendAction("ActionIncrement", "theActionInTheList");
			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->Find("_Actions")->at<Scope*>(0)->As<ActionIncrement>()->_datumKey = "_heroHealth";
			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->Find("_Actions")->at<Scope*>(0)->As<ActionIncrement>()->_incrementBy = 10.0f;
			h->Find("_actions")->at<Scope*>(0)->As<ActionListWhile>()->Find("_Actions")->at<Scope*>(0)->As<ActionIncrement>()->setParentGameObject(h);


			GameClock clock;
			GameTime time = clock.Current();

			h->Update(time);

			Assert::AreEqual(h->Find("_heroHealth")->at<int>(0), 121);

			delete s;
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}


