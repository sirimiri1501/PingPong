#include "AudioManager.h"


// Hàm phát âm thanh
void AudioManager::PlaySound(string Sound,bool Loop)// Nhận vào tên âm cần phát và chế độ phát 
{
	vector<SoundEffect>::iterator i;
	//Vòng For quét từng phần tử trong mảng vector hiệu ứng âm thanh
	for (i = arraySound.begin(); i != arraySound.end(); i++)
	{
		if (i->name == Sound) // Tìm thấy tên sound trùng với tên đã có trong mảng
		{
			i->soundplay.setBuffer(i->sound);// cài bộ nhớ tạm để phát âm thanh
			i->soundplay.setLoop(Loop);// cài đặt chế độ phạt: lặp hoặc không lặp
			i->soundplay.play();// cài đặt âm sẽ phát vào biến Âm Thanh
			
		}
	}
	return;// không tìm thấy return 
}


// hàm thêm âm thanh
void AudioManager::AddSound(string SoundLocate)// tên sound 
{
	SoundEffect temp;// biến tạm chứa hiệu ứng sound 
	temp.name = SoundLocate;// tên của hiệu ứng sound
	SoundLocate = "Sound/" + SoundLocate;// địa chỉ chứa file sound
;
	if (!temp.sound.loadFromFile(SoundLocate))// nếu không tìm thấy return
	{
		std::cout << "error when load" << std::endl;
	}
	arraySound.push_back(temp); // thêm vào vector hiệu ứng âm
}


//hàm tạm dừng âm thanh
void AudioManager::PauseSound()
{
	//tạm dừng âm lượng
	vector<SoundEffect>::iterator i;
	//Vòng For quét từng phần tử trong mảng vector hiệu ứng âm thanh
	for (i = arraySound.begin(); i != arraySound.end(); i++)
	{
		
			i->soundplay.pause();// cài đặt âm sẽ phát vào biến Âm Thanh

	}

}


//hàm chỉnh âm lượng
void AudioManager::SetVolume(int ValueChange)
{

	// cài đặt âm lượng theo biến ValueChange
	vector<SoundEffect>::iterator i;
	//Vòng For quét từng phần tử trong mảng vector hiệu ứng âm thanh
	for (i = arraySound.begin(); i != arraySound.end(); i++)
	{
		i->soundplay.setVolume(ValueChange*10.0f);// cài đặt âm sẽ phát vào biến Âm Thanh

	}
}

//hàm trả về âm lượng
int AudioManager::SoundVolumeValue()
{
	// Trả về âm lượng hiện tại
	return ((int)round(this->arraySound[0].soundplay.getVolume()/10));
}

// Constructor 
AudioManager::AudioManager()
{
}

//Destructor
AudioManager::~AudioManager()
{
}
