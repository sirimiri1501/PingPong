#include "Ball.h"

//constructor
Ball::Ball()
{
	//Cài đặt theo các thông số mặc định
	BallShape.setRadius(9.f);//cài đặt bán kính
	BallShape.setFillColor(Color::Green);//cài đặt màu bóng (đen)
	xVelocity = 2.0f;//vận tốc theo chiều x
	yVelocity = 2.0f;//vận tốc theo chiều y
}

//constructor theo các đặc điểm khung hình
Ball::Ball(VideoMode& mode, float Scale) {
	BallShape.setRadius(mode.width * 0.01f);//Bán kính theo tỉ lệ màn hình
	BallShape.setPointCount(100);//số lượng điểm biểu diễn
	BallShape.setFillColor(Color::White);//màu bóng
	BallShape.setOutlineColor(Color::Black);//màu viền bóng (đen)
	BallShape.setOutlineThickness(2.0f);//độ dày viền bóng
	BallShape.setOrigin(BallShape.getRadius(), BallShape.getRadius());//xác định toạ độ tâm
	BallShape.setPosition(Vector2f(mode.width / 2.0f, mode.height / 2.0f));//xác định vị trí (giữa khung hình)
	//lấy ngẫu nhiên vận tốc và chiều chuyển động theo trục x (từ 3-5)
	do {
		xVelocity = pow(-1, rand() % 2) * float(rand() % 60) / 10;
	} while (abs(xVelocity) <= 4);
	//tính vận tốc y theo x và lấy ngẫu nhiên chiều chuyển động
	yVelocity = pow(-1, rand() % 2) * sqrt(pow(6, 2) - pow(xVelocity, 2));

	//thay đổi các thông số vận tốc theo tỉ lệ khung hình
	SetScale(Scale);
}

