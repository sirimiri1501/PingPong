#include "Game.h"


// CÁC CÀI ĐẶT CĂN BẢN CHO TEXT
//1. SetSize (kích thước)
//2. SetFont (set font hiển thị)
//3. SetFillColor (set màu hiển thị)
//4. SetString (set nội dung cho Text)
//5. SetOrigin (set trọng tâm của toàn Text dùng cho xác định vị trí và xoay sau này)
//6. SetPosition (set vị trí hiển thị của Text trên màn hiển thị)


// Constructor
Game::Game()
{

	LoadSetting(); // đọc setting đã được lưu của người chơi
	
	//Cài đặt khung hình tối đa
	window.setFramerateLimit(60);

	//Cài đặt thời gian giữa 2 khung hình
	this->DeltaTime = 0.0f;

	//Cài đặt Font hiển thị
	if (!(this->DisplayFont.loadFromFile("Font/SFPixelate-Bold.ttf")))
	{
		cout<<"load file error"<<endl;
		system("pause");
	}

	//Cài đặt trạng thái chơi
	gamestate = GameState::Menu;

	//Cài đặt âm thanh cần thiết khi chơi
	audiomanager.AddSound("ballhit.wav");
	audiomanager.AddSound("losingsound.wav");
	audiomanager.AddSound("backgroundmusic.wav");
	audiomanager.AddSound("transitionbutton.wav");
	audiomanager.AddSound("enterbutton.wav");

	//Cài đặt các thuộc tính
	
	//1.Lựa chọn trong Menu chính và các Menu con
	CommandOrder = 1;
	
	//2.Điểm cần để thắng
	WinScore = 2;
	
	//3.Người chơi đã thắng
	WhoWin = false;
	
	//4.Chế độ chơi
	playmode = PlayerMode::Default;

	//5.Tạo các object cần cho 1 game đấu gồm: 2 người chơi, sân chơi 
	resetGame();
}

// Destructor
Game::~Game()
{
}



//Hàm chạy cả game
void Game::RunEntireGame()
{
	if (windowmode == 1)
	{
		//windowmode==1 mang ý nghĩa tạo cửa sổ chơi không có viền
		// tạo khung hình mang kiểu setting của người chơi
		window.create(mode, "PONG", Style::None); 
	}
	else
	{
		// tạo khung hình mang kiểu setting của người chơi
		window.create(mode, "PONG", Style::Default);
	}
	// bắt đầu dòng while chạy game
	while (gamestate != GameState::Exit)
	{
		Menu();// chạy menu chính
		RunGame();// chạy phần game chính
		LosingPanel();// chạy phần thông báo thắng thua
	}
}


enum MenuState
{
	PLAY = 1,
	CONTINUE,
	OPTION,
	EXIT
};

//hàm hiển thị và xử lý menu chính của game
void Game::Menu()
{


	/*PHÀN CODE DƯỚI ĐÂY CÀI ĐẶT THUỘC TÍNH PHÔNG NỀN CHO MENU CHÍNH*/
	Ball BgBall(mode, Scale);//	 banh dùng trong phông nền
	BgBall.SetVelocity(5.0f * Scale, 5.0f * Scale);// set vận tốc cho banh
	PlayGround playground(mode, DisplayFont, Scale);// tạo sân chơi cho background hiển thị ở menu chính
	Player player1(mode, PlayerMode::Computer2, Scale);// tạo 2 người chơi máy thể hiện cho phông nền
	Player player2(mode, PlayerMode::Computer, Scale);
	Item item;
	player1.getShape()->setPosition(Vector2f(70.0f * Scale, mode.height / 2.0f));



	//nhạc nền cho menu chính 
	audiomanager.PlaySound("backgroundmusic.wav", true);

	// Set lựa chọn lệnh mặc định của menu về 1 
	MenuState CommandOrder = MenuState::PLAY;


	/*PHẦN CODE CÀI ĐẶT CHO TEXT VÀ CÁC NÚT CỦA MENU CHÍNH*/
	Text GameTitle;
	Text PlayButton;
	Text ContinueButton;
	Text OptionButton;
	Text ExitButton;




	// cài đặt cho tên game hiển thị ở menu
	GameTitle.setFont(DisplayFont);
	GameTitle.setString("PONG GAME");
	GameTitle.setFont(DisplayFont);
	GameTitle.setCharacterSize(static_cast<int>(100 * Scale));
	GameTitle.setOrigin(GameTitle.getLocalBounds().left + GameTitle.getLocalBounds().width / 2.0f, GameTitle.getLocalBounds().top + GameTitle.getLocalBounds().height / 2.0f);
	GameTitle.setPosition(mode.width / 2.0f, mode.height * 0.5f / 2.0f);
	GameTitle.setLineSpacing(0);
	GameTitle.setFillColor(Color::White);


	// cài đặt cho nút Play
	PlayButton.setString("Play");
	PlayButton.setFont(DisplayFont);
	PlayButton.setCharacterSize(static_cast<int>(50 * Scale));
	PlayButton.setOrigin(PlayButton.getLocalBounds().width / 2.0f, PlayButton.getLocalBounds().height / 2.0f);
	PlayButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);
	PlayButton.setFillColor(Color::Black);

	//cài đặt cho nút Continue
	ContinueButton.setString("Continue");
	ContinueButton.setFont(DisplayFont);
	ContinueButton.setCharacterSize(static_cast<int>(50 * Scale));
	ContinueButton.setOrigin(ContinueButton.getLocalBounds().width / 2.0f, ContinueButton.getLocalBounds().height / 2.0f);
	ContinueButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);
	ContinueButton.setFillColor(Color::Black);

	//cài đặt cho nút Option
	OptionButton.setString("Option");
	OptionButton.setFont(DisplayFont);
	OptionButton.setCharacterSize(static_cast<int>(50 * Scale));
	OptionButton.setOrigin(OptionButton.getLocalBounds().width / 2.0f, OptionButton.getLocalBounds().height / 2.0f);
	OptionButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);
	OptionButton.setFillColor(Color::White);


	//cài đặt cho nút Exit 
	ExitButton.setString("Exit");
	ExitButton.setFont(DisplayFont);
	ExitButton.setCharacterSize(static_cast<int>(50 * Scale));
	ExitButton.setOrigin(ExitButton.getLocalBounds().width / 2.0f, ExitButton.getLocalBounds().height / 2.0f);
	ExitButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);
	ExitButton.setFillColor(Color::White);


	/*PHẦN CODE DƯỚI ĐÂY CÀI ĐẶT CHO 2 ICON MŨI TÊN TRÁI PHẢI */
	Texture arrow; // cài đặt texture
	//kiểm tra xem việc load texture từ file có được hay không 
	if (!arrow.loadFromFile("Texture/menuArrow.png"))
	{
		cout << "Load error" << endl;
	}

	Sprite leftarrow;
	Sprite rightarrow;
	arrow.setSmooth(true); // làm mịn texture 

	// cài đặt các thuộc tính của icon mũi tên phải 
	rightarrow.setTexture(arrow);// set texture cho sprite rightarrow
	rightarrow.setOrigin(Vector2f(rightarrow.getLocalBounds().width / 2.0f, rightarrow.getLocalBounds().height / 2.0f));  // set trọng tâm ở giữa sprite
	rightarrow.setPosition(ContinueButton.getGlobalBounds().left + ContinueButton.getGlobalBounds().width + 30.0f, mode.height / 2.0f + ContinueButton.getGlobalBounds().height / 4.0f);// set vị trí của icon canh theo vị trí nút option
	rightarrow.setScale(2 * Scale, 2 * Scale);


	// cài đặt các thuộc tính của icon mũi tên trái 
	leftarrow.setTexture(arrow);// set texture cho sprite leftarrow 
	leftarrow.rotate(180);// xoay 180 để rigghtarrow thành leftarrow 
	leftarrow.setOrigin(Vector2f(leftarrow.getLocalBounds().width / 2.0f, leftarrow.getLocalBounds().height / 2.0f)); // set trọng tâm ở giữa sprite
	leftarrow.setPosition(ContinueButton.getGlobalBounds().left - 30.0f, mode.height / 2.0f + ContinueButton.getGlobalBounds().height / 4.0f);// set vị trí của icon canh theo vị trí nút option
	leftarrow.setScale(2 * Scale, 2 * Scale);

	//set màu hiển thị của icon muixteen trái qua phải 
	Color color = leftarrow.getColor();// lấy màu hiện tại của mũi tên 
	Color holdcolor = leftarrow.getColor();// lưu màu hiện tại của mũi tên
	color.a = 75;// chỉnh lại đô trong của sprite 
	DeltaTime = 0;// set thời gian giữa các khung hình về 0
	Clock clock;// đồng hồ để đo thời gian giữa các khung hình 

	//lấy ngẫu nhiên giá trị để tạo độ ứng xử của 2 người chơi máy 
	float MIN_RAND = 0.3f, MAX_RAND = 1.0f;
	float range = MAX_RAND - MIN_RAND;
	float random = 1;


	/* PHẦN CODE DƯỚI ĐÂY XỬ LÝ LOGIC GAME VÀ HIỂN THỊ HÌNH ẢNH CỦA MENU CHÍNH*/
	//vòng lặp với trạng thái game là menu chính 
	while (gamestate == GameState::Menu)
	{

		//Kiểm tra xem các setting có bị thay đổi không 
		if (ChangeSetting == true)
		{
			//Thay đổi kích thước các vật thể hiển thị trong menu chính theo độ phân giải
			GameTitle.setCharacterSize(static_cast<int>(100 * Scale));
			GameTitle.setOrigin(GameTitle.getLocalBounds().left + GameTitle.getLocalBounds().width / 2.0f, GameTitle.getLocalBounds().top + GameTitle.getLocalBounds().height / 2.0f);
			GameTitle.setPosition(mode.width / 2.0f, mode.height * 0.5f / 2.0f);


			PlayButton.setCharacterSize(static_cast<int>(50 * Scale));
			PlayButton.setOrigin(PlayButton.getLocalBounds().width / 2.0f, PlayButton.getLocalBounds().height / 2.0f);
			PlayButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);


			ContinueButton.setCharacterSize(static_cast<int>(50 * Scale));
			ContinueButton.setOrigin(ContinueButton.getLocalBounds().width / 2.0f, ContinueButton.getLocalBounds().height / 2.0f);
			ContinueButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);


			OptionButton.setCharacterSize(static_cast<int>(50 * Scale));
			OptionButton.setOrigin(OptionButton.getLocalBounds().width / 2.0f, OptionButton.getLocalBounds().height / 2.0f);
			OptionButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);


			ExitButton.setCharacterSize(static_cast<int>(50 * Scale));
			ExitButton.setOrigin(ExitButton.getLocalBounds().width / 2.0f, ExitButton.getLocalBounds().height / 2.0f);
			ExitButton.setPosition(mode.width / 2.0f, mode.height / 2.0f);


			rightarrow.setOrigin(Vector2f(rightarrow.getLocalBounds().width / 2.0f, rightarrow.getLocalBounds().height / 2.0f));
			rightarrow.setPosition(OptionButton.getGlobalBounds().left + OptionButton.getGlobalBounds().width + 15.0f * Scale, mode.height / 2.0f + OptionButton.getGlobalBounds().height / 4.0f);
			rightarrow.setScale(1.0f * Scale, 1.0f * Scale);


			leftarrow.setOrigin(Vector2f(leftarrow.getLocalBounds().width / 2.0f, leftarrow.getLocalBounds().height / 2.0f));
			leftarrow.setPosition(OptionButton.getGlobalBounds().left - 15.0f * Scale, mode.height / 2.0f + OptionButton.getGlobalBounds().height / 4.0f);
			leftarrow.setScale(1.0f * Scale, 1.0f * Scale);


			// cài đặt lại phông nền theo tỉ lệ màn hình 
			playground.CreateWall(mode, Scale);
			player1.getShape()->setPosition(Vector2f(70.0f * Scale, mode.height / 2.0f));
			player2.getShape()->setPosition(Vector2f(mode.width - 70.0f * Scale, mode.height / 2.0f));
			player1.getShape()->setSize(Vector2f(mode.height * 0.02f, mode.width * 0.09f));
			player2.getShape()->setSize(Vector2f(mode.height * 0.02f, mode.width * 0.09f));


			//Đưa bóng về vị trí ở trung tâm
			BgBall.ReturnThisBall()->setPosition(mode.width / 2.0f, mode.height / 2.0f);
			BgBall.ReturnThisBall()->setRadius(mode.width * 0.01f);
			random = 1;
			ChangeSetting = false;
			BgBall.SetOriginBall(mode, 0, Scale);

			
		}

		//Vòng while xử lý logic game 
		Event event;
		while (window.pollEvent(event)) {


			switch (event.type)
			{
			case Event::Closed: // xử lý khi đóng cửa sổ bằng nút (X)
			{
				gamestate = GameState::Exit; // trạng thái game đặt về exit 
				window.close();// đóng cửa sổ 
				break;
			}
			case Event::KeyPressed: // xử lý khi nhấn nút trên bàn phím
			{
				//Khi nhấn nút sẽ phát ra âm thanh
				//xử lý khi nhấn nút sang phải (->) 
				if (Keyboard::isKeyPressed(Keyboard::Right) && CommandOrder < 4)
				{

					audiomanager.PlaySound("transitionbutton.wav", false);
					CommandOrder = MenuState(CommandOrder + 1);// tăng lựa chọn lệnh 1 đơn vị mỗi khi nhấn nút

				}

				//xử lý khi nhấn nút sang trái (<-) 
				else if (Keyboard::isKeyPressed(Keyboard::Left) && CommandOrder > 1)
				{

					audiomanager.PlaySound("transitionbutton.wav", false);
					CommandOrder = MenuState(CommandOrder - 1);// giảm lựa chọn lệnh 1 đơn vị mỗi khi nhấn nút

				}

				//xử lý khi phím enter được bấm 
				else if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					// lựa chọn lệnh bằng 1( nút Play)
					if (CommandOrder == MenuState::PLAY)
					{
						gamestate = GameState::PlayerMode;// trạng thái game chuyển qua Chọn Chế Độ Chơi
						resetGame();
					}

					// lựa chọn lệnh bằng 2 (nút Continue)
					else if (CommandOrder == MenuState::CONTINUE)
					{
						gamestate = GameState::Continue;// trạng thái game chuyển qua Option
					}

					// lựa chọn lệnh bằng 3 (nút Option)
					else if (CommandOrder == MenuState::OPTION)
					{
						gamestate = GameState::Option;// trạng thái game chuyển qua Option
					}

					//lựa chọn lệnh bằng 4 (nút Exit)
					else if (CommandOrder == MenuState::EXIT)
					{
						gamestate = GameState::Exit;//trạng thái game chuyển sang Exit và đóng cửa sổ 
						window.close();
					}
				}
				break;
			}
			default:
				break;
			}
		}
		/*PHẦN CODE NÀY ĐIỀU KHIỂN THANH TRƯỢT VÀ BÓNG DI CHUYỂN*/
		player1.MovementControl(playground.GetList().at(0), playground.GetList().at(2), DeltaTime, BgBall, random, Scale);
		player2.MovementControl(playground.GetList().at(0), playground.GetList().at(2), DeltaTime, BgBall, random, Scale);
		BgBall.Update(DeltaTime, mode, audiomanager, player1, player2, playground, item, Scale); // Update vị trí quả banh theo thời gian giữa 2 khung hình 



		/*PHẦN CODE DƯỚI ĐÂY DÙNG ĐỂ CÀI ĐẶT HIỂN THỊ MENU CHÍNH*/

		//lựa chọn lệnh bằng 1
		//Nút play được tô đen
		//Icon mũi tên trái được tắt (giảm độ trong)
		//Icon mũi tên phải set màu bình thường)
		if (CommandOrder == MenuState::PLAY)
		{
			PlayButton.setFillColor(Color::Black);
			leftarrow.setColor(color);
			rightarrow.setColor(holdcolor);

		}


		//lựa chọn lệnh bằng 2
		//Nút Continue được tô đen
		//Icon mũi tên trái và phải đều được set màu bình thường 
		else if (CommandOrder == MenuState::CONTINUE)
		{
			leftarrow.setColor(holdcolor);
			OptionButton.setFillColor(Color::Black);
			rightarrow.setColor(holdcolor);

		}


		//lựa chọn lệnh bằng 3
		//Nút Option được tô đen
		// Icon mũi tên trái và phải đều được set màu bình thường 
		else if (CommandOrder == MenuState::OPTION)
		{
			leftarrow.setColor(holdcolor);
			OptionButton.setFillColor(Color::Black);
			rightarrow.setColor(holdcolor);

		}


		//lựa chọn lệnh bằng 4
		//Nút Exit được tô đen
		// Icon mũi tên trái được set màu bình thường
		// Icon mũi tên phải được giảm độ trong 
		else if (CommandOrder == MenuState::EXIT)
		{
			leftarrow.setColor(holdcolor);
			ExitButton.setFillColor(Color::Black);
			rightarrow.setColor(color);

		}


		// vẽ các vật thể trong trạng thái game là Menu
		if (gamestate == GameState::Menu)
		{
			window.clear(Color(94, 183, 183, 250));
			for (int i = 0; i < 4; i++)
			{
				window.draw(playground.GetList().at(i));
			}
			window.draw(player1.ReturnShape());
			window.draw(player2.ReturnShape());
			window.draw(BgBall.getShape());
			window.draw(GameTitle);
			window.draw(rightarrow);
			window.draw(leftarrow);

			switch (CommandOrder)
			{
			case PLAY:
				window.draw(PlayButton);
				break;
			case CONTINUE:
				window.draw(ContinueButton);
				break;
			case OPTION:
				window.draw(OptionButton);
				break;
			case EXIT:
				window.draw(ExitButton);
				break;
			}
			window.display();
			DeltaTime = clock.getElapsedTime().asSeconds();

		}

		// sử dụng cho 2 menu con khác là chọn chế độ chơi
		else if (gamestate == GameState::PlayerMode)
		{
			PlayerMode();// hiển thị menu con Chọn Chế Độ Chơi
		}
		// load lại game cũ
		else if (gamestate == GameState::Continue)
		{
			loadGame();
		}

		// menu con Option
		else if (gamestate == GameState::Option)
		{
			MenuOption(); //hiển thị menu con Option (Tùy Chỉnh)

		}
		clock.restart();// restart lại thời gian giữa 2 khung hình
	}

}




