#include "Serialisation.h"
#include <string>

//SERIALISER

bool SWIFT::IO::SERIALISER::open(const std::string& path)
{
	close();
	stream.open(path);
	return stream.good();
}

void SWIFT::IO::SERIALISER::close()
{
	stream.close();
}

template<SWIFT::IO::TYPE type>
void SWIFT::IO::SERIALISER::register_type()
{
	stream << std::int8_t(type);
}

void SWIFT::IO::SERIALISER::serialize(const SERIALISABLE& object)
{
	register_type<TYPE::SERIALISABLE>();
	object.serialise(*this);
}

void SWIFT::IO::SERIALISER::serialize(float val)
{
	register_type<TYPE::FLOAT>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(double val)
{
	register_type<TYPE::DOUBLE>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(bool val)
{
	register_type<TYPE::BOOL>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(std::int8_t val)
{
	register_type<TYPE::INT8>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(std::int16_t val)
{
	register_type<TYPE::INT16>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(std::int32_t val)
{
	register_type<TYPE::INT32>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(std::int64_t val)
{
	register_type<TYPE::INT64>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialize(const std::string& str)
{
	register_type<TYPE::STRING>();
	stream << str;
}

//DESERIALISER

bool SWIFT::IO::DESERIALISER::open(const std::string& path)
{
	close();
	stream.open(path);
	return stream.good();
}

void SWIFT::IO::DESERIALISER::close()
{
	stream.close();
}

template<SWIFT::IO::TYPE type_required>
void SWIFT::IO::DESERIALISER::verify_type()
{
	std::int8_t type;
	stream >> type;
	
	if (type_required != TYPE(type))
		ASSERT(true);
}

void SWIFT::IO::DESERIALISER::deserialize(SERIALISABLE& object)
{
	verify_type<TYPE::SERIALISABLE>();
	object.deserialise(*this);
}

void SWIFT::IO::DESERIALISER::deserialize(float& val)
{
	verify_type<TYPE::FLOAT>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(double& val)
{
	verify_type<TYPE::DOUBLE>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(bool& val)
{
	verify_type<TYPE::BOOL>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int8_t& val)
{
	verify_type<TYPE::INT8>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int16_t& val)
{
	verify_type<TYPE::INT16>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int32_t& val)
{
	verify_type<TYPE::INT32>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int64_t& val)
{
	verify_type<TYPE::INT64>();
	stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::string& str)
{
	verify_type<TYPE::STRING>();
	stream >> str;
}