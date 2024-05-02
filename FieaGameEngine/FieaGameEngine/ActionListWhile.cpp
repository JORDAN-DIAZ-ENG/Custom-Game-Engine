#include "pch.h"
#include "ActionListWhile.h"
#include "TypeManager.h"

namespace Fiea::GameEngine
{
	using namespace std::string_literals;


	const void* ActionListWhile::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

	RTTI_DEFINITIONS(ActionListWhile);

	ActionListWhile::ActionListWhile() : ActionList(TypeIdClass())
	{
	}


	void ActionListWhile::Update(const GameTime& deltaTime)
	{
		if (!_isConditionSet) { return; }

		Datum* a = Find("_Actions");

		if (_preamble != nullptr)
		{
			_preamble->Update(deltaTime);
		}

		while(_condition > 0) //Any non zero value is considered true
		{
			a->at<Scope*>(0)->As<Action>()->Update(deltaTime); //update the actions

			if (_increment != nullptr)
			{
				_increment->Update(deltaTime);
			}

			--_condition;
		}

	}
	void ActionListWhile::setPreamble(Action* action)
	{
		_preamble = action;
		AppendScope("_preamble", action);
	}
	void ActionListWhile::setIncrement(Action* action)
	{
		_increment = action;
		AppendScope("_increment", action);

	}
	void ActionListWhile::setCondition(int condition)
	{
		_condition = condition;
		_isConditionSet = true;
	}
	ActionListWhile* ActionListWhile::Clone() const
	{
		return NEW ActionListWhile(*this);
	}
	ActionListWhile::ActionListWhile(RTTI::IdType passedInType) : ActionList(passedInType)
	{
	}
	std::vector<Signature> ActionListWhile::Signatures()
	{
		std::vector<Signature> parentSigs = Action::Signatures();
		parentSigs.push_back({ "_condition"s, Datum::DatumType::Integer, 1, offsetof(ActionListWhile, _condition)});
		parentSigs.push_back({ "_preamble"s, Datum::DatumType::Table, 0, 0 });
		parentSigs.push_back({ "_increment"s, Datum::DatumType::Table, 0, 0 });
		return parentSigs;
	}
}


