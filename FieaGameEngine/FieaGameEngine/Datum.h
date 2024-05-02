#pragma once
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "RTTI.h"


namespace Fiea::GameEngine
{
	class Scope; // Forward declare scope class
	class RTTI;  // Forward declare RTTI class


	/**
	 * @brief Datum: Sequential container which can represents scalar values or lists of type int, float, std::string, glm::vec4, or glm::mat4x4
	*/
	class Datum
	{
	public:
		/**
		 * @brief Default Constructor: Initializes Datum to be empty and doesn't allocate any memory
		*/
		Datum() = default;

		/**
		 * @brief Scalar Constructor: Initializes a scalar of type Integer
		 * @param item: The scalar value of type int
		*/
		Datum(int item);

		/**
		* @brief Scalar Constructor: Initializes a scalar of type Float
		* @param item: The scalar value of type float
		*/
		Datum(float item);
		/**
		* @brief Scalar Constructor: Initializes a scalar of type String
		* @param item: The scalar value of type std::string
		*/
		Datum(const std::string& item);

		/**
		* @brief Scalar Constructor: Initializes a scalar of type Vector4
		* @param item: The scalar value of type glm::vec4
		*/
		Datum(glm::vec4 item);

		/**
		* @brief Scalar Constructor: Initializes a scalar of type Matrix4x4
		* @param item: The scalar value of type glm::mat4x4
		*/
		Datum(glm::mat4x4 item);


		/**
		 * @brief Scalar Constructor: Initializes a scalar of type Scope*
		 * @param item: The scalar value of type Scope*
		*/
		Datum(Scope*& item);

		Datum(RTTI*& item);

		/**
		 * @brief Copy Constructor: Constructs a Datum object using another Datum (Deep-copy)
		 * @param other: The incoming Datum
		*/
		Datum(const Datum& other);

		/**
		 * @brief Copy Assignment Operator: Deep-copy the contents of the given Datum into this one
		 * 4 possible scenarios:
		 * lhs = internal; rhs = internal
		 * lhs = internal; rhs = external <- NOT SUPPORTED
		 * lhs = external; rhs = internal <- NOT SUPPORTED
		 * lhs = external; rhs = external
		 * @param rhs: The incoming Datum 
		 * @return The modified Datum
		*/
		Datum& operator= (const Datum& rhs);

		/**
		 * @brief treats the datum as a scalar value and 
		 * @param rhs: rhs 
		 * @return lhs scalar
		*/
		Datum& operator= (int rhs);

		/**
		 * @brief 
		 * @param rhs 
		 * @return 
		*/
		Datum& operator= (float rhs);
		/**
		 * @brief 
		 * @param rhs 
		 * @return 
		*/
		Datum& operator= (const std::string& rhs);
		/**
		 * @brief 
		 * @param rhs 
		 * @return 
		*/
		Datum& operator= (glm::vec4 rhs);
		/**
		 * @brief 
		 * @param rhs 
		 * @return 
		*/
		Datum& operator= (glm::mat4x4 rhs);



		/**
		 * @brief Move Constructor: Copies over the data from another Datum using Move Semantics
		 * @param other: A different Datum R Value
		*/
		Datum(Datum&& other) noexcept;



		/**
		 * @brief Move Assignment Operator: Copies the content of another Datum into the existing Datum
		 * @param other: A different Datum R Value
		 * @return 
		*/
		Datum& operator= (Datum&& other) noexcept;

		/**
		 * @brief Destructor: Frees any memory owned by this object
		*/
		~Datum();


		/**
		 * @brief addToBack: Adds an L value of a supported type to the back. Allocates memory if needed. Not supported with External
		 * @tparam T: Anonymous 
		 * @param value: item to add to the back 
		*/
		template<class T>
		void addToBack(const T& value);

		/**
		 * @brief addToBack: Adds an R value of a supported type to the back. Allocates memory if needed. Not supported with External
		 * @tparam T: Anonymous
		 * @param value: item to add to the back
		*/
		//template<class T>
		//void addToBack(T&& value);

		/**
		 * @brief removeFromBack: removes the last element of the Datum
		*/
		void removeFromBack();

		/*
		ToString
			* converts the item to string
		*/

