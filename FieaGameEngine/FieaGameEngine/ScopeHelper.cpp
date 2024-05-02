#include "pch.h"
#include "ScopeHelper.h"
#include "ScopeWrapper.h"

namespace Fiea::GameEngine
{
	void ScopeHelper::Initialize()
	{

	}
	void ScopeHelper::Cleanup()
	{
	}
	bool ScopeHelper::Start(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, const std::string& jsonKey, Json::Value object, bool isArrayElement)
	{
		ScopeWrapper* temp = incomingWrapper->As<ScopeWrapper>();

		if (!temp) return false;

		if (object.isObject())
		{
			temp->addScope(jsonKey, false);
		}
		else
		{
			if (object.isArray())
			{
				if (object[0].isInt())
				{
					size_t i = 0;
					for (auto& it : object)
					{
						temp->addIntDatum(jsonKey, it.asInt(), i);
						++i;
					}
				}
				else if (object[0].isDouble())
				{
					size_t i = 0;
					for (auto& it : object)
					{
						temp->addFloatDatum(jsonKey, it.asDouble(), i);
						++i;

					}
				}
				else if (object[0].isObject())
				{
					//temp->move;
				}
				else if (object[0].isString())
				{
					if (object[0].asString().compare(0, 4, "vec4") == 0)
					{
						size_t i = 0;
						for (auto& it : object)
						{
							temp->addVec4Datum(jsonKey, it.asString(), i);
							++i;
						}
					}
					else if (object[0].asString().compare(0, 6, "mat4x4") == 0)
					{
						size_t i = 0;
						for (auto& it : object)
						{
							temp->addMat4x4Datum(jsonKey, it.asString(), i);
							++i;
						}
					}
					else
					{
						size_t i = 0;
						for (auto& it : object)
						{
							temp->addStringDatum(jsonKey, it.asString(), i);
							++i;
						}
					}
				}
			}
			else
			{
				if (object.isInt())
				{
					temp->addIntDatum(jsonKey, object.asInt(), 0);
				}
				else if (object.isDouble())
				{
					temp->addFloatDatum(jsonKey, object.asDouble(), 0);
				}
				else if (object.isString())
				{
					if (object.asString().compare(0, 4, "vec4") == 0)
					{
						temp->addVec4Datum(jsonKey, object.asString(), 0);
					}
					else if (object.asString().compare(0, 6, "mat4x4") == 0)
					{
						temp->addMat4x4Datum(jsonKey, object.asString(), 0);

					}
					else
					{
						temp->addStringDatum(jsonKey, object.asString(), 0);
					}
				}
			}
		}





		return true;
	}
	bool ScopeHelper::End(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, Json::Value object, const std::string& jsonKey)
	{
		if (!object.isObject()) return true;

		ScopeWrapper* temp = incomingWrapper->As<ScopeWrapper>();

		temp->moveUpScopeTree();

		return true;
	}
}

