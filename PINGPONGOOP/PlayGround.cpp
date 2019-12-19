#include "PlayGround.h"

// hàm hiển thị Sân chơi trên màn hình
void PlayGround::DisplayPlayGround(RenderWindow &window)// hàm hiển thị sân chơi 
{
	vector<RectangleShape>::iterator i;
	for (i = PlayWall.begin(); i != PlayWall.end(); i++)
	{
		window.draw(*i); // vẽ từng biên chơi trong vector sân chơi
	}
}

//Hàm trả về các biên sân chơi
vector<RectangleShape> PlayGround::GetList()
{
	return PlayWall;
}

// hàm khởi tạo 1 biên trong sân chơi
//từ trái sáng phải : chiều dài ,rộng, độ xoay, màu, vị trị x,y,tỉ lệ căn chỉnh)
RectangleShape PlayGround::setWall(float length, float width, float rotation,Color color,float positionx,float positiony,Vector2f scale)
{
	RectangleShape rec;
	rec.setOrigin(Vector2f(length / 2, width / 2)); // cài đặt tọa độ tâm
	rec.setSize(Vector2f(length, width));// cài đặt kích thước
	rec.setRotation(rotation);// độ xoay vật thể
	rec.setPosition(positionx, positiony);//vị trị đặt vật thể 
	rec.setFillColor(color);//cài đặt màu sắc
	rec.setScale(scale);// cài đặt căn chỉnh
	return rec;
}


// hàm tạo các biên sân chơi
void PlayGround::CreateWall(VideoMode &mode,float Scale)
{

	// kiểm tra mảng nếu mảng có phần tử xóa sạch mảng
	if (PlayWall.empty() == false)
	{
		PlayWall.clear(); 
	}

	// cài đặt các thành phần biên của sân chơi bao gồm:biên giữa, biên trên biên dưới biên trái và phải
	RectangleShape midfield = setWall(static_cast<float>(mode.height*2), 180, 90, Color::White, static_cast<float>(mode.width)/2, 0, Vector2f(1, 0.01f));
	RectangleShape upperwall = setWall(static_cast<float>(mode.height*2), static_cast<float>(mode.width),0, Color::White, static_cast<float>(mode.height), 0,Vector2f(1,0.01f));
	RectangleShape bottomwall = setWall(static_cast<float>(mode.height * 2), static_cast<float>(mode.width), 0, Color::White, static_cast<float>(mode.height), static_cast<float>(mode.height), Vector2f(1, 0.01f));
	RectangleShape rightwall = setWall(static_cast<float>(mode.height*2), static_cast<float>(mode.width*0.1f), 90, Color::White, 50*Scale, 0,Vector2f(1,0.05f));
	RectangleShape leftwall = setWall(static_cast<float>(mode.height * 2), static_cast<float>(mode.width * 0.1f), 90, Color::White, static_cast<float>((mode.width-50*Scale)) , 0, Vector2f(1, 0.05f));
	
	
	//bỏ biên vào vector sân chơi
	PlayWall.push_back(upperwall);  
	PlayWall.push_back(leftwall); 
	PlayWall.push_back(bottomwall);
	PlayWall.push_back(rightwall); 
	PlayWall.push_back(midfield); 

}


// Constructor cho sân chơi nhận biến là thông số khung hình, font chữ, tỉ lệ căn chỉnh
PlayGround::PlayGround(VideoMode &mode,Font &font,float Scale)
{
	// tạo sân chơi nhận vào thông số khung hình và tỉ lệ căn chỉnh
	CreateWall(mode,Scale);


	// mảng tạm chứa các thuộc tính Text về điểm số

	Text arrScore[2]; 	
	

	// cài đặt font cho chữ cho mảng text
	arrScore[0].setFont(font); 
	arrScore[1].setFont(font);

	// cài đặt trọng tâm của từng text 
	arrScore[0].setOrigin(arrScore[0].getGlobalBounds().width / 2.0f, arrScore[0].getGlobalBounds().height / 2.0f);
	arrScore[1].setOrigin(arrScore[1].getGlobalBounds().width / 2.0f, arrScore[1].getGlobalBounds().height / 2.0f);
	
	//cài đặt nội dung Text;
	arrScore[0].setString("0"); 
	arrScore[1].setString("0");
	
	
	// cài đặt vị trí chữ trên màn hiển thị
	arrScore[0].setPosition(10*Scale, mode.height / 2.0f);
	arrScore[1].setPosition(mode.width-40*Scale, mode.height / 2.0f);
	
	// cài đặt kích thước cỡ chữ
	arrScore[0].setCharacterSize((int)round(50*Scale));
	arrScore[1].setCharacterSize((int)round(50*Scale));
	
	
	// cài đặt màu
	arrScore[0].setFillColor(Color(60, 66, 69,255)); 
	arrScore[1].setFillColor(Color(60, 66, 69, 255));
	
	
	// Add mảng Score đã cài vào vector để sử dụng
	Score.push_back(arrScore[0]);
	Score.push_back(arrScore[1]);

}

//Constructor
PlayGround::PlayGround()
{
}

//Destructor
PlayGround::~PlayGround()
{
}