// hàm hiển thị menu tùy chỉnh (option) của game
void Game::MenuOption()
{
	/*PHẦN CODE DƯỚI ĐÂY CÀI ĐẶT THUỘC TÍNH TEXT CẦN CHO MENU CON TÙY CHỈNH (OPTION)*/
	Text OptionTitle;
	Text ColorChangePlayer1;
	Text ColorChangePlayer2;
	Text OptionColorChange;
	Text Volume;
	Text MainMenuButton;
	Text Resolution;
	Text ApplyButton;
	RectangleShape Background;
	RectangleShape VolumeSlider[10];


	// cài đặt các icon mũi tên trái và phải
	Texture arrow;
	arrow.loadFromFile("Texture/menuArrow.png");
	arrow.setSmooth(true);
	Sprite rightarrow;
	Sprite leftarrow;

	rightarrow.setTexture(arrow);
	rightarrow.setOrigin(Vector2f(rightarrow.getLocalBounds().width / 2.0f, rightarrow.getLocalBounds().height / 2.0f));
	rightarrow.setScale(1 * Scale, 1 * Scale);


	leftarrow.setTexture(arrow);
	leftarrow.rotate(180);
	leftarrow.setOrigin(Vector2f(leftarrow.getLocalBounds().width / 2.0f, leftarrow.getLocalBounds().height / 2.0f));
	leftarrow.setScale(1 * Scale, 1 * Scale);


	//cài đặt các thuộc tính Text cho các độ phân giải khác nhau ( gồm 4 độ phân giải khác nhau và tỉ lệ là 16:9)
	Text res1280x720;
	Text res1600x900;
	Text res1920x1080;
	Text res800x450;
	Text WindowMode[2];

	res800x450.setFont(DisplayFont);
	res800x450.setString("800-450");
	res800x450.setFillColor(Color::Green);
	res800x450.setCharacterSize(static_cast<int>(30 * Scale));
	res800x450.setOrigin(res800x450.getLocalBounds().left, res800x450.getLocalBounds().height);
	res800x450.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);

	res1280x720.setFont(DisplayFont);
	res1280x720.setString("1280-720");
	res1280x720.setFillColor(Color::Green);
	res1280x720.setCharacterSize(static_cast<int>(30 * Scale));
	res1280x720.setOrigin(res1280x720.getLocalBounds().left, res1280x720.getLocalBounds().height);
	res1280x720.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);



	res1600x900.setFont(DisplayFont);
	res1600x900.setString("1600-900");
	res1600x900.setFillColor(Color::Green);
	res1600x900.setCharacterSize((static_cast<int>(30 * Scale)));
	res1600x900.setOrigin(res1600x900.getLocalBounds().left, res1600x900.getLocalBounds().height);
	res1600x900.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);


	res1920x1080.setFont(DisplayFont);
	res1920x1080.setString("1920-1080");
	res1920x1080.setFillColor(Color::Green);
	res1920x1080.setCharacterSize((static_cast<int>(30 * Scale)));
	res1920x1080.setOrigin(res1920x1080.getLocalBounds().left, res1920x1080.getLocalBounds().height);
	res1920x1080.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);



	// cài đặt cho tên menu Option
	OptionTitle.setFont(DisplayFont);
	OptionTitle.setFillColor(Color::Black);
	OptionTitle.setString("OPTION");
	OptionTitle.setCharacterSize((static_cast<int>(60 * Scale)));
	OptionTitle.setOrigin(OptionTitle.getLocalBounds().width / 2.0f, OptionTitle.getLocalBounds().height / 2.0f);
	OptionTitle.setPosition(mode.width / 2.0f, mode.height * 0.25f / 2.0f);


	// cài đặt nút chỉnh âm lượng
	Volume.setFont(DisplayFont);
	Volume.setFillColor(Color::Black);
	Volume.setString("Volume:");
	Volume.setCharacterSize(static_cast<int>(40 * Scale));
	Volume.setOrigin(Volume.getLocalBounds().width, Volume.getLocalBounds().height);
	Volume.setPosition(mode.width / 2.0f, mode.height * 0.7f / 2.0f);

	//Cài đặt nút chỉnh viền khung hình
	string YesNo[2]{
		" No"," Yes"
	};
	for (int i = 0; i < 2; i++)
	{
		WindowMode[i].setFont(DisplayFont);
		WindowMode[i].setFillColor(Color::White);
		WindowMode[i].setCharacterSize((static_cast<int>(40 * Scale)));
		WindowMode[i].setLetterSpacing(0.4f);
		WindowMode[i].setString("Borderless:");
		WindowMode[i].setOrigin(WindowMode[i].getLocalBounds().width, WindowMode[i].getLocalBounds().height);
		WindowMode[i].setPosition(mode.width / 2.0f, mode.height * 1.1f / 2.0f);
		
		
		WindowMode[i].setString("Borderless:" + YesNo[i]);

	}
	//cài đặt cho nút chỉnh độ phân giải
	Resolution.setFont(DisplayFont);
	Resolution.setFillColor(Color::White);
	Resolution.setString("Resolution:");
	Resolution.setCharacterSize((static_cast<int>(40 * Scale)));
	Resolution.setOrigin(Resolution.getLocalBounds().width, Resolution.getLocalBounds().height);
	Resolution.setPosition(mode.width / 2.0f, mode.height * 0.9f / 2.0f);


	// cài đặt cho nút xác nhận chuyển cài đặt
	ApplyButton.setFont(DisplayFont);
	ApplyButton.setFillColor(Color::White);
	ApplyButton.setString("Apply");
	ApplyButton.setCharacterSize(static_cast<int>(40 * Scale));
	ApplyButton.setOrigin(ApplyButton.getLocalBounds().width / 2.0f, ApplyButton.getLocalBounds().height / 2.0f);
	ApplyButton.setPosition(mode.width / 2.0f, mode.height * 1.4f / 2.0f);


	// cài đặt cho phần hình ảnh hiện thị thanh tăng giảm âm lượng

	for (int i = 0; i < 10; i++)
	{

		VolumeSlider[i].setFillColor(Color::White);
		VolumeSlider[i].setSize(Vector2f(5.0f * Scale, 20.0f * Scale));
		VolumeSlider[i].setOrigin(VolumeSlider[i].getLocalBounds().left / 2.0f, VolumeSlider[i].getLocalBounds().height / 2.0f);
		VolumeSlider[i].setPosition((mode.width / 2.0f) + i * Scale * 10.0f + 30.0f * Scale, mode.height * 0.7f / 2.0f);
		VolumeSlider[i].rotate(180.0f);
	}


	// cài đặt cho nút trở về menu chính
	MainMenuButton.setFont(DisplayFont);
	MainMenuButton.setFillColor(Color::White);
	MainMenuButton.setString("Back");
	MainMenuButton.setCharacterSize(static_cast<int>(30 * Scale));
	MainMenuButton.setOrigin(MainMenuButton.getLocalBounds().width / 2.0f, MainMenuButton.getLocalBounds().height / 2.0f);
	MainMenuButton.setPosition(mode.width / 2.0f, mode.height * 1.7f / 2.0f);



	// cài đặt cho phông nền
	Background.setSize(Vector2f(mode.width / 3.0f, mode.height * 3.0f / 4.0f));
	Background.setOrigin(Background.getLocalBounds().width / 2.0f, Background.getLocalBounds().height / 2.0f);
	Background.setFillColor(Color(255, 175, 176, 250));
	Background.setOutlineColor(Color::White);
	Background.setOutlineThickness(1.0f);
	Background.setPosition(mode.width / 2.0f, mode.height / 2.0f);


	// cài đặt thanh hiển thị âm lượng option đồng bộ với âm lượng hiện tại
	int ChangeVolume = 0;
	ChangeVolume = audiomanager.SoundVolumeValue();
	
	

	// set lựa chọn lệnh trong menu về lại 1
	CommandOrder = 1;

	// biến để quản lý lựa chọn thay đổi độ phân giải khác nhau
	int ChangeReso = 0;

	// cài đặt cho phần hiện thị hiện thị đúng độ phân giải hiện tại của game đang chạy
	if (mode.height == 450)
	{
		ChangeReso = 1;
	}
	if (mode.height == 720)
	{
		ChangeReso = 2;
	}
	if (mode.height == 900)
	{
		ChangeReso = 3;
	}
	if (mode.height == 1080)
	{
		ChangeReso = 4;
	}

	// cài đặt cho biến xác nhận đã chuyển tùy chỉnh
	bool IsApplyChange = false;


	/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ LOGIC GAME VÀ HIỂN THỊ KHUNG HÌNH CHƠI*/
	while (gamestate == GameState::Option)
	{
		// Lưu âm lượng trong trường hợp thay đổi nhưng không apply
		int TempVolume = ChangeVolume;
		//xử lý tình huống khi người chơi đã chọn thay đổi độ phân giải( căn chỉnh lại các chữ cho đúng vị trí)
		if (ChangeSetting == true)
		{
			OptionTitle.setCharacterSize((static_cast<int>(60 * Scale)));
			OptionTitle.setOrigin(OptionTitle.getLocalBounds().width / 2.0f, OptionTitle.getLocalBounds().height / 2.0f);
			OptionTitle.setPosition(mode.width / 2.0f, mode.height * 0.25f / 2.0f);

			Volume.setCharacterSize(static_cast<int>(40 * Scale));
			Volume.setOrigin(Volume.getLocalBounds().width, Volume.getLocalBounds().height);
			Volume.setPosition(mode.width / 2.0f, mode.height * 0.7f / 2.0f);

			for (int i = 0; i < 10; i++)
			{
				VolumeSlider[i].setSize(Vector2f(5.0f * Scale, 20.0f * Scale));
				VolumeSlider[i].setOrigin(VolumeSlider[i].getLocalBounds().left / 2.0f, VolumeSlider[i].getLocalBounds().height / 2.0f);
				VolumeSlider[i].setPosition((mode.width / 2.0f) + i * Scale * 10.0f + 30.0f * Scale, mode.height * 0.7f / 2.0f);
			}

			Resolution.setCharacterSize((static_cast<int>(40 * Scale)));
			Resolution.setOrigin(Resolution.getLocalBounds().width, Resolution.getLocalBounds().height);
			Resolution.setPosition(mode.width / 2.0f, mode.height * 0.9f / 2.0f);

			res800x450.setCharacterSize(static_cast<int>(30 * Scale));
			res800x450.setOrigin(res800x450.getLocalBounds().left, res800x450.getLocalBounds().height);
			res800x450.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);

			res1280x720.setCharacterSize(static_cast<int>(30 * Scale));
			res1280x720.setOrigin(res1280x720.getLocalBounds().left, res1280x720.getLocalBounds().height);
			res1280x720.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);

			res1600x900.setCharacterSize((static_cast<int>(30 * Scale)));
			res1600x900.setOrigin(res1600x900.getLocalBounds().left, res1600x900.getLocalBounds().height);
			res1600x900.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f / 2.0f);

			res1920x1080.setCharacterSize((static_cast<int>(30 * Scale)));
			res1920x1080.setOrigin(res1920x1080.getLocalBounds().left, res1920x1080.getLocalBounds().height);
			res1920x1080.setPosition(mode.width / 2.0f + 25.0f * Scale, mode.height * 0.91f/ 2.0f);


			for (int i = 0; i < 2; i++)
			{
				WindowMode[i].setCharacterSize((static_cast<int>(40 * Scale)));
				WindowMode[i].setLetterSpacing(0.4f);
				WindowMode[i].setString("Borderless:");
				WindowMode[i].setOrigin(WindowMode[i].getLocalBounds().width, WindowMode[i].getLocalBounds().height);
				WindowMode[i].setPosition(mode.width / 2.0f, mode.height * 1.1f / 2.0f);
				WindowMode[i].setString("Borderless:" + YesNo[i]);

			}

			ApplyButton.setCharacterSize(static_cast<int>(40 * Scale));
			ApplyButton.setOrigin(ApplyButton.getLocalBounds().width / 2.0f, ApplyButton.getLocalBounds().height / 2.0f);
			ApplyButton.setPosition(mode.width / 2.0f, mode.height * 1.4f / 2.0f);

			
			MainMenuButton.setCharacterSize(static_cast<int>(30 * Scale));
			MainMenuButton.setOrigin(MainMenuButton.getLocalBounds().width / 2.0f, MainMenuButton.getLocalBounds().height / 2.0f);
			MainMenuButton.setPosition(mode.width / 2.0f, mode.height * 1.7f / 2.0f);

		}

		// xử lý việc hiển thị của 2 icon mũi tên trái và phải
		rightarrow.setScale(1 * Scale, 1 * Scale);
		leftarrow.setScale(1 * Scale, 1 * Scale);
		if (CommandOrder == 1)
		{

			rightarrow.setPosition(mode.width*1.47f/2.0f, mode.height * 0.7f / 2.0f);
			leftarrow.setPosition(mode.width*1.03f/2.0f, mode.height * 0.7f / 2.0f);
		}
		else if (CommandOrder == 2)
		{

			rightarrow.setPosition(mode.width*1.47f/2.0f, mode.height * 0.91f / 2.0f);
			leftarrow.setPosition(mode.width*1.03f/2.0f, mode.height * 0.91f / 2.0f);
		}
		else if (CommandOrder == 3)
		{

			rightarrow.setPosition(mode.width*1.47f/2.0f, mode.height * 1.11f / 2.0f);
			leftarrow.setPosition(mode.width*1.03f/2.0f, mode.height * 1.11f / 2.0f);
		}


		// vòng while xử lý logic game
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed: // xử lý khi ấn đóng cửa sổ
			{
				gamestate = GameState::Exit; // set trạng thái game về lại exit
				window.close();// đóng cửa sổ 
				break;
			}
			case Event::KeyPressed: // xử lý sự kiện khi nút bàn phím được bấm
			{

				// xử lý khi nút mũi tên lên được bấm
				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					if (CommandOrder > 1)
					{
						audiomanager.PlaySound("transitionbutton.wav", false);
						CommandOrder--;// giảm lựa chọn trong menu đi 1 khi bấm 1 lần nút mũi tên lên
						
					}
				}
				// xử lý khi mũi tên xuống được bấm
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					if (IsApplyChange == false) // xử lý khi người chơi không đổi setting( dùng cho việc hiển thị)
					{
						if (CommandOrder < 4)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							CommandOrder++;// tăng lựa chọn lệnh lên 1 khi bấm 1 lần nút mũi tên xuống 
						}
					}
					else
					{
						if (CommandOrder < 5)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							CommandOrder++;
						}
					}
					
				}

				//xử lý khi phím BackSpace được nhấn 
				if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					// trạng thái game trở về menu chính ( hoạt động giống phím back)
					gamestate = GameState::Menu;
				}

				//xử lý khi lựa chọn lệnh menu là 1 và có phím nhấn 
				if (CommandOrder == 1)
				{
					// xử lý khi phím bấm mũi tên phải được nhấn 
					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						if (ChangeVolume < 10)
						{
							IsApplyChange = true;
							ChangeVolume++; // tăng giá trị lựa chọn lệnh ngang ở phần volume lên 1 khi bấm 1 lần nút mũi tên phải 
							audiomanager.SetVolume(ChangeVolume);// set giá trị âm lượng dựa trên lựa chọn lệnh ngang
						}

					}

					// xử lý khi phím bấm mũi tên trái được nhấn 
					if (Keyboard::isKeyPressed(Keyboard::Left))
					{
						if (ChangeVolume > 0)
						{
							IsApplyChange = true;
							ChangeVolume--; // giảm giá trị lựa chọn lệnh ngang ở phần volume lên 1 khi bấm 1 lần nút mũi tên trái 
							audiomanager.SetVolume(ChangeVolume);// set giá trị âm lượng dựa trên lựa chọn lệnh ngang
						}
					}

				}

				// xử lý khi lựa chọn lệnh bằng 2 và có nút được bấm 
				if (CommandOrder == 2)
				{
					//xử lý khi nút bấm mũi tên phải được nhấn 
					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						if (ChangeReso < 4)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							ChangeReso++; // tăng giá trị reso lên 1 khi nút bấm mũi tên phải được nhấn 
							IsApplyChange = true; // biến đã thay đổi độ phân giải chưa xác nhận được trả về true
						}

					}

					// xử lý khi nút bấm mũi tên trái được nhấn 
					if (Keyboard::isKeyPressed(Keyboard::Left))
					{
						if (ChangeReso > 1)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							ChangeReso--;// giảm giá trị reso đi 1 khi nút bấm mũi tên trái được nhấn 
							IsApplyChange = true;// biến đã thay đổi độ phân giải chưa xác nhận được trả về true
						}

					}
				}
				if (CommandOrder == 3)
				{
					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						if (windowmode < 1)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							windowmode++; // tăng giá trị reso lên 1 khi nút bấm mũi tên phải được nhấn 
							IsApplyChange = true; // biến đã thay đổi độ phân giải chưa xác nhận được trả về true
						}

					}

					// xử lý khi nút bấm mũi tên trái được nhấn 
					if (Keyboard::isKeyPressed(Keyboard::Left))
					{
						if (windowmode > 0)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							windowmode--;// giảm giá trị reso đi 1 khi nút bấm mũi tên trái được nhấn 
							IsApplyChange = true;// biến đã thay đổi độ phân giải chưa xác nhận được trả về true
						}

					}
				}

				// xử lý khi nút bấm enter được nhấn
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					// nếu nút apply có hiển thị do người dùng thay đổi cài đặt
					if (IsApplyChange == true && CommandOrder == 4)
					{
						ChangeSetting = true; // biến đã thay đổi độ phân giải được chuyển về true để căn chỉnh lại các Text và các nút
						IsApplyChange = false;// biến apply chưa xác nhận về lại false ( có nghĩa là đã xác nhận)
						CommandOrder = 2;// set giá trị lệnh menu về mặc định
						

						// dựa vào biến ChangeReso đã lựa chọn để căn chỉnh lại độ phân giải
						
						if (ChangeReso == 1)
						{

							mode.height = 450;
							mode.width = 800;
							Scale = 1;

						}
						else if (ChangeReso == 2)
						{
							mode.width = 1280;
							mode.height = 720;
							Scale = 1.6f;
						}
						else if (ChangeReso == 3)
						{

							mode.width = 1600;
							mode.height = 900;
							Scale = 2.0f;
							

						}
						else if (ChangeReso == 4)
						{
							mode.width = 1920;
							mode.height = 1080;
							Scale = 2.4f;
						}
						if (windowmode == 1)
						{
							window.create(mode, "PONG", Style::None);
						}
						else
						{
							window.create(mode, "PONG", Style::Close);
						}
						SaveSetting();
					}
					else if (CommandOrder == 5 || CommandOrder == 4)
					{
						audiomanager.SetVolume(TempVolume);
						gamestate = GameState::Menu;
					}
				}
				break;
			}
			default:
				break;
			}
		}


		/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ PHẦN HIỂN THỊ KHUNG HÌNH*/
		window.clear(Color(94, 183, 183, 250));
		window.draw(OptionTitle);
		window.draw(Volume);
		window.draw(MainMenuButton);
		window.draw(WindowMode[windowmode]);


		// xử lý việc vẽ các Text độ phân giải khác nhau dựa trên lựa chọn của người chơi
		if (ChangeReso == 1)
		{
			window.draw(res800x450);

		}
		else if (ChangeReso == 2)
		{

			window.draw(res1280x720);
		}
		else if (ChangeReso == 3)
		{

			window.draw(res1600x900);

		}
		else if (ChangeReso == 4)
		{

			window.draw(res1920x1080);

		}


		// xử lý việc vẽ icon mũi tên trái và phải dựa trên lựa chọn lệnh là nút Volume hay là nút Resolution
		if (CommandOrder == 1 || CommandOrder == 2 || CommandOrder == 3)
		{
			window.draw(rightarrow);
			window.draw(leftarrow);
		}
		window.draw(Resolution);

		// xử lý khi biến thay đổi chưa xác nhận là true để hiển thị nút apply, nút apply không mặc định hiển thị nếu người chơi không thay đổi độ phân giải
		if (IsApplyChange == true)
		{
			window.draw(ApplyButton);
		}

		// xử lý việc vẽ các nút khác nhau 


		// nếu lựa chọn lệnh là 1 ( nút chỉnh volume)
		// các nút reso và nút back sẽ tô màu trắng, tên menu Text Option sẽ là màu trắng
		// nút chỉnh volume sẽ được tô màu đen
		if (CommandOrder == 1)
		{
			MainMenuButton.setFillColor(Color::White);
			Volume.setFillColor(Color::Black);
			Resolution.setFillColor(Color::White);
			ApplyButton.setFillColor(Color::White);
			WindowMode[windowmode].setFillColor(Color::White);

		}


		else if (CommandOrder == 2)
		{
			Resolution.setFillColor(Color::Black);
			MainMenuButton.setFillColor(Color::White);
			Volume.setFillColor(Color::White);
			ApplyButton.setFillColor(Color::White);
			WindowMode[windowmode].setFillColor(Color::White);
		}
		else if (CommandOrder == 3)
		{
			Resolution.setFillColor(Color::White);
			MainMenuButton.setFillColor(Color::White);
			Volume.setFillColor(Color::White);
			ApplyButton.setFillColor(Color::White);
			WindowMode[windowmode].setFillColor(Color::Black);

		}

		// lựa chọn lệnh là 3 và biến thay đổi chưa xác nhận là true
		// các nút back, volume và reso sẽ được tô màu trắng
		// nút apply sẽ được tô màu đen
		else if (CommandOrder == 4  && IsApplyChange == true)
		{
			MainMenuButton.setFillColor(Color::White);
			Volume.setFillColor(Color::White);
			Resolution.setFillColor(Color::White);
			ApplyButton.setFillColor(Color::Black);
			WindowMode[windowmode].setFillColor(Color::White);
		}


		// lựa chọn lệnh là 3 và biến thay đổi chưa xác nhận là false
		// nút back sẽ được tô màu đèn
		// nút reso, volume, và tên Text option sẽ được tô màu trắng
		else if (CommandOrder == 5)
		{
			MainMenuButton.setFillColor(Color::Black);
			Volume.setFillColor(Color::White);
			Resolution.setFillColor(Color::White);
			ApplyButton.setFillColor(Color::White);
			WindowMode[windowmode].setFillColor(Color::White);
		}

		// lựa chọn lệnh là 2
		// nút reso sẽ được tô màu đen 
		// các nút còn lại được tô màu trắng 
		
		// lựa chọn lệnh là 4
		// nút back được tô màu đen
		// các nút khác được tô màu trắng 
		else if (CommandOrder == 4)
		{
			
			Resolution.setFillColor(Color::White);
			MainMenuButton.setFillColor(Color::Black);
			Volume.setFillColor(Color::White);
			ApplyButton.setFillColor(Color::White);
			WindowMode[windowmode].setFillColor(Color::White);
		}


		// xử lý việc hiển thị của thanh trượt âm lượng
		for (int i = 0; i < 10; i++)
		{
			if (i < ChangeVolume)// các thanh trượt sẽ nhận giá trị hiện thời của biến lựa chọn lệnh ngang để hiển thị đúng mức âm lượng
			{
				VolumeSlider[i].setFillColor(Color::Green);
			}
			else
			{
				VolumeSlider[i].setFillColor(Color::White);
			}

			window.draw(VolumeSlider[i]);


		}
		
		window.display();
	}

}




