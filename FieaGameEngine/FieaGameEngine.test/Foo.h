#pragma once
#include "RTTI.h"

namespace Fiea::GameEngine::Test
{
    class Foo : public RTTI
    {
        RTTI_DECLARATIONS(Foo, RTTI);

    public:
        Foo(){};
        Foo(RTTI* value);
    };
}