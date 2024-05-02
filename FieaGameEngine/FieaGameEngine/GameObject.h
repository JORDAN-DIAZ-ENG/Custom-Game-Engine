#pragma once
#include "Attributed.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

#include "signature.h"
#include "TypeManager.h"
#include "Factory.h"

#include "Action.h"

#include "GameClock.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{
	struct Transform {
		glm::vec4 Position;
		glm::vec4 Rotation;
		glm::vec4 Scale;
	};

	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	public:

		GameObject();
		[[nodiscard]] GameObject* Clone() const override;


		/**
		 * @brief Self updates, this class specific
		 * @param deltaTime 
		*/
		virtual void UpdateSelf(const GameTime& deltaTime)
		{
			
		}

		/**
		 * @brief Update all children of this class and self
		 * @param deltaTime 
		*/
		virtual void Update(const GameTime& deltaTime); // Calls update on all children
	
		/**
		 * @brief returns the name
		 * @return 
		*/
		const std::string& getName() const;

		/**
		 * @brief returns the transform position
		 * @return 
		*/
		const glm::vec4& getPosition() const;

		/**
		 * @brief returns the tranform rotation
		 * @return 
		*/
		const glm::vec4& getRotation() const;

		/**
		 * @brief returns the transform scale
		 * @return 
		*/
		const glm::vec4& getScale() const;

		/**
		 * @brief sets the name given a string
		 * @param newName 
		*/
		void setName(const std::string& newName);

		/**
		 * @brief sets the position given a vector 4
		 * @param newPosition 
		*/
		void setPosition(glm::vec4 newPosition);

		/**
		 * @brief sets the rotation given a vector 4
		 * @param newRotation 
		*/
		void setRotation(glm::vec4 newRotation);

		/**
		 * @brief sets the scale given a vector 4
		 * @param newScale 
		*/
		void setScale(glm::vec4 newScale);

		/**
		 * @brief returns the transform 
		 * @return 
		*/

		const Transform& getTransform() const;

		/**
		 * @brief retrieves a child from a string name
		 * @param name 
		 * @return 
		*/
		virtual GameObject* getChild(const std::string& name);

		/**
		 * @brief getter for returning the Actions datum
		 * @return 
		*/
		Datum* Actions();

		/**
		 * @brief Takes a class name and an instance name to instantiate a new action object and adopt it into the actions datum
		 * @param className 
		 * @param instanceName 
		 * @return 
		*/
		void CreateAction(const std::string& className, const std::string& instanceName);



		static std::vector<Signature> Signatures(); //Prescribed
		static const void* signaturesAppended;

	protected:
		GameObject(RTTI::IdType passedInTypeID);


	private:
		std::string Name;

		Transform transform;

		std::unordered_map<std::string, GameObject*> _childLookup;

	};

	MAKE_FACTORY(Scope, GameObject);
}