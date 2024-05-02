#pragma once
#include "IDataType.h"

class IntegerType : public IDataType<int>
{
	// Inherited via IDataType
	int getDataAtIndex(int index) const override;
};