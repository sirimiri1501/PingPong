#ifndef PLAYGROUND_H_
#define PLAYGROUND_H_

#include <SFML/Graphics.hpp>
#include <iterator>
#include <vector>
using namespace std;
using namespace sf;

// số thứ tự trong vector sân chơi
/*0 upperwall
1 rightwall
2 bottomwall
3 left wall
4 midfield
*/


class PlayGround
{
private:
	vector<RectangleShape> PlayWall; // mảng vector chứa các thành phần sân chơi 
	 // Score[0] điểm người chơi 1, Score[1] điểm của người chơi còn lại
public:

	// hiển thị sân chơi
	void DisplayPlayGround(RenderWindow& window);

	// mảng 2 phần tử chứa điểm 2 người chơi
	vector<Text> Score;


	// hàm lấy mảng chứa thành phần sân chơi
	vector<RectangleShape> GetList();

	// Input: chiều dài, chiều rộng, màu, tọa độ x,y,tỉ lệ căn chỉnh
	// OutPut: thành phần sân chơi đã được tạo
	RectangleShape setWall(float length, float width, float rotation, Color color, float positionx, float positiony, Vector2f scale);// cài đặt sân chơi

	//hàm tạo toàn bộ sân chơi
	//Input: thông số khung hình, tỉ lệ căn chỉnh
	void CreateWall(VideoMode& mode, float Scale);//hàm tạo toàn bộ sân chơi

	// constructor khởi tạo sân chơi 
	//Input: nhận vào thông số khung hình, font hiển thị, tỉ lệ căn chỉnh
	PlayGround(VideoMode& mode, Font& font, float Scale);


	PlayGround();



	~PlayGround();

};

#endif // !PLAYGROUND_H_