//hàm menu chọn chế độ chơi
void Game::PlayerMode()
{

	/*PHẦN CODE DƯỚI ĐÂY CÀI ĐẶT CÁC TEXT VÀ CÁC NÚT CẦN THIẾT CHO MENU CHỌN CHẾ ĐỘ CHƠI*/
	Text PlayerMode;
	Text OnePlayer;
	Text TwoPlayer;
	Text Back;


	// cài đặt Text tên tiêu đề menu
	PlayerMode.setFont(DisplayFont);
	PlayerMode.setFillColor(Color::Black);
	PlayerMode.setString("MODE");
	PlayerMode.setCharacterSize(static_cast<int>(60 * Scale));
	PlayerMode.setOrigin(PlayerMode.getLocalBounds().left + PlayerMode.getLocalBounds().width / 2.0f, PlayerMode.getLocalBounds().top + PlayerMode.getLocalBounds().height / 2.0f);
	PlayerMode.setPosition(mode.width / 2.0f, mode.height * 0.5f / 2.0f);


	// cài đặt nút lựa chọn 1 người chơi
	OnePlayer.setFont(DisplayFont);
	OnePlayer.setFillColor(Color::Black);
	OnePlayer.setString("1 player");
	OnePlayer.setCharacterSize(static_cast<int>(40 * Scale));
	OnePlayer.setOrigin(PlayerMode.getLocalBounds().left + PlayerMode.getLocalBounds().width / 2.0f, PlayerMode.getLocalBounds().top + PlayerMode.getLocalBounds().height / 2.0f);
	OnePlayer.setPosition(mode.width / 2.0f, mode.height * 1.0f / 2.0f);


	// cài đặt nút lựa chọn 2 người chơi
	TwoPlayer.setFont(DisplayFont);
	TwoPlayer.setFillColor(Color::White);
	TwoPlayer.setString("2 players");
	TwoPlayer.setCharacterSize(static_cast<int>(40 * Scale));
	TwoPlayer.setOrigin(PlayerMode.getLocalBounds().left + PlayerMode.getLocalBounds().width / 2.0f, PlayerMode.getLocalBounds().top + PlayerMode.getLocalBounds().height / 2.0f);
	TwoPlayer.setPosition(mode.width / 2.0f, mode.height * 1.25f / 2.0f);

	// cài đặt nút trờ về menu chính
	Back.setFont(DisplayFont);
	Back.setFillColor(Color::White);
	Back.setString("Back");
	Back.setCharacterSize(static_cast<int>(30 * Scale));
	Back.setOrigin(PlayerMode.getLocalBounds().left + PlayerMode.getLocalBounds().width / 2.0f, PlayerMode.getLocalBounds().top + PlayerMode.getLocalBounds().height / 2.0f);
	Back.setPosition(mode.width / 2.0f, mode.height * 1.75f / 2.0f);

	// set biến lựa chọn lệnh về 1
	CommandOrder = 1;

	// set biến xem xét chế độ chơi 1 hay 2 người về false (mặc định là 1 người chơi)
	bool Is2Player = false;

	/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ LOGIC VÀ HIỂN THỊ CỦA MENU CHỌN CHẾ ĐỘ */
	while (gamestate == GameState::PlayerMode) {
		Event event;
		//vòng while xử lý logic game 
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed: // xử lý khi ấn đóng cửa sổ
			{
				gamestate = GameState::Exit;
				window.close();
				break;
			}
			case Event::KeyPressed: // xử lý sự kiện khi có nút bàn phím được bấm 
			{
				// xử lý khi nút backspace được bấm
				if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				{
				audiomanager.PlaySound("enterbutton.wav", false);
				gamestate = GameState::Menu; // trạng thái game trở về lại menu
				}
				// xử lý khi nút mũi tên xuống được bấm 
				else if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					if (CommandOrder < 3)
					{
						audiomanager.PlaySound("transitionbutton.wav", false);
						CommandOrder++; //tăng lựa chọn lệnh 1 khi ấn 1 lần nút 
					}
				}
				// xử lý khi nút mũi tên lên được bấm 
				else if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					if (CommandOrder > 1)
					{
						audiomanager.PlaySound("transitionbutton.wav", false);
						CommandOrder--; // giảm lựa chọn menu đi 1 khi nhấn nút mũi tên lên 1 lần
					}
				}
				
				// xử lý khi nút enter được nhấn 
				else if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					if (CommandOrder == 1)   // nếu lựa chọn là 1 người chơi, biến is2player sẽ set về false và trạng thái game sẽ chuyển sang Chọn Độ Khó
					{
						Is2Player = false;
						gamestate = GameState::ChooseDifficult;
						MenuChoosingDifficult(Is2Player);

					}
					else if (CommandOrder == 2) // nếu lựa chọn là 2 người chơi , biến is2player sẽ  set về true và trạng thái game sẽ chuyển sang Chọn Độ khó
					{
						Is2Player = true;
						gamestate = GameState::ChooseDifficult;
						MenuChoosingDifficult(Is2Player);

					}
					else if (CommandOrder == 3) // nếu lựa chọn là nút back, trạng thái game sẽ quay về menu chính
					{
						gamestate = GameState::Menu;
					}
				}
				break;
			}
			}


			/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ VIỆC HIỂN THỊ CỦA MENU CHỌN CHẾ ĐỘ CHƠI*/

			//lựa chọn lệnh là 1 
			//nút 1 người chơi sẽ được tô đen
			// nút 2 người chơi và nút back sẽ được tô trắng
			if (CommandOrder == 1)
			{
				OnePlayer.setFillColor(Color::Black);
				TwoPlayer.setFillColor(Color::White);
				Back.setFillColor(Color::White);
			}

			//lựa chọn lệnh là 2
			// nút 2 người chơi sẽ được tô trắng
			// nút 1 người chơi và nút back sẽ được tô đen
			else if (CommandOrder == 2)
			{
				OnePlayer.setFillColor(Color::White);
				TwoPlayer.setFillColor(Color::Black);
				Back.setFillColor(Color::White);
			}

			// lựa chọn lệnh là 3
			// nút back sẽ được tô đen
			// nút 1 người chơi và 2 người chơi sẽ được tô trắng
			else if (CommandOrder == 3)
			{
				OnePlayer.setFillColor(Color::White);
				TwoPlayer.setFillColor(Color::White);
				Back.setFillColor(Color::Black);
			}
		}

		// dùng để hiển thị tiếp menu chọn độ khó 
		


		// các lệnh để vẽ các text các nút đã được cài đặt
		window.clear(Color(94, 183, 183, 250));
		window.draw(PlayerMode);
		window.draw(OnePlayer);
		window.draw(TwoPlayer);
		window.draw(Back);
		window.display();
	}

}




