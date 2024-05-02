#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "Scope.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

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
	TEST_CLASS(ScopeTests)
	{

	public:


		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
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

		TEST_METHOD(TestRemoveAt)
		{
			Datum a;
			Scope* s = new Scope();
			Scope* ss = new Scope();
			a.addToBack(s);
			a.addToBack(ss);

			a.removeAt(0);
			a.removeAt(0);

			
			Assert::AreEqual(a.size(), (size_t)0);

			delete s;
			delete ss;
		}

		//SCOPE TESTS
		TEST_METHOD(TestConstructionandDestruction)
		{
			//Default Construction
			{
				Scope defaultScope;
				Assert::IsTrue(defaultScope.Find("Non existing key") == nullptr);
			}
		}

		TEST_METHOD(TestFind)
		{
			{
				Scope defaultScope(1);

				//Const
				const Scope& ref = defaultScope;

				Assert::IsTrue(ref.Find("THIS IS NOT A KEY") == nullptr);
				Assert::IsTrue(defaultScope.Find("THIS IS NOT A KEY") == nullptr);

				defaultScope.Append("key1");




				Assert::AreEqual(*defaultScope.Find("key1"), Datum());
				Assert::IsTrue(defaultScope.Find("THIS IS NOT A KEY") == nullptr);


				Assert::AreEqual(*ref.Find("key1"), Datum());
				Assert::IsTrue(ref.Find("THIS IS NOT A KEY") == nullptr);
			}
		}

		TEST_METHOD(TestAppend)
		{
			{
				//Create a default scope
				Scope defaultScope(1);

				//Append to create a key that doesn't exist
				Assert::IsTrue(defaultScope.Find("Mario") == nullptr);
				defaultScope.Append("Mario"); //Mario now exists
				Assert::IsFalse(defaultScope.Find("Mario") == nullptr);

				//Appending a key that already exists
				Assert::AreEqual(defaultScope.Append("Mario"), Datum());


			}
		}

		TEST_METHOD(TestAccessOperator)
		{
			//Append version
			{
				//Create a default scope and test that find works
				Scope defaultScope(1);
				defaultScope.Append("key1");
				Assert::AreEqual(*defaultScope.Find("key1"), Datum());
				Assert::IsTrue(defaultScope.Find("THIS IS NOT A KEY") == nullptr);

				//Append to create a key that doesn't exist
				Assert::IsTrue(defaultScope.Find("Mario") == nullptr);
				defaultScope["Mario"]; //Mario now exists
				Assert::IsFalse(defaultScope.Find("Mario") == nullptr);

				//Appending a key that already exists
				Assert::AreEqual(defaultScope["Mario"], Datum());
			}

			//Index version
			{
				//Create a default scope with no elements
				Scope defaultScope;
				defaultScope.Append("first"); //Add datum with key first

				Assert::AreEqual(defaultScope[0], Datum());

				//const
				const Scope& sRef = defaultScope;
				Assert::AreEqual(sRef[0], Datum());

			}
		}

		TEST_METHOD(TestComparison)
		{
			Scope scope1;
			Scope scope2;

			//make sure they are the same
			scope1.Append("Similar keys");
			scope1.Append("Similar keys1");
			//scope1.AppendScope("Similar Scope");

			scope2.Append("Similar keys");
			scope2.Append("Similar keys1");
			//scope2.AppendScope("Similar Scope");


			Assert::IsTrue(scope1 == scope2);
			Assert::IsFalse(scope1 != scope2);


			//make them separate
			scope2.Append("NOT SIMILAR");
			Assert::IsTrue(scope1 != scope2);
			Assert::IsFalse(scope1 == scope2);

			//Close but not quite
			scope1.Append("DIFFERENT");
			Assert::IsTrue(scope1 != scope2);
			Assert::IsFalse(scope1 == scope2);

		}

		TEST_METHOD(TestGetParent)
		{
			Scope a;
			Scope* b = new Scope();
			a.Adopt("adopted key", *b);
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == &a);
		}

		TEST_METHOD(TestAppendScope)
		{
			Scope* a = new Scope(); //base scope
			Scope* b = &a->AppendScope("ScopeChild");
			Scope* c = &a->AppendScope("ScopeChild2");

			Assert::AreEqual(Datum::DatumType::Table, a->Find("ScopeChild")->getDecidedType());

			delete a;
		}

		TEST_METHOD(TestAdopt)
		{
			Scope a;
			Scope* b = new Scope();
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == nullptr);

			//Adopt a non parented datum
			a.Adopt("adopted key", *b);
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == &a);


			//Adopt a parented scope
			Scope c;
			c.Adopt("Snatched datum", *b);
			Assert::IsTrue(b->GetParent() != &a);
			Assert::IsTrue(b->GetParent() == &c);


			Assert::ExpectException<std::exception>([&] { c.Adopt("adopted key" ,*b); });

		}

		TEST_METHOD(TestSearch)
		{
			//Key Exists
			{
				Scope a;
				Scope* b = new Scope();
				Scope* c = new Scope();
				Scope* d = new Scope();

				a.Adopt("Child1", *b);
				b->Adopt("Child2", *c);

				Assert::AreSame(a, *b->GetParent());

				b->Adopt("Child3", *d);

				//Assert::AreSame(b, *d.GetParent());
				//Assert::AreSame(d, b.Search("Child3"));

				Assert::AreSame(*a.Find("Child1"), *c->Search("Child1"));

				//Key doesn't exist
				Assert::IsTrue(nullptr == c->Search("THIS DOES NOT EXIST"));

				Scope** ss = new Scope*();
				Assert::AreEqual(*a.Find("Child1"), *c->Search("Child1", ss));

				Assert::IsTrue(a == **ss);
				delete ss;

				//const

				const Scope* const& cRef = c;
				const Scope** ssRef = new const Scope*();
				Assert::AreEqual(*a.Find("Child1"), *cRef->Search("Child1", ssRef));
				Assert::IsTrue(nullptr == cRef->Search("THIS DOES NOT EXIST"));

				
				delete ssRef;

			}

			



		}


		TEST_METHOD(TestIsDescendant)
		{
			Scope a;
			Scope* b = new Scope();
			Scope* c = new Scope();
			Scope* d = new Scope();

			a.Adopt("child1", *b);
			b->Adopt("child2", *c);
			c->Adopt("child2", *d);

			Assert::AreSame(a, *b->GetParent());
			Assert::AreSame(*b, *c->GetParent());
			Assert::AreSame(*c, *d->GetParent());

			Assert::IsTrue(b->isDescendantOf(a));
			Assert::IsTrue(c->isDescendantOf(a));
			Assert::IsTrue(d->isDescendantOf(a));

			Scope z;
			Assert::IsFalse(z.isDescendantOf(a));
		}

		TEST_METHOD(TestIsAncestor)
		{
			Scope a;
			Scope* b = new Scope();
			Scope* c = new Scope();
			Scope* d = new Scope();

			a.Adopt("child1", *b);
			b->Adopt("child2", *c);
			c->Adopt("child3", *d);

			Assert::AreSame(a, *b->GetParent());
			Assert::AreSame(*b,*c->GetParent());
			Assert::AreSame(*c, *d->GetParent());

			Assert::IsTrue(a.isAncestorOf(*b));
			Assert::IsTrue(a.isAncestorOf(*c));
			Assert::IsTrue(a.isAncestorOf(*d));

			Scope z;
			Assert::IsFalse(a.isAncestorOf(z));


		}

		TEST_METHOD(TestOrphan)
		{
			Scope a; 
			Scope* b = new Scope();
			b->AppendScope("Troublemaker");
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == nullptr);

			a.Adopt("adopted key", *b);
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == &a);

			//Become an orphan
			Scope* orphanedChild = a.Orphan(*b);
			Assert::IsTrue(orphanedChild == b);
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == nullptr);

			//Already an orphan
			a.Orphan(*b);
			Assert::IsTrue(a.GetParent() == nullptr);
			Assert::IsTrue(b->GetParent() == nullptr);

			delete b;
		}

		TEST_METHOD(FindContainedScope)
		{
			Scope a;
			Scope &returnVal = a.AppendScope("first");
			int i = 0;
			Datum d = *a.FindContainedScope(returnVal, i);

			Assert::AreEqual(i, 0);

			Scope b; 
			b.Append("NOT A SCOPE");
			b.FindContainedScope(returnVal, i);
		}

		TEST_METHOD(CopyConstructor)
		{
			Scope parent;
			parent.AppendScope("Scope Child");
			parent.AppendScope("Scope Child2");
			parent.Append("Non Scope");

			//Scope* copy = new Scope(parent);
			Scope copy = parent;

			Assert::AreNotSame(parent, copy);
			//Assert::AreEqual(parent, copy);

			parent.AppendScope("Scope Child");

			Scope copy2 = parent;
			
		}

		TEST_METHOD(AssignmentOperator)
		{
			Scope parent;
			parent.AppendScope("Scope Child");
			parent.AppendScope("Scope Child2");
			parent.Append("Non Scope");

			//Scope* copy = new Scope(parent);
			Scope copy;
			copy = parent;

			Assert::AreNotSame(parent, copy);
			//Assert::AreEqual(parent, copy);




			parent.AppendScope("Scope Child");

			copy = copy; // Nothing is done here
			Assert::AreNotSame(parent, copy);

			Scope copy2;
			copy2 = parent;

		}

			private:
				inline static _CrtMemState _startMemState;
	};
}


