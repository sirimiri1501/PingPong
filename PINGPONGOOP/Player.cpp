#include "Player.h"
#include "Ball.h"

Player::Player()
{
	//đặt các tính chất mặc định của player
	PlayerShape.setSize(Vector2f(100, 50));
	PlayerShape.setFillColor(Color::Blue);
	PlayerShape.setScale(Vector2f(1, 0.2f));
	PlayerShape.setOrigin(Vector2f(PlayerShape.getSize().x / 2, PlayerShape.getSize().y / 2));
	PlayerShape.setRotation(90);
	PlayerShape.setPosition(Vector2f(150, 450));
	Velocity = 0;//vận tốc ban đầu bằng 0
	PlayerType = PlayerMode::Default;//loại player mặc định
	Score = 0;//điểm người chơi
}
Player::Player(VideoMode& mode, PlayerMode playerType, float Scale)
{
	PlayerType = playerType;//đặt loại người chơi
	PlayerShape.setSize(Vector2f(mode.height * 0.02f, mode.width * 0.09f));
	PlayerShape.setOrigin(Vector2f(PlayerShape.getSize().x / 2, PlayerShape.getSize().y / 2));

	//cài đặt tính chất player người 1 (bên trái)
	if (PlayerType == PlayerMode::player1)
	{
		//đặt vị trí player người 1 (bên trái)
		PlayerShape.setPosition(Vector2f(70.0f * Scale, mode.height / 2.0f));
		PlayerShape.setFillColor(Color::Black);
	}
	else
	{
		//đặt vị trí player người 2 (bên phải)
		PlayerShape.setPosition(Vector2f(mode.width - 70.0f * Scale, mode.height / 2.0f));
		PlayerShape.setFillColor(Color::Black);
	}
	Velocity = 0;//vận tốc ban đầu bằng 0
	Score = 0;//điểm người chơi bằng 0
}
const double Player::Multiplier = 3.0f;
Player& Player::operator=(const Player& player)
{
	this->Velocity = player.Velocity;
	this->PlayerShape = player.PlayerShape;
	this->PlayerType = player.PlayerType;
	this->Score = player.Score;
	return *this;
}
Player::~Player()
{
}

double Player::getVelocity()
{
	return this->Velocity;
}

void Player::setScore(int score)
{
	this->Score = score;
}

void Player::setVelocity(double velocity)
{
	this->Velocity = velocity;
}

FloatRect Player::getPosition()
{
	//tra về hình dạng player
	return PlayerShape.getGlobalBounds();
}

RectangleShape* Player::getShape()
{
	//trả về con trỏ trỏ đến hình dạng player
	return &PlayerShape;
}

RectangleShape Player::ReturnShape()
{
	//trả về hình dạng player
	return PlayerShape;
}



