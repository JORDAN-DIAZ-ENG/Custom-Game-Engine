#include "pch.h"
#include "Datum.h"
#include <sstream>
#include <string>

namespace Fiea::GameEngine
{

	Datum::Datum(int item) : _type(DatumType::Integer), isTypeDecided(true)
	{
		initialize<int>(item);
	}
	Datum::Datum(float item) : _type(DatumType::Float), isTypeDecided(true)
	{
		initialize<float>(item);
	}
	Datum::Datum(const std::string& item) : _type(DatumType::String), isTypeDecided(true)
	{
		initialize<std::string>(item);
	}

	Datum::Datum(glm::vec4 item) : _type(DatumType::Vector4), isTypeDecided(true)
	{
		initialize<glm::vec4>(item);
	}

	Datum::Datum(glm::mat4x4 item) : _type(DatumType::Matrix4x4), isTypeDecided(true)
	{
		initialize<glm::mat4x4>(item);
	}

	Datum::Datum(Scope*& item) : _type(DatumType::Table), isTypeDecided(true)
	{
		initialize(item);

		*_data._scope = item;
	}

	Datum::Datum(RTTI*& item) : _type(DatumType::Pointer), isTypeDecided(true)
	{
		initialize(item);

		*_data._rtti = item;

	}

	Datum::Datum(const Datum& other)
	{

		//Set the capacity and size
		_type = other._type;
		_size = other._size;
		_capacity = other._capacity;
		isTypeDecided = other.isTypeDecided;
		isExternal = other.isExternal;

		//Supports lhs = external; rhs = external
		//Supports lhs = external; rhs = external

		if (isExternal)
		{
			_data = other._data;
		}
		else
		{
			allocateMemory(other._capacity, other._type);

			switch (other._type)
			{
			case Fiea::GameEngine::Datum::DatumType::Integer:
				for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._int + i) int(other.at<int>(i));
				}
				break;
			case Fiea::GameEngine::Datum::DatumType::Float:
				for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._float + i) float(other.at<float>(i));
				}
				break;
			case Fiea::GameEngine::Datum::DatumType::String:
				for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._string + i) std::string(other.at<std::string>(i));
				}
				break;
			case Fiea::GameEngine::Datum::DatumType::Vector4:
				for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._vec4 + i) glm::vec4(other.at<glm::vec4>(i));
				}
				break;
			case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
				for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._mat4x4 + i) glm::mat4x4(other.at<glm::mat4x4>(i));
				}
				break;
			case Fiea::GameEngine::Datum::DatumType::Table:
	/*			for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._scope + i) Scope**(other.at<Scope**>(i));
				}*/
				break;
			case Fiea::GameEngine::Datum::DatumType::Pointer:
				//for (size_t i = 0; i < other._size; ++i)
				//{
				//	new (_data._rtti + i) RTTI** (other.at<RTTI**>(i));
				//}
				break;
			case Fiea::GameEngine::Datum::DatumType::Unknown:
	/*			for (size_t i = 0; i < other._size; ++i)
				{
					new (_data._anonymous + i) void*(other.at<void*>(i));
				}*/
				break;
			default:
				throw std::exception("Error: Cannot copy construct with unknown type datum");
				//// don't silently succeed in this case... somethings wrong, right?
				//assert(false);
				break;
			}
		}
	



		

	}

	Datum& Datum::operator=(const Datum& rhs)
	{
		if (isExternal != rhs.isExternal)
		{
			throw std::invalid_argument("only supports lhs = external; rhs = external or lhs = internal; rhs = internal");
		}
		
		if (!isExternal && !rhs.isExternal)
		{
			if (this != &rhs)
			{
				_type = rhs._type;

				clear();
				_size = rhs._size;
				if (rhs._capacity > _capacity)
				{
					_capacity = rhs._capacity;
				}

				free(_data._string);

				allocateMemory(rhs._capacity, rhs._type);


				switch (rhs._type)
				{
				case Fiea::GameEngine::Datum::DatumType::Integer:
					for (size_t i = 0; i < _size; ++i)
					{
						new (_data._int + i) int(rhs.at<int>(i));
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Float:
					for (size_t i = 0; i < _size; ++i)
					{
						new (_data._float + i) float(rhs.at<float>(i));
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::String:
					for (size_t i = 0; i < _size; ++i)
					{
						new (_data._string + i) std::string(rhs.at<std::string>(i));
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Vector4:
					for (size_t i = 0; i < _size; ++i)
					{
						new (_data._vec4 + i) glm::vec4(rhs.at<glm::vec4>(i));
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
					for (size_t i = 0; i < _size; ++i)
					{
						new (_data._mat4x4 + i) glm::mat4x4(rhs.at<glm::mat4x4>(i));
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Table:
					_data._scope = rhs._data._scope; //this is a shallow copy, TODO: figure out what to do here
					//for (size_t i = 0; i < _size; ++i)
					//{
					//	new (_data._ + i) glm::mat4x4(rhs.at<glm::mat4x4>(i));
					//}
					break;
				default:
					break;
				}

			}
		}
		else if (isExternal && rhs.isExternal)
		{
			_data = rhs._data;
			_type = rhs._type;
			_capacity = rhs._capacity;
		}

		return *this;

	}

	Datum& Datum::operator=(int rhs)
	{
		//This Datum is not an Integer and Not Unknown
		if (_type != DatumType::Integer && _type != DatumType::Unknown)
		{
			throw std::invalid_argument("This Datum has been set to a non-int type");
		}

		if (_size == 1 && this->at<int>(0) == rhs) 
		{
			return *this;
		}

		//This Datum has not been set yet
		if (!isTypeDecided)
		{
			_type = DatumType::Integer;
			isTypeDecided = true;
			addToBack<int>(rhs);
		}
		else
		{
			resize(0);
			addToBack<int>(rhs);
		}

		return *this;
	}

	Datum& Datum::operator=(float rhs)
	{
		//This Datum is not an Integer and Not Unknown
		if (_type != DatumType::Float && _type != DatumType::Unknown)
		{
			throw std::invalid_argument("This Datum has been set to a non-int type");
		}

		if (_size == 1 && this->at<float>(0) == rhs)
		{
			return *this;
		}

		//This Datum has not been set yet
		if (!isTypeDecided)
		{
			_type = DatumType::Float;
			isTypeDecided = true;
			addToBack<float>(rhs);
		}
		else
		{
			resize(0);
			addToBack<float>(rhs);
		}

		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		//This Datum is not an Integer and Not Unknown
		if (_type != DatumType::String && _type != DatumType::Unknown)
		{
			throw std::invalid_argument("This Datum has been set to a non-int type");
		}

		if (_size == 1 && this->at<std::string>(0) == rhs)
		{
			return *this;
		}

		//This Datum has not been set yet
		if (!isTypeDecided)
		{
			_type = DatumType::String;
			isTypeDecided = true;
			addToBack<std::string>(rhs);
		}
		else
		{
			resize(0);
			addToBack<std::string>(rhs);
		}

		return *this;
	}

	Datum& Datum::operator=(glm::vec4 rhs)
	{
		//This Datum is not an Integer and Not Unknown
		if (_type != DatumType::Vector4 && _type != DatumType::Unknown)
		{
			throw std::invalid_argument("This Datum has been set to a non-int type");
		}

		if (_size == 1 && this->at<glm::vec4>(0) == rhs)
		{
			return *this;
		}

		//This Datum has not been set yet
		if (!isTypeDecided)
		{
			_type = DatumType::Vector4;
			isTypeDecided = true;
			addToBack<glm::vec4>(rhs);
		}
		else
		{
			resize(0);
			addToBack<glm::vec4>(rhs);
		}

		return *this;
	}

	Datum& Datum::operator=(glm::mat4x4 rhs)
	{
		//This Datum is not an Integer and Not Unknown
		if (_type != DatumType::Matrix4x4 && _type != DatumType::Unknown)
		{
			throw std::invalid_argument("This Datum has been set to a non-int type");
		}

		if (_size == 1 && this->at<glm::mat4x4>(0) == rhs)
		{
			return *this;
		}

		//This Datum has not been set yet
		if (!isTypeDecided)
		{
			_type = DatumType::Matrix4x4;
			isTypeDecided = true;
			addToBack<glm::mat4x4>(rhs);
		}
		else
		{
			resize(0);
			addToBack<glm::mat4x4>(rhs);
		}

		return *this;
	}

	Datum::Datum(Datum&& other) noexcept : _size(0), _capacity(0), isTypeDecided(false)
	{
		//Set this object
		_data = other._data;

		//Set the capacity and size
		_type = other._type;
		_size = other._size;
		_capacity = other._capacity;
		isTypeDecided = other.isTypeDecided;

		//Reset the other object
		other._data._anonymous = nullptr;
		other._size = 0;
		other._capacity = 0;

		isExternal = other.isExternal;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (this == &other) return *this;

		//release the resources of this object
		this->~Datum();

		//Move ownership of resources
		_data = other._data;
		_type = other._type;

		_size = other._size;
		_capacity = other._capacity;
		isTypeDecided = other.isTypeDecided;

		//Reset the other object
		other._data._anonymous = nullptr;
		other._size = 0;
		other._capacity = 0;

		return *this;
	}

	Datum::~Datum()
	{
		if (isExternal) return;

		//Destruction for POD types
		if (_type != DatumType::String)
		{
			delete(getUnionArrayGivenType<char>()); //triggers code coverage for default
			return;
		}

		//Clearing Memory
		clear();

		free(_data._string);
	}

	void Datum::removeFromBack()
	{
		if (isExternal) throw std::invalid_argument("Cannot remove from external memory");

		if (_size <= 0) return;

		if (_type == DatumType::String)
		{
			(_data._string + _size - 1)->~basic_string();
		}

		--_size;
	}

	std::string Datum::ToString(int index)
	{
		std::string str = "";
		switch (_type)
		{
		case Fiea::GameEngine::Datum::DatumType::Integer:
			str = std::to_string(at<int>(index));
			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			str = std::to_string(at<float>(index));
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			str = at<std::string>(index);
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:
			str = glm::to_string(at<glm::vec4>(index));
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			str = glm::to_string(at<glm::mat4x4>(index));
			break;
		default:
			//UNSUPPORTED TYPE 
			//TODO - handle case where the allocateMemory has a type of Unknown
			return str;
		}
		return str;
	}



	void Datum::clear()
	{
		if (isExternal) throw std::invalid_argument("Cannot clear external memory");

		if (_type == DatumType::String)
		{
			//Destruction for strings
			for (size_t i = 0; i < _size; ++i)
			{
				(_data._string + i)->~basic_string();
			}
		}

		_size = 0;
	}

	bool Datum::operator==(const Datum& rhs) const
	{
		if (_type != rhs._type) return false;

		if (_size != rhs._size) return false;

		if (empty() && rhs.empty()) return true;

		//member wise comparison, we know they're the same type and size so only need to check one
		switch (_type)
		{
		case Fiea::GameEngine::Datum::DatumType::Integer:
			for (size_t i = 0; i < _size; ++i)
			{
				if (at<int>(i) != rhs.at<int>(i)) {
					return false;
				}
			}
			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			for (size_t i = 0; i < _size; ++i)
			{
				if (at<float>(i) != rhs.at<float>(i)) {
					return false;
				}
			}
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			for (size_t i = 0; i < _size; ++i)
			{
				if (at<std::string>(i) != rhs.at<std::string>(i)) {
					return false;
				}
			}
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:
			for (size_t i = 0; i < _size; ++i)
			{
				if (at<glm::vec4>(i) != rhs.at<glm::vec4>(i)) {
					return false;
				}
			}
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			for (size_t i = 0; i < _size; ++i)
			{
				if (at<glm::mat4x4>(i) != rhs.at<glm::mat4x4>(i)) {
					return false;
				}
			}
			break;
		case Fiea::GameEngine::Datum::DatumType::Table:
			for (size_t i = 0; i < _size; ++i)
			{
				if (at<Scope*>(i) != rhs.at<Scope*>(i)) {
					return false;
				}
			}
			break;
		default:
			//UNSUPPORTED TYPE 
			//TODO - handle case where the allocateMemory has a type of Unknown
			return false;
		}
		return true;

	}

	bool Datum::operator==(int rhs) const
	{
		if (_size != 1) return false;
		if (_type != DatumType::Integer) return false;

		return at<int>(0) == rhs;
	}

	bool Datum::operator==(float rhs) const
	{
		if (_size != 1) return false;
		if (_type != DatumType::Float) return false;

		return at<float>(0) == rhs;
	}

	bool Datum::operator==(std::string rhs) const
	{
		if (_size != 1) return false;
		if (_type != DatumType::String) return false;

		return at<std::string>(0) == rhs;
	}

	bool Datum::operator==(glm::vec4 rhs) const
	{
		if (_size != 1) return false;
		if (_type != DatumType::Vector4) return false;

		return at<glm::vec4>(0) == rhs;
	}

	bool Datum::operator==(glm::mat4x4 rhs) const
	{
		if (_size != 1) return false;
		if (_type != DatumType::Matrix4x4) return false;

		return at<glm::mat4x4>(0) == rhs;
	}

	bool Datum::operator==(Scope* rhs) const
	{
		if (_size != 1) return false;
		if (_type != DatumType::Table) return false;

		return at<Scope*>(0) == rhs;
	}

	bool Datum::operator!=(const Datum& rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(int rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(float rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(std::string rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(glm::vec4 rhs) const
	{
		return !operator==(rhs);
	}

	bool Datum::operator!=(glm::mat4x4 rhs) const
	{
		return !operator==(rhs);
	}

	void Datum::resize(size_t newSize)
	{
		if (isExternal)
		{
			throw std::invalid_argument("Cannot resize an external");
		}

		if (_type == DatumType::Unknown) return;

		if (newSize < _size) //Shrink and throw away data outside
		{
			shrink(newSize);
		}
		else if (newSize > _size)
		{
			if (newSize == _capacity) 
			{
				//just fill items size-newSize
				switch (_type)
				{
				case Fiea::GameEngine::Datum::DatumType::Integer:
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._int + i)  int();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Float:
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._float + i)  float();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::String:
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._string + i)  std::string();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Vector4:
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._vec4 + i)  glm::vec4();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._mat4x4 + i)  glm::mat4x4();
					}
					break;

				}
			}
			else
			{
				//realloc, copy old and fill items size - 
				switch (_type)
				{
				case Fiea::GameEngine::Datum::DatumType::Integer:
					reserve<int>(newSize);
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._int + i)  int();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Float:
					reserve<float>(newSize);
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._float + i)  float();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::String:
					reserve<std::string>(newSize);
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._string + i)  std::string();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Vector4:
					reserve<glm::vec4>(newSize);
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._vec4 + i)  glm::vec4();
					}
					break;
				case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
					reserve<glm::mat4x4>(newSize);
					for (size_t i = _size; i < newSize; ++i)
					{
						new (_data._mat4x4 + i)  glm::mat4x4();
					}
					break;

				}
			}

		}
		else if (newSize == _size) //Shrink to fit
		{
			shrink_to_fit();
		}

		_size = newSize;
		_capacity = newSize;

		////Destruction for POD types
		//if (_type != DatumType::String)
		//{
		//	delete(_data._anonymous);
		//	return;
		//}
		//else
		//{
		//	//Clearing Memory
		//	clear();
		//}

		//free(_data._anonymous);

	}

	void Datum::shrink(const size_t& newSize)
	{
		//Allocate memory using malloc
		void* temp = malloc(getSizeOfType() * newSize);
		assert(temp != nullptr);

		//Assign the values to the new data
		switch (_type)
		{
		case Fiea::GameEngine::Datum::DatumType::Integer:
			for (size_t i = 0; i < newSize; ++i)
			{
				new (static_cast<int*>(temp) + i)  int(_data._int[i]);
			}

			delete(_data._anonymous);

			_data._int = static_cast<int*>(temp);

			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			for (size_t i = 0; i < newSize; ++i)
			{
				new (static_cast<float*>(temp) + i)  float(_data._float[i]);
			}

			delete(_data._anonymous);

			_data._float = static_cast<float*>(temp);
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			for (size_t i = 0; i < newSize; ++i)
			{
				new (static_cast<std::string*>(temp) + i)  std::string(_data._string[i]);
			}

			clear();

			free(_data._string);

			_data._string = static_cast<std::string*>(temp);
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:
			for (size_t i = 0; i < newSize; ++i)
			{
				new (static_cast<glm::vec4*>(temp) + i)  glm::vec4(_data._vec4[i]);
			}

			delete(_data._anonymous);

			_data._vec4 = static_cast<glm::vec4*>(temp);
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			for (size_t i = 0; i < newSize; ++i)
			{
				new (static_cast<int*>(temp) + i)  int(_data._int[i]);
			}

			delete(_data._anonymous);

			_data._mat4x4 = static_cast<glm::mat4x4*>(temp);
			break;

		}
	}

	void Datum::shrink_to_fit()
	{


		//Allocate memory using malloc
		void* temp = malloc(getSizeOfType() * _size);
		assert(temp != nullptr);

		//Assign the values to the new data
		switch (_type)
		{
		case Fiea::GameEngine::Datum::DatumType::Integer:
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<int*>(temp) + i)  int(_data._int[i]);
			}

			delete(_data._anonymous);

			_data._int = static_cast<int*>(temp);

			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<float*>(temp) + i)  float(_data._float[i]);
			}

			delete(_data._anonymous);

			_data._float = static_cast<float*>(temp);
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<std::string*>(temp) + i)  std::string(_data._string[i]);
			}

			clear();

			free(_data._string);

			_data._string = static_cast<std::string*>(temp);
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<glm::vec4*>(temp) + i)  glm::vec4(_data._vec4[i]);
			}

			delete(_data._anonymous);

			_data._vec4 = static_cast<glm::vec4*>(temp);
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			for (size_t i = 0; i < _size; ++i)
			{
				new (static_cast<int*>(temp) + i)  int(_data._int[i]);
			}

			delete(_data._anonymous);

			_data._mat4x4 = static_cast<glm::mat4x4*>(temp);
			break;
		}

		_capacity = _size;

	}


	bool Datum::empty() const
	{
		 return _size <= 0; 
	}

	 Datum::DatumType Datum::getDecidedType()
	{
		return _type;
	}

	const Datum::DatumType Datum::getDecidedType() const
	{
		return _type;
	}


	void Datum::SetAsString(const std::string& s, size_t index)
	{
		int intValue;
		float floatValue;
		std::vector<float> values;
		std::istringstream iss(s);

		size_t successes;
		float a, b, c, d;
		float e, f, g, h;
		float i, j, k, l;
		float m, n, o, p;

		switch (_type)
		{
		case Fiea::GameEngine::Datum::DatumType::Unknown:
			break;
		case Fiea::GameEngine::Datum::DatumType::Integer:
			intValue = std::stoi(s); //Throws invalid argument if cannot be converted
			setItemAt(intValue, index);
			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			floatValue = std::stof(s); //Throws invalid argument if cannot be converted
			setItemAt(floatValue, index);
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			setItemAt(s, index);
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:

			successes = sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)", &a, &b, &c, &d);

			if (successes != 4)
			{
				throw std::invalid_argument("Invalid number of elements in the string");
			}

			setItemAt(glm::vec4(a, b, c, d), index);
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			
			successes = sscanf_s(s.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p);

			if (successes != 16)
			{
				throw std::invalid_argument("Invalid number of elements in the string");
			}

			glm::mat4x4 matrix(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);

			setItemAt(matrix, index);

			break;
		default:
			break;
		}

	}

	void Datum::addToBackFromString(const std::string& s, DatumType type)
	{
		int intValue;
		float floatValue;
		std::vector<float> values;
		std::istringstream iss(s);

		size_t successes;
		float a, b, c, d;
		float e, f, g, h;
		float i, j, k, l;
		float m, n, o, p;

		switch (type)
		{
		case Fiea::GameEngine::Datum::DatumType::Unknown:
			break;
		case Fiea::GameEngine::Datum::DatumType::Integer:
			intValue = std::stoi(s); //Throws invalid argument if cannot be converted
			addToBack(intValue);
			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			floatValue = std::stof(s); //Throws invalid argument if cannot be converted
			addToBack(floatValue);
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			addToBack(s);
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:

			successes = sscanf_s(s.c_str(), "vec4(%f, %f, %f, %f)", &a, &b, &c, &d);

			if (successes != 4)
			{
				throw std::invalid_argument("Invalid number of elements in the string");
			}
			addToBack(glm::vec4(a, b, c, d));
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:

			successes = sscanf_s(s.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p);

			if (successes != 16)
			{
				throw std::invalid_argument("Invalid number of elements in the string");
			}

			glm::mat4x4 matrix(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);
			addToBack(matrix);
			break;
		default:
			break;
		}
	}

	Scope** Datum::getScopeUnionData()
	{
		return _data._scope;
	}

	RTTI** Datum::getRttiUnionData()
	{
		return _data._rtti;
	}

	size_t Datum::getSizeOfType() const
	{
		switch (_type)
		{
		case DatumType::Integer:
			return sizeof(int);
		case DatumType::Float:
			return sizeof(float);
		case DatumType::String:
			return sizeof(std::string);
		case DatumType::Vector4:
			return sizeof(std::string);
		case DatumType::Matrix4x4:
			return sizeof(std::string);
		default:
			// what? what does this even mean?
			return 8;
		}
	}

	void Datum::removeAt(size_t index)
	{
		size_t begin;
		size_t end;

		switch (_type)
		{
		case Fiea::GameEngine::Datum::DatumType::Unknown:
			break;
		case Fiea::GameEngine::Datum::DatumType::Integer:
			begin = (size_t)_data._int;
			end = (size_t)(_data._int + _size - 1);

			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			break;
		case Fiea::GameEngine::Datum::DatumType::Table:
			begin = (size_t)_data._scope;
			end = (size_t)(_data._scope + _size - 1);
			if (index >= begin && index < end)
			{
				for (size_t it = index + 1; it != end; ++it)
				{
					*(_data._scope + it - 1) = std::move(*(_data._scope + it));
				}
			}
			--_size;
			break;
		case Fiea::GameEngine::Datum::DatumType::Pointer:
			break;
		default:
			break;
		}


	}

	void Datum::allocateMemory(size_t capacity, DatumType type)
	{
		size_t elementSize = 0;

		switch (type)
		{
		case Fiea::GameEngine::Datum::DatumType::Integer:
			elementSize = sizeof(int);
			break;
		case Fiea::GameEngine::Datum::DatumType::Float:
			elementSize = sizeof(float);
			break;
		case Fiea::GameEngine::Datum::DatumType::String:
			elementSize = sizeof(std::string);
			break;
		case Fiea::GameEngine::Datum::DatumType::Vector4:
			elementSize = sizeof(glm::vec4);
			break;
		case Fiea::GameEngine::Datum::DatumType::Matrix4x4:
			elementSize = sizeof(glm::mat4x4);
			break;
		default:
			//UNSUPPORTED TYPE 
			//TODO - handle case where the allocateMemory has a type of Unknown
			return;
			
		}

		//Allocate memory using malloc
		_data._anonymous = malloc(elementSize * capacity);
		assert(_data._anonymous != nullptr);

	}


}
