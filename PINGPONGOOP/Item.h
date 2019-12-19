#ifndef ITEM
#define ITEM
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <time.h>
using namespace sf;

enum ItemType
{
	ORANGE = 0,
	RED,
	VIOLET
};

class Item
{
protected:
	CircleShape ItemShape;
	bool rise = false;
	int bonus = 1;
public:
	static bool isPrime(int x)
	{
		if (x < 2)
			return false;
		int i = 2;
		int tmp = sqrt(x);
		while (i <= tmp && x % i != 0)
			i++;
		return (i > tmp);
	}
	Item();
	Item(VideoMode&, float);

	//Hàm lấy vị trí của item
	CircleShape& getShape();
	void disappear();
	void appear();
	bool appeared();
	void roll(VideoMode&);
	int getBonus();
};

#endif // !ITEM
