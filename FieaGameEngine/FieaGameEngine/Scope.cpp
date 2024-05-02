#include "pch.h"
#include "Scope.h"
#include <memory>
#include "Factory.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Scope);

	Fiea::GameEngine::Scope::Scope(size_t initialCapacity)
	{
		_data.reserve(initialCapacity);

		//for (size_t i = 0; i < initialCapacity; ++i)
		//{
		//	_data.emplace("Construction Key: " + std::to_string(i), Datum());
		//	_order.push_back(&(_data.at("Construction Key: " + std::to_string(i))));
		//}
	}

	Scope::Scope(const Scope& other)
	{
		//intentionally take the parent
		_parent = other._parent;

		for (auto& pair : other._data) //For all items in the map
		{
			if (pair.second.getDecidedType() == Datum::DatumType::Table) // if the datum is a scope
			{
				for (size_t i = 0; i < pair.second.size(); ++i)
				{
					Scope* newScope = (*pair.second.getUnionArrayGivenType<Scope*>())->Clone();
					newScope->_parent = this;

					if (_data.count(pair.first) == 1) // Datum Found
					{
						_data.at(pair.first).addToBack(newScope);
					}
					else
					{
						_data.emplace(pair.first, newScope); // Deep copy
						_order.push_back(&_data.at(pair.first));
					}

				}
			}
			else
			{
				//if (_data.count(pair.first) == 1) // Datum Found
				//{
				//	_data.at(pair.first).addToBack(pair.second);
				//}
				//else
				//{
					_data.emplace(pair.first, pair.second);
					_order.push_back(&_data.at(pair.first));
				//}


			}
		}
	}

	Scope::Scope(Scope&& other) : _data(std::move(other._data)), _order(std::move(other._order)), _parent(other._parent)
	{
		//Go through all my scopes
		//reparent them

		for (auto& datum : _order)
		{
			if (datum->getDecidedType() == Datum::DatumType::Table)
			{
				datum->at<Scope*>(0)->_parent = this;
			}
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		// Self Assignment
		if (this == &rhs)
		{
			return *this;
		}

		//ClearDatumsOfTypeScope();
		Clear();

		for (auto& pair : rhs._data) //For all items in the map
		{
			if (pair.second.getDecidedType() == Datum::DatumType::Table) // if the datum is a scope
			{
				for (size_t i = 0; i < pair.second.size(); ++i)
				{
					Scope* newScope = (*pair.second.getUnionArrayGivenType<Scope*>())->Clone();
					newScope->_parent = this;

					if (_data.count(pair.first) == 1) // Datum Found
					{
						_data.at(pair.first).addToBack(newScope);
					}
					else
					{
						_data.emplace(pair.first, newScope); // Deep copy
						_order.push_back(&_data.at(pair.first));
					}

				}
			}
			else
			{
				_data.emplace(pair.first, pair.second);
				_order.push_back(&_data.at(pair.first));
			}
		}
		return *this;
	}



	Scope::~Scope()
	{
		Clear();
	}

	void Scope::ClearDatumsOfTypeScope()
	{
		for (size_t i = 0; i < _order.size(); ++i) // all datums in scope
		{
			if (_order[i]->getDecidedType() == Datum::DatumType::Table) // scopes only
			{
				for (size_t j = 0; j < _order[i]->size(); ++j) // all scopes in datum
				{
					delete _order[i]->at<Scope*>(j); // destruct 
				}
			}
		}
	}

	void Scope::Clear()
	{

		ClearDatumsOfTypeScope();

		_data.clear();
		_order.clear();
	}

	Scope* Scope::Clone() const
	{
		return new Scope(*this);
	}

	Datum* Scope::Find(const std::string& key)
	{
		if (_data.empty()) return nullptr;

		return _data.count(key) > 0 ? &(_data.at(key)) : nullptr;
	}

	const Datum* Fiea::GameEngine::Scope::Find(const std::string& key) const
	{
		if (_data.empty()) return nullptr;

		return _data.count(key) > 0 ? &(_data.at(key)) : nullptr;
	}
	Datum* Scope::FindContainedScope(const Scope& child, int& idx)
	{
		for (Datum* datum : _order)
		{
			if (datum->getDecidedType() == Datum::DatumType::Table)
			{
				for (size_t i = 0; i < datum->size(); ++i)
				{
					//Scope** &s = *datum->getUnionArrayGivenType<Scope**>();
					if (&child == *datum->getScopeUnionData())
					{
						idx = (int)i;
						return datum;
					}
				}
			}
		}
		return nullptr;

	}
	Datum& Scope::Append(const std::string& key)
	{
		if (_data.count(key) > 0) // Key found
		{
			return _data.at(key);
		}
		else // Key Doesn't exist
		{
			_data.emplace(key, Datum());
			_order.push_back(&_data[key]);

			return (_data.at(key));
		}
	}
	Scope& Scope::AppendScope(const std::string& key)
	{
		Datum& d = Append(key);

		//if (d.getDecidedType() != Datum::DatumType::Table) throw std::runtime_error("");
		

		Scope* s = new Scope();
		s->_parent = this;

		d.addToBack(s);
		return *s;
	}
	Scope& Scope::AppendScope(const std::string& name, const std::string& type)
	{
		if (type == "" || type == "Scope")
		{
			return AppendScope(name);
		}
		else
		{
			//Get the datum
			Datum& d = Append(name);


			//Make the scope
			IFactory<Scope>* factory = FactoryManager<Scope>::Find(type);
			Scope* factoriedScope = factory->Create();
			factoriedScope->_parent = this;

			//Add it to the datum
			d.addToBack(factoriedScope);
			return *factoriedScope;
		}
	}
	Scope& Scope::AppendScope(const std::string& name, Scope* scope)
	{
		//Get the datum
		Datum& d = Append(name);

		scope->_parent = this;

		//Add it to the datum
		d.addToBack(scope);

		return *scope;

	}
	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}
	Datum& Scope::operator[](size_t idx)
	{
		return *_order[idx];
	}
	const Datum& Scope::operator[](size_t idx) const
	{
		return *_order[idx];
	}
	bool Scope::operator==(const Scope& rhs)
	{
		//check if size is equal
		if (_order.size() != rhs._order.size())
		{
			return false;
		}

		// Check keys
		for (const auto& pair : _data)
		{

			const auto& rhsPair = rhs._data.find(pair.first);


			if (rhsPair == rhs._data.end())
			{
				return false;

			}
			else if(pair.second != rhsPair->second)
			{
				if (rhsPair->first != "this")
				{
					return false;

				}
			}

		}

		//return true;
		return true;
	}
	bool Scope::operator!=(const Scope& rhs)
	{
		return !(operator==(rhs));
	}
	Datum* Scope::Search(const std::string& key, Scope** scope)
	{
		Datum* findResult = Find(key);

		if (findResult == nullptr) // Not found
		{
			if (_parent != nullptr)
			{
				return _parent->Search(key, scope);
			}

			return nullptr;

		}
		else // Found
		{
			if (scope != nullptr)
			{
				*scope = this;
			}
			
			return findResult;
		}

	}
	const Datum* Scope::Search(const std::string& key,  const Scope** scope) const
	{
		const Datum* findResult = Find(key);

		if (findResult == nullptr) // Not found
		{
			if (_parent != nullptr)
			{
				return _parent->Search(key, scope);
			}

			return nullptr;

		}
		else // Found
		{
			if (scope != nullptr)
			{
				*scope = this;
			}
			
			return findResult;
		}
	}
	void Scope::Adopt(const std::string& key, Scope& child)
	{
		//Check that the scope to adopt is not already in the family
		//Make sure that !isDescendent and !isAncestor of this
		//ancestor - comes before you in ownership
		//descendent - comes after you in ownership

		if (child.isAncestorOf(*this) || child.isDescendantOf(*this))
			throw std::exception("Cannot adopt a descendant or ancestor");

		Datum& d = Append(key);


		if (child._parent == nullptr)
		{
			Scope* sPtr = &child;
			d.addToBack(sPtr);
			child._parent = this;
		}
		else
		{
			Scope* sPtr = &child;
	
			child._parent->Orphan(child);
			d.addToBack(sPtr);
			child._parent = this;
		}
	}
	Scope* Scope::Orphan(Scope& child)
	{
		for (auto it = _data.begin(); it != _data.end(); ++it) // For all datums in the table
		{
			if (it->second.getDecidedType() == Datum::DatumType::Table) // Datums that are of type scope
			{
				for (size_t i = 0; i < it->second.size(); ++i) // For all scopes in the datum
				{
					if (*it->second.at<Scope*>(i) == child)
					{
						it->second.removeAt(i);
						child._parent = nullptr;
						return &child;
					}
				}
			}
		}
		return nullptr;

	}
	Scope* Scope::GetParent() const
	{
		return _parent;
	}
	bool Scope::isAncestorOf(const Scope& child) const
	{
		// Base case
		if (this == &child)
			return true;

		// Reached highest scope
		if (child._parent == nullptr)
			return false;

		// Climb up parent
		return this->isAncestorOf(*child._parent);
	}
	bool Scope::isDescendantOf(const Scope& parent) const
	{
		return parent.isAncestorOf(*this);
	}
}