#include <iostream>
#include "Types/include/vector.h"
#include "Types/include/event.h"

using BIF = st::EVENT<bool, int, float>;

class TEST : public BIF::LISTENER {
public:
	std::string name;
	void notify(bool b, int i, float f)
	{
		std::cout << name << "\t" << b << "\t" << i << "\t" << f << std::endl;
	}
};

int main()
{
	BIF evt;

	TEST t;
	t.name = "TEST1";

	std::cout << "Test1:\n";
	evt.invoke(true, 2, 1.5f);

	t.listen(&evt);

	std::cout << "Test2:\n";
	evt.invoke(true, 2, 1.5f);

	TEST t2(t);
	t2.name = "TEST2";

	std::cout << "Test3:\n";
	evt.invoke(true, 2, 1.5f);

	TEST t3(std::move(t));
	t3.name = "TEST3";

	std::cout << "Test4:\n";
	evt.invoke(true, 2, 1.5f);

	BIF evt2(std::move(evt));

	std::cout << "Test5:\n";
	evt.invoke(true, 2, 1.5f);

	std::cout << "Test6:\n";
	evt2.invoke(true, 2, 1.5f);
}