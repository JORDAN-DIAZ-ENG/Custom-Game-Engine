#pragma once
#include "Attributed.h"
#include "Factory.h"

namespace Fiea::GameEngine
{

	class Monster : public Attributed
	{
		RTTI_DECLARATIONS(Monster, Attributed);

	public:
		Monster();
		[[nodiscard]] Monster* Clone() const override;
		int intValue = 0;
		float floatValue = 0.0f;
		std::string stringValue = "";
		glm::vec4 vec4Value = glm::vec4(0);
		glm::mat4x4 mat4Value = glm::mat4x4(0);
		int intArr[2];
		float floatArr[2];
		std::string stringArr[2];
		glm::vec4 vec4Arr[2];
		glm::mat4x4 mat4x4Arr[2];


		//Scope genericScope;



		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;
	};


	MAKE_FACTORY(Scope, Monster);

}