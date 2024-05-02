#pragma once
#include "RTTI.h"
#include "Datum.h"
#include <unordered_map>
#include <vector>
#include <functional>


namespace Fiea::GameEngine {
	//A scope may either be parentless (root scope no ownning scope)
	//or if it lives in a datum, it must have a parent

	//Root scope, does not live in a datum

	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		/**
		 * @brief Default Constructor: optionally reserves size
		 * @param initialCapacity: optional sizing of map
		*/
		Scope(size_t initialCapacity = 0); //Default Constructor

		/**
		 * @brief Copy Constructor: copies another Scope
		 * @param other: the other scope
		*/
		Scope(const Scope& other); //Copy Constructor

		Scope(Scope&& other); //Move Constructor

		/**
		 * @brief Operator=: Copy assignment
		 * @param rhs: right hand side
		 * @return current scope reference
		*/
		Scope& operator=(const Scope& rhs); //Assignment Operator

		
		/**
		 * @brief Destructor
		*/
		virtual ~Scope(); //Virtual destructor so that children also get destroyed



		/**
		 * @brief Clears the contents of the map
		*/
		void Clear();

		/**
		 * @brief Clone: clones a scope
		 * @param other: incoming scope
		 * @return scope pointer to the clone
		*/
		virtual [[nodiscard]] Scope* Clone() const;

		/**
		 * @brief Find: finds a datum in the existing scope
		 * @param key: key to look for
		 * @return datum pointer of found datum, if not found return nullptr
		*/
		Datum* Find(const std::string& key);
		const Datum* Find(const std::string& key) const;

		/**
		 * @brief Finds the datum that a scope is contained in
		 * @param child the scope to search
		 * @param idx output param to take in
		 * @return 
		*/
		Datum* FindContainedScope(const Scope& child, int& idx);
		//std::pair<Datum*, std::size_t> FindContainedScope(const Scope& child, int& idx) const;

		/**
		 * @brief add a datum to the scope
		 * @param key 
		 * @return 
		*/
		Datum& Append(const std::string& key);


		/**
		 * @brief add a scope to the scope
		 * @param key 
		 * @return 
		*/
		Scope& AppendScope(const std::string& key);

		Scope& AppendScope(const std::string& name, const std::string& type);

		Scope& AppendScope(const std::string& name, Scope* scope);

		/**
		 * @brief same as append
		 * @param key 
		 * @return 
		*/
		Datum& operator[](const std::string& key);

		/**
		 * @brief datum at index
		 * @param idx 
		 * @return 
		*/
		Datum& operator[](size_t idx);
		const Datum& operator[](size_t idx) const;

		/**
		 * @brief comparison between two scopes
		 * @param rhs 
		 * @return 
		*/
		bool operator==(const Scope& rhs);
		bool operator!=(const Scope& rhs);

		/**
		 * @brief searches ancestors for key
		 * @param key 
		 * @param scope 
		 * @return 
		*/
		Datum* Search(const std::string& key, Scope** scope = nullptr); //the opposite of using a tuple
		const Datum* Search(const std::string& key, const Scope** scope = nullptr) const; //the opposite of using a tuple

		/**
		 * @brief melds a scope to be a child of this
		 * @param key 
		 * @param child 
		*/
		void Adopt(const std::string& key, Scope& child);

		/**
		 * @brief removes parents
		 * @param child 
		*/
		Scope* Orphan(Scope& child);

		/**
		 * @brief returns parent
		 * @return 
		*/
		Scope* GetParent() const;

		/**
		 * @brief determines if this is an ancestor of a given scope
		 * @param child 
		 * @return 
		*/
		bool isAncestorOf(const Scope& child) const;

		/**
		 * @brief determines if this is a descendant of a given scope
		 * @param parent 
		 * @return 
		*/
		bool isDescendantOf(const Scope& parent) const;

		//RTTI Implementations
		virtual std::string ToString() const override { return ""; }
		virtual bool Equals(const RTTI* rhs) const override { return true; }

	private:
		/**
		* @brief Clears datums of type scope
		*/
		void ClearDatumsOfTypeScope();

		Scope* _parent = nullptr;
		std::unordered_map<std::string, Datum> _data{};
		std::vector<Datum*> _order{};
	};
}