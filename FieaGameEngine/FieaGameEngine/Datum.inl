#include "Datum.h"
#include <assert.h>
#include <type_traits>
#include <stdexcept>

using namespace Fiea::GameEngine;



template<class T>
inline void Fiea::GameEngine::Datum::addToBack(const T& value)
{
	if (isExternal) throw std::invalid_argument("Cannot add to an external value");

	//if adding unknown type return
	DatumType incomingType = getTypeEnum<T>();
	if (incomingType == DatumType::Unknown)
	{
		throw std::runtime_error("Cannot Add Unknown Type to back");
	}

	//only do if currently unknown type and not been decided; or has been decided and incoming is same type
	if ((_type == DatumType::Unknown && !isTypeDecided) || (isTypeDecided && _type == incomingType))
	{
		
		if (_size == _capacity)
		{
			_size == 0 ? reserve<T>(1) : reserve<T>(_size * 2);
		}

		new (getUnionArrayGivenType<T>() + _size) T(value);
		++_size;

		_type = incomingType;
		isTypeDecided = true;
	}
	else
	{
		throw std::runtime_error("Cannot Add Mismatched Type to back");
		return;
	}
	
}

//template<class T>
//inline void Fiea::GameEngine::Datum::addToBack(T&& value)
//{
//	if (isExternal) throw std::invalid_argument("Cannot add to an external value");
//
//	//if adding unknown type return
//	DatumType incomingType = getTypeEnum<T>();
//	if (incomingType == DatumType::Unknown)
//	{
//		throw std::runtime_error("Cannot Add Unknown Type to back");
//	}
//
//	//only do if currently unknown type and not been decided; or has been decided and incoming is same type
//	if ((_type == DatumType::Unknown && !isTypeDecided) || (isTypeDecided && _type == incomingType))
//	{
//
//		if (_size == _capacity)
//		{
//			_size == 0 ? reserve<T>(1) : reserve<T>(_size * 2);
//		}
//
//
//		auto cringe = getUnionArrayGivenType<T>() + _size;
//		new (cringe) T(std::move(value));
//		++_size;
//
//		_type = incomingType;
//		isTypeDecided = true;
//	}
//	else
//	{
//		throw std::runtime_error("Cannot Add Mismatched Type to back");
//		return;
//	}
//}

template<class T>
inline void Fiea::GameEngine::Datum::reserve(size_t capacity)
{
	if (capacity > _capacity)
	{
		//Allocate new memory
		T* newData = (T*)malloc(sizeof(T) * capacity);
		assert(newData != nullptr);

		//Copy the elements to the allocated memory
		if (_size > 0)
		{
			for (size_t i = 0; i < _size; ++i)
			{
				new(newData + i) T(getUnionArrayGivenType<T>()[i]);
			}
		}

		//Delete the old data
		for (size_t i = 0; i < _size; ++i)
		{
			(getUnionArrayGivenType<T>() + i)->~T();
		}
		free(getUnionArrayGivenType<T>());

		//Set capacity and data
		_capacity = capacity;
		_data._anonymous = newData;
	}
}

template<class T>
inline T& Fiea::GameEngine::Datum::at(size_t idx)
{
	if (_size > 0)
	{
		if (getTypeEnum<T>() != _type)
		{
			throw std::runtime_error("Type Mismatch");
		}

		if (idx < _size)
		{
			return getUnionArrayGivenType<T>()[idx];
		}
		else
		{
			throw std::out_of_range("Index out of range");
		}
	}
	throw std::out_of_range("Index out of range");
}

template<class T>
inline const T& Fiea::GameEngine::Datum::at(size_t idx) const
{
	if (_size > 0)
	{
		if (getTypeEnum<T>() != _type)
		{
			throw std::runtime_error("Type Mismatch");
		}

		if (idx < _size)
		{
			return getUnionArrayGivenType<T>()[idx];
		}
		else
		{
			throw std::out_of_range("Index out of range");
		}
	}
	throw std::out_of_range("Index out of range");
}



