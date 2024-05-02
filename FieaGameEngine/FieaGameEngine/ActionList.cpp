#include "pch.h"
#include "ActionList.h"
#include "TypeManager.h"



namespace Fiea::GameEngine
{
	using namespace std::string_literals;


	const void* ActionList::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

	RTTI_DEFINITIONS(ActionList);

	ActionList::ActionList() : Action(TypeIdClass())
	{
	}

	void ActionList::Update(const GameTime& deltaTime)
	{
		Datum* d = Find("_Actions");
		assert(d != nullptr);

		for (size_t i = 0; i < d->size(); ++i)
		{
			d->at<Scope*>(i)->As<Action>()->Update(deltaTime);
		}

		//Datum* d = Search(_datumKey);
		//assert(d != nullptr);

		//if (d->getDecidedType() == Datum::DatumType::Integer)
		//{
		//	d->at<int>(0) += (int)_incrementBy;
		//}
		//else if (d->getDecidedType() == Datum::DatumType::Float)
		//{
		//	d->at<float>(0) += _incrementBy;
		//}
	}

	Action* ActionList::Clone() const
	{
		return NEW ActionList(*this);
	}

	void ActionList::AppendAction(const std::string& className, const std::string& instanceName)
	{
		Datum* d = Find("_Actions");

		Scope* action = FactoryManager<Scope>::Create(className);
		assert(action != nullptr);

		action->As<Action>()->setName(instanceName);

		AppendScope("_Actions", action);
	}

	ActionList::ActionList(RTTI::IdType passedInType) : Action(passedInType)
	{
	}

	std::vector<Signature> ActionList::Signatures()
	{
		std::vector<Signature> parentSigs = Action::Signatures();
		parentSigs.push_back({ "_Actions"s, Datum::DatumType::Table, 0, 0});
		return parentSigs;
	}
}