//Menu con chỉnh độ khó 
//Input: biến xác định xem 2 hay 1 người chơi
void Game::MenuChoosingDifficult(bool Is2player)
{

	/* PHÀN DƯỚI ĐÂY DÙNG ĐỂ CÀI ĐẶT BAN ĐẦU CHO CÁC THUỘC TINH TRONG HÀM*/


	//Các biến Text hiển thị các lựa chọn trong Menu chỉnh độ khó 

	//1.Text có nội dung là "Difficulty", dùng như 1 nút để lựa chọn
	Text ChooseDiff;
	RectangleShape DiffScale[3]; // thang đo độ khó
	Color colorred[3];// màu thang

	//2.Text có nội dung là "Play", dùng như 1 nút để lựa chọn
	Text Play;

	//3.Text có nội dung là "Back", dùng như 1 nút để lựa chọn 
	Text Back;

	//4.Text có nội dung là "Set Score:"+<WinScore>, dùng như 1 nút để lựa chọn
	Text SetWinScore[7];



	// Chế độ chơi là 1 người -> Is2player=false
	//Các text cần hiển thị trong chế độ 1 người là ChooseDiff,DiffScale,SetWinScore,Play,Back
	//Các text cần hiển thị trong chế độ 2 người là SetWinScore,Play,Back
	if (Is2player == false)
	{
		//Cài đặt các thuộc tính cần thiết cho Text ChooseDiff
		ChooseDiff.setFont(DisplayFont);
		ChooseDiff.setFillColor(Color::Black);
		ChooseDiff.setString("Difficulty");
		ChooseDiff.setCharacterSize(static_cast<int>(60 * Scale));
		ChooseDiff.setOrigin(ChooseDiff.getLocalBounds().width / 2.0f, ChooseDiff.getLocalBounds().height / 2.0f);
		ChooseDiff.setPosition(mode.width / 2.0f, mode.height * 0.4f / 2.0f);


		//Cài đặt các sắc màu hiện thị thang đo độ khó
		for (int i = 0; i < 3; i++)
		{
			colorred[i].r = 255;
			colorred[i].b = 0;
			colorred[i].g = 0;
			colorred[i].a = 120 + i * 50;
		}


		//Cài đặt các thuộc tính cho thang đo độ khó (3 mức màu: càng đậm là càng khó)
		for (int i = 0; i < 3; i++)
		{

			DiffScale[i].setFillColor(Color::White);
			DiffScale[i].setSize(Vector2f(20.0f * Scale, 40.0f * Scale));
			DiffScale[i].setOrigin(ChooseDiff.getLocalBounds().width / 2.0f, ChooseDiff.getLocalBounds().top + ChooseDiff.getLocalBounds().height / 2.0f);
			DiffScale[i].setPosition((mode.width / 2.0f) + i * Scale * 50 - 200.0f * Scale, mode.height * 0.7f / 2.0f);
			DiffScale[i].rotate(180);
		}
	}

	// mảng string dùng để cài đặt Text SetWinScore
	string number[7] = {
		"1","2","3","4","5","6","7"
	};
	for (int i = 0; i < 7; i++)
	{
		// String Set Score sẽ cộng với phần tử i của mảng number để tạo 1 text hoàn chỉnh
		string temp = "Set score:" + number[i];
		SetWinScore[i].setFont(DisplayFont);
		SetWinScore[i].setFillColor(Color::White);
		SetWinScore[i].setString(temp);
		SetWinScore[i].setCharacterSize(static_cast<int>(40 * Scale));
		SetWinScore[i].setOrigin(SetWinScore[i].getLocalBounds().width / 2.0f, SetWinScore[i].getLocalBounds().height / 2.0f);
		if (Is2player == false)
		{
			// vị trí của Text SetWinScore thay đổi dựa trên chế độ chơi
			SetWinScore[i].setPosition(mode.width / 2.0f, mode.height * 1.0f / 2.0f); 
		}
		else
		{
			SetWinScore[i].setPosition(mode.width / 2.0f, mode.height * 0.8f / 2.0f);
		}
	}


	//Cài đặt thuộc tính cho Text Play
	Play.setFont(DisplayFont);
	Play.setFillColor(Color::White);
	Play.setString("Play");
	Play.setCharacterSize(static_cast<int>(40 * Scale));
	Play.setOrigin(Play.getLocalBounds().width / 2.0f, Play.getLocalBounds().height / 2.0f);
	if (Is2player == false)
	{
		// vị trí của Text Play thay đổi dựa trên chế độ chơi
		Play.setPosition(mode.width / 2.0f, mode.height * 1.4f / 2.0f); 
	}
	else
	{
		Play.setPosition(mode.width / 2.0f, mode.height * 1.2f / 2.0f);
	}


	//Cài đặt thuộc tính cho Text Back
	Back.setFont(DisplayFont);
	Back.setFillColor(Color::White);
	Back.setString("Back");
	Back.setCharacterSize(static_cast<int>(30 * Scale));
	Back.setOrigin(Back.getLocalBounds().width / 2.0f, Back.getLocalBounds().height / 2.0f);

	// vị trị của Text Back thay đổi dựa trên chế độ chơi
	// cài đặt giá trị của lệnh lựa chọn trong Menu chọn độ khó dựa vào chế độ chơi
	// nếu là chế độ 2 người chơi, lựa chọn mặc định sẽ là nút SetWinScore
	// nếu là chế độ 1 người chơi , lựa chọn mặc định sẽ là nút ChooseDiff
	if (Is2player == false)
	{
		Back.setPosition(mode.width / 2.0f, mode.height * 1.8f / 2.0f); 
		CommandOrder = 1;
	}
	else
	{
		Back.setPosition(mode.width / 2.0f, mode.height * 1.6f / 2.0f);
		CommandOrder = 2;
	}

	int horizonCommand = 1;// lựa chọn theo chiều ngang mặc định là 1



	/*PHẦN CODE DƯỚI ĐÂY LÀ VÒNG LẶP DỰNG KHUNG HÌNH, XỬ LÝ LOGIC GAME VÀ HIỂN THỊ*/


	// bắt đầu vòng while để dựng khung hình với điều kiện trạng thái game đang là Chọn Độ Khó
	while (gamestate == GameState::ChooseDifficult)
	{
		Event event;
		while (window.pollEvent(event)) // vòng while xử lý các logic game
		{
			switch (event.type)
			{
			case Event::Closed: // xử lý sự kiện xảy ra khi đóng cửa sổ
			{
				//trạng thái game quay về exit để thoát game đồng thời đóng cửa sổ
				gamestate = GameState::Exit;
				window.close();
				break;
			}
			case Event::KeyPressed: // xử lý sự kiện khi bàn phím được ấn
			{
				//xử lý khi phím mũi tên xuống được bấm
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					if (CommandOrder < 4)// do có chỉ có tối đa 4 nút để xử lý  
					{
						audiomanager.PlaySound("transitionbutton.wav", false);
						CommandOrder++; // tăng lựa chọn lệnh trong Menu lệnh (đi xuống)
					}
				}

				// xử lý khi nút backspace được nhấn, trạng thái game sẽ trở về chọn chế độ chơi
				else if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					if (Is2player == true)
					{
						CommandOrder = 2;
					}
					else
					{
						CommandOrder = 1;
					}
					gamestate = GameState::PlayerMode;
				}

				// xử lý khi phím mũi tên lên được bấm
				else if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					if (CommandOrder > 1)
					{
						audiomanager.PlaySound("transitionbutton.wav", false);
						CommandOrder--; // giảm lựa chọn lệnh trong Menu lệnh( đi ngược lên)
					}
				}

				// xử lý khi lệnh đang là nút ChooseDiff ( tăng độ khó)
				if (CommandOrder == 1)
				{

					// xử lý khi nút mũi tên phải được nhấn
					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						if (horizonCommand < 3) // do độ khó là 3 nấc 
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							horizonCommand++; // tăng độ khó lên 1 nấc khi bấm 1 lần nút phải 
						}
					}

					//xử lý khi nút mũi tên trái được nhấn 
					else if (Keyboard::isKeyPressed(Keyboard::Left))
					{
						if (horizonCommand > 1)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							horizonCommand--;// giảm độ khó đi 1 nấc khi bấm 1 lần nút trái

						}
					}
				}
				// xử lý khi lựa chọn đang là nút SetWinScore
				else if (CommandOrder == 2)
				{
					// xử lý khi nút mũi tên phải được bấm 
					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						if (WinScore < 6) //WinScore đi từ 0 đến 6 
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							WinScore++; // tăng điểm cần thắng 1 số khi bấm 1 lần nút phải
						}
					}

					// xử lý khi nút mũi tên trái được bấm 
					else if (Keyboard::isKeyPressed(Keyboard::Left))
					{
						if (WinScore > 0) // WinScore đi từ 0 đến 6
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							WinScore--;// giảm điểm cần thắng đi 1 số khi bấm 1 lần nút trái 
						}
					}
				}

				// xử lý khi nút enter được bấm 
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					//xử lý khi enter trong khi lựa chọn lệnh đang là nút Play
					if (CommandOrder == 3 )
					{
						// Nếu là chế độ 2 người chơi, sẽ set chế độ chơi về mặc định
						if (Is2player == true)
						{
							playmode = PlayerMode::Default;

						}

						// Nếu là chế độ 1 người chơi, sẽ set chế độ chơi về Computer (máy tính)
						else
						{
							playmode = PlayerMode::Computer;
						}
						if (playmode == PlayerMode::Computer) // nếu là chế độ chơi là máy thì sẽ set lại chế độ chơi là máy 
						{
							objs.player2.ChangeMode(PlayerMode::Computer);
						}

						//Dựa trên lựa chọn lệnh ngang để set độ khó của game (không áp dụng cho chế độ 2 người chơi)
						if (horizonCommand == 1)
						{
							difficult = 0.6f;
						}
						if (horizonCommand == 2)
						{
							difficult = 0.8f;
						}
						if (horizonCommand == 3)
						{
							difficult = 1.0f;
						}
						// Set điểm cần thằng +1 do WinScore ở trên chạy từ 0 đến 6
						WinScore = WinScore + 1;

						//Set trạng thái chơi về Đang Chơi
						gamestate = GameState::Playing;

					}

					// Xử lý khi enter được nhấn khi lựa chọn lệnh
					else if (CommandOrder == 4)
					{
						//Set trạng thái chơi về Chọn Chế Độ Chơi
						if (Is2player == true)
						{
							CommandOrder = 2;
						}
						else
						{
							CommandOrder = 1;
						}
						gamestate = GameState::PlayerMode;
						

					}
				}
				break;
			}
			}

			/* PHÀN CODE DƯỚI ĐÂY DÙNG ĐỂ XỬ LÝ XÂY DỰNG ĐỒ HỌA HIỂN THỊ KHÔNG LIÊN QUAN ĐẾN LOGIC GAME*/


			//Lựa chọn menu là 1 ( nút ChooseDiff: xử lý phần tăng/giảm độ khó)
			//Các nút Back, SetWinScore, Play được set về màu trắng
			// Nút ChooseDiff sẽ là màu đen do đang được chọn
			if (CommandOrder == 1)
			{
				ChooseDiff.setFillColor(Color::Black);
				Play.setFillColor(Color::White);
				Back.setFillColor(Color::White);
				SetWinScore[WinScore].setFillColor(Color::White); // Nút SetWinScore thứ i sẽ được vẽ mang nội dung là Set Score + "i+1" i chạy từ 0 đến 6
			}


			//Lựa chọn menu là 2 ( nút SetWinScore: xử lý phần tăng/giảm điểm số cần để chiến thắng)
			//Các nút Back, Play, ChooseDiff được set về màu trắng
			//Nút SetWinScore được set về màu đen
			else if (CommandOrder == 2)
			{
				ChooseDiff.setFillColor(Color::White);
				Play.setFillColor(Color::White);
				Back.setFillColor(Color::White);
				SetWinScore[WinScore].setFillColor(Color::Black); // Nút SetWinScore thứ i sẽ được vẽ mang nội dung là Set Score + "i+1" i chạy từ 0 đến 6
			}

			//Lựa chọn menu là 3 ( nút Play: xử lý phần tiếp tục sang phần game chính)
			//Các nút Back,ChooseDiff, SetWinScore được set về màu trắng
			//Nút Play được set về màu đen

			else if (CommandOrder == 3)
			{
				ChooseDiff.setFillColor(Color::White);
				Play.setFillColor(Color::Black);
				SetWinScore[WinScore].setFillColor(Color::White); // Nút SetWinScore thứ i sẽ được vẽ mang nội dung là Set Score + "i+1" i chạy từ 0 đến 6
				Back.setFillColor(Color::White);
			}

			//Lựa chọn menu là 4 ( nút Back: xử lý quay về phần trước)
		   //Các nút Play,ChooseDiff, SetWinScore được set về màu trắng
		   //Nút Back được set về màu đen
			else if (CommandOrder == 4)
			{

				ChooseDiff.setFillColor(Color::White);
				Play.setFillColor(Color::White);
				SetWinScore[WinScore].setFillColor(Color::White); // Nút SetWinScore thứ i sẽ được vẽ mang nội dung là Set Score + "i+1" i chạy từ 0 đến 6
				Back.setFillColor(Color::Black);
			}


			// xử lý khi chế độ chơi là 1 người 
			if (Is2player == false)
			{
				// Các màu sắc của DiffScale sẽ đậm nhạt tùy theo độ khó người chơi lựa chọn
				if (horizonCommand == 1)
				{
					DiffScale[0].setFillColor(colorred[0]);
					DiffScale[1].setFillColor(Color::White);
					DiffScale[2].setFillColor(Color::White);
				}
				else if (horizonCommand == 2)
				{
					DiffScale[0].setFillColor(colorred[0]);
					DiffScale[1].setFillColor(colorred[1]);
					DiffScale[2].setFillColor(Color::White);
				}
				else if (horizonCommand == 3)
				{
					DiffScale[0].setFillColor(colorred[0]);
					DiffScale[1].setFillColor(colorred[1]);
					DiffScale[2].setFillColor(colorred[2]);
				}
			}
			window.clear(Color(94, 183, 183, 250)); // lệnh tạo nền có màu rgb là 94,183,183 và độ trong là 250


			// Vẽ các Text và các hình đã được cài đặt 
			for (int i = 0; i < 3; i++)
			{
				window.draw(DiffScale[i]);
			}
			window.draw(ChooseDiff);
			window.draw(Back);
			window.draw(Play);
			window.draw(SetWinScore[WinScore]);
			window.display();
		}
	}
}

