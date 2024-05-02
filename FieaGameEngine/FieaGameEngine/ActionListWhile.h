#pragma once
#include "ActionList.h"
#include "RTTI.h"
#include "signature.h"

namespace Fiea::GameEngine
{
	class ActionListWhile : public ActionList
	{
		RTTI_DECLARATIONS(ActionListWhile, ActionList);

	public:
		ActionListWhile();

		//Defaults
		ActionListWhile(const ActionListWhile& other) = default;
		ActionListWhile(ActionListWhile&& rhs) noexcept = default;
		ActionListWhile& operator=(const ActionListWhile& rhs) = default;
		ActionListWhile& operator=(ActionListWhile&& rhs) noexcept = default;
		virtual ~ActionListWhile() override = default;

		virtual void Update(const GameTime& deltaTime) override;

		/**
		 * @brief setPreamble: Set the preamble action
		 * @param action 
		*/
		void setPreamble(Action* action);

		/**
		* @brief setIncrement: Set the increment action
		* @param action 
		*/
		void setIncrement(Action* action);
		
		/**
		* @brief setCondition: Set the condition
		* @param condition 
		*/
		void setCondition(int condition);

		[[nodiscard]] ActionListWhile* Clone() const override;



	protected:
		ActionListWhile(RTTI::IdType passedInType);

	private:
		bool _isConditionSet = false;
		int _condition = 0;
		Action* _preamble = nullptr;
		Action* _increment = nullptr;

		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;

	};

	MAKE_FACTORY(Scope, ActionListWhile);
}