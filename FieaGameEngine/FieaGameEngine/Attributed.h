#pragma once
#include "Scope.h"
#include "signature.h"

namespace Fiea::GameEngine
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		virtual ~Attributed() = default; //Destructor - must be virtual

		//Copy semantics

		/**
		 * @brief Copy Constructor
		 * @param rhs: value to copy from
		*/
		Attributed(const Attributed& rhs);

		/**
		 * @brief Move Copy Constructor
		 * @param rhs: value to copy from. will result in rhs being unusable
		*/
		Attributed(Attributed&& rhs) noexcept;	

		/**
		 * @brief Assignment Copy
		 * @param rhs: value to copy from
		 * @return: This with the new data
		*/
		Attributed& operator=(const Attributed& rhs);

		/**
		 * @brief Move Assignment Copy
		 * @param rhs: value to copy from. will result in rhs being unusable
		 * @return: This with the new data
		*/
		Attributed& operator=(Attributed&& rhs) noexcept;

		/**
		 * @brief Virtual constructor, handles construction on the any inheritance level using co-variance
		 * @return: A clone of the attribute
		*/
		virtual [[nodiscard]] Attributed* Clone() const override = 0;


		/**
		 * @brief mechanism to populate the scope with prescribed members
		 * @param typeID: given from any class who inherits RTTI
		*/
		void populateWithPrescribedMembers(size_t typeID);

		/**
		 * @brief isAttribute
		 * @param s: string key
		 * @return: bool indicating whether it is the name of an attribute for this object
		*/
		bool isAttribute(const std::string& s) const;

		/**
		 * @brief isPrescribedAttribute
		 * @param s: string key
		 * @return: bool indicating whether it is the name of a prescribed attribute for this class
		*/
		bool isPrescribedAttribute(const std::string& s) const;

		/**
		 * @brief isAuxiliaryAttribute
		 * @param s: string key
		 * @return: bool indicating whether it is the name of an auxiliary attribute for this object
		*/
		bool isAuxiliaryAttribute(const std::string& s) const;

		/**
		 * @brief AppendAuxiliaryAttribute
		 * @param key: string key
		 * @return: datum that is an auxiliary attribute
		*/
		Datum& AppendAuxiliaryAttribute(const std::string& key);

	protected:
		/**
		 * @brief Constructor
		 * @param typeID: RTTI type ID
		*/
		explicit Attributed(RTTI::IdType typeID);

		/**
		 * @brief Stored Type ID
		*/
		size_t incomingTypeID;

	private:
		
		
	};
}