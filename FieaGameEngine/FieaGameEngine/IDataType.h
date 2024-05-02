#pragma once

template<class T>
class IDataType 
{
public:
	virtual T getDataAtIndex(int index) const = 0;

};