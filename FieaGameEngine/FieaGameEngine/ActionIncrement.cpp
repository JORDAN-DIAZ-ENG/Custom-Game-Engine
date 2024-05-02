#include "pch.h"
#include "ActionIncrement.h"
#include "TypeManager.h"
#include "ActionListWhile.h"
#include "ActionList.h"

using namespace std::string_literals;


namespace Fiea::GameEngine
{

	const void* ActionIncrement::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

		RTTI_DEFINITIONS(ActionIncrement);

		ActionIncrement::ActionIncrement() : Action(TypeIdClass())
		{
		}

	void ActionIncrement::Update(const GameTime& deltaTime)
	{
		Datum* d;

		if (GetParent()->As<ActionListWhile>() != nullptr || GetParent()->As<ActionList>() != nullptr)
		{
			d = GetParent()->GetParent()->Find(_datumKey);
		}
		else
		{
			d = GetParent()->Find(_datumKey);
		}

		if (d->getDecidedType() == Datum::DatumType::Integer)
		{
			d->at<int>(0) += (int)_incrementBy;
		}
		else if (d->getDecidedType() == Datum::DatumType::Float)
		{
			d->at<float>(0) += _incrementBy;
		}
	}


		

	ActionIncrement* ActionIncrement::Clone() const
	{
		return NEW ActionIncrement(*this);
	}
	std::vector<Signature> ActionIncrement::Signatures()
	{
		std::vector<Signature> parentSigs = Action::Signatures();
		parentSigs.push_back({ "_incrementBy"s, Datum::DatumType::Float, 1, offsetof(ActionIncrement, _incrementBy) });
		return parentSigs;
	}

}

