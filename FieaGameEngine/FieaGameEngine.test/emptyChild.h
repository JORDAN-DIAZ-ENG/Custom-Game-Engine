#pragma once
#include "pch.h"
#include "RTTI.h"
#include "empty.h"

namespace Fiea::GameEngine::Test
{
	class EmptyChild : public Empty
	{
		RTTI_DECLARATIONS(EmptyChild, Empty);



	};
}

