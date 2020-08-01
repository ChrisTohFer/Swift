
#if CONFIGURATION == DEBUG

#include "Console/console.h"

#define DEBUG_ONLY(...)			__VA_ARGS__
#define CONSOLE_WARNING(msg)	SWIFT::console().output(std::wstring(L"WARNING:\t") + std::wstring(msg) + L"\n");
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