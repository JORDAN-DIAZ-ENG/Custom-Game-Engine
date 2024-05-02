#pragma once

#include <stdexcept>
#include <algorithm>
#include "Attributed.h"
#include "signature.h"
//#include "Foo.h"

namespace Fiea::GameEngine
{


	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:
		static const std::size_t arraySize = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo& other) = default;
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;

		bool operator==(const AttributedFoo& other);

		bool operator!=(const AttributedFoo& other);

		~AttributedFoo() = default;
		[[nodiscard]] AttributedFoo* Clone() const override;

		int externalInteger = 0;
		float externalFloat = 0;
		std::string externalString;
		glm::vec4 externalVector;
		glm::mat4 externalMatrix;
		//Foo* externalPointer;

		int externalIntegerArray[arraySize];
		float externalFloatArray[arraySize];
		std::string externalStringArray[arraySize];
		glm::vec4 externalVectorArray[arraySize];
		glm::mat4 externalMatrixArray[arraySize];
		//Foo* externalPointerArray[arraySize];

		//bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override { return Attributed::ToString(); }

		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;

	protected:
		AttributedFoo(RTTI::IdType passedInTypeID);


	};


}