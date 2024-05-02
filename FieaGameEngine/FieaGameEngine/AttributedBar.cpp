#include "pch.h"
#include "AttributedBar.h"
#include "AttributedFoo.h"
#include "signature.h"
#include "TypeManager.h"
#include "Attributed.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{

	const void* AttributedBar::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

	RTTI_DEFINITIONS(AttributedBar);

	AttributedBar::AttributedBar() : AttributedFoo(TypeIdClass())
	{

	}

	bool AttributedBar::operator==(const AttributedBar& other)
	{
		return Scope::operator==(other);
	}

	bool AttributedBar::operator!=(const AttributedBar& other)
	{
		return Scope::operator!=(other);
	}

	AttributedBar* AttributedBar::Clone() const
	{
		return NEW AttributedBar(*this);
	}
	std::vector<Signature> AttributedBar::Signatures()
	{
		std::vector<Signature> parentSigs = AttributedFoo::Signatures();
		parentSigs.push_back({ "barSpecificExternalInt"s, Datum::DatumType::Integer, 1, offsetof(AttributedBar, barSpecificExternalInt) });
		return parentSigs;
	}
}