		/**
		 * @brief ToString: Converts item at index to its string representation
		 * @param index: the index of the datum array
		 * @return string representation of the item at index
		*/
		std::string ToString(int index);

		/**
		 * @brief DatumType: enum representing the supported types
		*/
		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			String,
			Vector4,
			Matrix4x4,
			Table,
			Pointer
		};

		void removeAt(size_t index);


		/*
		Clear
			* removes all the elemnts and sets size to 0
		*/


		/**
		 * @brief Clear: removes all the elements and sets size to 0; NOT SUPPORTED BY EXTERNAL
		*/
		void clear();

		/*
		operator==
			* does a member wise comparison and returns true if both containers have equal members otherwise it is false
		*/

		/**
		 * @brief operator==: does a member wise comparison and returns true if both containers have equal members otherwise it is false
		 * @param rhs: right hand side datum
		 * @return true or false, if they are equivalent return true, else return false
		*/
		bool operator==(const Datum& rhs) const;

		/**
		 * @brief operator==: supported only if datum is a scalar, compares the rhs if equal values, types must match
		 * @param rhs: right hand side int 
		 * @return true or false, if they are equivalent return true, else return false
		*/
		bool operator==(int rhs) const;

		/**
		 * @brief operator==: supported only if datum is a scalar, compares the rhs if equal values, types must match
		 * @param rhs: right hand side float
		 * @return true or false, if they are equivalent return true, else return false
		*/
		bool operator==(float rhs) const;

		/**
		 * @brief operator==: supported only if datum is a scalar, compares the rhs if equal values, types must match
		 * @param rhs: right hand side string
		 * @return true or false, if they are equivalent return true, else return false
		*/
		bool operator==(std::string rhs) const;

		/**
		 * @brief operator==: supported only if datum is a scalar, compares the rhs if equal values, types must match
		 * @param rhs: right hand side vector4
		 * @return true or false, if they are equivalent return true, else return false
		*/
		bool operator==(glm::vec4 rhs) const;

		/**
		 * @brief operator==: supported only if datum is a scalar, compares the rhs if equal values, types must match
		 * @param rhs: right hand side float
		 * @return true or false, if they are equivalent return true, else return false
		*/
		bool operator==(glm::mat4x4 rhs) const;


		bool operator==(Scope* rhs) const;

		/**
		 * @brief operator!=: does a member wise comparison returns true if there is inequality
		 * @param rhs: right hand side datum
		 * @return true or false, if they are NOT equivalent return true, else return false
		*/
		bool operator!=(const Datum& rhs) const;

		/**
		 * @brief operator!=: supported only if datum is a scalar, compares the rhs if not equal values, types must match
		 * @param rhs: right hand side int
		 * @return true or false, if they are NOT equivalent return true, else return false
		*/
		bool operator!=(int rhs) const;

		/**
		 * @brief operator!=: supported only if datum is a scalar, compares the rhs if not equal values, types must match
		 * @param rhs: right hand side float
		 * @return true or false, if they are NOT equivalent return true, else return false
		*/
		bool operator!=(float rhs) const;

		/**
		 * @brief operator!=: supported only if datum is a scalar, compares the rhs if not equal values, types must match
		 * @param rhs: right hand side string
		 * @return true or false, if they are NOT equivalent return true, else return false
		*/
		bool operator!=(std::string rhs) const;

		/**
		 * @brief operator!=: supported only if datum is a scalar, compares the rhs if not equal values, types must match
		 * @param rhs: right hand side vector4
		 * @return true or false, if they are NOT equivalent return true, else return false
		*/
		bool operator!=(glm::vec4 rhs) const;

		/**
		 * @brief operator!=: supported only if datum is a scalar, compares the rhs if not equal values, types must match
		 * @param rhs: right hand side matrix4x4
		 * @return true or false, if they are NOT equivalent return true, else return false
		*/
		bool operator!=(glm::mat4x4 rhs) const;



		/**
		 * @brief Resize: Changes the size of the 
		 * @param newSize: the new size to resize to
		*/
		void resize(size_t newSize);

		/*
		at
			* returns the item at the index, throws an error if out of range 
		*/