void Game::resetGame()
{
	objs.playground = PlayGround(mode, DisplayFont, Scale); // sân chơi được tạo dựa vào constructor mode, font và tỉ lệ căn chỉnh độ phân giải
	objs.player1 = Player(mode, PlayerMode::player1, Scale);// tạo 2 player , với chế độ chơi mặc định là 2 người chơi người 1 và 2
	objs.player2 = Player(mode, PlayerMode::player2, Scale);
	objs.ball = Ball(mode, Scale);
	objs.item = Item(mode, Scale);
	// reset điểm 2 người chơi về lại 0
	objs.player1.ResetScore();
	objs.player2.ResetScore();

}

void Game::loadGame()
{
	fstream file("savegame.txt", ios::in);
	if (file.is_open() == false)
		cout << "Load save file error";
	resetGame();

	float x;
	float y;
	int score;
	float velocity;
	float velocity2;

	//Ghi thông tin người chơi thứ nhất và file
	file >> x >> y >> score >> velocity;
	objs.player1.getShape()->setPosition(Vector2f(x,y));
	objs.player1.setScore(score);
	objs.player1.setVelocity(velocity);


	//Ghi thông tin người chơi thứ nhất và file
	file >> x >> y >> score >> velocity;
	objs.player2.getShape()->setPosition(Vector2f(x,y));
	objs.player2.setScore(score);
	objs.player2.setVelocity(velocity);


	//Ghi thông tin banh vào file
	file >> x >> y >> velocity >> velocity2;
	objs.ball.getShape().setPosition(Vector2f(x, y));
	objs.ball.SetVelocity(velocity, velocity2);
	

	//Ghi một số thông tin của game đấu
	file >> this->WinScore;
	file >> this->difficult;
	int tmp;
	file >> tmp;
	if (tmp == 0)
	{
		playmode = PlayerMode::Computer;
		objs.player2.ChangeMode(playmode);
	}
	objs.playground.Score[0].setString(to_string(objs.player1.GetScore()));
	objs.playground.Score[1].setString(to_string(objs.player2.GetScore()));
	file.close();
	gamestate = GameState::Playing;
}

