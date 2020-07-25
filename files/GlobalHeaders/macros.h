
#if CONFIGURATION == DEBUG

#include <iostream>

#define DEBUG_ONLY(...)			__VA_ARGS__
#define CONSOLE_WARNING(msg)	std::cout << "WARNING:\t" << msg << std::endl;
#define ASSERT(...)             _ASSERT(__VA_ARGS__)

#else

#define DEBUG_ONLY(...)			{}
#define CONSOLE_WARNING(msg)	{}
#define ASSERT(...)             {}

#endif

#if CONFIGURATION == RELEASE

#define RELEASE_ONLY(...)		__VA_ARGS__

#else

#define RELEASE_ONLY(...)		{}

#endif