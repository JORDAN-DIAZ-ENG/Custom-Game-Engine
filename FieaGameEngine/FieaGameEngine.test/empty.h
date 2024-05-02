#pragma once
#include "pch.h"
#include "RTTI.h"

namespace Fiea::GameEngine::Test
{
	class Empty : public RTTI
	{
		RTTI_DECLARATIONS(Empty, RTTI);

	public:
		Empty();
	};
}
