#include "pch.h"
#include "Monster.h"
#include "Hero.h"

#include "TypeManager.h"
#include "signature.h"
#include <vector>

#include "GameObject.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{

	const void* Hero::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

	RTTI_DEFINITIONS(Hero);



	Hero::Hero() : GameObject(TypeIdClass())
	{

	}

	Hero* Hero::Clone() const
	{
		return nullptr;
	}

	const int Hero::getHealth() const
	{
		return health;
	}

	Hero* Hero::getChild(const std::string& name)
	{
		Datum* d = Find("_children");

		for (size_t i = 0; i < d->size(); ++i)
		{
			Scope* child = d->at<Scope*>(i);
			Hero* h = child->As<Hero>();
			if (h->getName() == name)
			{
				return h;
			}
		}
		return nullptr;
	}

	std::vector<Signature> Hero::Signatures()
	{
		std::vector<Signature> parentSigs = GameObject::Signatures();
		parentSigs.push_back({ "_heroHealth"s, Datum::DatumType::Integer, 1, offsetof(Hero, health) });
		return parentSigs;
	}

}