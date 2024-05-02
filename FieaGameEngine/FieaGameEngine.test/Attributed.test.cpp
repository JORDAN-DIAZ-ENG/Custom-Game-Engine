#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "Scope.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "TypeManager.h"



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

	template<>
	std::wstring ToString<Attributed>(const Attributed& f)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString<Attributed>(const Attributed* f)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString<Attributed>(Attributed* f)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString <AttributedFoo> (const AttributedFoo& f)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString <AttributedBar> (const AttributedBar& b)
	{
		RETURN_WIDE_STRING("");
	}
}

namespace Fiea::GameEngine::Test
{
	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(Constructor)
		{
			//Default Contribute
			{
				AttributedFoo f;
				f.externalInteger = 54;
				Datum* a = f.Find("this");
				Assert::IsTrue(a != nullptr);
				Datum* d = f.Find(std::string("externalInteger"));
				Assert::AreEqual(d->at<int>(0), f.externalInteger);

			}


		}



		TEST_METHOD(CopyConstructor)
		{
			//Copy constructor
			AttributedFoo f;
			f.externalInteger = 54;
			AttributedFoo b = f;

			//Comparisons
			Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
			Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
			Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
			Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
			Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

			for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
			{
				Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
			{
				Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
			{
				Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
			{
				Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
			{
				Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
			}
			Assert::IsTrue(b == f); //fs external integer was set to be different
			Assert::IsFalse(b != f); //fs external integer was set to be different
			Assert::AreNotSame(b, f); //fs external integer was set to be different
		}

		TEST_METHOD(TestIsAttribute)
		{
			//IsAttribute
			{
				//IsAttribute
				AttributedFoo f;

				Assert::IsTrue(f.isAttribute(std::string("this")));
				Assert::IsTrue(f.isAttribute(std::string("externalInteger")));

				Assert::IsFalse(f.isAttribute(std::string("MARIO HEHE")));
			}

		}

		TEST_METHOD(TestIsAuxiliary)
		{
			//IsAuxiliary
			AttributedFoo f;


			Assert::IsTrue(f.isAuxiliaryAttribute("this"));

			Assert::IsFalse(f.isAuxiliaryAttribute("externalInteger"));
			Assert::IsFalse(f.isAuxiliaryAttribute("MARIO HEHE"));

		}

		TEST_METHOD(TestIsPrescribed)
		{
			//IsPrescribed
			{
				AttributedFoo f;

				//IsPrescribed
				Assert::IsTrue(f.isPrescribedAttribute("externalInteger"));

				Assert::IsFalse(f.isPrescribedAttribute("MARIO HEHE"));
			}

		}

		TEST_METHOD(TestAppenAuxiliaryAttribute)
		{
			AttributedFoo f;
			f.AppendAuxiliaryAttribute("SUPER MARIO");
			Assert::ExpectException<std::exception>([&] {f.AppendAuxiliaryAttribute("externalInteger"); });
			Assert::IsTrue(f.isAuxiliaryAttribute("SUPER MARIO"));
			Assert::IsFalse(f.isPrescribedAttribute("SUPER MARIO"));
			Assert::IsTrue(f.isAttribute("SUPER MARIO"));

		}

		TEST_METHOD(TestAssignment)
		{
			AttributedFoo f;
			f.externalInteger = 54;
			AttributedFoo b;

			b = f;

			b = b; // Nothing happens

			//Comparisons
			Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
			Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
			Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
			Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
			Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

			for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
			{
				Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
			{
				Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
			}
			
			for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
			{
				Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
			}
						
			for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
			{
				Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
			}
									
			for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
			{
				Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
			}

			Assert::IsTrue(b == f); //fs external integer was set to be different
			//Assert::IsFalse(b != f); //fs external integer was set to be different
			//Assert::AreNotSame(b, f); //fs external integer was set to be different

		}


		TEST_METHOD(TestMoveAssignment)
		{
			AttributedFoo f;
			f.externalInteger = 54;
			AttributedFoo b;

			b = std::move(f);

			//Comparisons
			Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
			Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
			Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
			Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
			Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

			for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
			{
				Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
			{
				Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
			{
				Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
			{
				Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
			{
				Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
			}
			Assert::IsTrue(b == f); //fs external integer was set to be different
			Assert::IsFalse(b != f); //fs external integer was set to be different
			Assert::AreNotSame(b, f); //fs external integer was set to be different
		}

		TEST_METHOD(TestMoveConstructor)
		{
			AttributedFoo f;
			f.externalInteger = 54;
			AttributedFoo b = std::move(f);

			//Comparisons
			Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
			Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
			Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
			Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
			Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

			for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
			{
				Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
			{
				Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
			{
				Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
			{
				Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
			}

			for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
			{
				Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
			}
			Assert::IsTrue(b != f); //NOT THE SAME
			Assert::AreNotSame(b, f); //fs external integer was set to be different
		}

		TEST_METHOD(TestComparison)
		{
			AttributedFoo f;
			AttributedFoo d;

			//Comparisons
			Assert::IsTrue(d == f); //fs external integer was set to be different
			Assert::IsFalse(d != f); //fs external integer was set to be different
			Assert::AreNotSame(d, f); //fs external integer was set to be different

			f.externalInteger = 54;
			Assert::IsTrue(d != f); //fs external integer was set to be different
			d.externalInteger = 54;
			Assert::IsTrue(d == f); //fs external integer was set to be different

			f.externalFloat = 3.0f;
			Assert::IsTrue(d != f); //fs external integer was set to be different
			d.externalFloat = 3.0f;
			Assert::IsTrue(d == f); //fs external integer was set to be different

			f.externalString = "Mario";
			Assert::IsTrue(d != f); //fs external integer was set to be different
			d.externalString = "Mario";
			Assert::IsTrue(d == f); //fs external integer was set to be different

			f.externalVector = glm::vec4(3.0f);
			Assert::IsTrue(d != f); //fs external integer was set to be different
			d.externalVector = glm::vec4(3.0f);
			Assert::IsTrue(d == f); //fs external integer was set to be different

			f.externalMatrix = glm::mat4x4(3.0f);
			Assert::IsTrue(d != f); //fs external integer was set to be different
			d.externalMatrix = glm::mat4x4(3.0f);
			Assert::IsTrue(d == f); //fs external integer was set to be different

		}

		TEST_METHOD(TestClone)
		{
			AttributedFoo f;

			Attributed* a = f.Clone();
			Assert::IsTrue(a->Is(Attributed::TypeIdClass()));

			delete a;
		}

		TEST_METHOD(TestDeepInheritance)
		{
			//Constructor
			{
				//AttributedFoo f;
				//f.externalInteger = 54;
				//Datum* a = f.Find("this");
				//Assert::IsTrue(a != nullptr);
				//Datum* d = f.Find(std::string("externalInteger"));
				//Assert::AreEqual(d->at<int>(0), f.externalInteger);

				AttributedBar b;
				b.externalInteger = 54;
				Datum* a = b.Find("this");
				Assert::IsTrue(a != nullptr);
				Datum* d = b.Find(std::string("externalInteger"));
				Assert::AreEqual(d->at<int>(0), b.externalInteger);
			}

			//Copy Construct
			{
				//Copy constructor
				AttributedBar f;
				f.externalInteger = 54;
				AttributedBar b = f;

				//Comparisons
				Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
				Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
				Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
				Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
				Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact


				Assert::IsTrue(b.Find("barSpecificExternalInt") != nullptr);
				Assert::AreEqual(b.barSpecificExternalInt, b.Append("barSpecificExternalInt").at<int>(0));


				for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
				{
					Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
				{
					Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
				{
					Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
				{
					Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
				{
					Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
				}
				Assert::IsTrue(b == f); //fs external integer was set to be different
				Assert::IsFalse(b != f); //fs external integer was set to be different
				Assert::AreNotSame(b, f); //fs external integer was set to be different
			}

			//Append Auxiliary Attribute
			{
				AttributedBar f;
				f.AppendAuxiliaryAttribute("SUPER MARIO");
				Assert::ExpectException<std::exception>([&] {f.AppendAuxiliaryAttribute("externalInteger"); });
				Assert::IsTrue(f.isAuxiliaryAttribute("SUPER MARIO"));
				Assert::IsFalse(f.isPrescribedAttribute("SUPER MARIO"));
				Assert::IsTrue(f.isAttribute("SUPER MARIO"));
			}

			//Assignment
			{
				AttributedBar f;
				f.externalInteger = 54;
				AttributedBar b;

				b = f;

				b = b; // Nothing happens

				//Comparisons
				Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
				Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
				Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
				Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
				Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

				for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
				{
					Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
				{
					Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
				{
					Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
				{
					Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
				{
					Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
				}

				Assert::IsTrue(b == f); //fs external integer was set to be different
				//Assert::IsFalse(b != f); //fs external integer was set to be different
				//Assert::AreNotSame(b, f); //fs external integer was set to be different
			}

			//Clone
			{
				AttributedBar f;

				Attributed* a = f.Clone();
				Assert::IsTrue(a->Is(Attributed::TypeIdClass()));

				delete a;
			}

			//Comparison
			{
				AttributedBar f;
				AttributedBar d;

				//Comparisons
				Assert::IsTrue(d == f); //fs external integer was set to be different
				Assert::IsFalse(d != f); //fs external integer was set to be different
				Assert::AreNotSame(d, f); //fs external integer was set to be different

				f.externalInteger = 54;
				Assert::IsTrue(d != f); //fs external integer was set to be different
				d.externalInteger = 54;
				Assert::IsTrue(d == f); //fs external integer was set to be different

				f.externalFloat = 3.0f;
				Assert::IsTrue(d != f); //fs external integer was set to be different
				d.externalFloat = 3.0f;
				Assert::IsTrue(d == f); //fs external integer was set to be different

				f.externalString = "Mario";
				Assert::IsTrue(d != f); //fs external integer was set to be different
				d.externalString = "Mario";
				Assert::IsTrue(d == f); //fs external integer was set to be different

				f.externalVector = glm::vec4(3.0f);
				Assert::IsTrue(d != f); //fs external integer was set to be different
				d.externalVector = glm::vec4(3.0f);
				Assert::IsTrue(d == f); //fs external integer was set to be different

				f.externalMatrix = glm::mat4x4(3.0f);
				Assert::IsTrue(d != f); //fs external integer was set to be different
				d.externalMatrix = glm::mat4x4(3.0f);
				Assert::IsTrue(d == f); //fs external integer was set to be different
			}

			//Move Assignment
			{
				AttributedBar f;
				f.externalInteger = 54;
				AttributedBar b = std::move(f);

				//Comparisons
				Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
				Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
				Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
				Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
				Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

				for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
				{
					Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
				{
					Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
				{
					Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
				{
					Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
				{
					Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
				}
				Assert::IsTrue(b != f); //NOT THE SAME
				Assert::AreNotSame(b, f); //fs external integer was set to be different
			}

			//Move Construction
			{
				AttributedBar f;
				f.externalInteger = 54;
				AttributedBar b = std::move(f);

				//Comparisons
				Assert::AreEqual(b.externalInteger, b.Append("externalInteger").at<int>(0)); // Data is in tact
				Assert::AreEqual(b.externalFloat, b.Append("externalFloat").at<float>(0)); // Data is in tact
				Assert::AreEqual(b.externalString, b.Append("externalString").at<std::string>(0)); // Data is in tact
				Assert::AreEqual(b.externalVector, b.Append("externalVector").at<glm::vec4>(0)); // Data is in tact
				Assert::AreEqual(b.externalMatrix, b.Append("externalMatrix").at<glm::mat4x4>(0)); // Data is in tact

				for (size_t i = 0; i < b.Append("externalIntegerArray").size(); ++i)
				{
					Assert::AreEqual(b.externalIntegerArray[i], b.Append("externalIntegerArray").at<int>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalFloatArray").size(); ++i)
				{
					Assert::AreEqual(b.externalFloatArray[i], b.Append("externalFloatArray").at<float>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalStringArray").size(); ++i)
				{
					Assert::AreEqual(b.externalStringArray[i], b.Append("externalStringArray").at<std::string>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorArray").size(); ++i)
				{
					Assert::AreEqual(b.externalVectorArray[i], b.Append("externalVectorArray").at<glm::vec4>(i)); // Data is in tact
				}

				for (size_t i = 0; i < b.Append("externalVectorMatrix").size(); ++i)
				{
					Assert::AreEqual(b.externalMatrixArray[i], b.Append("externalVectorMatrix").at<glm::mat4x4>(i)); // Data is in tact
				}
				Assert::IsTrue(b != f); //NOT THE SAME
				Assert::AreNotSame(b, f); //fs external integer was set to be different
			}

		}


	private:
		inline static _CrtMemState _startMemState;
	};
}


