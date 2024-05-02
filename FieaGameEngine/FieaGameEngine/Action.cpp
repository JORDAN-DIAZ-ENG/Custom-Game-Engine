#include "pch.h"
#include <vector>
#include "Action.h"
#include "TypeManager.h"
#include "GameObject.h"

using namespace std::string_literals;


namespace Fiea::GameEngine
{
	const void* Action::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

	RTTI_DEFINITIONS(Action);

	const std::string& Action::Name()
	{
		return _name;
	}

	void Action::setName(const std::string& ActionName)
	{
		_name = ActionName;
	}

	std::vector<Signature> Action::Signatures()
	{
		return std::vector<Signature>
		{
			{ "_name"s, Datum::DatumType::String, 1, offsetof(Action, _name) }
		};
	}
	void Action::setParentGameObject(GameObject* gameObject)
	{
		parentGameObject = gameObject;
	}
	Action::Action(RTTI::IdType passedInTypeID) : Attributed(passedInTypeID)
	{

	}
}


