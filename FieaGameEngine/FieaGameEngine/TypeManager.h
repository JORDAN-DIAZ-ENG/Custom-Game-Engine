#pragma once
#include <unordered_map>
#include <vector>
#include "signature.h"

namespace Fiea::GameEngine
{
	class TypeManager
	{
	public:
		static const std::vector<Signature>& get(size_t typeID);

		static void add(size_t typeID, const std::vector<Signature>& s);

	private:
		inline static std::unordered_map<size_t, std::vector<Signature>>* _map;
	};
}

