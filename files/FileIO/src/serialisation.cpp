#include "Serialisation.h"
#include <string>

//SERIALISER

SWIFT::IO::SERIALISER::SERIALISER(const std::string& path)
{
	open(path);
}

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

void SWIFT::IO::SERIALISER::pad()
{
	stream << " ";
}

void SWIFT::IO::SERIALISER::add_separator()
{
	register_type<TYPE::SEPARATOR>();
}

void SWIFT::IO::SERIALISER::serialise(SERIALISABLE& object)
{
	register_type<TYPE::SERIALISABLE>();
	object.serialise(*this);
}

void SWIFT::IO::SERIALISER::serialise(float val)
{
	register_type<TYPE::FLOAT>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(double val)
{
	register_type<TYPE::DOUBLE>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(bool val)
{
	register_type<TYPE::BOOL>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::int8_t val)
{
	register_type<TYPE::INT8>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::int16_t val)
{
	register_type<TYPE::INT16>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::int32_t val)
{
	register_type<TYPE::INT32>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::int64_t val)
{
	register_type<TYPE::INT64>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::uint8_t val)
{
	register_type<TYPE::UINT8>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::uint16_t val)
{
	register_type<TYPE::UINT16>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::uint32_t val)
{
	register_type<TYPE::UINT32>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(std::uint64_t val)
{
	register_type<TYPE::UINT64>();
	stream << val;
}

void SWIFT::IO::SERIALISER::serialise(const std::string& str)
{
	register_type<TYPE::STRING>();
	stream << str.length();
	pad();
	stream << str;
}

//DESERIALISER

SWIFT::IO::DESERIALISER::DESERIALISER(const std::string& path)
{
	open(path);
}

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

void SWIFT::IO::DESERIALISER::next_separator()
{
	std::underlying_type<TYPE>::type type;
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
		case TYPE::UINT8:
			std::uint8_t ui8;
			stream >> ui8;
			break;
		case TYPE::UINT16:
			std::uint16_t ui16;
			stream >> ui16;
			break;
		case TYPE::UINT32:
			std::uint32_t ui32;
			stream >> ui32;
			break;
		case TYPE::UINT64:
			std::uint64_t ui64;
			stream >> ui64;
			break;
		case TYPE::STRING:
			size_t size;
			stream >> size;
			stream.ignore(size);
		}

		stream >> type;
	}
}

void SWIFT::IO::DESERIALISER::deserialise(SERIALISABLE& object)
{
	if (verify_type<TYPE::SERIALISABLE>())
		object.deserialise(*this);
}

void SWIFT::IO::DESERIALISER::deserialise(float& ref)
{
	if (verify_type<TYPE::FLOAT>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(double& ref)
{
	if (verify_type<TYPE::DOUBLE>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(bool& ref)
{
	if (verify_type<TYPE::BOOL>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::int8_t& ref)
{
	if (verify_type<TYPE::INT8>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::int16_t& ref)
{
	if (verify_type<TYPE::INT16>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::int32_t& ref)
{
	if (verify_type<TYPE::INT32>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::int64_t& ref)
{
	if (verify_type<TYPE::INT64>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::uint8_t& ref)
{
	if (verify_type<TYPE::UINT8>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::uint16_t& ref)
{
	if (verify_type<TYPE::UINT16>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::uint32_t& ref)
{
	if (verify_type<TYPE::UINT32>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::uint64_t& ref)
{
	if (verify_type<TYPE::UINT64>())
		stream >> ref;
}

void SWIFT::IO::DESERIALISER::deserialise(std::string& str)
{
	if (verify_type<TYPE::STRING>())
	{
		size_t length;
		stream >> length;
		
		stream.ignore(1); //ignore padded space

		str.resize(length);
		stream.read(&str[0], length);
	}
}
