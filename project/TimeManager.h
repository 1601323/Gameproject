#pragma once
class TimeManager
{
private:
	bool timerFlag;
	int flameCnt;
	int timerCnt;
	//表示用関数
	int numberImage;
	int second;
	int tenex;
	int hunex;
public:
	TimeManager();
	~TimeManager();
	void StartTimer();
	void StopTimer();
	int ShowTimer();
	void Updata();
	void Draw();
};

