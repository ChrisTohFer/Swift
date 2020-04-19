#pragma once

#include <vector>
#include <fstream>
#include <memory>
#include <tuple>

namespace SWIFT::IO
{
	enum class TYPE : std::int32_t
	{
		SEPARATOR,
		SERIALISABLE,
		FLOAT,
		DOUBLE,
		BOOL,
		INT8,
		INT16,
		INT32,
		INT64,
		UINT8,
		UINT16,
		UINT32,
		UINT64,
		STRING
	};

	class SERIALISABLE;
	class SERIALISER;
	class DESERIALISER;

	using UNIQUE_SERIALISABLE = std::unique_ptr<SERIALISABLE>;

	//SERIALISER class for sending information to a stream

	class SERIALISER
	{
		std::ofstream stream;

	public:
		SERIALISER() = default;
		SERIALISER(const std::string& path);

		bool open(const std::string& path);
		void close();

	private:
		void pad();
		template<TYPE type>
		void register_type();

	public:
		void add_separator();
		void serialise(SERIALISABLE&);
		void serialise(float);
		void serialise(double);
		void serialise(bool);
		void serialise(std::int8_t);
		void serialise(std::int16_t);
		void serialise(std::int32_t);
		void serialise(std::int64_t);
		void serialise(std::uint8_t);
		void serialise(std::uint16_t);
		void serialise(std::uint32_t);
		void serialise(std::uint64_t);
		void serialise(const std::string&);
		void serialise(const char*);		//undefined as SWIFT does not support c_string serialisation, convert to std::string instead
	};

	//DESERIALISER class for collecting information from a stream

	class DESERIALISER
	{
		std::ifstream stream;

	public:
		DESERIALISER() = default;
		DESERIALISER(const std::string& path);

		bool open(const std::string& path);
		void close();

	private:
		template<TYPE required_type>
		bool verify_type();

	public:
		void next_separator();
		void deserialise(SERIALISABLE&);
		void deserialise(float&);
		void deserialise(double&);
		void deserialise(bool&);
		void deserialise(std::int8_t&);
		void deserialise(std::int16_t&);
		void deserialise(std::int32_t&);
		void deserialise(std::int64_t&);
		void deserialise(std::uint8_t&);
		void deserialise(std::uint16_t&);
		void deserialise(std::uint32_t&);
		void deserialise(std::uint64_t&);
		void deserialise(std::string&);
	};

	//SERIALISABLE base class for encapsulating more complex types we want to be able to serialise
	
	class SERIALISABLE
	{
	public:
		virtual void serialise(SERIALISER&) = 0;
		virtual void deserialise(DESERIALISER&) = 0;

		virtual ~SERIALISABLE() = default;
	};

	//SIMPLIFIED serialisable class to unify definitions of serialise and deserialise for trivial objects via a variadic constructor

	template<typename ... MEMBERS>
	class SIMPLIFIED : public SERIALISABLE
	{
		std::tuple<MEMBERS&...> m_members;	//Store reference to each member

	public:
		SIMPLIFIED(MEMBERS&...);
		virtual void serialise(SERIALISER&);
		virtual void deserialise(DESERIALISER&);
	};

	template<typename ... ARGS>
	UNIQUE_SERIALISABLE simplify(ARGS&...);
}

//SWIFT::IO::SIMPLIFIED

template<typename ... MEMBERS>
SWIFT::IO::SIMPLIFIED<MEMBERS...>::SIMPLIFIED(MEMBERS&... members)
	: m_members(members...)
{
}

template<typename ... MEMBERS>
void SWIFT::IO::SIMPLIFIED<MEMBERS...>::serialise(SERIALISER& serialiser)
{
	std::apply(
		[&](MEMBERS& ... args) {
			(serialiser.serialise(args), ...);	//calls serialiser.serialise() for all args
		},
		m_members
	);
}

template<typename ... MEMBERS>
void SWIFT::IO::SIMPLIFIED<MEMBERS...>::deserialise(DESERIALISER& deserialiser)
{
	std::apply(
		[&](MEMBERS& ... args) {
			(deserialiser.deserialise(args), ...);	//calls deserialiser.deserialise() for all args
		},
		m_members
	);
}

//Non-member functions

template<typename ... ARGS>
SWIFT::IO::UNIQUE_SERIALISABLE SWIFT::IO::simplify<ARGS...>(ARGS&... args)
{
	return UNIQUE_SERIALISABLE(new SIMPLIFIED(args...));
}