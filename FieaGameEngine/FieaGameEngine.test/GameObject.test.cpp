#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "empty.h"
#include "emptyChild.h"
#include "Foo.h"
#include "Hero.h"

#include "ScopeWrapper.h"
#include "ScopeHelper.h"

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
	TEST_CLASS(GameObjectTests)
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

		TEST_METHOD(TestGameObjectTransformBinding)
		{
			GameObject g;
			g.setPosition(glm::vec4(1));
			g.setRotation(glm::vec4(1));
			g.setScale(glm::vec4(1));
			g.setName("Test");

		}

		TEST_METHOD(HeroBindingFromJson)
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

			Assert::IsTrue(h->getName() == "Mario");
			Assert::IsTrue(h->getHealth() == 10);
			Assert::IsTrue(h->getTransform().Position == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));
			Assert::IsTrue(h->getTransform().Rotation == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));
			Assert::IsTrue(h->getTransform().Scale == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));

			//Hero* child1 = h->Find("_children")->at<Scope*>(0)->As<Hero>();

			Hero* luigi = h->getChild("Luigi");
			Assert::IsTrue(luigi->getName() == "Luigi");
			Assert::IsTrue(luigi->getHealth() == 3);
			Assert::IsTrue(luigi->getTransform().Position == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));
			Assert::IsTrue(luigi->getTransform().Rotation == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));
			Assert::IsTrue(luigi->getTransform().Scale == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));

			Hero* Bowser = h->getChild("Bowser");
			Assert::IsTrue(Bowser->getName() == "Bowser");
			Assert::IsTrue(Bowser->getHealth() == 100);
			Assert::IsTrue(Bowser->getTransform().Position == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));
			Assert::IsTrue(Bowser->getTransform().Rotation == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));
			Assert::IsTrue(Bowser->getTransform().Scale == glm::vec4(1.000000, 2.000000, 3.000000, 4.000000));

			GameClock clock;
			GameTime time = clock.Current();

			h->Update(time);
			Assert::IsTrue(h->getName() == "PEACH");
			Assert::IsTrue(luigi->getName() == "PEACH");
			Assert::IsTrue(Bowser->getName() == "PEACH");


			delete s;
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}


