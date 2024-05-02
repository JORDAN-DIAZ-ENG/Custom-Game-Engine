#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include <stdexcept>
#include "Scope.h"
#include "empty.h"

using namespace std::string_literals;


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::GameEngine;

//custom literal operator
//allows you to say what type a variable is in the same way that float uses an f at the end
constexpr std::size_t operator "" _z(unsigned long long n) { return size_t(n); }

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
		std::string vecString = glm::to_string(v);
		RETURN_WIDE_STRING(std::wstring(vecString.begin(), vecString.end()));
	}

	template<>
	std::wstring ToString<glm::mat4x4>(const glm::mat4x4& v)
	{
		std::string matString = glm::to_string(v);

		RETURN_WIDE_STRING(std::wstring(matString.begin(), matString.end()));
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
	std::wstring ToString<RTTI>(const RTTI& rtti)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString<RTTI>(const RTTI* rtti)
	{
		RETURN_WIDE_STRING("");
	}

	template<>
	std::wstring ToString<RTTI>(RTTI* rtti)
	{
		RETURN_WIDE_STRING("");
	}

}

namespace Fiea::GameEngine::Test
{
	TEST_CLASS(FieaGameEnginetest)
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

		TEST_METHOD(DefaultConstruction)
		{
			Datum a;
			Assert::IsTrue(a.empty());
			Assert::AreEqual(a.size(), (size_t)0);
			Assert::AreEqual(a.capacity(), (size_t)0);
			Assert::AreEqual(a.getDecidedType(), Datum::DatumType::Unknown);
		}

		TEST_METHOD(SpecializedConstruction)
		{
			//Ensure that you check that on construction:
			//	-Type is not Unknown
			//	-isTypeDecided = true;
		}

		TEST_METHOD(DATUMGetTypeEnum)
		{
			Datum a;

			//Supported Types
			Assert::AreEqual(a.getTypeEnum<int>(), Datum::DatumType::Integer);
			Assert::AreEqual(a.getTypeEnum<float>(), Datum::DatumType::Float);
			Assert::AreEqual(a.getTypeEnum<std::string>(), Datum::DatumType::String);
			Assert::AreEqual(a.getTypeEnum<glm::vec4>(), Datum::DatumType::Vector4);
			Assert::AreEqual(a.getTypeEnum<glm::mat4x4>(), Datum::DatumType::Matrix4x4);
			Assert::AreEqual(a.getTypeEnum<Scope*>(), Datum::DatumType::Table);
			Assert::AreEqual(a.getTypeEnum<RTTI*>(), Datum::DatumType::Pointer);

			//Unsupported Types
			Assert::AreEqual(a.getTypeEnum<char>(), Datum::DatumType::Unknown);
			Assert::AreEqual(a.getTypeEnum<double>(), Datum::DatumType::Unknown);
			Assert::AreEqual(a.getTypeEnum<void*>(), Datum::DatumType::Unknown);
		}

		TEST_METHOD(DATUMSetter)
		{
			//Integer
			{
				Datum a(1);
				Assert::AreEqual(a.at<int>(0), 1);
				a.setItemAt(15, 0);
				Assert::AreEqual(a.at<int>(0), 15);
			}
			
			//Float
			{
				Datum a(1.0f);
				Assert::AreEqual(a.at<float>(0), 1.0f);
				a.setItemAt(15.0f, 0);
				Assert::AreEqual(a.at<float>(0), 15.0f);
			}

			//String
			{
				Datum a("hello"s);
				Assert::AreEqual(a.at<std::string>(0), "hello"s);
				a.setItemAt("Mario"s, 0);
				Assert::AreEqual(a.at<std::string>(0), "Mario"s);
			}

			//Vector4
			{
				Datum a(glm::vec4(1.0f));
				Assert::AreEqual(a.at<glm::vec4>(0), glm::vec4(1.0f));
				a.setItemAt(glm::vec4(2.0f), 0);
				Assert::AreEqual(a.at<glm::vec4>(0), glm::vec4(2.0f));
			}

			//Matrix4x4
			{
				Datum a(glm::mat4x4(1.0f));
				Assert::AreEqual(a.at<glm::mat4x4>(0), glm::mat4x4(1.0f));
				a.setItemAt(glm::mat4x4(2.0f), 0);
				Assert::AreEqual(a.at<glm::mat4x4>(0), glm::mat4x4(2.0f));
			}
		
			//Table - SCOPE
			{
				Scope* s = new Scope(); //base scope
				Scope* s2 = new Scope(); //base scope
				s2->Append("Goomba");
				Datum a(s);
				Assert::AreEqual(a.at<Scope*>(0), s);
				a.setItemAt(s2, 0);
				Assert::AreEqual(a.at<Scope*>(0), s2);

				delete s;
				delete s2;
			}

			//Pointer - RTTI
			{
				RTTI* empty = new Empty();
				RTTI* diffEmpty = new Empty();
				Datum a(empty);
				Assert::AreEqual(a.at<RTTI*>(0), empty);
				a.setItemAt(diffEmpty, 0);
				Assert::AreEqual(a.at<RTTI*>(0), diffEmpty);

				delete empty;
				delete diffEmpty;
			}

		}