void Game::saveGame()
{
	fstream file("savegame.txt", ios::out);
	if (file.is_open() == false)
		cout << "Load save file error";
	
	//Ghi thông tin người chơi thứ nhất và file
	file << objs.player1.getShape()->getPosition().x << endl;
	file << objs.player1.getShape()->getPosition().y << endl;
	file << objs.player1.GetScore() << endl;
	file << objs.player1.getVelocity() << endl;
	
	//Ghi thông tin người chơi thứ hai và file
	file << objs.player2.getShape()->getPosition().x << endl;
	file << objs.player2.getShape()->getPosition().y << endl;
	file << objs.player2.GetScore() << endl;
	file << objs.player2.getVelocity() << endl;

	//Ghi thông tin banh vào file
	file << objs.ball.getShape().getPosition().x << endl;
	file << objs.ball.getShape().getPosition().y << endl;
	file << objs.ball.GetxVelocity() << endl;
	file << objs.ball.GetyVelocity() << endl;

	//Ghi một số thông tin của game đấu
	file << this->WinScore << endl;
	file << this->difficult << endl;
	if (playmode == PlayerMode::Computer)
		file << 0;
	else
		file << 1;

	file.close();
	gamestate = GameState::Playing;
}





//Menu chạy game chính
void Game::RunGame()
{
	/*PHÀN CODE DƯỚI ĐÂY CÀI ĐẶT CÁC THUỘC TÍNH CHO HÀM CHƠI GAME CHÍNH*/

	//Dừng âm thanh background lại
	audiomanager.PauseSound();

	// đồng hồ đo thời gian giữa 2 khung hình
	Clock clock;

	// Set thời gian giữa 2 khung hình về 0
	DeltaTime = 0.0f;

	// Set biến kiểm tra game có tạm dừng hay không về false
	

	// Set biến lựa chọn lệnh trong menu con khi tạm dừng về 0
	CommandOrder = 1;

	// Set lượt đánh qua lại giữa 2 người chơi về 0
	int Turn = 1;


	/*ĐOẠN CODE DƯỚI ĐÂY XỬ LÝ VIỆC DỰNG HÌNH VÀ XỬ LÝ LOGIC GAME*/
	// vòng lặp xây dựng khung hình
	while (gamestate == GameState::Playing)
	{
		
		Event event;
		// vòng lặp xử lý logic game
		while (window.pollEvent(event))
		{

			switch (event.type)
			{
			case Event::Closed: // xử lý khi ấn đóng cửa sổ 
			{
				gamestate = GameState::Exit; // chuyển trạng thái chơi về exit
				window.close();// đóng cửa sổ 
				break;
			}

			case Event::KeyPressed: // xử lý sự kiện khi có nút bàn phím được bấm 
			{
				//xử lý khi nút BackSpace được bấm 
				if (Keyboard::isKeyPressed(Keyboard::BackSpace))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					//chuyển trạng thái game về Pause
					gamestate = GameState::Pause;
				}
				break;
			}
			}

		}

		/* PHÀN CODE DƯỚI ĐÂY ĐIỀU KHIỂN HÀNH VI CỦA 2 PLAYER VÀ BANH*/

		if (gamestate == GameState::Playing)
		{
		// Hàm sử dụng để điều khiển 2 player
			objs.player1.MovementControl(objs.playground.GetList().at(0), objs.playground.GetList().at(2), DeltaTime, objs.ball, difficult, Scale);

			objs.player2.MovementControl(objs.playground.GetList().at(0), objs.playground.GetList().at(2), DeltaTime, objs.ball, difficult, Scale);

	
			objs.ball.Update(DeltaTime, mode, audiomanager, objs.player1, objs.player2, objs.playground, objs.item, Scale);// cập nhật vị trí banh
		}
		
		/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ PHẦN HIỆN THỊ*/

		//hiện thị phông nền
		window.clear(Color(64, 191, 193, 250));


		// hiển thị điểm trên sân chơi
		window.draw(objs.playground.Score[0]);
		window.draw(objs.playground.Score[1]);

		// hiển thị 2 người chơi 
		window.draw(objs.player1.ReturnShape());
		window.draw(objs.player2.ReturnShape());

		// hiển thị sân chơi
		objs.playground.DisplayPlayGround(window);

		// hiển thị banh
		window.draw(objs.ball.getShape());

		// hiển thị item
		if (objs.item.appeared())
			window.draw(objs.item.getShape());

		//Xuất hiện item ngẫu nhiên nếu roll được số nguyên tố
		if ((1 + rand() % 1000000) % 10007)
			objs.item.appear();


		/* PHẦN CODE DƯỚI ĐÂY XỬ LÝ THỜI GIAN VÀ MENU CON TẠM DỪNG*/
		// lấy giá trị của thời gian trôi qua để lấy thời gian giữa 2 khung hình 



		// nếu biến tạm dừng là true sẽ dừng thời gian lại 

		//Chạy menu con tạm dừng khi biến tạm dừng là true
	


		// xử lý thắng thua của game 
		if (objs.player1.GetScore() >= WinScore || objs.player2.GetScore() >= WinScore) // kiểm tra xem người chơi nào đạt tới WinScore trước
		{
			gamestate = GameState::Losing;
			if (objs.player1.GetScore() > objs.player2.GetScore())
			{
				WhoWin = true; // true khi người chơi 1 chiến thắng
			}
			else
			{
				WhoWin = false;// false khi người chơi 2 chiến thắng

			}
		}
		window.display(); // vẽ tất cả lên khung hình
		DeltaTime = clock.getElapsedTime().asSeconds();
		//Chạy menu pause;
		RunPauseMenu();
		clock.restart(); // reset thời gian 2 khung hình về 0 bắt đầu khung hình tiếp theo
	}
}