template<class T>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum() const
{
	return DatumType::Unknown;
}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<int>() const
{
	return DatumType::Integer;
}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<float>() const
{
	return DatumType::Float;
}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<std::string>() const
{
	return DatumType::String;
}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<glm::vec4>() const
{
	return DatumType::Vector4;
}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<glm::mat4x4>() const
{
	return DatumType::Matrix4x4;
}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<Scope*>() const
{
	return DatumType::Table;
}

//template<>
//inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<Scope**>() const
//{
//	return DatumType::Table;
//}

template<>
inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<RTTI*>() const
{
	return DatumType::Pointer;
}

//template<>
//inline Datum::DatumType Fiea::GameEngine::Datum::getTypeEnum<RTTI**>() const
//{
//	return DatumType::Pointer;
//}

template<class T>
inline void Fiea::GameEngine::Datum::initialize(const T& item)
{
	//Allocate Memory
	_data._anonymous = (T*)malloc(sizeof(T) * 1); //Allocate memory for a pointer (8 bytes)
	assert(_data._anonymous != nullptr);

	//Create memory to store the number of pointers of data
	new (_data._anonymous) T(item); //Place the value of i into 

	_size = 1;
	_capacity = 1;
}


template<class T>
inline void Fiea::GameEngine::Datum::SetStorage(T* storage, size_t size)
{
		
	//Only support if _type == Unknown or an if isExternal and the incoming type matches

	if (_type == DatumType::Unknown || isExternal)
	{
		if (isExternal && _type == getTypeEnum<T>())
		{
			//Copy your data into the storage


			_data._anonymous = storage;
			_capacity = size;
			_size = size;
		}
		else
		{
			_data._anonymous = storage;
			_type = getTypeEnum<T>();
			isTypeDecided = true;
			_capacity = size;
			_size = size;
			isExternal = true;
		}
	}
	else
	{
		throw std::invalid_argument("Cannot SetStorage on set Types");
	}

}

template<class T>
inline void Fiea::GameEngine::Datum::SetStorage(T* storage, size_t size, DatumType type)
{
	//Only support if _type == Unknown or an if isExternal and the incoming type matches

	if (_type == DatumType::Unknown || isExternal)
	{
		if (isExternal && _type == getTypeEnum<T>())
		{
			//Copy your data into the storage


			_data._anonymous = storage;
			_capacity = size;
			_size = size;
		}
		else
		{
			_data._anonymous = storage;
			_type = type;
			isTypeDecided = true;
			_capacity = size;
			_size = size;
			isExternal = true;
		}
	}
	else
	{
		throw std::invalid_argument("Cannot SetStorage on set Types");
	}
}

template<class T>
inline T* Fiea::GameEngine::Datum::getUnionArrayGivenType() const
{
	// why are you typecasting the outputs here? this is dangerous and could hide issues

	switch (getTypeEnum<T>())
	{
	case DatumType::Integer:
		return reinterpret_cast<T*>(_data._int);
	case DatumType::Float:
		return reinterpret_cast<T*>(_data._float);
	case DatumType::String:
		return reinterpret_cast<T*>(_data._string);
	case DatumType::Vector4:
		return reinterpret_cast<T*>(_data._vec4);
	case DatumType::Matrix4x4:
		return reinterpret_cast<T*>(_data._mat4x4);
	case DatumType::Table:
		return reinterpret_cast<T*>(_data._scope);
	default:
		return reinterpret_cast<T*>(_data._anonymous);
	}
}

template<class T>
inline void Fiea::GameEngine::Datum::setItemAt(const T& item, size_t index)
{
	DatumType incomingType = getTypeEnum<T>();
	if (incomingType != DatumType::Unknown && _type == incomingType)
	{
		getUnionArrayGivenType<T>()[index] = item;
	}


}