void Player::MovementControl(RectangleShape& upper, RectangleShape& bottom, double DeltaTime, Ball ball, float difficult, float Scale)
{
	Velocity = 0.0f;
	//nếu loại player là người chơi 1
	//điều kiển thông qua các phím W,S
	if (PlayerType == PlayerMode::player1)
	{
		//khi nhấn phím W (đi lên)
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			//vận tốc bằng -150 (đi lên)
			Velocity = -150.0f;
			//chạm phải biên trên thì dừng lại
			if (PlayerShape.getGlobalBounds().intersects(upper.getGlobalBounds()))
			{
				Velocity = 0.0f;//vận tốc bằng 0
			}
		}
		//khi nhấn phím S (đi xuống)
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			//Vận tốc bằng 150 (đi xuống)
			Velocity = 150.0f;
			///chạm phải biên dưới thì dừng lại
			if (PlayerShape.getGlobalBounds().intersects(bottom.getGlobalBounds()))
			{
				Velocity = 0.0f;
			}
		}
		//di chuyển player theo trục y dựa trên vận tốc và các thông số:
		//<DeltaTime>  : thời gian giữa 2 khung hình (rất nhỏ)
		//<Multiplier> : hệ số nhân cho <DeltaTime>
		PlayerShape.move(0, static_cast<float>(Velocity * DeltaTime * Multiplier * Scale));
	}
	//nếu loại player là người chơi 2
	//điều kiển thông qua các phím Up, Down
	else if (PlayerType == PlayerMode::player2)
	{
		//khi nhấn phím Up (đi lên)
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			//vận tốc bằng -150 (đi lên)
			Velocity = -150.0f;
			//chạm phải biên trên thì dừng lại
			if (PlayerShape.getGlobalBounds().intersects(upper.getGlobalBounds()))
			{
				Velocity = 0.0f;
			}
		}
		//khi nhấn phím Down (đi xuống)
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			//Vận tốc bằng 150 (đi xuống)
			Velocity = 150.0f;
			///chạm phải biên dưới thì dừng lại
			if (PlayerShape.getGlobalBounds().intersects(bottom.getGlobalBounds()))
			{
				Velocity = 0.0f;
			}
		}
		//di chuyển player theo trục y dựa trên vận tốc và các thông số:
		//<DeltaTime>  : thời gian giữa 2 khung hình (rất nhỏ)
		//<Multiplier> : hệ số nhân cho <DeltaTime>
		PlayerShape.move(0, static_cast<float>(Velocity * DeltaTime * Multiplier * Scale));
	}
	//nếu loại player là máy (bên phải)
	else if (PlayerType == PlayerMode::Computer)
	{
		//Khi bóng di chuyển về phía player (từ trái sang phải)
		//vận tốc bóng dương
		if (ball.GetxVelocity() > 0)
		{
			//Nếu vị trí player cao hơn bóng
			//di chuyển player đi lên
			//chạm biên trên thì dừng lại
			if (PlayerShape.getPosition().y > ball.ReturnThisBall()->getPosition().y)
			{
				Velocity = -150.0f;
				if (PlayerShape.getGlobalBounds().intersects(upper.getGlobalBounds()))
				{
					Velocity = 0.0f;
				}
			}
			//Nếu vị trí player thấp hơn bóng
			//di chuyển player đi xuống
			//chạm biên dưới thì dừng lại
			else if (PlayerShape.getPosition().y < ball.ReturnThisBall()->getPosition().y)
			{
				Velocity = 150.0f;
				if (PlayerShape.getGlobalBounds().intersects(bottom.getGlobalBounds()))
				{
					Velocity = 0.0f;
				}
			}
		}
		//di chuyển player theo trục y dựa trên vận tốc và các thông số:
		//<DeltaTime>  : thời gian giữa 2 khung hình (rất nhỏ)
		//<Multiplier> : hệ số nhân cho <DeltaTime>
		//<Difficult>  : hệ số độ khó (nhỏ hơn 1), giảm tốc độ player
		PlayerShape.move(0, static_cast<float>(Velocity * DeltaTime * Multiplier * difficult * Scale));
	}
	//nếu loại player là máy 2 (bên trái)
	else if (PlayerType == PlayerMode::Computer2)
	{
		//Khi bóng di chuyển về phía player (từ phải sang trái)
		//vận tốc bóng âm
		if (ball.GetxVelocity() < 0)
		{
			//Nếu vị trí player cao hơn bóng
			//di chuyển player đi lên
			//chạm biên trên thì dừng lại
			if (PlayerShape.getPosition().y > ball.getPosition().top)
			{
				Velocity = -150.0f;
				if (PlayerShape.getGlobalBounds().intersects(upper.getGlobalBounds()))
				{
					Velocity = 0.0f;
				}
			}
			//Nếu vị trí player thấp hơn bóng
			//di chuyển player đi xuống
			//chạm biên dưới thì dừng lại
			else if (PlayerShape.getPosition().y < ball.ReturnThisBall()->getPosition().y)
			{
				Velocity = 150.0f;
				if (PlayerShape.getGlobalBounds().intersects(bottom.getGlobalBounds()))
				{
					Velocity = 0.0f;
				}
			}
		}
		//di chuyển player theo trục y dựa trên vận tốc và các thông số:
		//<DeltaTime>  : thời gian giữa 2 khung hình (rất nhỏ)
		//<Multiplier> : hệ số nhân cho <DeltaTime>
		//<Difficult>  : hệ số độ khó (nhỏ hơn 1), giảm tốc độ player
		PlayerShape.move(0, static_cast<float>(Velocity * DeltaTime * Multiplier * difficult * Scale));
	}
}

int Player::GetScore()
{
	//trả về điểm hiện tại của player
	return this->Score;
}

void Player::UpdateScore(int bonus)
{
	//tăng điểm player 1 đơn vị
	Score+=bonus;
	if (Score < 0)
		Score = 0;
}

void Player::ResetScore()
{
	//reset điểm về 0
	Score = 0;
}

void Player::ChangeMode(PlayerMode playmode)
{
	PlayerType = playmode;
}




