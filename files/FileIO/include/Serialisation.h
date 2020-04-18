#pragma once

#include <vector>
#include <fstream>
#include <memory>
#include <tuple>

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

	//SERIALISABLE base class for encapsulating more complex types we want to be able to serialise
	
	class SERIALISABLE
	{
	public:
		virtual void serialise(SERIALISER&) const = 0;
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
		virtual void serialise(SERIALISER&) const;
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
void SWIFT::IO::SIMPLIFIED<MEMBERS...>::serialise(SERIALISER& serialiser) const
{
	std::apply(
		[&](MEMBERS& ... args) {
			(serialiser.serialize(args), ...);	//calls serialiser.serialise() for all args
		},
		m_members
	);
}

template<typename ... MEMBERS>
void SWIFT::IO::SIMPLIFIED<MEMBERS...>::deserialise(DESERIALISER& deserialiser)
{
	std::apply(
		[&](MEMBERS& ... args) {
			(deserialiser.deserialize(args), ...);	//calls deserialiser.deserialise() for all args
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