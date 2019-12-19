#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics.hpp>
class Ball;
#include "Ball.h"
using namespace sf;
using namespace std;
enum class PlayerMode {
	player1, player2, Computer, Computer2, Default
};	// loại người chơi: 
	//1.người chơi người 1	(bên trái)
	//2.người chơi người 2	(bên phải)
	//3.người chơi máy 1	(bên phải)
	//4.người chơi máy 2	(bên trái)
	//5.mặc định
	//Lưu ý: được tái sử dụng trong class Game để hiển thị loại màn chơi
class Player
{
protected:
	static const double Multiplier;			// hệ sô nhân tăng tốc
	double Velocity;						// vận tốc người chơi theo chiều y
	RectangleShape PlayerShape;				// hình dạng cái vợt của người chơi
	PlayerMode PlayerType;					// chế độ chơi
	int Score;
	// Điểm số;
public:

	//Constructor cho người chơi 
	Player();
	// Constructor cho người chơi dựa trên khung hình, loại người chơi và căn chỉnh độ phân giải 
	// Input:  1. khung hình chơi, 2.Loại người chơi, 3.Tỉ lệ căn chỉnh độ phân gỉải
	Player(VideoMode& mode, PlayerMode TypePlayer, float Scale);
	Player& operator=(const Player&);
	~Player();

	//Hàm lấy vận tốc
	double getVelocity();

	//hàm thay đổi điểm số
	void setScore(int);

	//Hàm thay đổi vận tốc
	void setVelocity(double);

	//Hàm lấy giới hạn 4 mặt của người chơi trong tọa độ
	// Output: giới hạn 4 góc của người chơi ở tọa độ trong game
	FloatRect getPosition();

	//hàm lấy "cái vợt" của người chơi trả về địa chỉ để chỉnh sửa trực tiếp 
	RectangleShape* getShape();

	//hàm trả về thông số người chơi
	//output: thông số người chơi
	RectangleShape ReturnShape();

	//hàm điều khiển chuyển động của người chơi
	// Input: giới hạn di chuyển trên, giới hạn di chuyển dưới,tốc độ giữa 2 khung hình, vật thể banh, độ khó, tỉ lệ căn chỉnh độ phân giải)
	void MovementControl(RectangleShape& upper, RectangleShape& bottom, double DeltaTime, Ball ball, float difficult, float Scale);

	//hàm trả về điểm chơi đã đạt được
	//Output: điểm chơi đã đạt
	int GetScore();

	//hàm tăng điểm khi người chơi thắng 1 banh
	void UpdateScore(int bonus = 1);

	// reset điểm thắng về 0
	void ResetScore();

	//hàm thay đổi chế độ chơi của người chơi
	//Input: chế độ chơi
	void ChangeMode(PlayerMode playmode);
};



#endif // !PLAYER_H_