		/**
		 * @brief at: returns the item at the index, throws an error if out of range
		 * @tparam T: the type of the datum 
		 * @param idx: the index to look at
		 * @return returns the item at the index if in range
		*/
		template<class T>
		T& at(size_t idx);

		/**
		 * @brief at: returns the item at the index, throws an error if out of range, CONST CORRECT
		 * @tparam T: the type of the datum
		 * @param idx: the index to look at
		 * @return returns the item at the index if in range
		*/
		template<class T>
		const T& at(size_t idx) const;


		/**
		 * @brief empty: tells you the status of the container being used or empty
		 * @return true if size = 0; else return false
		*/
		bool empty() const;

		/*
		size
			* returns the size
		*/

		/**
		 * @brief size: reveals the size of the
		 * @return number of elements in the container
		*/
		size_t size() const { return _size; }

		/**
		 * @brief Capacity: returns the capacity
		 * @return: max number of elements available in the container
		*/
		size_t capacity() const { return _capacity; }


		/**
		 * @brief getDecidedType: 
		 * @return gives the datum type that is currently set
		*/
		DatumType getDecidedType();


		const DatumType getDecidedType() const;
		
		/**
		 * @brief getTypeEnum: converts type to supported type enum
		 * @tparam T: type to convert to enum
		 * @return datumType enum
		*/
		template<class T>
		DatumType getTypeEnum() const;




		/**
		 * @brief setItemAt: sets an item at an index
		 * @tparam T: type
		 * @param item: the item to place 
		 * @param index: the index of the item
		*/
		template<class T>
		void setItemAt(const T& item, size_t index);

		/**
		 * @brief SetAsString: converts string to item at index
		 * @param s: string representation of known type 
		 * @param index: index of item
		*/
		void SetAsString(const std::string& s, size_t index); // converts string to item and puts it into the data

		/**
		 * @brief addToBackFromString: converts string to item and adds it to the back
		 * @param s 
		*/
		void addToBackFromString(const std::string& s, DatumType type);

		/**
		 * @brief SetStorage: sets datum to external, only works if unknown of already external
		 * @tparam T: type of the incoming array 
		 * @param storage: the incoming array 
		 * @param size: number of elements in array 
		*/
		template <class T>
		void SetStorage(T* storage, size_t size);

		template <class T>
		void SetStorage(T* storage, size_t size, DatumType type);

		template<class T>
		T* getUnionArrayGivenType() const;

		Scope** getScopeUnionData();

		RTTI** getRttiUnionData();

	private:

		/*
		reserve
			* creates new memory and sets capacity to given parameter
		*/

		/**
		 * @brief Reserve: creates new memory and sets capacity to given parameter; NOT SUPPORTED BY EXTERNAL
		 * @tparam T: passed in type
		 * @param capacity: the size to set, if less than current capacity it will do nothing
		*/
		template<class T>
		void reserve(size_t capacity);

		/**
		 * @brief shrink: handles the resize case of shrinking
		 * @param newSize: the new size to shrink to
		*/
		void shrink(const size_t& newSize);

		/**
		 * @brief shrink_to_fit: let the counter become the same size as size, capacity is set to size and the memory reflects that, NOT SUPPORTED BY EXTERNAL
		*/
		void shrink_to_fit();

		/**
		 * @brief initialize: initializes a datum
		 * @tparam T: type of datum
		 * @param item: first element
		*/
		template<class T>
		void initialize(const T& item);




		size_t getSizeOfType() const;





		void allocateMemory(size_t capacity, DatumType type);

		//Using pointers ensures that Type will always be the same size (8 bytes on 64 bit hardware)
		//pointers also allow for the type to be in an array
		union TypeUnion
		{
			int* _int;
			float* _float;
			std::string* _string;
			glm::vec4* _vec4;
			glm::mat4x4* _mat4x4;
			void* _anonymous; //Represents the memory of the Union
			Scope** _scope;
			RTTI** _rtti;
			
		};

		//Set as the default members when default constructing
		DatumType _type = DatumType::Unknown;
		TypeUnion _data{};
		size_t _size = 0;
		size_t _capacity = 0;
		bool isTypeDecided = false;
		bool isExternal = false;

};
}

#include "Datum.inl"