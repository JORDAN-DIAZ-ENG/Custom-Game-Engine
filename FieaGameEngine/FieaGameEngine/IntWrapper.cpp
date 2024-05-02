#pragma once
#include "pch.h"
#include "IntWrapper.h"

namespace Fiea::GameEngine
{
	IntWrapper::IntWrapper(int* Integer) : _incomingInt(Integer)
	{

	}

	IntWrapper::IntWrapper(std::vector<int>* IntegerVector) : _incomingIntVector(IntegerVector)
	{

	}


	void IntWrapper::setInt(int Integer)
	{
		*_incomingInt = Integer;
	}

	void IntWrapper::setIntAtIndex(int Integer, size_t index)
	{
		_incomingIntVector->insert(_incomingIntVector->begin() + index, Integer);
		//_incomingIntVector->push_back(Integer);
	}

	bool IntWrapper::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;

		const ParseCoordinator::Wrapper* other = rhs->As<ParseCoordinator::Wrapper>();

		return !other;
	}



	//bool IntWrapper::Equals(const RTTI* rhs) const
	//{
	//	return false;
	//}
}