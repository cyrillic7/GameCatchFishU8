#include "GameChainManager.h"
#include "GameScene.h"
#include "GameFishManager.h"
#include "SessionManager.h"
#include "GamePlayerManager.h"
#include "CommonFunction.h"
#include "GameCoinManager.h"

static GameChainManager* __instance = nullptr;

GameChainManager::GameChainManager()
{
	memset(mChainFlag,0,sizeof(mChainFlag));
	memset(mChainFishCnt,0,sizeof(mChainFishCnt));
	memset(mCatchFishs,0,sizeof(mCatchFishs));
	memset(mArriveCnt,0,sizeof(mArriveCnt));
	mScene = nullptr;

	for (int i = 0;i < GAME_PLAYER;i++)
	{
		same_catch_fish_[i] = __Array::create();
		CC_SAFE_RETAIN(same_catch_fish_[i]);
		action_move_to_same_vector_[i] = __Array::create();
		CC_SAFE_RETAIN(action_move_to_same_vector_[i]);
		same_catch_fish_animation_[i] = __Array::create();
		CC_SAFE_RETAIN(same_catch_fish_animation_[i]);
		action_same_kind_event_finish_[i] = __Array::create();
		CC_SAFE_RETAIN(action_same_kind_event_finish_[i]);
	}
}

GameChainManager::~GameChainManager()
{
	for (int i = 0;i < GAME_PLAYER;i++)
	{
		CC_SAFE_RELEASE_NULL(same_catch_fish_[i]);
		CC_SAFE_RELEASE_NULL(action_move_to_same_vector_[i]);
		CC_SAFE_RELEASE_NULL(same_catch_fish_animation_[i]);
		CC_SAFE_RELEASE_NULL(action_same_kind_event_finish_[i]);
	}
}

GameChainManager* GameChainManager::shareInstance()
{
	if (nullptr == __instance)
	{
		__instance = new GameChainManager();
	}
	return __instance;
}

void GameChainManager::purgeInstance()
{
	CC_SAFE_DELETE(__instance);
	__instance = nullptr;
}

void GameChainManager::chainFish(GameScene* scene , WORD chair_id,const CatchFish* fishArr,int fishcnt)
{

	mScene = scene;

	if (mChainFlag[chair_id])
	{
		chainFinish(chair_id);
		//清空上一个闪电鱼到达数的个数
		mArriveCnt[chair_id] = 0;
		//重置初始值
		mChainFishCnt[chair_id] = 0;
		mChainFlag[chair_id] = false;
	}
	mChainFlag[chair_id] = true;
	for (int i = 0; i < fishcnt;i++)
	{
		mCatchFishs[chair_id][i]= fishArr[i];
		mChainFishCnt[chair_id]++;
	}
	scene->onChainpro(chair_id,fishArr,fishcnt);
	
}

void GameChainManager::chainFinish(WORD chair_id)
{
	//隐藏闪电标志
	if(mScene)
		mScene->hideChainPro(chair_id);
	SCORE total_score = 0;
	for (int i= 0; i < mChainFishCnt[chair_id];i++)
	{
		CatchFish catchCache = static_cast<CatchFish>(mCatchFishs[chair_id][i]);
		GameFish* fish = GameFishManager::sharedInstance()->getFish(catchCache.fish_id);
		if (fish)
		{
			total_score +=catchCache.fish_score;
			fish->chainFish();
			fish->catchFish(chair_id,catchCache.fish_score);
		}
	}

	if (mScene)
	{
		mScene->addScore(chair_id,total_score,true,true);
	}
	

}

void GameChainManager::chainFishArrive(WORD chair_id)
{
	mArriveCnt[chair_id]++;
	if (mArriveCnt[chair_id] == mChainFishCnt[chair_id])
	{
		//所有鱼开始播放死亡动画
		chainFinish(chair_id);
	}
}

void GameChainManager::SameKindChainFish(WORD view_chair_id, const std::vector<__Dictionary*>& catch_fish,GameScene* scene)
{
	if (catch_fish.empty())
		return;
	__Dictionary* fishDic = catch_fish[0];
	int fishid = ((__Integer*)fishDic->objectForKey("fishId"))->getValue();
	GameFish* fish = GameFishManager::sharedInstance()->getFish(fishid);
	if (fish == nullptr)
		return;
	Vec2 center_pos = fish->action_fish()->getPosition();
	fish->set_pause(true);
	same_catch_fish_[view_chair_id]->addObject(fishDic);


	for (int i = 1; i < (int)catch_fish.size(); ++i) {
		fishid = ((__Integer*)catch_fish[i]->objectForKey("fishId"))->getValue();
		fish = GameFishManager::sharedInstance()->getFish(fishid);
		if (fish == NULL)
			continue;
		fish->set_pause(true);
		same_catch_fish_[view_chair_id]->addObject(catch_fish[i]);

		Vec2 fishPos  = fish->action_fish()->getPosition();

		float distance = CommonFunction::CalcDistance(fishPos.x,fishPos.y,center_pos.x,center_pos.y);
		float speed = distance/0.5f;
		ActionFishMoveLinear* action_move = ActionFishMoveLinear::create(speed, center_pos, fishPos);
		action_move_to_same_vector_[view_chair_id]->addObject(action_move);


		GameAnimation* chainSp = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByName("LightChain"));
		chainSp->setScale(0);
		scene->addChain(chainSp,-1);
		same_catch_fish_animation_[view_chair_id]->addObject(chainSp);

	}

	DelayTime* delay = DelayTime::create(0.5f);
	action_same_kind_event_finish_[view_chair_id]->addObject(delay);


	if(!mScene)
		mScene = scene;
}
	

