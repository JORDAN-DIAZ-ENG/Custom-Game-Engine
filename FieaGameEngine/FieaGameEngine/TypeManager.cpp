#include "pch.h"
#include "TypeManager.h"

namespace Fiea::GameEngine
{
	const std::vector<Signature>& TypeManager::get(size_t typeID)
	{
		assert(_map != nullptr);
		return _map->at(typeID);
	}

	void TypeManager::add(size_t typeID, const std::vector<Signature>& s)
	{
		//lazy initialization
		if (_map == nullptr) _map = new std::unordered_map<size_t, std::vector<Signature>>();

		auto ret = _map->insert(std::make_pair(typeID, s));

		assert(ret.second); // avoid double registration
	}
}