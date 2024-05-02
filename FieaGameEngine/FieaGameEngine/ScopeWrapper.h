#pragma once
#include "ParseCoordinator.h"
#include "Scope.h"


namespace Fiea::GameEngine
{

	class ScopeWrapper : public ParseCoordinator::Wrapper
	{
	public:

		ScopeWrapper() = delete;

		ScopeWrapper(Scope* scope);

		void addScope(const std::string& s, bool isInArray);

		void addIntDatum(const std::string& s, int item, size_t index);

		void addFloatDatum(const std::string& s, double item, size_t index);

		void addVec4Datum(const std::string& s, const std::string& item, size_t index);

		void addMat4x4Datum(const std::string& s, const std::string& item, size_t index);

		void addStringDatum(const std::string& s, const std::string& item, size_t index);

		void moveUpScopeTree();

		void moveDownScopeTree(Scope* newCurrentScope);

		virtual bool Equals(const RTTI* rhs) const override;

	private:
		//Incoming Output Params to be mutated
		//Scope* _incomingScope = nullptr;

		Scope* _currentScope = nullptr;

		//Hint: What qualifier should the destructor have?

		//RTTI Overrides
		//virtual bool Equals(const RTTI* rhs) const override;

	};
}

