#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "empty.h"
#include "emptyChild.h"
#include "Foo.h"

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
	TEST_CLASS(RTTITests)
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


		TEST_METHOD(isTest)
		{
			Foo f;
			Empty empty;
			EmptyChild emptyChild;

			Assert::AreEqual(true, emptyChild.Is(Empty::TypeIdClass()));
			Assert::AreEqual(true, emptyChild.Is(EmptyChild::TypeIdClass()));
			Assert::AreEqual(true, empty.Is(Empty::TypeIdClass()));
			Assert::AreEqual(true, empty.Is(Empty::TypeIdClass()));
			Assert::AreEqual(true, f.Is(Foo::TypeIdClass()));


			Assert::AreEqual(false, empty.Is(EmptyChild::TypeIdClass()));
			Assert::AreEqual(false, f.Is(EmptyChild::TypeIdClass()));
			Assert::AreEqual(false, f.Is(Empty::TypeIdClass()));
		}

		TEST_METHOD(asTest)
		{
			Foo f;
			Empty empty;
			EmptyChild emptyChild;

			EmptyChild* childFTest = f.As<EmptyChild>();
			Assert::IsTrue(childFTest == nullptr);

			Empty* fTest = f.As<Empty>();
			Assert::IsTrue(fTest == nullptr);

			Empty* TestEmpty = emptyChild.As<Empty>();
			Assert::IsTrue(TestEmpty != nullptr);
		}


	private:
		inline static _CrtMemState _startMemState;
	};
}


