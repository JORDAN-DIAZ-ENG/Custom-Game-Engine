#include "pch.h"
#include "Monster.h"
#include "TypeManager.h"
#include "signature.h"
#include <vector>
#include "Datum.h"

using namespace std::string_literals;


namespace Fiea::GameEngine
{
	const void* Monster::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();


	RTTI_DEFINITIONS(Monster);

	Monster::Monster() :  Attributed(TypeIdClass())
	{

	}

	Monster* Monster::Clone() const
	{
		return NEW Monster(*this);
	}

	std::vector<Signature> Monster::Signatures()
	{
		//{"GenericScope", Datum::DatumType::Table, 1, offsetof(Monster, genericScope)}
		return std::vector<Signature>
		{
			{ "intValue"s, Datum::DatumType::Integer, 1, offsetof(Monster, intValue) },
			{ "floatValue"s, Datum::DatumType::Float, 1, offsetof(Monster, floatValue) },
			{ "stringValue"s, Datum::DatumType::String, 1, offsetof(Monster, stringValue) },
			{ "vec4Value"s, Datum::DatumType::Vector4, 1, offsetof(Monster, vec4Value) },
			{ "mat4Value"s, Datum::DatumType::Matrix4x4, 1, offsetof(Monster, mat4Value) },
			{ "intArr"s, Datum::DatumType::Integer, 2, offsetof(Monster, intArr) },
			{ "floatArr"s, Datum::DatumType::Float, 2, offsetof(Monster, floatArr) },
			{ "stringArr"s, Datum::DatumType::String, 2, offsetof(Monster, stringArr) },
			{ "vec4Arr"s, Datum::DatumType::Vector4, 2, offsetof(Monster, vec4Arr) },
			{ "mat4x4Arr"s, Datum::DatumType::Matrix4x4, 2, offsetof(Monster, mat4x4Arr) }
			
		};
	}
}