//hàm chạy Menu con tạm dừng 
//Input: biến kiểm tra xem có tạm dừng không , lựa chọn lệnh trong menu con tạm dừng 
void Game::RunPauseMenu()
{
	/*PHẦN CODE NÀY CÀI ĐẶT CÁC THUỘC TÍNH CẦN CỦA MENU CON TẠM DỪNG */
	if (gamestate == GameState::Pause)
	{




		Text PauseTitle; // tên hiệu của menu tạm dừng 
		Text ResumeButton;// nút tiếp tục 
		Text SaveGameButton; //Nút lưu game
		Text MainMenuButton;// nút trở về menu chính
		Text QuitButton;// nút thoát game
		RectangleShape Background; //phông nền 


		// cài đặt phông nền 
		Background.setSize(Vector2f(mode.width / 2.5f, mode.height * 3.2f / 4.0f));
		Background.setOrigin(Background.getLocalBounds().width / 2.0f, Background.getLocalBounds().height / 2.0f);
		Background.setFillColor(Color(255, 175, 176, 250));
		Background.setOutlineColor(Color::White);
		Background.setOutlineThickness(1.0f);
		Background.setPosition(mode.width / 2.0f, mode.height / 2.0f);

		// cài đặt tên hiệu của menu tạm dừng 
		PauseTitle.setFont(DisplayFont);
		PauseTitle.setFillColor(Color::Black);
		PauseTitle.setString("PAUSE");
		PauseTitle.setCharacterSize(static_cast<int>(60 * Scale));
		PauseTitle.setOrigin(PauseTitle.getLocalBounds().left + PauseTitle.getLocalBounds().width / 2.0f, PauseTitle.getLocalBounds().top + PauseTitle.getLocalBounds().height / 2.0f);
		PauseTitle.setPosition(mode.width / 2.0f, mode.height * 0.5f / 2.0f);


		// cài đặt nút tiếp tục game
		ResumeButton.setFont(DisplayFont);
		ResumeButton.setFillColor(Color::White);
		ResumeButton.setString("Resume");
		ResumeButton.setCharacterSize(static_cast<int>(40 * Scale));
		ResumeButton.setOrigin(PauseTitle.getLocalBounds().left + PauseTitle.getLocalBounds().width / 2.0f, PauseTitle.getLocalBounds().top + PauseTitle.getLocalBounds().height / 2.0f);
		ResumeButton.setPosition(mode.width / 2.0f, mode.height * 1.0f / 2.0f);

		

		// cài đặt nút lưu game
		SaveGameButton.setFont(DisplayFont);
		SaveGameButton.setFillColor(Color::White);
		SaveGameButton.setString("Save Game");
		SaveGameButton.setCharacterSize(static_cast<int>(40 * Scale));
		SaveGameButton.setOrigin(PauseTitle.getLocalBounds().left + PauseTitle.getLocalBounds().width / 2.0f, PauseTitle.getLocalBounds().top + PauseTitle.getLocalBounds().height / 2.0f);
		SaveGameButton.setPosition(mode.width / 2.0f, mode.height * 1.25f / 2.0f);


		// cài đặt nút trở về menu chính
		MainMenuButton.setFont(DisplayFont);
		MainMenuButton.setFillColor(Color::White);
		MainMenuButton.setString("Main Menu");
		MainMenuButton.setCharacterSize(static_cast<int>(40 * Scale));
		MainMenuButton.setOrigin(PauseTitle.getLocalBounds().left + PauseTitle.getLocalBounds().width / 2.0f, PauseTitle.getLocalBounds().top + PauseTitle.getLocalBounds().height / 2.0f);
		MainMenuButton.setPosition(mode.width / 2.0f, mode.height * 1.5f / 2.0f);


		//cài đặt nút thoát game
		QuitButton.setFont(DisplayFont);
		QuitButton.setFillColor(Color::White);
		QuitButton.setString("Quit");
		QuitButton.setCharacterSize(static_cast<int>(40 * Scale));
		QuitButton.setOrigin(PauseTitle.getLocalBounds().left + PauseTitle.getLocalBounds().width / 2.0f, PauseTitle.getLocalBounds().top + PauseTitle.getLocalBounds().height / 2.0f);
		QuitButton.setPosition(mode.width / 2.0f, mode.height * 1.75f / 2.0f);
		CommandOrder = 1;
		while (gamestate == GameState::Pause)
		{

			
			Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case Event::Closed:
				{
					gamestate = GameState::Exit;
					window.close();
					break;
				}
				case Event::KeyPressed:
				{
					if (Keyboard::isKeyPressed(Keyboard::BackSpace))
					{
						audiomanager.PlaySound("enterbutton.wav", false);
						gamestate = GameState::Playing;
					}
					else if (Keyboard::isKeyPressed(Keyboard::Down))
					{
						if (CommandOrder < 4)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							CommandOrder++; // tăng lựa chọn lệnh trong Menu tạm dừng lên 1 khi bấm 1 nút xuống
						}
					}

					// xử lý khi nút mũi tên lên được bấm và biến tạm dừng đang là true
					else if (Keyboard::isKeyPressed(Keyboard::Up))
					{
						if (CommandOrder > 1)
						{
							audiomanager.PlaySound("transitionbutton.wav", false);
							CommandOrder--;// giảm lựa chọn trong Menu tạm dừng xuống 1 khi bấm 1 nút lên
						}
					}

					// xử lý khi nút enter được bấm 
					else if (Keyboard::isKeyPressed(Keyboard::Enter))
					{
						audiomanager.PlaySound("enterbutton.wav", false);
						// xử lý khi lựa chọn trong Menu tạm dừng đang là 1 ( nút Resume) và biến tạm dừng đang là true
						if (CommandOrder == 1)
						{
							gamestate = GameState::Playing;
						}
						// xử lý khi lựa chọn trong Menu tạm dừng đang là 2 ( nút lưu game)
						if (CommandOrder == 2)
						{
							gamestate = GameState::Save;
							saveGame();
						}
						// xử lý khi lựa chọn trong Menu tạm dừng đang là 3 ( nút trở về Menu chính) và biến tạm dừng đang là true
						if (CommandOrder == 3)
						{
							gamestate = GameState::Menu;// trạng thái game về lại menu chính 
						}

						// xử lý khi lựa chọn trong Menu tạm dừng đang là 4 ( nút thoát game Quit) và biến tạm dừng đang là true
						if (CommandOrder == 4)
						{
							gamestate = GameState::Exit;// trạng thái game về lại exit
							window.close();// đóng cửa sổ 
						}
					}
					break;
				}
				break;
				}
			}
		

			// Nếu game đang tạm dừng các Text sẽ được hiển thị

			// lựa chọn lệnh trong menu là 1
			// các nút Main Menu và quit sẽ được tô trắng
			// Nút resume sẽ được tô màu đen
			if (CommandOrder == 1)
			{
				ResumeButton.setFillColor(Color::Black);
				SaveGameButton.setFillColor(Color::White);
				MainMenuButton.setFillColor(Color::White);
				QuitButton.setFillColor(Color::White);
			}

			//lựa chọn lệnh trong menu là 2
			//các nút Quit và Play sẽ được tô trắng
			//Nút Main Menu sẽ được tô trắng 
			if (CommandOrder == 2)
			{
				ResumeButton.setFillColor(Color::White);
				SaveGameButton.setFillColor(Color::Black);
				MainMenuButton.setFillColor(Color::White);
				QuitButton.setFillColor(Color::White);
			}


			//lựa chọn lệnh trong menu là 3
			//các nút Main Menu và Play sẽ được tô trắng
			//Nút Quit sẽ được tô trắng 
			if (CommandOrder == 3)
			{
				ResumeButton.setFillColor(Color::White);
				SaveGameButton.setFillColor(Color::White);
				MainMenuButton.setFillColor(Color::Black);
				QuitButton.setFillColor(Color::White);
			}

			

			//lựa chọn lệnh trong menu là 4
			//các nút Main Menu và Play sẽ được tô trắng
			//Nút Quit sẽ được tô trắng 
			if (CommandOrder == 4)
			{
				ResumeButton.setFillColor(Color::White);
				SaveGameButton.setFillColor(Color::White);
				MainMenuButton.setFillColor(Color::White);
				QuitButton.setFillColor(Color::Black);
			}


			/*PHẦN CODE NÀY DÙNG ĐỂ HIỂN THỊ CÁC TEXT ĐÃ ĐƯỢC CÀI ĐẶT*/
			window.draw(Background);
			window.draw(PauseTitle);
			window.draw(ResumeButton);
			window.draw(SaveGameButton);
			window.draw(MainMenuButton);
			window.draw(QuitButton);
			window.display();
		}
	}
	
	

}





