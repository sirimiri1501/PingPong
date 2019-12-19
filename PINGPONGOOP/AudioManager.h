#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include "SFML/Audio.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace sf;
using namespace std;
struct SoundEffect // kiểu dữ liệu tự định nghĩa cho phần hiệu ứng âm thanh gồm 2 biến là buffer chứa file âm và tên file âm
{
	Sound soundplay;
	SoundBuffer sound;// Load file từ bộ nhớ buffer lên 
	string name;		// tên sound cần phát 
};
class AudioManager // lớp quản lý phần âm thanh của game
{
private:
	vector <SoundEffect> arraySound;     // mảng chứa các âm khi chơi
public:
	// hàm phát nhạc
	//Input: tên âm cần phát và chế độ phát: lặp hoặc không lặp
	void PlaySound(string Sound,bool Loop);
	
	// hàm thêm nhạc vào buffer
	//Input: vị trí chứa âm( mặc định: Sound/XXX.wav)
	void AddSound(string SoundLocate);
	
	
	// hàm tạm dừng nhạc
	void PauseSound();
	
	// hàm chỉnh âm lượng
	//Input:giá trị âm lượng thay đổi
	void SetVolume(int ValueChange);
	
	// hàm lấy giá trị âm lượng 
	int SoundVolumeValue();
	
	// khởi tạo object audiomanager
	AudioManager();

	// destructor cho object;
	~AudioManager();
};
#endif

