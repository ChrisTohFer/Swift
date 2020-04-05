#pragma once

#include <vector>
#include <fstream>

namespace SWIFT::IO
{
	enum class TYPE : std::int8_t
	{
		SERIALISABLE,
		FLOAT,
		DOUBLE,
		BOOL,
		INT8,
		INT16,
		INT32,
		INT64,
		STRING,
		C_STRING,

		SEPARATOR = std::numeric_limits<std::int8_t>::max()
	};

	class SERIALISABLE;
	class SERIALISER;
	class DESERIALISER;

	class SERIALISER
	{
		std::ofstream stream;

	public:
		bool open(const std::string& path);
		void close();

	private:
		template<TYPE type>
		void register_type();

	public:
		void add_separator();
		void serialize(const SERIALISABLE&);
		void serialize(float);
		void serialize(double);
		void serialize(bool);
		void serialize(std::int8_t);
		void serialize(std::int16_t);
		void serialize(std::int32_t);
		void serialize(std::int64_t);
		void serialize(const std::string&);
	};

	class DESERIALISER
	{
		std::ifstream stream;

	public:
		bool open(const std::string& path);
		void close();

	private:
		template<TYPE required_type>
		bool verify_type();

	public:
		void next_separator();
		void deserialize(SERIALISABLE&);
		void deserialize(float&);
		void deserialize(double&);
		void deserialize(bool&);
		void deserialize(std::int8_t&);
		void deserialize(std::int16_t&);
		void deserialize(std::int32_t&);
		void deserialize(std::int64_t&);
		void deserialize(std::string&);
	};

	class SERIALISABLE
	{
	public:
		virtual void serialise(SERIALISER&) const = 0;
		virtual void deserialise(DESERIALISER&) = 0;
	};
}