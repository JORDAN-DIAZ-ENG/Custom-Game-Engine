#include "pch.h"
#include "IntegerHelper.h"
#include "IntWrapper.h"


namespace Fiea::GameEngine
{
    void IntegerHelper::Initialize()
    {
    }
    bool IntegerHelper::Start(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, const std::string& jsonKey, Json::Value object, bool isArrayElement)
    {
        IntWrapper* temp = incomingWrapper->As<IntWrapper>();

        if (!temp) return false;

        if (object.isArray())
        {
            for (auto it = object.begin(); it != object.end(); ++it)
            {
                temp->setIntAtIndex((*it).asInt(), std::distance(object.begin(), it));
            }
        }
        else
        {
            temp->setInt(object.asInt());
        }

        return true;
    }
    bool IntegerHelper::End(const std::shared_ptr<ParseCoordinator::Wrapper>& incomingWrapper, Json::Value object, const std::string& jsonKey)
    {
        IntWrapper* temp = incomingWrapper->As<IntWrapper>();

        if (!temp) return false;

        return true;
    }

    void IntegerHelper::Cleanup()
    {

    }
}
