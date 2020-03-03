#include <iostream>
#include "Types/include/vector.h"
#include "Types/include/event.h"

using BIF = st::EVENT<bool, int, float>;

class TEST : public BIF::LISTENER {
	void notify(bool b, int i, float f)
	{
		std::cout << b << "\t" << i << "\t" << f << std::endl;
	}
};

int main()
{
	BIF evt;

	TEST t;

	std::cout << "Test1:\n";
	evt.invoke(true, 2, 1.5f);

	t.listen(&evt);

	std::cout << "Test2:\n";
	evt.invoke(true, 2, 1.5f);

	TEST t2(t);

	std::cout << "Test3:\n";
	evt.invoke(true, 2, 1.5f);

	TEST t3(std::move(t));

	std::cout << "Test4:\n";
	evt.invoke(true, 2, 1.5f);

	BIF evt2(std::move(evt));

	std::cout << "Test5:\n";
	evt.invoke(true, 2, 1.5f);

	std::cout << "Test6:\n";
	evt2.invoke(true, 2, 1.5f);
}