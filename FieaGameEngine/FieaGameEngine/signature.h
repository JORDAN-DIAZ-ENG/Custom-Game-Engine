#pragma once
#include <string>
#include "Datum.h"

namespace Fiea::GameEngine 
{
	struct Signature
	{
		std::string Name;
		Datum::DatumType Type;
		size_t Size;
		size_t offset;
	};
}