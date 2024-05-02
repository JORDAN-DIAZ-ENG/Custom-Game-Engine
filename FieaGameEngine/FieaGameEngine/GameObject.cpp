#include "pch.h"
#include <vector>
#include "GameObject.h"
#include "signature.h"
#include "TypeManager.h"
#include "Action.h"


namespace Fiea::GameEngine
{
	const void* GameObject::signaturesAppended = []() {
		TypeManager::add(TypeIdClass(), Signatures());
		return nullptr;
		}();

	RTTI_DEFINITIONS(GameObject);

	
	GameObject::GameObject() : Attributed(TypeIdClass())
	{

	}
	GameObject* GameObject::Clone() const
	{
		return NEW GameObject(*this);
	}

	void GameObject::Update(const GameTime& deltaTime)
	{
		Datum* d = Find("_children");
		assert(d != nullptr);

		//if (!active) return;

		for (size_t i = 0; i < d->size(); ++i)
		{
			Scope* child = d->at<Scope*>(i);
			assert(child != nullptr);

			//GameObject* childObj = childObj->As<GameObject>();//SLOW DON'T USE ALSO VIRTUAL
			//INSTEAD USE A STATIC CAST
			GameObject* childObj = static_cast<GameObject*>(child); //Cast could fail
			assert(childObj != nullptr);

			childObj->Update(deltaTime);
		}

		d = Find("_actions");
		assert(d != nullptr);


		for (size_t i = 0; i < d->size(); ++i)
		{
			d->at<Scope*>(i)->As<Action>()->Update(deltaTime);
		}

		UpdateSelf(deltaTime);
	}

	const std::string& GameObject::getName() const
	{
		return Name;
	}

	const glm::vec4& GameObject::getPosition() const
	{
		return transform.Position;
	}

	const glm::vec4& GameObject::getRotation() const
	{
		return transform.Rotation;
	}

	const glm::vec4& GameObject::getScale() const
	{
		return transform.Scale;
	}

	void GameObject::setName(const std::string& newName)
	{
		Name = newName;
	}

	void GameObject::setPosition(glm::vec4 newPosition)
	{
		transform.Position = newPosition;
	}

	void GameObject::setRotation(glm::vec4 newRotation)
	{
		transform.Rotation = newRotation;
	}

	void GameObject::setScale(glm::vec4 newScale)
	{
		transform.Scale = newScale;
	}

	const Transform& GameObject::getTransform() const
	{
		return transform;
	}

	GameObject* GameObject::getChild(const std::string& name)
	{
		Datum* d = Find("children");

		for (size_t i = 0; i < d->size(); ++i)
		{
			Scope* child = d->at<Scope*>(i);
			GameObject* go = child->As<GameObject>();
			if (go->getName() == name)
			{
				return go;
			}
		}
		return nullptr;
	}

	void GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* action = FactoryManager<Scope>::Create(className);
		assert(action != nullptr);

		action->As<Action>()->setName(instanceName);	

		AppendScope("_actions", action);

		//s->Adopt(instanceName, *action);
	}

	std::vector<Signature> GameObject::Signatures()
	{
		return std::vector<Signature>
		{
			{ "_name"s, Datum::DatumType::String, 1, offsetof(GameObject, Name) },
			{ "_pos"s, Datum::DatumType::Vector4, 1, offsetof(GameObject, transform.Position) },
			{ "_rot"s, Datum::DatumType::Vector4, 1, offsetof(GameObject, transform.Rotation) },
			{ "_scale"s, Datum::DatumType::Vector4, 1, offsetof(GameObject, transform.Scale) },
			{"_children", Datum::DatumType::Table, 0, 0},
			{"_actions", Datum::DatumType::Table, 0, 0},
		};
	}

	GameObject::GameObject(RTTI::IdType passedInTypeID) : Attributed(passedInTypeID)
	{

	}
}

