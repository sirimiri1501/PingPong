 
#ifndef GAME_H_
#define GAME_H_

#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "Player.h"
#include "Item.h"
#include "PlayGround.h"
#include "SFML/Audio.hpp"
#include "AudioManager.h"
#include <fstream>
using namespace sf;
using namespace std;
enum class GameState
{
	Pause, Save, Playing, Losing, PlayerMode, ChooseDifficult, Menu, Option, Continue, Exit
};// các GameState của lớp game: Đang chơi, Đã có người thua, Chọn chế độ chơi, Chọn độ khó, Menu chính, Menu option, Thoát game

struct GameObj
{
	PlayGround playground;
	Player player1;
	Player player2;
	Ball ball;
	Item item;
};

class Game // lớp đối tượng quản lý cả game
{
private:
	RenderWindow window; // Dựng khung hình
	VideoMode mode;		// đặc tính khung hình
	Font DisplayFont;	// font hiển thị toàn game
	double DeltaTime;	// thời gian giữa 2 khung hình được dựng
	GameState gamestate;// trạng thái game
	int WinScore;// điểm để chiến thắng
	int CommandOrder;// sử dụng trong menu lựa chọn
	AudioManager audiomanager;// quản lý âm thanh
	bool WhoWin;// quản lý người thắng cuộc
	bool ChangeSetting = false;// đã thay đổi thuộc tính khung hình
	PlayerMode playmode;// chế độ chơi
	float Scale = 1.0f;// tỉ lệ phóng to nhỏ màn hình dùng khi chuyển đổi độ phân giải
	float difficult = 1.0f;// độ khó của game
	int windowmode = 0;
	GameObj objs;
public:
	//Constructor sử dụng tạo game
	Game();

	//Menu chọn độ khó: 
	//1.điều chỉnh điểm để thắng thông qua biến WinScore , 
	//2.Điều chỉnh độ khó của người chơi máy tính thông qua biến diffcult)
	//Input: chọn chế độ nào (true là 2 người chơi, false: chơi với máy)
	void MenuChoosingDifficult(bool Is2player);

	//Reset cac obj trong game
	void resetGame();
	
	//Load cac obj duoc luu
	void loadGame();
	
	//Luu cac obj 
	void saveGame();

	//Hàm chạy game: 
	//1.Chạy phần game dựa trên playmode, diffcult và WinScore đã chọn)
	void RunGame();

	//hàm chạy toàn bộ game: 
	//1.Chạy khi trạng thái game gamestate chưa là exit hoặc cửa sổ chạy chưa bị đóng)
	void RunEntireGame();

	//Hàm chạy menu tạm dừng trò chơi:
	//Chạy khi gamestate là Pause
	void RunPauseMenu();


	//Hàm hiện thị menu chính
	//1.Chọn các gamestate khác nhau của game gồm : play, option và exit
	void Menu();

	//Hàm hiện thị menu tinh chỉnh của game 
	//1.Tinh chỉnh các độ phân giải của game và âm lượng game)
	void MenuOption();

	//hàm hiện thị menu kết thúc phần chơi:
	//1. hiện thị tên người thắng
	//2. các lựa chọn sau khi kết thúc phần chơi
	void LosingPanel();


	//hàm hiện thị menu chọn chế độ chơi:
	//1. Chỉnh chế độ chơi thông qua biến playmode
	void PlayerMode();

	//Hàm lưu thông tin các cài đặt người chơi đã chọn xuống file Setting.txt
	void SaveSetting();

	//Hàm đọc thông tin cài đặt đã lưu của người chơi khi khởi động lại game 
	void LoadSetting();

	//Destructor của object game 
	~Game();
};


#endif // !GAME_H_



