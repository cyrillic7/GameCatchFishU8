#include "GameTimerManager.h"
#include "SessionManager.h"

static TimerManager* __instance = nullptr;

TimerManager::TimerManager()
{
	mServerTick = SessionManager::shareInstance()->getCurrentTime();
	mClientTick = SessionManager::shareInstance()->getCurrentTime();
	sync_elapsed_ = 0;
	sc_diffrence_ = 0;
}

TimerManager::~TimerManager()
{

}

TimerManager *TimerManager::shareInstance()
{
	if (__instance == nullptr)
	{
		__instance = new TimerManager;
	}
	return __instance;
}

void TimerManager::purgeInstance()
{
	CC_SAFE_DELETE(__instance);
}

VOID TimerManager::TimerSync(DWORD client_tick, DWORD server_tick)
{
	DWORD now_tick = SessionManager::shareInstance()->getCurrentTime();
	DWORD trade = (now_tick - client_tick) / 2;
	mServerTick = server_tick + trade;
	sc_diffrence_ = mServerTick - now_tick;
}

DWORD TimerManager::GetDelayTick(DWORD packet_tick) {
	DWORD delay = GetServerTick() - packet_tick;
	if (delay >= kMaxDiffrence)
		delay = kMaxDiffrence;
	return delay;
}

DWORD TimerManager::GetServerTick() {
	return SessionManager::shareInstance()->getCurrentTime() + (int)sc_diffrence_;
}

void TimerManager::Update(float delta_time) {
	sync_elapsed_ += delta_time;
}


