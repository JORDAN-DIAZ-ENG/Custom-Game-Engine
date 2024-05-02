#pragma once
#include "Action.h"
#include "RTTI.h"
#include "Scope.h"
#include "Factory.h"

namespace Fiea::GameEngine
{
	class ActionList : public Action 
	{
		RTTI_DECLARATIONS(ActionList, Action);


	public:
		ActionList();

		//Defaults
		ActionList(const ActionList& other) = default;
		ActionList(ActionList&& rhs) noexcept = default;
		ActionList& operator=(const ActionList& rhs) = default;
		ActionList& operator=(ActionList&& rhs) noexcept = default;
		virtual ~ActionList() override = default;

		virtual void Update(const GameTime& deltaTime) override;
		[[nodiscard]] Action* Clone() const override;

		/**
		* @brief AppendAction: Append an action to the list
		* @param className 
		* @param instanceName 
		*/
		void AppendAction(const std::string& className, const std::string& instanceName);

		
	protected:
		ActionList(RTTI::IdType passedInType);

	private:
		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;
	};

	MAKE_FACTORY(Scope, ActionList);

}