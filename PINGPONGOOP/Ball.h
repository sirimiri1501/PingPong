
#ifndef BALL_H_
#define BALL_H_

#include <SFML/Graphics.hpp>
#include <math.h>
#include <SFML/Audio.hpp>
#include "AudioManager.h"
#include "PlayGround.h"
#include "Item.h"
#include <iostream>
using namespace sf;
class Player;
#include "Player.h"
class Ball
{
protected:
	CircleShape BallShape;//hình dạng bóng (hình tròn)
	double xVelocity = 0;//vận tốc theo trục x, giá trị dương khi bóng di chuyển từ trái sang phải
	double yVelocity = 0;//vận tốc theo trục y, giá trị dương khi bóng di chuyển từ trên xuống dưới
	double MaxVelocity = 24.0f;//vận tốc	 tối đa

public:
	//constructor mặc định
	Ball();

	//constructor theo các đặc điểm khung hình
	//input: đặc tính khung hình <mode>; tỉ lệ khung hình <scale> dựa trên mặc định 800x450
	Ball(VideoMode& mode, float Scale);

	//Hàm tính khoảng cách giữa 2 điểm
	static double distance(Vector2f a, Vector2f b);

	//Di chuyển bóng về vị trí giữa sân sau khi 1 người chơi không hứng được bóng
	//input: <mode> tính chất của khung hình, <direction> phát bóng về phía người chơi vừa ghi điểm
	//input: <scale> tỉ lệ thay đổi khung hình
	void SetOriginBall(VideoMode& mode, int direction, float Scale);

	//Trả về hình dạng của bóng
	//output:: <CircleShape> hình tròn, có thể truy xuất đến các tính chất khác của bóng
	//ví dụ: vị trí, màu sắc, bán kính
	CircleShape& getShape();

	//Trả về vị trí của bóng
	//output: trả về vị trí bóng, có thể truy xuất đến biên trái/trên/dưới/phải
	FloatRect getPosition();

	//Đặt vận tốc bóng
	//input: vận tốc theo trục x, y
	void SetVelocity(float xVelocity, float yVelocity);

	//Trả về con trỏ trỏ đến hình dạng bóng nhằm điều chỉnh trực tiếp các tính chất của bóng
	//output: con trỏ trỏ tới hình dạng của quả bóng
	CircleShape* ReturnThisBall();

	//Xác định tính chất di chuyển khi bóng va chạm biên trên hay biên dưới sân chơi
	void BounceTopOrBottom();

	//Cài đặt các tính chất bóng theo tỉ lệ khung hình (vận tốc tối đa, vận tốc theo x,y)
	//input: tỉ lệ thay đổi khung hình
	void SetScale(float Scale);

	//Xác định tính chất vận tốc bóng khi va chạm player
	//input: <count> số lần chạm (hạn chế bóng va chạm với 1 player nhiều lần liên tục)
	//input: <check> nhận true nếu bóng sau khi chạm player đã vượt qua biên giữa (mục đích tương tự <count>)
	void BouncePlayer();

	//Di chuyển bóng
	//Thay đổi tính chất bóng khi va chạm vật thể
	//input: 
	//<deltaTime> thời gian giữa 2 khung hình
	//<mode> các tính chất khung hình (dùng để xác định vị trí tương đối của bóng với khung hình
	//<audio> phát âm thanh khi bóng va chạm vật thể
	//<player1>,<player2> các tính chất của người chơi (xác định vị trí của bóng và thanh trượt, thay đổi điểm của người chơi
	//<playground> các tính chất của sân chơi (xác định vị trí của bóng và sân, thay đổi điểm hiển thị)
	void Update(double deltaTime, VideoMode mode, AudioManager& audio, Player& player1, Player& player2, PlayGround& playground, Item & item, float Scale);

	//Lấy vận tốc theo chiều x
	//output: vận tốc bóng theo chiều x (xác định hướng bóng phục vụ computer player)
	double GetxVelocity();
	double GetyVelocity();

};


#endif // !BALL_H_