		TEST_METHOD(AddToTheBack)
		{
			//Test incompatible Values
			{
				//Default Construction
				Datum a; //Type is unknown
				Assert::AreEqual(a.size(), (size_t)0);
				Assert::AreEqual(a.capacity(), (size_t)0);
				
				//Expect Exceptions when adding unsupported types L Values
				char l = 'l';
				double dub = 5.5;
				std::vector<int> v = { 1,2,3 };
				Assert::ExpectException<std::runtime_error>([&] {a.addToBack<char>(l); });
				Assert::ExpectException<std::runtime_error>([&] {a.addToBack<double>(dub); });
				Assert::ExpectException<std::runtime_error>([&] { a.addToBack<std::vector<int>>(v); });

				//Expect Exceptions when adding unsupported types R Values
				Assert::ExpectException<std::runtime_error>([&] {a.addToBack<char>('L'); });
				Assert::ExpectException<std::runtime_error>([&] {a.addToBack<double>(5.5); });
				Assert::ExpectException<std::runtime_error>([&] { a.addToBack<std::vector<int>>({ 1,2,3 }); });
				
				//additions were rejected and container did not change
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
				Assert::AreEqual(a.capacity(), (size_t)0);

				//Setting to Integer type
				a.addToBack<int>(3);

				//Check for type mismatch R Values
				float f = 3.5;
				Assert::ExpectException<std::runtime_error>([&] {a.addToBack<float>(f); });
				Assert::AreEqual(a.size(), (size_t)1);

				//Check for type mismatch L Values
				Assert::ExpectException<std::runtime_error>([&] {a.addToBack<float>(3.5f); });
				Assert::AreEqual(a.size(), (size_t)1);
			}

			//Integers - R Values
			{
				//Default 
				Datum d;
				Assert::AreEqual(d.size(), (size_t)0);
				Assert::AreEqual(d.capacity(), (size_t)0);
				Assert::IsTrue(d.empty());

				//Add to back L value
				d.addToBack(2);

				Assert::AreEqual(d.getDecidedType(), Datum::DatumType::Integer);

				//Non-Default
				Datum a(1);

				//Add to back 
				//R values
				a.addToBack<int>(3);
				a.addToBack<int>(3);
				a.addToBack<int>(3);
				a.addToBack<int>(3);
				a.addToBack<int>(5);
				Assert::AreEqual(a.at<int>(5), 5);
				Assert::AreEqual(a.size(), (size_t)6);
			}

			//Integers - L Values
			{
				int number2 = 2;
				int number3 = 3;
				int number5 = 5;
				//Default 
				Datum d;
				Assert::AreEqual(d.size(), (size_t)0);
				Assert::AreEqual(d.capacity(), (size_t)0);
				Assert::IsTrue(d.empty());

				//Add to back L value
				d.addToBack<int>(number2);
				d.addToBack<int>(number2);
				d.addToBack<int>(number2);
				d.addToBack<int>(number2);

				Assert::AreEqual(d.getDecidedType(), Datum::DatumType::Integer);

				//Non-Default
				Datum a(3);

				//Add to back 
				//R values
				a.addToBack<int>(number3);
				a.addToBack<int>(number3);
				a.addToBack<int>(number3);
				a.addToBack<int>(number3);
				a.addToBack<int>(number5);
				Assert::AreEqual(a.at<int>(5), 5);
				Assert::AreEqual(a.size(), (size_t)6);
			}

			//Float
			{
				Datum a(1.1f);
				a.addToBack<float>(3.5f);
				a.addToBack<float>(3.5f);
				a.addToBack<float>(8.8f);

				Assert::AreEqual(a.at<float>(3), 8.8f);
				Assert::AreEqual(a.size(), (size_t)4);
			}

			//String
			{
				std::string s = "Super";
				Datum a(s);
				std::string b = "Mario";
				a.addToBack<std::string>(b);
				a.addToBack<std::string>(b);
				a.addToBack<std::string>(b);
				a.addToBack<std::string>(s);

				Assert::AreEqual(a.at<std::string>(4), s);
				Assert::AreEqual(a.size(), (size_t)5);
			}

			//External move
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				Datum a;
				a.SetStorage<int>(ptr, size);
				Assert::AreEqual(a.size(), (size_t)3);
				Assert::ExpectException<std::invalid_argument>([&] {a.addToBack<int>(3); });
				free(ptr);

			}

