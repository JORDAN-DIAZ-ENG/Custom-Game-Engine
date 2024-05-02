#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"



namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Attributed);


	Attributed::Attributed(RTTI::IdType typeID) : incomingTypeID(typeID)
	{
		populateWithPrescribedMembers(typeID);
	}

	Attributed::Attributed(const Attributed& rhs) : Scope(rhs), incomingTypeID(rhs.incomingTypeID)
	{
		populateWithPrescribedMembers(incomingTypeID);
	}
	Attributed::Attributed(Attributed&& rhs) noexcept : Scope(std::move(rhs)), incomingTypeID(rhs.incomingTypeID)
	{
		populateWithPrescribedMembers(incomingTypeID);
	}
	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		//Self Assignment
		if (this == &rhs)
		{
			return *this;
		}

		Scope::operator=(rhs);
		incomingTypeID = rhs.incomingTypeID;
		populateWithPrescribedMembers(incomingTypeID);
		return *this;
	}
	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		//Self Assignment
		if (this == &rhs)
		{
			return *this;
		}

		Scope::operator=(std::move(rhs));
		incomingTypeID = rhs.incomingTypeID;
		populateWithPrescribedMembers(incomingTypeID);
		return *this;
	}

	//Attributed* Attributed::Clone() const
	//{
	//	return new Attributed(*this);
	//}
	void Attributed::populateWithPrescribedMembers(size_t typeID)
	{
		auto signatures = TypeManager::get(typeID);


		Append("this").SetStorage(static_cast<RTTI*>(this), 1, Datum::DatumType::Pointer);

		//Append("this").push_back<RTTI*>(this);

		unsigned char* ch = (unsigned char*)this;

		for (auto signature : signatures)
		{
			//int* address = reinterpret_cast<int*>(this) + signature.offset / sizeof(int);

			if (signature.offset == 0)
			{
				Append(signature.Name);
			}
			else
			{
				Append(signature.Name).SetStorage((ch + signature.offset), signature.Size, signature.Type);

			}

		}
	}
	bool Attributed::isAttribute(const std::string& s) const
	{
		const Datum* d = Find(s);

		return d != nullptr;

		//if (d == nullptr)
		//{
		//	return false;
		//}
		//else
		//{
		//	return true;
		//}

	}
	bool Attributed::isPrescribedAttribute(const std::string& s) const
	{
		const auto& signatures = TypeManager::get(incomingTypeID);
		for (const auto& sig : signatures)
		{
			if (s == sig.Name)
			{
				return true;
			}
		}
		return false;
	}
	bool Attributed::isAuxiliaryAttribute(const std::string& s) const
	{
		if (isAttribute(s) && !isPrescribedAttribute(s))
		{
			return true;
		}

		return false;
	}
	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& key)
	{
		if (isPrescribedAttribute(key)) {
			throw std::exception("This key is a prescribed attribute");
		}
		return Append(key);
	}

}