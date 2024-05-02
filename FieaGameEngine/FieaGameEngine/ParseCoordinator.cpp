#include "pch.h"
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include <fstream>

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ParseCoordinator::Wrapper);

	void ParseCoordinator::ParseMembers(Json::Value object)
	{

		_wrapper.get()->IncrementDepth();

		
		for (auto it = object.begin(); it != object.end(); ++it)
		{

			Parse(it.key().asString(), *it, (*it).isArray());


		}

		_wrapper.get()->DecrementDepth();


	}

	void ParseCoordinator::Parse(std::string key, Json::Value object, bool isElementOfArray)
	{
		//Walk through the list of helpers until one responds that it has handled the pair

		//The handler that responds true to StartHandler should be the same handler that accepts the EndHandler call

		//If you have arrays or objects you'll need to recursively parse


	
		for (auto handler : _handlers)
		{
			handler->Initialize();
			if (handler->Start(_wrapper, key, object, isElementOfArray))
			{
				//if json has children
				if (object.isObject())
				{
					ParseMembers(object); //process child object
				}
				else if (object.isArray())
				{

					for (auto& value : object)
					{
						if (value.isObject())
						{
							handler->Start(_wrapper, key, value, true);
							ParseMembers(value);
							handler->End(_wrapper, value, key);
						}

					}
				}

				handler->End(_wrapper, object, key);
				handler->Cleanup();
				break; //once done stop
			}
		}


	}
	ParseCoordinator::ParseCoordinator(std::shared_ptr<Wrapper> incomingWrapper) : _wrapper(incomingWrapper)
	{

	}

	ParseCoordinator::~ParseCoordinator()
	{
	}

	void ParseCoordinator::AddHelper(std::shared_ptr<IParseHandler> handler)
	{
		_handlers.push_back(handler);
	}

	void ParseCoordinator::RemoveHelper(std::shared_ptr<IParseHandler> handler)
	{
		auto it = std::find(_handlers.begin(), _handlers.end(), handler);
		if (it != _handlers.end())
		{
			_handlers.erase(it);
		}
	}

	void ParseCoordinator::DeserializeObject(const std::string& jsonString)
	{
		Json::Value root;
		Json::CharReaderBuilder builder;
		std::string errors;

		std::unique_ptr<Json::CharReader> reader(builder.newCharReader()); //ptr to the reader

		if (!reader)
		{
			//Handle the case where reader creation failed (allocation failure)
			throw std::runtime_error("Failed to create JSON reader");
		}

		bool parsingSuccessful = reader->parse(jsonString.c_str(), jsonString.c_str() + jsonString.size(), &root, &errors);

		if (parsingSuccessful)
		{
			ParseMembers(root);
		}
		else
		{
			//Handle parsing errors
			throw std::runtime_error("Failed to parse JSON: " + errors);
		}

	}

	void ParseCoordinator::DeserializeObject(std::istream& jsonInputStreamData)
	{
		std::ostringstream oss;
		oss << jsonInputStreamData.rdbuf(); //Read stream into string stream
		if (jsonInputStreamData.bad())
		{
			throw std::runtime_error("Error reading from input stream");
		}

		std::string jsonString = oss.str();
		DeserializeObject(jsonString);
	}

	void ParseCoordinator::DeserializeObjectFromFile(const std::string& filename)
	{
		//Open the file
		std::ifstream file(filename);
		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file: " + filename);
		}

		//Read the contents of the file into a string
		std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		//Deserialize the json string
		DeserializeObject(jsonString);
	}

	std::shared_ptr<ParseCoordinator::Wrapper>& ParseCoordinator::GetWrapper()
	{
		return _wrapper;
	}

	const std::shared_ptr<ParseCoordinator::Wrapper>& ParseCoordinator::GetWrapper() const
	{
		return _wrapper;
	}

	void ParseCoordinator::Wrapper::IncrementDepth()
	{
		++_depth;
	}

	void ParseCoordinator::Wrapper::DecrementDepth()
	{
		--_depth;
	}

	size_t ParseCoordinator::Wrapper::Depth() const
	{
		return _depth;
	}

	bool ParseCoordinator::Wrapper::Equals(const RTTI* rhs) const
	{
		return this == rhs;
	}

	
}