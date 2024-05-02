#pragma once
#include "ParseCoordinator.h"


namespace Fiea::GameEngine
{

	class IntWrapper : public ParseCoordinator::Wrapper
	{
	public:

		IntWrapper() = delete;

		IntWrapper(int* Integer);


		IntWrapper(std::vector<int>* IntegerVector);

		int getData(int index);

		void setInt(int Integer);

		void setIntAtIndex(int Integer, size_t index);

		virtual bool Equals(const RTTI* rhs) const override;


	private:
		//Incoming Output Params to be mutated
		int* _incomingInt = nullptr;
		std::vector<int>* _incomingIntVector = nullptr;

		//Hint: What qualifier should the destructor have?

		//RTTI Overrides
		//virtual bool Equals(const RTTI* rhs) const override;

	};
}

