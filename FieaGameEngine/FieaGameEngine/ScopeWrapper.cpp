#pragma once
#include "pch.h"
#include "ScopeWrapper.h"
#include <regex>
#include "Factory.h"
#include "Attributed.h"

namespace Fiea::GameEngine
{
	ScopeWrapper::ScopeWrapper(Scope* scope) : /*_incomingScope(scope),*/ _currentScope(scope)
	{
	}
	void ScopeWrapper::addScope(const std::string& key, bool isInArray)
	{

		std::regex pattern("\\{([^\\}]*)\\}([^\\s]*)"); // regex pattern for "{type}name"
		std::smatch match;

		if (std::regex_search(key, match, pattern))
		{
			if (match.size() == 3)
			{
				std::string type = match[1].str();
				std::string name = match[2].str();


				//check if the key is a valid factory
				//Create a new object of that class

				IFactory<Scope>* Factory = FactoryManager<Scope>::Find(type);

				if (Factory != nullptr)
				{
					//Might need to set the current scope to be the new guy
					//Scope* s = Factory->Create();

					Scope* s = Factory->Create();
					_currentScope->Adopt(name, *s);
					_currentScope = s;

				}
				else
				{
					_currentScope = &_currentScope->AppendScope(name, type);

				}

				//Monster* newMonster1 = monsterFactory->Create()->As<Monster>();

				//Assert::IsTrue(newMonster1->intValue == 0);

				//delete(newMonster1);



			}
		}
		else
		{
			//INPUT DOES NOT MATCH THE PATTERN
			throw std::exception("INPUT DOES NOT MATCH THE PATTERN");
		}


		

		//_currentScope = &_incomingScope->AppendScope(key);
	}
	void ScopeWrapper::addIntDatum(const std::string& s, int item, size_t index)
	{
		//_incomingScope->Append(s).addToBack(item);

		if (_currentScope->Is(Attributed::TypeIdClass()))
		{
			if (_currentScope->As<Attributed>()->isPrescribedAttribute(s))
			{
				_currentScope->As<Attributed>()->Find(s)->at<int>(index) = item;
			}
		}
		else
		{
			_currentScope->Append(s).addToBack(item);

		}

	}

	void ScopeWrapper::addFloatDatum(const std::string& s, double item, size_t index)
	{
		if (_currentScope->Is(Attributed::TypeIdClass()))
		{
			if (_currentScope->As<Attributed>()->isPrescribedAttribute(s))
			{
				_currentScope->As<Attributed>()->Find(s)->at<float>(index) = static_cast<float>(item);
			}
		}
		else
		{
			_currentScope->Append(s).addToBack(static_cast<float>(item));

		}

	}

	void ScopeWrapper::addVec4Datum(const std::string& s, const std::string& item, size_t index)
	{
		if (_currentScope->Is(Attributed::TypeIdClass()))
		{
			if (_currentScope->As<Attributed>()->isPrescribedAttribute(s))
			{
				_currentScope->As<Attributed>()->Find(s)->SetAsString(item, index);

			}
		}
		else
		{
			_currentScope->Append(s).addToBackFromString(item, Datum::DatumType::Vector4);

		}

		
	}

	void ScopeWrapper::addMat4x4Datum(const std::string& s, const std::string& item, size_t index)
	{
		if (_currentScope->Is(Attributed::TypeIdClass()))
		{
			if (_currentScope->As<Attributed>()->isPrescribedAttribute(s))
			{
				_currentScope->As<Attributed>()->Find(s)->SetAsString(item, index);

			}
		}
		else
		{
			_currentScope->Append(s).addToBackFromString(item, Datum::DatumType::Matrix4x4);

		}

	}

	void ScopeWrapper::addStringDatum(const std::string& s, const std::string& item, size_t index)
	{
		if (_currentScope->Is(Attributed::TypeIdClass()))
		{
			if (_currentScope->As<Attributed>()->isPrescribedAttribute(s))
			{
				_currentScope->As<Attributed>()->Find(s)->at<std::string>(index) = item;
			}
		}
		else
		{
			_currentScope->Append(s).addToBackFromString(item, Datum::DatumType::String);

		}

	}

	void ScopeWrapper::moveUpScopeTree()
	{
		_currentScope = _currentScope->GetParent();
	}
	void ScopeWrapper::moveDownScopeTree(Scope* newCurrentScope)
	{
		_currentScope = newCurrentScope;
	}
	bool ScopeWrapper::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) return false;

		const ParseCoordinator::Wrapper* other = rhs->As<ParseCoordinator::Wrapper>();
	
		return !other;
	}
}