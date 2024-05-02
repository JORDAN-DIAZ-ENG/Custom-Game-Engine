#pragma once
#include "Action.h"


namespace Fiea::GameEngine
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

	public:

		ActionIncrement();

		//Defaults
		ActionIncrement(const ActionIncrement& other) = default;
		ActionIncrement(ActionIncrement&& rhs) noexcept = default;
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;
		ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;
		virtual ~ActionIncrement() override = default;


		// Inherited via Action
		virtual void Update(const GameTime& deltaTime) override;
		ActionIncrement* Clone() const override;

		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;

		std::string _datumKey;
		float _incrementBy;

	private:

	};

	MAKE_FACTORY(Scope, ActionIncrement);
}