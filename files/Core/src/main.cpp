#include <iostream>
#include "Types/include/vector.h"
#include "Types/include/event.h"
#include "Types/include/static_map.h"

enum class FRUIT{
	BANNANA,
	ORANGE,
	APPLE,
	PEAR,

	ENUM_SIZE
};

int main()
{

	st::STATIC_MAP<FRUIT, const char*> map("Placeholder");
	std::cout << map[FRUIT::APPLE] << "\t" << map[FRUIT::BANNANA] << "\t" << map[FRUIT::ORANGE] << "\t" << map[FRUIT::PEAR] << std::endl;

	map[FRUIT::APPLE] = "apple";
	map[FRUIT::BANNANA] = "bannana";
	map[FRUIT::ORANGE] = "orange";
	map[FRUIT::PEAR] = "pear";


	std::cout << map[FRUIT::APPLE] << "\t" << map[FRUIT::BANNANA] << "\t" << map[FRUIT::ORANGE] << "\t" << map[FRUIT::PEAR] << std::endl;


}