void GameChainManager::SameChainFinish(WORD view_chair_id)
{
	log("SameChainFinish view_chair_Id = %d",view_chair_id);
	if (same_catch_fish_[view_chair_id]->count() <=0)
		return;

	int fish_id =-1;
	FishKind kind = FISH_WONIUYU;
	int fish_score =0;
	GameFish* fish = nullptr;
	for (int i = 1 ; i < same_catch_fish_[view_chair_id]->count(); i++ )
	{
		__Dictionary* fishDic = (__Dictionary*)same_catch_fish_[view_chair_id]->getObjectAtIndex(i);
		fish_id = ((__Integer*)fishDic->objectForKey("fishId"))->getValue();
		kind = (FishKind)((__Integer*)fishDic->objectForKey("fishKind"))->getValue();
		fish_score = atoi(((__String*)fishDic->objectForKey("fishScore"))->getCString());
		fish = GameFishManager::sharedInstance()->catchFish(view_chair_id,kind,fish_id,fish_score);
		if(fish) 
		{
			Vec2 fishPos = fish->action_fish()->getPosition();
			CoinManager::shareInstance()->BuildCoins(fishPos,fish->action_fish()->getAngle(),mScene->GetTurretPosition(view_chair_id),fish_score,kind,mScene);
		}
	}

	__Dictionary* firstFish = (__Dictionary*)same_catch_fish_[view_chair_id]->getObjectAtIndex(0);
	fish_id = ((__Integer*)firstFish->objectForKey("fishId"))->getValue();
	kind = (FishKind)((__Integer*)firstFish->objectForKey("fishKind"))->getValue();
	fish_score = atoi(((__String*)firstFish->objectForKey("fishScore"))->getCString());
	fish = GameFishManager::sharedInstance()->catchFish(view_chair_id,kind,fish_id,fish_score);
	
	//modify 10 27mScene->getPlayerManager()->AddScore(view_chair_id,fish_score,true,true);
	mScene->getPlayerManager()->AddScore(CommonFunction::SwitchViewChairID(view_chair_id),fish_score,true,true);

	same_catch_fish_[view_chair_id]->removeAllObjects();
	action_move_to_same_vector_[view_chair_id]->removeAllObjects();
	for (int k = 0 ; k <same_catch_fish_animation_[view_chair_id]->count();k++ )
	{
	GameAnimation* pChain = (GameAnimation*)same_catch_fish_animation_[view_chair_id]->getObjectAtIndex(k);
	if (pChain)
	{
		pChain->removeFromParent();
	}

	}
	same_catch_fish_animation_[view_chair_id]->removeAllObjects();
	action_same_kind_event_finish_[view_chair_id]->removeAllObjects();
}

void GameChainManager::update(float dt)
{
	int j = 0;
	int i = 0;

	for ( i = 0; i < GAME_PLAYER; ++i) {
		if (same_catch_fish_[i]->count() > 0 ) {
			for ( j = 0; j < action_move_to_same_vector_[i]->count(); j++) {
				ActionFishMove* pAction = (ActionFishMove*)action_move_to_same_vector_[i]->getObjectAtIndex(j);
				if (!pAction->isDone())
				{
					pAction->step(dt);
				}

			}
		}
	
		if (same_catch_fish_animation_[i]->count()>0)
		{
			for (j = 0; j < same_catch_fish_animation_[i]->count(); j++) {
				GameAnimation* pChain = (GameAnimation*)same_catch_fish_animation_[i]->getObjectAtIndex(j);
				pChain->update(dt);
			}
				
		}

		if (action_same_kind_event_finish_[i]->count() > 0 )
		{
			for (j = 0; j < action_same_kind_event_finish_[i]->count(); j++) {
				DelayTime* pDelay = (DelayTime*)action_same_kind_event_finish_[i]->getObjectAtIndex(j);
				if(pDelay && !pDelay->isDone())
					pDelay->step(dt);
				else
				{
					SameChainFinish(i);
				}
			}
		}
	}
}

void GameChainManager::render()
{
	for (int i = 0; i < GAME_PLAYER; ++i) {
		if (same_catch_fish_[i]->count() <=0)
			continue;
		__Dictionary* fishDic = (__Dictionary*)same_catch_fish_[i]->getObjectAtIndex(0);
		int fishId = ((__Integer*)fishDic->objectForKey("fishId"))->getValue();
		GameFish* fish = GameFishManager::sharedInstance()->getFish(fishId);
		if (fish == NULL)
			continue;
		Vec2 src_pos = fish->action_fish()->getPosition();
		for (int j = 1; j < (int)same_catch_fish_[i]->count(); ++j) {
			ActionFishMove* pAction = (ActionFishMove*)action_move_to_same_vector_[i]->getObjectAtIndex(j-1);
			Vec2 pos = pAction->getPosition();

			GameAnimation* pChainSp = (GameAnimation*)same_catch_fish_animation_[i]->getObjectAtIndex(j-1);
			
			Vec2 delta;
			Vec2::subtract(src_pos,pos,&delta);

			float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			float rote = 0.f;
			float scale = 1.f;
			float angle = 0.f;
			if (length > 0) {
				if (delta.y > 0) {
					rote = -std::acos(delta.x / length);
				} else {
					rote = std::acos(delta.x / length);
				}
				scale = length / pChainSp->getFrameWidth();
				angle = CommonFunction::CalcAngle(pos.x,pos.y,src_pos.x,src_pos.y);
			}
			delta.x = src_pos.x + (length / 2) * std::sin(angle);
			delta.y = src_pos.y + (length / 2) * std::cos(angle);
			pChainSp->render(Vec2(delta.x, delta.y), CC_RADIANS_TO_DEGREES(rote), scale);

			if (pAction->isDone())
			{
				pChainSp->setVisible(false);
			}
		}
	}
}

