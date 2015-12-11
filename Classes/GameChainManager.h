#ifndef _GAME_CHAIN_MANAGER_H_H_
#define _GAME_CHAIN_MANAGER_H_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
#include "GameAnimation.h"
#include "GameFishAction.h"


USING_NS_CC;

class  GameScene;

class GameChainManager : public Ref
{
public:
	GameChainManager();
	~GameChainManager();
	static GameChainManager* shareInstance();
	void purgeInstance();

	void update(float dt);
	void render();

	void chainFish(GameScene* scene,WORD chair_id,const CatchFish* fishArr,int fishcnt);
	void chainFinish(WORD chair_id);
	void chainFishArrive(WORD chair_id);
	
	void SameKindChainFish(WORD view_chair_id, const std::vector<__Dictionary*>& catch_fish,GameScene* scene);
	void SameChainFinish(WORD view_chair_id);
protected:
	GameScene* mScene;
	int  mArriveCnt[GAME_PLAYER];
	bool mChainFlag[GAME_PLAYER];
	int  mChainFishCnt[GAME_PLAYER];
	CatchFish mCatchFishs[GAME_PLAYER][kMaxChainFishCount];
	/*std::vector<__Dictionary*> same_catch_fish_[GAME_PLAYER];
	std::vector<ActionFishMove*> action_move_to_same_vector_[GAME_PLAYER];
	std::vector<GameAnimation*> same_catch_fish_animation_[GAME_PLAYER];*/
	__Array* same_catch_fish_[GAME_PLAYER];
	__Array* action_move_to_same_vector_[GAME_PLAYER];
	__Array* same_catch_fish_animation_[GAME_PLAYER];
	__Array* action_same_kind_event_finish_[GAME_PLAYER];
};


#endif//_GAME_CHAIN_MANAGER_H_H_