#pragma once

#include <stdexcept>
#include <algorithm>
#include "Attributed.h"
#include "signature.h"
#include "AttributedFoo.h"
//#include "Foo.h"

namespace Fiea::GameEngine
{


	class AttributedBar final : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, Attributed);

	public:
		static const std::size_t arraySize = 5;

		AttributedBar();
		AttributedBar(const AttributedBar& other) = default;
		AttributedBar(AttributedBar&& rhs) noexcept = default;
		AttributedBar& operator=(const AttributedBar& rhs) = default;
		AttributedBar& operator=(AttributedBar&& rhs) noexcept = default;

		bool operator==(const AttributedBar& other);

		bool operator!=(const AttributedBar& other);

		virtual ~AttributedBar() = default;
		[[nodiscard]] AttributedBar* Clone() const override;

		int barSpecificExternalInt;

		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;
	};


}