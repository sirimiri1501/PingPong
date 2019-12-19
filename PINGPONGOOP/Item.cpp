#include "Item.h"

Item::Item()
{
	//Cài đặt theo các thông số mặc định
	ItemShape.setRadius(9.f);//cài đặt bán kính
	ItemShape.setFillColor(Color::Green);//cài đặt màu bóng (đen)
}

Item::Item(VideoMode& mode, float scale)
{
	ItemShape.setRadius(mode.width * 0.01f);//Bán kính theo tỉ lệ màn hình
	ItemShape.setPointCount(100);//số lượng điểm biểu diễn
	ItemShape.setOutlineColor(Color::Black);//màu viền bóng (đen)
	ItemShape.setOutlineThickness(2.0f);//độ dày viền bóng
	ItemShape.setOrigin(ItemShape.getRadius(), ItemShape.getRadius());//xác định toạ độ tâm
	srand(time(NULL));
	ItemShape.setPosition(Vector2f((rand() % mode.width - 400) + 200, (rand() % mode.height - 400) + 200));//xác định vị trí (giữa khung hình)
	int tmp = rand() % 4;
	if (tmp == 0)
	{
		ItemShape.setFillColor(Color(255, 165, 0));
		bonus = 1;
	}
	else if (tmp == 1)
	{
		ItemShape.setFillColor(Color::Red);
		bonus = 2;
	}
	else if(tmp == 2)
	{
		ItemShape.setFillColor(Color(153, 50, 204));
		bonus = 3;
	}
	else if (tmp == 3)
	{
		ItemShape.setFillColor(Color::Black);
		bonus = -3;
	}
}

CircleShape& Item::getShape()
{
	return this->ItemShape;
}

void Item::disappear()
{
	rise = false;
}

void Item::appear()
{
	rise = true;
}

bool Item::appeared()
{
	return rise;
}

void Item::roll(VideoMode& mode)
{
	srand(time(NULL));
	ItemShape.setPosition(Vector2f((rand() % mode.width - 400) + 200, (rand() % mode.height - 400) + 200));
	int tmp = rand() % 4;
	if (tmp == 0)
	{
		ItemShape.setFillColor(Color(255, 165, 0));
		this->bonus = 1;
	}
	else if (tmp == 1)
	{
		ItemShape.setFillColor(Color::Red);
		this->bonus = 2;
	}
	else if (tmp == 2)
	{
		ItemShape.setFillColor(Color(153, 50, 204));
		this->bonus = 3;
	}
	else
	{
		ItemShape.setFillColor(Color::Black);
		this->bonus = -3;
	}
}

int Item::getBonus()
{
	return this->bonus;
}
