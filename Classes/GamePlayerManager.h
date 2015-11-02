#ifndef _GAME_PLAYER_MANAGER_H_H_
#define _GAME_PLAYER_MANAGER_H_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
#include "GameShooter.h"

class GameScene;
class  GamePlayer : public Ref{
public:
	  GamePlayer();
	~ GamePlayer();

	static GamePlayer* create();
	bool init();
	void setShooter(GameShooter* pShooter);
	GameShooter* getShooter() const {return mShooter;}
	void setAngle(float angle);
	float getAngle();
	int   getChairID() const {return mChairId;}
	void  setChairID(int id) {mChairId = id;}
	void  switchGun(BulletKind kind);
	void  fire();//发射
	void  setBulletMulriple(int power);//字弹的倍数
	int   getBulletMulriple();
	void  exchangeScore(SCORE score);
	void  AddScore(SCORE score,bool coin_effect,bool bingo_effect);
	bool  getInitUIFalg() const {return m_bInitUI;}
	void  setInitUIFalg(bool flag) { m_bInitUI = flag;}
private:
	int mChairId;//椅子ID
	BulletKind mBulletKind;//子弹种类
	GameShooter* mShooter;//发射器类
	float mTurrentAngle;//炮的旋转角度（弧度值）
	int   mBulletMulriple;//玩家子弹的倍数
	bool  m_bInitUI;// for enter a room with player, wait for 自已坐下来后再去初始化其他玩家的UI
};


class GamePlayerManager : public Ref
{
public:
	GamePlayerManager();
	~GamePlayerManager();
	static GamePlayerManager* create(GameScene* pScene);
	bool init(GameScene* pScene);
	void addPlayer(int chairId);
	void removePlayer(int chairId);
	void fire(int chairId);
	bool playerIsExist(int chairId);
	GamePlayer* getPlayerByChairId(int chairId);
	void exchangeScore(int chairId ,SCORE score);
	void AddScore(int chairId, SCORE score, bool coin_effect, bool bingo_effect);
	void isAllPalyersIsOnSit();//检查是否所有的玩家都已初始化UI
private:
	//std::map<int, GamePlayer* > mPlayerMap;
	__Array* mPlayers;
	GameScene* mGameScene;
};















#endif//_GAME_PLAYER_MANAGER_H_H_