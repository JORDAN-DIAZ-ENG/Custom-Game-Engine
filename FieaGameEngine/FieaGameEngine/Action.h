#pragma once
#include "Attributed.h"
#include "signature.h"
#include "Factory.h"
#include "GameClock.h"

namespace Fiea::GameEngine
{
	class GameObject;

	class Action : public Attributed
	{


		RTTI_DECLARATIONS(Action, Attributed);


	public:

		Action(const Action& other) = default;
		Action(Action&& rhs) noexcept = default;
		Action& operator=(const Action& rhs) = default;
		Action& operator=(Action&& rhs) noexcept = default;
		virtual ~Action() = default;

		bool operator==(const Action& other);
		bool operator!=(const Action& other);

		/**
		 * @brief Update: Pure virtual function to be implemented by derived classes
		 * @param deltaTime 
		*/
		virtual void Update(const GameTime& deltaTime) = 0;

		/**
		 * @brief Name: Retrieve the name of the action
		 * @return 
		*/
		const std::string& Name();

		/**
		 * @brief setName: Set the name of the action
		 * @param ActionName 
		*/
		void setName(const std::string& ActionName);

		/**
		 * @brief Clone: Pure virtual function to be implemented by derived classes
		 * @return 
		*/
		[[nodiscard]] Action* Clone() const override = 0;

		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;

		/**
		 * @brief setParentGameObject: Set the parent game object
		 * @param gameObject 
		*/
		void setParentGameObject(GameObject* gameObject);

	protected:
		Action(RTTI::IdType passedInTypeID);

		GameObject* parentGameObject;

	private:
		//prescribed Attributes
		std::string _name;
	};
}