			//External normal
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				Datum a;
				a.SetStorage<int>(ptr, size);
				Assert::AreEqual(a.size(), (size_t)3);
				int i = 3;
				Assert::ExpectException<std::invalid_argument>([&] {a.addToBack<int>(i); });
				free(ptr);
			}

		}

		TEST_METHOD(Clear)
		{
			//Integer
			{
				Datum a;
				a.addToBack<int>(3);
				a.addToBack<int>(5);
				a.addToBack<int>(6);
				Assert::AreEqual(a.size(), (size_t)3);
				a.clear();
				Assert::ExpectException<std::out_of_range>([&] {a.at<int>(0); });
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
			}

			//Float
			{
				Datum a;
				a.addToBack<float>(3.0f);
				a.addToBack<float>(3.0f);
				a.addToBack<float>(3.0f);
				Assert::AreEqual(a.size(), (size_t)3);
				a.clear();
				Assert::ExpectException<std::out_of_range>([&] {a.at<float>(0); });
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
			}

			//String
			{
				Datum a;
				a.addToBack<std::string>("Hello");
				a.addToBack<std::string>("Hello");
				a.addToBack<std::string>("Hello");
				Assert::AreEqual(a.size(), (size_t)3);
				a.clear();
				Assert::ExpectException<std::out_of_range>([&] {a.at<std::string>(0); });
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
			}

			//Vector4
			{
				Datum a;
				a.addToBack<glm::vec4>(glm::vec4(1.0f));
				a.addToBack<glm::vec4>(glm::vec4(1.0f));
				a.addToBack<glm::vec4>(glm::vec4(1.0f));
				Assert::AreEqual(a.size(), (size_t)3);
				a.clear();
				Assert::ExpectException<std::out_of_range>([&] {a.at<glm::vec4>(0); });
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
			}


			//Matrix4x4
			{
				Datum a;
				a.addToBack<glm::mat4x4>(glm::mat4x4(1.0f));
				a.addToBack<glm::mat4x4>(glm::mat4x4(1.0f));
				a.addToBack<glm::mat4x4>(glm::mat4x4(1.0f));
				Assert::AreEqual(a.size(), (size_t)3);
				a.clear();
				Assert::ExpectException<std::out_of_range>([&] {a.at<glm::mat4x4>(0); });
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
			}

			//WILL NOT WORK ON EXTERNAL
			//Table - SCOPE
			//Pointer - RTTI
		}

		TEST_METHOD(TestAt)
		{
			//Type mismatching
			{
				Datum a(1); //type int
				 //not the same type as int
				Assert::ExpectException<std::runtime_error>([&] {a.at<float>(0); });

				const Datum& aRef = a;
				Assert::ExpectException<std::runtime_error>([&] {aRef.at<float>(0); });

			}
			//Internal
			//Integer
			{
				Datum a;
				a.addToBack<int>(1);
				Assert::ExpectException<std::out_of_range>([&] {a.at<int>(10); });
				Assert::AreEqual(a.at<int>(0), 1);
				Assert::ExpectException<std::out_of_range>([&] {a.at<int>(-1); });

				//const
				const Datum& aRef = a;
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<int>(10); });
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<int>(-1); });
				Assert::AreEqual(aRef.at<int>(0), 1);

				Datum empty;
				Assert::ExpectException<std::out_of_range>([&] {empty.at<int>(0); });

				const Datum& emptyRef = empty;
				Assert::ExpectException<std::out_of_range>([&] {emptyRef.at<int>(0); });
			}

			//Float
			{
				Datum a;
				a.addToBack<float>(1.0f);
				Assert::ExpectException<std::out_of_range>([&] {a.at<float>(10); });
				Assert::AreEqual(a.at<float>(0), 1.0f);
				Assert::ExpectException<std::out_of_range>([&] {a.at<float>(-1); });

				//const
				const Datum& aRef = a;
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<float>(10); });
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<float>(-1); });
				Assert::AreEqual(aRef.at<float>(0), 1.0f);

				Datum empty;
				Assert::ExpectException<std::out_of_range>([&] {empty.at<float>(0); });

				const Datum& emptyRef = empty;
				Assert::ExpectException<std::out_of_range>([&] {emptyRef.at<float>(0); });
			}

			//String
			{
				Datum a;
				a.addToBack<std::string>("Hi"s);
				Assert::ExpectException<std::out_of_range>([&] {a.at<std::string>(10); });
				Assert::AreEqual(a.at<std::string>(0), "Hi"s);
				Assert::ExpectException<std::out_of_range>([&] {a.at<std::string>(-1); });

				//const
				const Datum& aRef = a;
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<std::string>(10); });
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<std::string>(-1); });
				Assert::AreEqual(aRef.at<std::string>(0), "Hi"s);

				Datum empty;
				Assert::ExpectException<std::out_of_range>([&] {empty.at<std::string>(0); });

				const Datum& emptyRef = empty;
				Assert::ExpectException<std::out_of_range>([&] {emptyRef.at<std::string>(0); });
			}

			//Vector4
			{
				Datum a;
				a.addToBack<glm::vec4>(glm::vec4(1.0f));
				Assert::ExpectException<std::out_of_range>([&] {a.at<glm::vec4>(10); });
				Assert::AreEqual(a.at<glm::vec4>(0), glm::vec4(1.0f));
				Assert::ExpectException<std::out_of_range>([&] {a.at<glm::vec4>(-1); });

				//const
				const Datum& aRef = a;
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<glm::vec4>(10); });
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<glm::vec4>(-1); });
				Assert::AreEqual(aRef.at<glm::vec4>(0), glm::vec4(1.0f));

				Datum empty;
				Assert::ExpectException<std::out_of_range>([&] {empty.at<glm::vec4>(0); });

				const Datum& emptyRef = empty;
				Assert::ExpectException<std::out_of_range>([&] {emptyRef.at<glm::vec4>(0); });
			}


			//Matrix4x4
			{
				Datum a;
				a.addToBack<glm::mat4x4>(glm::mat4x4(1.0f));
				Assert::ExpectException<std::out_of_range>([&] {a.at<glm::mat4x4>(10); });
				Assert::AreEqual(a.at<glm::mat4x4>(0), glm::mat4x4(1.0f));
				Assert::ExpectException<std::out_of_range>([&] {a.at<glm::mat4x4>(-1); });

				//const
				const Datum& aRef = a;
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<glm::mat4x4>(10); });
				Assert::ExpectException<std::out_of_range>([&] {aRef.at<glm::mat4x4>(-1); });
				Assert::AreEqual(aRef.at<glm::mat4x4>(0), glm::mat4x4(1.0f));

				Datum empty;
				Assert::ExpectException<std::out_of_range>([&] {empty.at<glm::mat4x4>(0); });

				const Datum& emptyRef = empty;
				Assert::ExpectException<std::out_of_range>([&] {emptyRef.at<glm::mat4x4>(0); });
			}

			//External
			

			//Table - SCOPE
			{
				//Scope s;

				//Datum a;
				//a.addToBack<Scope**>(glm::mat4x4(1.0f));
				//Assert::ExpectException<std::out_of_range>([&] {a.at<glm::mat4x4>(10); });
				//Assert::AreEqual(a.at<glm::mat4x4>(0), glm::mat4x4(1.0f));
				//Assert::ExpectException<std::out_of_range>([&] {a.at<glm::mat4x4>(-1); });

				////const
				//const Datum& aRef = a;
				//Assert::ExpectException<std::out_of_range>([&] {aRef.at<glm::mat4x4>(10); });
				//Assert::ExpectException<std::out_of_range>([&] {aRef.at<glm::mat4x4>(-1); });
				//Assert::AreEqual(aRef.at<glm::mat4x4>(0), glm::mat4x4(1.0f));

				//Datum empty;
				//Assert::ExpectException<std::out_of_range>([&] {empty.at<glm::mat4x4>(0); });

				//const Datum& emptyRef = empty;
				//Assert::ExpectException<std::out_of_range>([&] {emptyRef.at<glm::mat4x4>(0); });
			}

			//Pointer - RTTI


		}

		TEST_METHOD(MoveAssign)
		{
			Datum a(4); //int

			Datum b(3.5f); //float

			Assert::AreEqual(b.getDecidedType(), Datum::DatumType::Float);

			a = std::move(b); //b cannot be used anymore

			Assert::AreEqual(a.getDecidedType(), Datum::DatumType::Float);
			Assert::AreEqual(a.at<float>(0), 3.5f);
		}

		TEST_METHOD(CopyAssignment)
		{
			//external
			{
				Datum a;
				Datum b(3);
				size_t size = 3;

				int* ptr = (int*)malloc(sizeof(int) * size);

				Assert::ExpectException<std::invalid_argument>([&] {
					Datum b(3);
					a.SetStorage<int>(ptr, size);
					b = a;

					});
				free(ptr);


			}

			//lhs = INTERNAL; rhs = INTERNAL
			//Empty Datum copy assignment
			{
				Datum a(1);
				Assert::AreEqual(a.at<int>(0), 1);
				Assert::AreEqual(a.size(), (size_t)1);

				Datum b;
				Assert::IsTrue(b.empty());

				b = a;
				Assert::AreEqual(a, b);
				Assert::AreNotSame(a, b);
				Assert::IsFalse(b.empty());
			}

			//Two empty Datums
			{
				Datum a;
				Datum b;
				a = b;
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
			}

			//Different types
			{
				Datum a(5);
				Datum b(5.5f);
				
				a = b; //convert a to a float
				Assert::AreEqual(a, b);
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a.getDecidedType(), b.getDecidedType());
			}

			//string
			{
				Datum a(std::string("hi"));
				Assert::AreEqual(a.at<std::string>(0), std::string("hi"));
				Assert::AreEqual(a.size(), (size_t)1);

				Datum b;
				Assert::IsTrue(b.empty());

				b = a;
				Assert::AreEqual(a, b);
				Assert::AreNotSame(a, b);
				Assert::IsFalse(b.empty());
			}

			//Vector
			{
				Datum a(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(a.at<glm::vec4>(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(a.size(), (size_t)1);

				Datum b;
				Assert::IsTrue(b.empty());

				b = a;
				Assert::AreEqual(a, b);
				Assert::AreNotSame(a, b);
				Assert::IsFalse(b.empty());
			}

			//Matrix
			{
				Datum a(glm::mat4x4(1.0f));
				Assert::AreEqual(a.at<glm::mat4x4>(0), glm::mat4x4(1.0f));
				Assert::AreEqual(a.size(), (size_t)1);

				Datum b;
				Assert::IsTrue(b.empty());

				b = a;
				Assert::AreEqual(a, b);
				Assert::AreNotSame(a, b);
				Assert::IsFalse(b.empty());
			}
			
		}

		TEST_METHOD(CopyConstructor)
		{
			//external
			{
				Datum a;
				size_t size = 3;

				int* ptr = (int*)malloc(sizeof(int) * size);
				a.SetStorage<int>(ptr, size);


				Datum b(a);


				free(ptr);


			}

			//lhs = internal; rhs = internal
			//Copy
			{
				Datum a(5);
				a.addToBack(4);
				a.addToBack(3);

				Datum b = a; //Calls copy constructor
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
			}

			//Empty Copy Construction
			{
				Datum a;
				Datum b(a);
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
				Assert::AreEqual(a.getDecidedType(), b.getDecidedType());
				Assert::IsTrue(a.empty() && b.empty());
			}

			//float
			{
				Datum a(5.5f);
				a.addToBack(4.3f);
				a.addToBack(3.2f);

				Datum b = a; //Calls copy constructor
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
			}


			//string
			{
				Datum a(std::string("h"));
				a.addToBack(std::string("h"));
				a.addToBack(std::string("h"));

				Datum b = a; //Calls copy constructor
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
			}

			//Vector
			{
				Datum a(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				a.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				a.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

				Datum b = a; //Calls copy constructor
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
			}

			//Matrix
			{
				Datum a(glm::mat4x4(1.0f));
				a.addToBack(glm::mat4x4(1.0f));
				a.addToBack(glm::mat4x4(1.0f));

				Datum b = a; //Calls copy constructor
				Assert::AreNotSame(a, b);
				Assert::AreEqual(a, b);
			}

			//External
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				Datum a;
				Datum b;
				a.SetStorage<int>(ptr, size);
				b.SetStorage<int>(ptr, size);
				Assert::AreEqual(a.size(), (size_t)3);

				a = b;

				free(ptr);
			}

		}

		TEST_METHOD(MoveConstructor)
		{
			Datum a(5);
			Datum c = a; //normal deep copy

			Datum b(std::move(a)); //cannot use a after this

			Assert::AreNotSame(c, b);
		}



		TEST_METHOD(RemoveFromBack)
		{
			//Case, removing from an empty
			{
				Datum a;
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
				a.removeFromBack(); //Do nothing
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);

			}

			//Integers
			{
				Datum a(1);
				Assert::IsFalse(a.empty());
				Assert::AreEqual(a.size(), (size_t)1);
				Assert::AreEqual(a.capacity(), (size_t)1);
				a.removeFromBack(); //Remove one element
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.size(), (size_t)0);
				Assert::AreEqual(a.capacity(), (size_t)1);

				//can add after
				a.addToBack<int>(5);
				Assert::IsFalse(a.empty());
				Assert::AreEqual(a.size(), (size_t)1);
				Assert::AreEqual(a.capacity(), (size_t)1);
			}

			//String
			{
				Datum s;
				s.addToBack<std::string>(std::string(""));
				s.removeFromBack();
			}
		}

		TEST_METHOD(RESIZE)
		{
			//Shrinking

			//External
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				Datum a;
				a.SetStorage<int>(ptr, size);
				Assert::AreEqual(a.size(), (size_t)3);
				Assert::ExpectException<std::invalid_argument>([&] {a.resize(3); });

				free(ptr);
			}

			//Resize to 0
			{
				Datum a(1);
				a.addToBack(1);
				a.resize(0);
				Assert::IsTrue(a.empty());
				Assert::AreEqual(a.getDecidedType(), Datum::DatumType::Integer);

				Datum b(std::string("hello"));
				b.addToBack(std::string("bye"));
				b.resize(0);
				Assert::IsTrue(b.empty());
				Assert::AreEqual(b.getDecidedType(), Datum::DatumType::String);
			}
			
			//Integer
			{
				Datum i(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<int>(3), 1);

				i.resize(3);
				Assert::AreEqual(i.size(), (size_t)3);
				Assert::AreEqual(i.capacity(), (size_t)3);
				Assert::ExpectException<std::out_of_range>([&] {i.at<int>(3); });
			}

			//Float
			{
				Datum f(5.5f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				Assert::AreEqual(f.size(), (size_t)5);
				Assert::AreEqual(f.capacity(), (size_t)8);
				Assert::AreEqual(f.at<float>(3), 1.1f);

				f.resize(3);
				Assert::AreEqual(f.size(), (size_t)3);
				Assert::AreEqual(f.capacity(), (size_t)3);
				Assert::ExpectException<std::out_of_range>([&] {f.at<float>(3); });
			}

			//String
			{
				Datum s(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
	
				Assert::AreEqual(s.size(), (size_t)5);
				Assert::AreEqual(s.capacity(), (size_t)8);
				Assert::AreEqual(s.at<std::string>(3), static_cast<std::string>("hello"));

				s.resize(3);
				Assert::AreEqual(s.size(), (size_t)3);
				Assert::AreEqual(s.capacity(), (size_t)3);
				Assert::ExpectException<std::out_of_range>([&] {s.at<std::string>(3); });
			}

			//Vector4
			{
				glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
				//glm::vec4 vec;
				Datum v(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(v.size(), (size_t)5);
				Assert::AreEqual(v.capacity(), (size_t)8);
				Assert::AreEqual(v.at<glm::vec4>(3), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));

				v.resize(3);
				Assert::AreEqual(v.size(), (size_t)3);
				Assert::AreEqual(v.capacity(), (size_t)3);
				Assert::ExpectException<std::out_of_range>([&] {v.at<glm::vec4>(3); });
			}

			//Matrix4x4
			{
				Datum m(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				Assert::AreEqual(m.size(), (size_t)5);
				Assert::AreEqual(m.capacity(), (size_t)8);
				Assert::AreEqual(m.at<glm::mat4x4>(3), glm::mat4x4(1.0f));

				m.resize(3);
				Assert::AreEqual(m.size(), (size_t)3);
				Assert::AreEqual(m.capacity(), (size_t)3);
				Assert::ExpectException<std::out_of_range>([&] {m.at<glm::mat4x4>(3); });
			}

			//shrinking to fit the same size

			//Integer
			{
				Datum i(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreNotEqual(i.size(), i.capacity());

				i.resize(i.size());
				Assert::AreEqual(i.size(), i.capacity());
			}

			//Float
			{
				Datum f(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				Assert::AreEqual(f.size(), (size_t)5);
				Assert::AreEqual(f.capacity(), (size_t)8);
				Assert::AreNotEqual(f.size(), f.capacity());

				f.resize(f.size());
				Assert::AreEqual(f.size(), f.capacity());
			}

			//String
			{
				Datum s(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				s.addToBack(static_cast<std::string>("hello"));
				Assert::AreEqual(s.size(), (size_t)5);
				Assert::AreEqual(s.capacity(), (size_t)8);
				Assert::AreNotEqual(s.size(), s.capacity());

				s.resize(s.size());
				Assert::AreEqual(s.size(), s.capacity());
			}

			//Vector4
			{
				Datum v(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				v.addToBack(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(v.size(), (size_t)5);
				Assert::AreEqual(v.capacity(), (size_t)8);
				Assert::AreNotEqual(v.size(), v.capacity());

				v.resize(v.size());
				Assert::AreEqual(v.size(), v.capacity());

			}

			//Matrix4x4
			{
				Datum m(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				m.addToBack(glm::mat4x4(1.0f));
				Assert::AreEqual(m.size(), (size_t)5);
				Assert::AreEqual(m.capacity(), (size_t)8);
				Assert::AreNotEqual(m.size(), m.capacity());

				m.resize(m.size());
				Assert::AreEqual(m.size(), m.capacity());
			}

			//Size greater and capacity same

			//Integer
			{
				Datum i(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<int>(3), 1);

				i.resize(8);
				Assert::AreEqual(i.size(), i.capacity());
			}

			//float
			{
				Datum i(1.1f);
				i.addToBack(1.1f);
				i.addToBack(1.1f);
				i.addToBack(1.1f);
				i.addToBack(1.1f);
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<float>(3), 1.1f);

				i.resize(8);
				Assert::AreEqual(i.size(), i.capacity());
			}

			//string
			{
				Datum i(std::string("hello"));
				i.addToBack(std::string("hello"));
				i.addToBack(std::string("hello"));
				i.addToBack(std::string("hello"));
				i.addToBack(std::string("hello"));
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<std::string>(3), std::string("hello"));

				i.resize(8);
				Assert::AreEqual(i.size(), i.capacity());
			}

			//Vector
			{
				Datum i(glm::vec4(1.0f));
				i.addToBack(glm::vec4(1.0f));
				i.addToBack(glm::vec4(1.0f));
				i.addToBack(glm::vec4(1.0f));
				i.addToBack(glm::vec4(1.0f));
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<glm::vec4>(3), glm::vec4(1.0f));

				i.resize(8);
				Assert::AreEqual(i.size(), i.capacity());
			}

			//Matrix
			{
				Datum i(glm::mat4x4(1.0f));
				i.addToBack(glm::mat4x4(1.0f));
				i.addToBack(glm::mat4x4(1.0f));
				i.addToBack(glm::mat4x4(1.0f));
				i.addToBack(glm::mat4x4(1.0f));
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<glm::mat4x4>(3), glm::mat4x4(1.0f));

				i.resize(8);
				Assert::AreEqual(i.size(), i.capacity());
			}


			//Size greater and capacity > size
			
			//Integer
			{
				Datum i(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				i.addToBack(1);
				Assert::AreEqual(i.size(), (size_t)5);
				Assert::AreEqual(i.capacity(), (size_t)8);
				Assert::AreEqual(i.at<int>(3), 1);

				i.resize(7);
				Assert::AreEqual(i.size(), i.capacity());

			}

			//float
			{
				Datum f(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				f.addToBack(1.1f);
				Assert::AreEqual(f.size(), (size_t)5);
				Assert::AreEqual(f.capacity(), (size_t)8);
				Assert::AreEqual(f.at<float>(3), 1.1f);

				f.resize(7);
				Assert::AreEqual(f.size(), f.capacity());

			}

			//string
			{
				Datum f(std::string("hi"));
				f.addToBack(std::string("hi"));
				f.addToBack(std::string("hi"));
				f.addToBack(std::string("hi"));
				f.addToBack(std::string("hi"));
				Assert::AreEqual(f.size(), (size_t)5);
				Assert::AreEqual(f.capacity(), (size_t)8);
				Assert::AreEqual(f.at<std::string>(3), std::string("hi"));

				f.resize(7);
				Assert::AreEqual(f.size(), f.capacity());

			}


			//vector
			{
				Datum f(glm::vec4(1.0f));
				f.addToBack(glm::vec4(1.0f));
				f.addToBack(glm::vec4(1.0f));
				f.addToBack(glm::vec4(1.0f));
				f.addToBack(glm::vec4(1.0f));
				Assert::AreEqual(f.size(), (size_t)5);
				Assert::AreEqual(f.capacity(), (size_t)8);
				Assert::AreEqual(f.at<glm::vec4>(3), glm::vec4(1.0f));

				f.resize(7);
				Assert::AreEqual(f.size(), f.capacity());
			}

			//Matrix
			{
				Datum f(glm::mat4x4(1.0f));
				f.addToBack(glm::mat4x4(1.0f));
				f.addToBack(glm::mat4x4(1.0f));
				f.addToBack(glm::mat4x4(1.0f));
				f.addToBack(glm::mat4x4(1.0f));
				Assert::AreEqual(f.size(), (size_t)5);
				Assert::AreEqual(f.capacity(), (size_t)8);
				Assert::AreEqual(f.at<glm::mat4x4>(3), glm::mat4x4(1.0f));

				f.resize(7);
				Assert::AreEqual(f.size(), f.capacity());
			}


		}

		TEST_METHOD(AssignmentOperatorOverloads)
		{
			{
				//Changing from list to scalar
				Datum a(5);
				a.addToBack(3);
				a.addToBack(6);
				Assert::AreEqual(a.size(), (size_t)3);


				a = 8;
				Assert::AreEqual(a.size(), (size_t)1);
				Assert::AreEqual(a.at<int>(0), 8);
			}

			//Changing from one type to another type
		
			//int 
			{
				Datum b(1);
				Assert::AreEqual(b.getDecidedType(), Datum::DatumType::Integer);
				Assert::ExpectException<std::invalid_argument>([&] {b = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f); });
				Assert::ExpectException<std::invalid_argument>([&] {b = "b"; });

				//Self Assignment
				b = 1;
				Assert::AreEqual(b.at<int>(0), 1);
			}

			//float 
			{
				Datum b(1.1f);
				Assert::AreEqual(b.getDecidedType(), Datum::DatumType::Float);
				Assert::ExpectException<std::invalid_argument>([&] {b = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f); });
				Assert::ExpectException<std::invalid_argument>([&] {b = "b"; });

				//Self Assignment
				b = 1.1f;
				Assert::AreEqual(b.at<float>(0), 1.1f);
			}
		
			//String
			{
				 
				Datum b(std::string("Hello"));
				Assert::AreEqual(b.getDecidedType(), Datum::DatumType::String);
				Assert::ExpectException<std::invalid_argument>([&] {b = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f); });
				Assert::ExpectException<std::invalid_argument>([&] {b = 'b'; });

				//Self Assignment
				b = std::string("Hello");
				Assert::AreEqual(b.at<std::string>(0), std::string("Hello"));
			}

			//vector 
			{
				Datum b(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(b.getDecidedType(), Datum::DatumType::Vector4);
				Assert::ExpectException<std::invalid_argument>([&] {b = std::string("hi"); });
				Assert::ExpectException<std::invalid_argument>([&] {b = "b"; });

				//Self Assignment
				b = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
				Assert::AreEqual(b.at<glm::vec4>(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
			}

			//matrix 
			{
				Datum b(glm::mat4x4(1.0f));
				Assert::AreEqual(b.getDecidedType(), Datum::DatumType::Matrix4x4);
				Assert::ExpectException<std::invalid_argument>([&] {b = std::string("hi"); });
				Assert::ExpectException<std::invalid_argument>([&] {b = "b"; });

				//Self Assignment
				b = glm::mat4x4(1.0f);
				Assert::AreEqual(b.at<glm::mat4x4>(0), glm::mat4x4(1.0f));
			}

			//constructing
			//string
			{
				Datum s;
				s = std::string("hi");
				Assert::AreEqual(s.getDecidedType(), Datum::DatumType::String);

				Datum i(1);
				Assert::ExpectException<std::invalid_argument>([&] {i = std::string("hi"); });
				
			}

			//test fails 
			{
				Datum a(1);
				Assert::ExpectException<std::invalid_argument>([&] {a = glm::mat4x4(1.0f); });
				Assert::ExpectException<std::invalid_argument>([&] {a = glm::vec4(1.0f); });
				Assert::ExpectException<std::invalid_argument>([&] {a = 1.1f; });
			}

			//empty sets
			{
				Datum a;
				a = 5;
				Datum b; 
				b = 1.3f;
				Datum c;
				c = glm::vec4(1.0f);
				Datum d;
				d = glm::mat4x4(1.0f);
			}

			//already defined
			{
				Datum a(1), b(1.3f), c(glm::vec4(1.0f)), d(glm::mat4x4(1.0f));
				a = 5;
				b = 3.3f;
				c = glm::vec4(1.4f);
				d = glm::mat4x4(3.0f);
			}

			//string
			{
				//Changing from list to scalar
				Datum a(std::string("Hello"));

				a = std::string("goodbye");
				Assert::AreEqual(a.size(), (size_t)1);
				Assert::AreEqual(a.at<std::string>(0), std::string("goodbye"));
			}

		}

		TEST_METHOD(ComparisonOperator)
		{
			//Datum comparison
			{
				Datum a;
				Datum b;

				Assert::IsTrue(a == b);
				Assert::AreEqual(a, b);
				b.addToBack(3);
				Assert::IsTrue(a != b);
				Assert::AreNotEqual(a, b);
			}



			//array comparison
			//int
			{
				Datum a(1);
				a.addToBack(1);

				Datum b(1);
				b.addToBack(2);

				Assert::IsFalse(a == b);
			}

			//float
			{
				Datum a(1.1f);
				a.addToBack(1.1f);

				Datum b(1.1f);
				b.addToBack(2.1f);

				Assert::IsFalse(a == b);
			}

			//string
			{
				Datum a(std::string("h"));
				a.addToBack(std::string("h"));

				Datum b(std::string("h"));
				b.addToBack(std::string("hi"));

				Assert::IsFalse(a == b);
			}

			//vector
			{
				Datum a(glm::vec4(1.0f));
				a.addToBack(glm::vec4(1.0f));

				Datum b(glm::vec4(1.0f));
				b.addToBack(glm::vec4(1.2f));

				Assert::IsFalse(a == b);
			}

			//Matrix
			{
				Datum a(glm::mat4x4(1.0f));
				a.addToBack(glm::mat4x4(1.0f));

				Datum b(glm::mat4x4(1.0f));
				b.addToBack(glm::mat4x4());

				Assert::IsFalse(a == b);
			}

			//Scalar comparison

			//int 
			{
				Datum i(1);
				Assert::IsTrue(i == 1);
				Assert::IsFalse(i == 3);
				Assert::IsFalse(i == 1.5f);
				Assert::IsFalse(i == std::string("hello"));
				Assert::IsFalse(i == glm::vec4(1.0f));
				Assert::IsFalse(i == glm::mat4x4(1.0f));
			}

			//float
			{
				Datum i(1.2f);
				Assert::IsTrue(i == 1.2f);
			}

			//string
			{
				Datum i(std::string("hi"));
				Assert::IsTrue(i == std::string("hi"));
			}

			//vector
			{
				Datum i(glm::vec4(1.0f));
				Assert::IsTrue(i == glm::vec4(1.0f));
			}

			//matrix
			{
				Datum i(glm::mat4x4(1.0f));
				Assert::IsTrue(i == glm::mat4x4(1.0f));
			}

			//size greater than one
			{
				Datum a(3.3f);
				Assert::IsTrue(a == 3.3f);
				Assert::IsFalse(a == 3);
				a.addToBack<float>(4.0f);
				Assert::IsFalse(a == 1);
				Assert::IsFalse(a == 3);
				Assert::IsFalse(a == 1.5f);
				Assert::IsFalse(a == std::string("hello"));
				Assert::IsFalse(a == glm::vec4(1.0f));
				Assert::IsFalse(a == glm::mat4x4(1.0f));

				Datum b(1);
				b.addToBack(2);
				Assert::IsFalse(a == 1);
			}

			//not equal
			{
				Datum a(1);
				Assert::IsTrue(a != 3);
				Assert::IsTrue(a != 0);
				Assert::IsTrue(a != 3);
				Assert::IsTrue(a != 1.5f);
				Assert::IsTrue(a != std::string("hello"));
				Assert::IsTrue(a != glm::vec4(1.0f));
				Assert::IsTrue(a != glm::mat4x4(1.0f));
			}

		}

		TEST_METHOD(ToString)
		{
			//Integers
			{
				Datum i(123456789);
				Assert::AreEqual(i.ToString(0), std::to_string(i.at<int>(0)));
			}

			//Floats
			{
				Datum f(3.14159f);
				Assert::AreEqual(f.ToString(0), std::string("3.141590"));
			}

			//Strings
			{
				Datum s(std::string("Hello, World!"));
				Assert::AreEqual(s.ToString(0), std::string("Hello, World!"));
			}

			//Vector4
			{
				Datum v(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::AreEqual(v.ToString(0), std::string("vec4(1.000000, 2.000000, 3.000000, 4.000000)"));
			}

			//Matrix4x4
			{
				Datum m(glm::mat4x4(1.0f));
				Assert::AreEqual(m.ToString(0), std::string("mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"));
			}
		}

		TEST_METHOD(SetAsString)
		{
			//int
			{
				Datum a(1);
				a.SetAsString("200", 0);
				Assert::AreEqual(a.at<int>(0), 200);
				Assert::ExpectException<std::invalid_argument>([&] {a.SetAsString("Mario", 0); });
			}

			//float
			{
				Datum a(1.1f);
				a.SetAsString("3.1f", 0);
				Assert::AreEqual(a.at<float>(0), 3.1f);
				Assert::ExpectException<std::invalid_argument>([&] {a.SetAsString("Mario", 0); });
			}

			//string
			{
				Datum a(std::string("hi"));
				a.SetAsString("12a3seklfj", 0);
				Assert::AreEqual(a.at<std::string>(0), std::string("12a3seklfj"));
			}

			//Vector
			{
				Datum a(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				a.SetAsString("vec4(1.000000, 2.000000, 3.000000, 4.000000)", 0);
				Assert::AreEqual(a.at<glm::vec4>(0), glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
				Assert::ExpectException<std::invalid_argument>([&] {a.SetAsString("Mario", 0); });
			}

			//Matrix
			{
				Datum a(glm::mat4x4(1.0f));
				a.SetAsString("mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))", 0);
				Assert::AreEqual(a.at<glm::mat4x4>(0), glm::mat4x4(1.0f));
				Assert::ExpectException<std::invalid_argument>([&] {a.SetAsString("Mario", 0); });
			}

		}

		TEST_METHOD(SetStorage)
		{
			//Ints
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				Datum a;
				a.SetStorage<int>(ptr, size);
				Assert::AreEqual(a.size(), (size_t)3);

				free(ptr);
			}

			//already external
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				Datum a;
				a.SetStorage<int>(ptr, size);
				Assert::AreEqual(a.size(), (size_t)3);
				free(ptr);

				int* ptr2 = (int*)malloc(sizeof(int) * 18);

				a.SetStorage<int>(ptr2, 18);

				free(ptr2);

				
			}

			//invalid
			{
				size_t size = 3;
				int* ptr = (int*)malloc(sizeof(int) * size);
				free(ptr);

				Datum d(5);
				Assert::ExpectException<std::invalid_argument>([&] {d.SetStorage(ptr, size); });
				
			}
		}

		

	private:
		inline static _CrtMemState _startMemState;
		
	};
}
