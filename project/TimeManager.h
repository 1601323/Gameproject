#pragma once
class TimeManager
{
private:
	bool timerFlag;
	int flameCnt;
	int timerCnt;
public:
	TimeManager();
	~TimeManager();
	void StartTimer();
	void StopTimer();
	int ShowTimer();
	void Updata();
};