//hàm hiển thị Menu con khi có người chơi thắng/thua
void Game::LosingPanel()
{
	/*PHẦN CODE DƯỚI ĐÂY CÀI ĐẶT CÁC THUỘC TÍNH CHO TEXT VÀ NÚT CHO MENU CON KẾT THÚC PHẦN CHƠI*/
	Text EndTitle;
	Text RestartButton;
	Text MainMenuButton;
	Text Announce;

	// cài đặt text hiển thị tên menu
	EndTitle.setFont(DisplayFont);
	EndTitle.setFillColor(Color::Black);
	EndTitle.setString("GAME OVER");
	EndTitle.setCharacterSize(static_cast<int>(60*Scale));
	EndTitle.setOrigin(EndTitle.getLocalBounds().left + EndTitle.getLocalBounds().width / 2.0f, EndTitle.getLocalBounds().top + EndTitle.getLocalBounds().height / 2.0f);
	EndTitle.setPosition(mode.width / 2.0f, mode.height * 0.5f / 2.0f);


	// cài đặt text hiển thị tên người thắng cuộc
	Announce.setFont(DisplayFont);
	Announce.setFillColor(Color::Black);
	Announce.setCharacterSize((static_cast<int>(40 * Scale)));
	Announce.setOrigin(EndTitle.getLocalBounds().left + EndTitle.getLocalBounds().width / 2.0f, EndTitle.getLocalBounds().top + EndTitle.getLocalBounds().height / 2.0f);
	Announce.setPosition(mode.width / 2.0f, mode.height * 0.9f / 2.0f);
	if (WhoWin == true) // nếu biến WhoWin là true người thắng là người chơi 1 nằm ở bên tay trái 
	{
		Announce.setString("P1 won!");
	}
	else
	{
		Announce.setString("P2 won!");// nếu biến WhoWin là false người thắng là người chơi 2 nằm ở bên tay phải
	}


	// cài đặt nút Restart Game
	RestartButton.setFont(DisplayFont);
	RestartButton.setFillColor(Color::Black);
	RestartButton.setString("Restart");
	RestartButton.setCharacterSize(static_cast<int>(40 * Scale));
	RestartButton.setOrigin(EndTitle.getLocalBounds().left + EndTitle.getLocalBounds().width / 2.0f, EndTitle.getLocalBounds().top + EndTitle.getLocalBounds().height / 2.0f);
	RestartButton.setPosition(mode.width / 2.0f, mode.height * 1.25f / 2.0f);
	

	// cài đặt nút Quay về menu chính
	MainMenuButton.setFont(DisplayFont);
	MainMenuButton.setFillColor(Color::White);
	MainMenuButton.setString("Main Menu");
	MainMenuButton.setCharacterSize((static_cast<int>(40 * Scale)));
	MainMenuButton.setOrigin(EndTitle.getLocalBounds().left + EndTitle.getLocalBounds().width / 2.0f, EndTitle.getLocalBounds().top + EndTitle.getLocalBounds().height / 2.0f);
	MainMenuButton.setPosition(mode.width / 2.0f, mode.height * 1.5f / 2.0f);

	// set lựa chọn lệnh menu con về 1
	CommandOrder = 1;

	
	/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ LOGIC GAME VÀ HIỂN THỊ CỦA MENU CON LOSINGPANEL*/
	while (gamestate == GameState::Losing)
	{
		Event event;
		// vòng while xử lý logic game
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed: // xử lý việc đóng cửa sổ đang mở 
			{
				gamestate = GameState::Exit;// set trạng thái game về lại exit
				window.close();// đóng cửa sổ đang mở 
				break;
			}
			case Event::KeyPressed: // xử lý khi có phím được ấn 
			{

				// xử lý khi có phím enter được nhấn 
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					audiomanager.PlaySound("enterbutton.wav", false);
					// nếu lựa chọn lệnh đang là 1 ( ở nút restart) 
					if (CommandOrder == 1)
					{

						gamestate = GameState::Playing;// trạng thái game chuyển về lại đang chơi
						resetGame();
						if (playmode == PlayerMode::Computer) // nếu là chế độ chơi là máy thì sẽ set lại chế độ chơi là máy 
						{
							objs.player2.ChangeMode(PlayerMode::Computer);
						}
					}

					// nếu lựa chọn lệnh đang là 2 ( ở nút main menu)
					else
					{
						gamestate = GameState::Menu;// trạng thái game sẽ trả về menu chính của game
					}
				}

				// xử lý khi nút mũi tên lên được bấm 
				if (Keyboard::isKeyPressed(Keyboard::Up) && CommandOrder > 1)
				{
					audiomanager.PlaySound("transitionbutton.wav", false);
					CommandOrder--;// giảm lựa chọn lệnh đi 1 khi bấm 1 lần nút mũi tên lên
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) && CommandOrder < 2)
				{
					audiomanager.PlaySound("transitionbutton.wav", false);
					CommandOrder++;// tăng lựa chọn lệnh lên 1 khi bấm 1 lần nút mũi tên xuống 
				}
				
				break;
			}
			}

		}

		
		/*PHẦN CODE DƯỚI ĐÂY XỬ LÝ VIỆC HIỂN THỊ CỦA MENU*/

		// xử lý khi lựa chọn lệnh là 1
		// nút Restart được tô đen
		// nút MainMenu được tô trắng 
		if (CommandOrder == 1)
		{
			RestartButton.setFillColor(Color::Black);
			MainMenuButton.setFillColor(Color::White);
		}

		// xử lý khi lựa chọn lệnh là 2
		// nút Restart được tô trắng
		//nút MainMenu được tô đen
		else
		{
			RestartButton.setFillColor(Color::White);
			MainMenuButton.setFillColor(Color::Black);
		}
		window.clear(Color(255, 175, 176, 250));
		window.draw(Announce);
		window.draw(RestartButton);
		window.draw(EndTitle);
		window.draw(MainMenuButton);
		window.display();
	}
}



//hàm lưu cài đặt, tùy chỉnh của người dùng
void Game::SaveSetting()
{
	Style::Close;
	ofstream fout;
	fout.open("Setting.txt"); // thực hiện ghi vào file Setting.txt để lưu cài đặt
	fout << mode.width << "," << mode.height << "," << audiomanager.SoundVolumeValue() << "," << windowmode << endl;;// các thông tin được lưu bao gồm: thông số khung hình và âm lượng
	fout.close();// đóng file sau khi ghi
}




// hàm đọc thông tin cài đặt đã được lưu vào file
void Game::LoadSetting()
{
	// tạo các biến tạm để lưu trữ thông tin đọc từ file sau đó gán vào thuộc tính <mode> trong class
	int height;
	int width;
	int audio;
	int WindowMode;
	ifstream fin;
	fin.open("Setting.txt"); // mở file Setting.txt để đọc
	if (fin.is_open()) // kt xem có mở được file không
	{
		fin >> width;
		fin.seekg(1, 1); // dời con trỏ file sang 1 để bỏ dấu phẩy và tiến hành đọc tiếp số tiếp theo
		fin >> height;
		fin.seekg(1, 1);
		fin >> audio;
		fin.seekg(1, 1);
		fin >> WindowMode;
	}
	else // nếu không mở được set các giá trị về độ phân giải mặc định
	{
		width = 800;
		height = 450;
		audio = 5;
		Scale = 1;
		WindowMode = 0;
	}
	// set các thuộc tính khung hình dựa trên các biến tạm đã đọc
	Scale = height / 450.0f;
	mode.width = width;
	mode.height = height;
	audiomanager.SetVolume(audio);
	windowmode = WindowMode;
}


