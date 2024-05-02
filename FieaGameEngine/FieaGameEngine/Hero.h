#pragma once
#include "GameObject.h"
#include "Factory.h"
#include "Action.h"
#include "GameClock.h"


namespace Fiea::GameEngine
{

	class Hero : public GameObject
	{

		RTTI_DECLARATIONS(Hero, Attributed);


	public:
		Hero();
		[[nodiscard]] Hero* Clone() const override;

		/**
		 * @brief Retrieve the health
		 * @return 
		*/
		const int getHealth() const;

		/**
		 * @brief Retrieve the desired child
		 * @param name 
		 * @return 
		*/
		Hero* getChild(const std::string& name);

		/**
		 * @brief Update for this hero
		 * @param deltaTime 
		*/
		virtual void UpdateSelf(const GameTime& deltaTime) override
		{
			setName("PEACH");
		}

		/**
		 * @brief override update for children and self
		 * @param deltaTime 
		*/
		virtual void Update(const GameTime& deltaTime) override // Calls update on all children
		{

			Datum* d = Find("_children");
			assert(d != nullptr);

			//if (!active) return;

			for (size_t i = 0; i < d->size(); ++i)
			{
				Scope* child = d->at<Scope*>(i);
				assert(child != nullptr);

				GameObject* childObj = static_cast<GameObject*>(child); //Cast could fail
				assert(childObj != nullptr);

				childObj->Update(deltaTime);
			}

			Datum* a = Find("_actions");
			assert(a != nullptr);



			for (size_t i = 0; i < a->size(); ++i)
			{
				Action* act = a->at<Scope*>(i)->As<Action>();
				assert(act != nullptr);
				act->Update(deltaTime);

			}

			UpdateSelf(deltaTime);
		}


		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;

	private:
		int health;
	};

	MAKE_FACTORY(Scope, Hero);

}