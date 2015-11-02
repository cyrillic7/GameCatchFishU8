#ifndef TIMER_MANAGER_H_
#define TIMER_MANAGER_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
USING_NS_CC;

#define kMaxDiffrence 2000

class TimerManager : public Ref {
public:
	TimerManager();
	~TimerManager();
	static TimerManager* shareInstance();
	void purgeInstance();
	void TimerSync(DWORD client_tick, DWORD server_tick);
	DWORD GetDelayTick(DWORD packet_tick);
	DWORD GetServerTick();
	void Update(float delta_time);
	float sync_elapsed() const { return sync_elapsed_; }
	void Reset() { sync_elapsed_ = 0; }

private:

	DWORD mClientTick;
	DWORD mServerTick;;
	long long sc_diffrence_;//服务端与客户端之前的差异
	float sync_elapsed_;
};

#endif  // TIMER_MANAGER_H_
