#include "pch.h"
#include <vector>
#include "AttributedFoo.h"
#include "signature.h"
#include "TypeManager.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{
	const void* AttributedFoo::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures()); 
		return nullptr;
		}();


	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() : Attributed(TypeIdClass()) 
	{

	}

	AttributedFoo* AttributedFoo::Clone() const
	{
		return NEW AttributedFoo(*this);
	}

	bool AttributedFoo::operator==(const AttributedFoo& other)
	{
		return Scope::operator==(other);
	}

	bool AttributedFoo::operator!=(const AttributedFoo& other)
	{
		return !operator==(other);
	}

	std::vector<Signature> AttributedFoo::Signatures()
	{
		return std::vector<Signature>
		{
			{ "externalInteger"s, Datum::DatumType::Integer, 1, offsetof(AttributedFoo, externalInteger) },
			{ "externalFloat"s, Datum::DatumType::Float, 1, offsetof(AttributedFoo, externalFloat) },
			{ "externalString"s, Datum::DatumType::String, 1, offsetof(AttributedFoo, externalString) },
			{ "externalVector"s, Datum::DatumType::Vector4, 1, offsetof(AttributedFoo, externalVector) },
			{ "externalMatrix"s, Datum::DatumType::Matrix4x4, 1, offsetof(AttributedFoo, externalMatrix) },
			{ "externalIntegerArray"s, Datum::DatumType::Integer, arraySize, offsetof(AttributedFoo, externalIntegerArray) },
			{ "externalFloatArray"s, Datum::DatumType::Float, arraySize, offsetof(AttributedFoo, externalFloatArray) },
			{ "externalStringArray"s, Datum::DatumType::String, arraySize, offsetof(AttributedFoo, externalStringArray) },
			{ "externalVectorArray"s, Datum::DatumType::Vector4, arraySize, offsetof(AttributedFoo, externalVectorArray) },
			{ "externalVectorMatrix"s, Datum::DatumType::Matrix4x4, arraySize, offsetof(AttributedFoo, externalMatrixArray) }
		};
	}

	AttributedFoo::AttributedFoo(RTTI::IdType passedInTypeID) : Attributed(passedInTypeID)
	{
	}

}
