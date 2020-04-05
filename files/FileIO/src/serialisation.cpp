#include "Serialisation.h"
#include "GlobalHeaders/macros.h"
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
void SWIFT::IO::SERIALISER::register_type()	//Provide a bit more context so we can detect errors when deserialising
{
	stream << std::int8_t(type);
}

void SWIFT::IO::SERIALISER::add_separator()
{
	register_type<TYPE::SEPARATOR>();
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
	stream << str.length();
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
bool SWIFT::IO::DESERIALISER::verify_type()	//Don't read data that is unexpected; we can always skip it later
{
	std::int8_t type(-1);
	auto pos = stream.tellg();

	stream >> type;
	
	if (type_required != TYPE(type))
	{
		CONSOLE_WARNING("encountered an incorrect type during deserialisation.")
		stream.seekg(pos);
		return false;
	}
	
	return true;
}

void SWIFT::IO::DESERIALISER::next_separator()
{
	std::int8_t type;
	stream >> type;
	while (TYPE(type) != TYPE::SEPARATOR)	//If type is separator, we can exit
	{
		switch (TYPE(type))
		{
		case TYPE::SERIALISABLE:
			break;
		case TYPE::FLOAT:
			float f;
			stream >> f;
			break;
		case TYPE::DOUBLE:
			double d;
			stream >> d;
			break;
		case TYPE::BOOL:
			bool b;
			stream >> b;
			break;
		case TYPE::INT8:
			std::int8_t i8;
			stream >> i8;
			break;
		case TYPE::INT16:
			std::int16_t i16;
			stream >> i16;
			break;
		case TYPE::INT32:
			std::int32_t i32;
			stream >> i32;
			break;
		case TYPE::INT64:
			std::int64_t i64;
			stream >> i64;
			break;
		case TYPE::STRING:
			size_t size;
			stream >> size;
			stream.ignore(size);
		}

		stream >> type;
	}
}

void SWIFT::IO::DESERIALISER::deserialize(SERIALISABLE& object)
{
	if (verify_type<TYPE::SERIALISABLE>())
		object.deserialise(*this);
}

void SWIFT::IO::DESERIALISER::deserialize(float& val)
{
	if (verify_type<TYPE::FLOAT>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(double& val)
{
	if (verify_type<TYPE::DOUBLE>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(bool& val)
{
	if (verify_type<TYPE::BOOL>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int8_t& val)
{
	if (verify_type<TYPE::INT8>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int16_t& val)
{
	if (verify_type<TYPE::INT16>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int32_t& val)
{
	if (verify_type<TYPE::INT32>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::int64_t& val)
{
	if (verify_type<TYPE::INT64>())
		stream >> val;
}

void SWIFT::IO::DESERIALISER::deserialize(std::string& str)
{
	if (verify_type<TYPE::STRING>())
	{
		size_t length;
		stream >> length;
		
		str.resize(length);
		stream.read(&str[0], length);
	}
}