double Ball::distance(Vector2f a, Vector2f b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

//Di chuyển bóng về vị trí giữa sân sau khi 1 người chơi không hứng được bóng
void Ball::SetOriginBall(VideoMode& mode, int direction, float Scale)
{
	//xác định toạ độ tâm
	BallShape.setOrigin(BallShape.getRadius(), BallShape.getRadius());
	//đặt bóng về vị trí giữa sân
	BallShape.setPosition(Vector2f(mode.width / 2.0f, mode.height / 2.0f));
	//vận tốc ban đầu là 6.0
	//lấy ngẫu nhiên vận tốc và chiều chuyển động theo trục x (từ 3-5)
	do {
		xVelocity = pow(-1, direction) * float(rand() % 60) / 10;
	} while (abs(xVelocity) <= 4);
	//tính vận tốc y theo x và vận tốc ban đầu, lấy ngẫu nhiên chiều chuyển động
	yVelocity = pow(-1, rand() % 2) * sqrt(pow(6, 2) - pow(xVelocity, 2));
	//thay đổi các thông số vận tốc theo tỉ lệ khung hình
	SetScale(Scale);
}

//Trả về hình dạng của bóng
CircleShape& Ball::getShape()
{
	return BallShape;
}

//Trả về vị trí của bóng
FloatRect Ball::getPosition()
{
	//trả về vị trí bóng, có thể truy xuất đến biên trái / trên / dưới / phải
	return BallShape.getGlobalBounds();
}

//Đặt vận tốc bóng
void Ball::SetVelocity(float xVelocity, float yVelocity)
{
	//đặt lại vận tốc theo trục x, y
	this->xVelocity = xVelocity;
	this->yVelocity = yVelocity;
}

//Trả về con trỏ trỏ đến hình dạng bóng nhằm điều chỉnh trực tiếp các tính chất của bóng
CircleShape* Ball::ReturnThisBall()
{
	//trả về con trỏ trỏ đến vị trí lưu hình dạng bóng
	return &BallShape;
}

//Xác định tính chất di chuyển khi bóng va chạm biên trên hay biên dưới sân chơi
void Ball::BounceTopOrBottom()
{
	//đổi chiều chuyển động theo trục y khi va biên trên/dưới
	yVelocity = -yVelocity;
}

//Cài đặt các tính chất bóng theo tỉ lệ khung hình (vận tốc tối đa, vận tốc theo x,y)
void Ball::SetScale(float Scale)
{
	//đặt vận tốc tối đa theo tỉ lệ khung hình
	MaxVelocity = MaxVelocity * Scale;
	//đặt vận tốc trục x theo tỉ lệ khung hình
	xVelocity = xVelocity * Scale;
	//đặt vận tốc trục y theo tỉ lệ khung hình
	yVelocity = yVelocity * Scale;
}

//Xác định tính chất vận tốc bóng khi va chạm player
void Ball::BouncePlayer()
{
	//đổi chiều chuyển động theo trục x và tăng 10% vận tốc
	xVelocity = xVelocity * -1.1;
	yVelocity = yVelocity * 1.1;

	//nếu vận tốc bóng vượt quá vận tốc tối đa
	//đặt vận tốc về bằng vận tốc tối đa
	double Velocity = sqrt(xVelocity * xVelocity + yVelocity * yVelocity);
	if (Velocity > MaxVelocity)
	{
		xVelocity = xVelocity * MaxVelocity / Velocity;
		yVelocity = yVelocity * MaxVelocity / Velocity;
	}

}

//Di chuyển bóng
void Ball::Update(double deltaTime, VideoMode mode, AudioManager& audio, Player& player1, Player& player2, PlayGround& playground, Item & item, float Scale)
{
	//Điều kiện khi bóng đụng biên trên
	if (BallShape.getPosition().y < BallShape.getRadius())
	{
		//Thay đổi tính chất bóng khi chạm biên trên/dưới
		BounceTopOrBottom();
		//Đặt lại bóng về vị trí ngay lúc chạm
		BallShape.setPosition(BallShape.getPosition().x, BallShape.getRadius());
		//phát âm thanh khi bóng chạm biên trên
		audio.PlaySound("ballhit.wav", false);
	}
	//Điều kiện để bóng chạm biên dưới
	else if (BallShape.getPosition().y + BallShape.getRadius() > mode.height)
	{
		//Thay đổi tính chất bóng khi chạm biên trên/dưới
		BounceTopOrBottom();
		//Đặt bóng về vị trí ngay lúc chạm
		BallShape.setPosition(BallShape.getPosition().x, mode.height - BallShape.getRadius() + 0.1f);
		//Phát âm thanh khi bóng chạm biên dưới
		audio.PlaySound("ballhit.wav", false);
	}
	//Điều kiện để bóng chạm thanh trượt bên trái (người chơi/player 1)
	else if (BallShape.getPosition().x - BallShape.getRadius() < player1.getShape()->getPosition().x + player1.getPosition().width / 2.0f
		&& BallShape.getPosition().y > player1.getShape()->getPosition().y - player1.getPosition().height / 2.0f
		&& BallShape.getPosition().y < player1.getShape()->getPosition().y + player1.getPosition().height / 2.0f
		&& BallShape.getPosition().x - BallShape.getRadius() > player1.getShape()->getPosition().x - player1.getPosition().width / 2.0f)
	{
		//Thay đổi tính chất bóng khi chạm thanh trượt
		BouncePlayer();
		//Đặt lại bóng về vị trí ngay lúc chạm thanh trượt
		BallShape.setPosition(player1.getShape()->getPosition().x + player1.getPosition().width / 2.0f + BallShape.getRadius(), BallShape.getPosition().y);
		//Phát âm thanh khi  bóng chạm thanh trượt
		audio.PlaySound("ballhit.wav", false);
	}
	//Điều kiện để bóng chạm thanh trượt bên phải (người chơi/player 2)
	else if (BallShape.getPosition().x + BallShape.getRadius() > player2.getShape()->getPosition().x - player2.getPosition().width / 2.0f
		&& BallShape.getPosition().y > player2.getShape()->getPosition().y - player2.getPosition().height / 2.0f
		&& BallShape.getPosition().y < player2.getShape()->getPosition().y + player2.getPosition().height / 2.0f
		&& BallShape.getPosition().x + BallShape.getRadius() < player2.getShape()->getPosition().x + player2.getPosition().width / 2.0f)
	{
		//Thay đổi tính chất bóng khi chạm thanh trượt
		BouncePlayer();
		//Đặt lại bóng về vị trí ngay lúc chạm thanh trượt
		BallShape.setPosition(player2.getShape()->getPosition().x - player2.getPosition().width / 2.0f - BallShape.getRadius(), BallShape.getPosition().y);
		//Phát âm thanh khi thanh trượt chạm bóng
		audio.PlaySound("ballhit.wav", false);
	}
	//xử lý khi banh tương tác biên phải ( xử lý thua cho người chơi 2)
	else if (BallShape.getPosition().x + BallShape.getRadius()> mode.width)
	{
		player1.UpdateScore(); // nâng điểm người chơi 1 lên 1 điểm
		playground.Score[0].setString(to_string(player1.GetScore()));// cập nhật điểm người chơi 1 lên sân chơi
		audio.PlaySound("losingsound.wav", false);// âm thanh phát ra báo hiệu thua
		SetOriginBall(mode, 1, Scale);//set vị trí banh về mặc định giữa sân chuẩn bị cho lần chơi tiếp theo


	}

	// xử lý khi banh tương tác với biên trái ( xử lý thua cho người chơi 1 )
	else if (BallShape.getPosition().x < BallShape.getRadius())
	{
		player2.UpdateScore();// nâng điểm người chơi 2 lên 1 điểm 
		playground.Score[1].setString(to_string(player2.GetScore())); // cập nhật điểm người chơi 2 lên sân chơi
		audio.PlaySound("losingsound.wav", false);// âm thanh phát ra báo hiệu thua
		BallShape.setPosition(mode.width / 2.0f, mode.height / 2.0f);// set vị trí banh về mặc định giữa sân chuẩn bị cho lần chơi tiếp theo
		SetOriginBall(mode, 0, Scale);
	}
	// xử lý khi banh chạm vào vật thưởng 
	else if (item.appeared() && distance(BallShape.getPosition(), item.getShape().getPosition()) <= BallShape.getRadius() + item.getShape().getRadius())
	{
		//Người chơi 1 được điểm thường
		if (xVelocity > 0)
			player1.UpdateScore(item.getBonus());
		//Người chơi 2 được điểm thưởng
		else
			player2.UpdateScore(item.getBonus());
		playground.Score[0].setString(to_string(player1.GetScore())); // cập nhật điểm người chơi 1 lên sân chơi
		playground.Score[1].setString(to_string(player2.GetScore())); // cập nhật điểm người chơi 2 lên sân chơi
		audio.PlaySound("losingsound.wav", false);// âm thanh phát ra báo hiệu thua
		item.disappear();
		item.roll(mode);
	}
	//di chuyển bóng theo vận tốc, tỉ lệ khung hình
	BallShape.move(static_cast<float>(xVelocity * deltaTime * 60.0f), static_cast<float>(60.0f * deltaTime * yVelocity));
}

//Lấy vận tốc theo chiều x
double Ball::GetxVelocity()
{
	//trả về vận tốc theo trục x
	return this->xVelocity;
}

double Ball::GetyVelocity()
{
	return this->yVelocity;
}
