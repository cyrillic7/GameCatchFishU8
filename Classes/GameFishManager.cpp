#include"GameFishManager.h"
#include "GameResourcesManager.h"
#include "CommonFunction.h"
#include "SessionManager.h"
#include "GameScene.h"
#include "SoundManager.h"

static GameFishManager* __instance = nullptr;

GameFish::GameFish()
{
	mFishAction = nullptr;
	mFishwalking =nullptr;
	mFishDead =  nullptr;
	mFishEndPos = Vec2(0,0);
	mChainAnimation = nullptr;
	mScale = 0.f;
	mScene = nullptr;
	mGreenLight = nullptr;
	mViewChairId = -1;
	mIsArrive = false;
	mScoreLable = nullptr;
	mActionScore = nullptr;
	mCircleSp = nullptr;
	mKindJc20Sprite = nullptr;
}

GameFish::~GameFish()
{
	if(mFishAction)
		CC_SAFE_RELEASE_NULL(mFishAction);
	if(mFishwalking)
		CC_SAFE_RELEASE_NULL(mFishwalking);
	if (mFishDead)
		CC_SAFE_RELEASE_NULL(mFishDead);
	if (mChainAnimation)
		CC_SAFE_RELEASE_NULL(mChainAnimation);
	if (mActionScore)
		CC_SAFE_RELEASE_NULL(mActionScore);
	if (mScoreLable)
		CC_SAFE_RELEASE_NULL(mScoreLable);
	if (mKindJc20Sprite)
	{
		CC_SAFE_RELEASE_NULL(mKindJc20Sprite);
	}
	if (mCircleSp)
	{
		mCircleSp->removeFromParent();
		mCircleSp = nullptr;
	}
}

GameFish* GameFish::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFish* fish = new GameFish();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{
		fish->autorelease();
		return fish;
	}
    CC_SAFE_DELETE(fish);
    return nullptr;
	
}


bool GameFish::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	
	mFishKind = fish_kind;
	mFishId = fish_id;
	mTag = tag;
	mBoundingRadius = bounding_radius;
	mBoundingCount = bounding_count;
	mFishAction = action;
	CC_SAFE_RETAIN(mFishAction);

	AnimationInfo* walk_info = ResourcesManager::getInstance()->getAnimationInfoByKind(mFishKind);
	if(SessionManager::shareInstance()->getGameKind() == GameKindJcpy && mFishKind == FISH_KIND_JC20)
	{
		createJcSprite();
	}
	else
	{
		if(walk_info)
		{
			mFishwalking = GameAnimation::create(walk_info);
			if(nullptr != mFishwalking)
			{
				mFishwalking->setPosition(mFishAction->getPosition());
				addChild(mFishwalking);
				CC_SAFE_RETAIN(mFishwalking);
				mFishwalking->play();
			}
		}
	}
	

	//dead
	if (mFishKind == FISH_KIND_JC18)
	{
		int j = 0;
	}
	AnimationInfo* dead_info = ResourcesManager::getInstance()->getAnimationInfoByKind(mFishKind,false);
	if (dead_info && fish_kind != FISH_BGLU)
	{
		mFishDead = GameAnimation::create(dead_info);
		addChild(mFishDead,5);
		mFishDead->setVisible(false);
		CC_SAFE_RETAIN(mFishDead);
	}
	else
	{
		//add by 8 -13
		//如果没有死亡动画用行走动画替换，只播放一次
		if(walk_info)
		{
			mFishDead = GameAnimation::create(walk_info);
			mFishDead->setLoop(false);
			mFishDead->setVisible(false);
			CC_SAFE_RETAIN(mFishDead);
		}
	}
	
	mStatus =  MOVE;
	mBpause = false;

	if(SessionManager::shareInstance()->getGameKind() == GameKindDntg)
	{
		if(mFishwalking)
		{
			GameAnimation* greenLight = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_CHAIN));
			float mScaleX10 = MAX(mFishwalking->getFrameWidth(),mFishwalking->getFrameHeight())*10.f/greenLight->getFrameWidth();
			mScale = mScaleX10/10.0f;
		}
	}

	return true;

}

void GameFish::createJcSprite()
{
	mKindJc20Sprite = Sprite::create();
	mKindJc20Sprite->setPosition(mFishAction->getPosition());
	mKindJc20Sprite->retain();
	addChild(mKindJc20Sprite);
	__String* pathStr = String::createWithFormat("GameResources/jcpy/fish/%s.png",__String::createWithFormat("%s_walking","fish20")->getCString());

	Sprite* fishImg = Sprite::create(pathStr->getCString());
	int row = fishImg->getContentSize().width/256;
	int col = fishImg->getContentSize().height/256;

	Vector<SpriteFrame*> frames;
	SpriteFrame* frame = nullptr;
	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			if(i*row+j < 3 )
			{

				frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(j*256,i*256,256,256));
				if(i==0 && j==0)
				{
					mKindJc20Sprite->setSpriteFrame(frame);
				}
				if (NULL != frame) {
					frames.pushBack(frame);
				}
			}
		}
	}
	Animation* pAction = Animation::createWithSpriteFrames(frames,0.25f);
	Animate* pAmate = Animate::create(pAction);


	mKindJc20Sprite->runAction(RepeatForever::create(pAmate));
}

void GameFish::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		//if (!mBpause || mStatus == CHAIN_LOCK)
		
		if (!mBpause)
			mFishAction->step(delta_time);
		if(mFishwalking)
			mFishwalking->update(delta_time);

		//闪电链
		if (mChainAnimation)
			mChainAnimation->update(delta_time);

		if (mFishAction->isDone())
		{
			if(mStatus == MOVE)
				mStatus = DESTROY;
			else if (mStatus == CHAIN_LOCK && mChainAnimation)
			{
				if(!mIsArrive)
				{
					mIsArrive = true;
					mChainAnimation->removeFromParentAndCleanup(true);
					mChainAnimation = nullptr;

				
					mCircleSp->removeFromParent();
					mCircleSp = nullptr;
					
					RotateBy* rb = RotateBy::create(0.1f,15);
					mFishwalking->runAction(RepeatForever::create(rb));

					if (mGreenLight)
					{
						mGreenLight->reset(ResourcesManager::getInstance()->getAnimationInfoByName("BlueLight"));
						mGreenLight->play();

						RotateBy* rb2 = RotateBy::create(0.1f,15);
						mGreenLight->runAction(RepeatForever::create(rb2));
					}


					//发送消息
					__Dictionary* dic = __Dictionary::create();
					dic->setObject(__Integer::create(mViewChairId),"chairId");
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(FishArriveMsg,dic);
				}
				

			}
		}

		//green light
		if (mGreenLight)
		{
			mGreenLight->update(delta_time);
			mScaleAction->step(delta_time);
		}

	} else if (mStatus == DEAD) {
			//播放死亡动画，
			if(mFishDead)
			{
				mFishDead->update(delta_time);
			}
			//产生数字
			if(mActionScore && mScoreLable) 
			{
				if (!mActionScore->isDone())
				{
					mActionScore->step(delta_time);
				}
				else
				{
					DelayTime* delay = DelayTime::create(0.5f);
					FadeOut*   fo    = FadeOut::create(0.5f);
					Spawn* span = Spawn::createWithTwoActions(delay,fo);
					CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFish::removeScore));
					mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
				}
			}
			else//闪电连起来的鱼特别处理
			{
				DelayTime* delay = DelayTime::create(1.5f);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFish::changeState));
				runAction(Sequence::createWithTwoActions(delay,call));
			}
	}
	
}

void GameFish::changeState(Node* pSender)
{
	mStatus = DESTROY;
}

void GameFish::Render()
{
	int i = 0 ;
	//金蝉专有
	if ( mStatus == MOVE || mStatus == CHAIN_LOCK)
	{
		if (mKindJc20Sprite)
		{
			mKindJc20Sprite->setPosition(mFishAction->getPosition());
			mKindJc20Sprite->setRotation(CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		}
	}

	if(mFishwalking && (mStatus == MOVE || mStatus == CHAIN_LOCK))
	{
		
		mFishwalking->render(mFishAction->getPosition(),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		
		//当前鱼的位置
		Vec2 pos = mFishwalking->getPosition();
		Vec2 delta;
		float angle = 0.0f;
		if(mChainAnimation)
		{
			Vec2::subtract(mChainStartPos,pos,&delta);
			float chain_len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			float rote_angle = 0.f;
			float scale = 1.f;
			if (chain_len > 0) {
				if (delta.y > 0) {
					rote_angle = -std::acos(delta.x / chain_len);
				} else {
					rote_angle = std::acos(delta.x / chain_len);
				}
				angle =  CommonFunction::CalcAngle(pos.x,pos.y,mChainStartPos.x,mChainStartPos.y);
				scale = chain_len / mChainAnimation->getFrameWidth();
			}
			delta.x = mChainStartPos.x + (chain_len / 2) * std::sin(angle);
			delta.y = mChainStartPos.y + (chain_len / 2) * std::cos(angle);

			mChainAnimation->render(delta,CC_RADIANS_TO_DEGREES(rote_angle),scale);
		}
		if (mGreenLight)
		{
			Vec2 green_scale = mScaleAction->getCurScale();
			mGreenLight->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),green_scale.x);
			//
			if (mCircleSp)
			{
				Vec2::subtract(mChainStartPos,pos,&delta);
				float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
				angle =  CommonFunction::CalcAngle(pos.x,pos.y,mChainStartPos.x,mChainStartPos.y);
				Vec2 circle_pos;
				circle_pos.x = mChainStartPos.x + (len - (mGreenLight->getContentSize().width/2 - mCircleSp->getContentSize().width/2)*green_scale.x ) * std::sin(angle);
				circle_pos.y = mChainStartPos.y + (len - (mGreenLight->getContentSize().width/2 - mCircleSp->getContentSize().width/2)*green_scale.x) * std::cos(angle);
				mCircleSp->setPosition(circle_pos);
				mCircleSp->setScale(green_scale.x);
				mCircleSp->setRotation(CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
			}

		}

	}
	else if(mStatus == DEAD && mFishDead)
	{
		if (mFishwalking && mFishwalking->isVisible())
			mFishwalking->setVisible(false);
		if(mFishDead)
		{
			//在调整位置的时候设为可见
			if(!mFishDead->isVisible())
				mFishDead->setVisible(true);
			mFishDead->render(mFishAction->getPosition(),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		}
		
		RenderScoreNum();

	}
}

 void GameFish::RenderScoreNum()
 {
	 if(mScoreLable)
		 mScoreLable->setPosition(mActionScore->position());
 }


bool GameFish::BulletHitTest(GameBullet* bullet)
{
	if (mStatus != MOVE)
		return false;
	
	Vec2 bullet_pos = bullet->position();

	float center_distance = mBoundingRadius + bullet->getBulletRadius();

	float distance;
	Vec2 pos = mFishAction->getPosition();
	float offset = 0;
	if (mBoundingCount > 1)
		offset = mBoundingCount * mBoundingRadius;
	float center_x, center_y;
	float angle = mFishAction->getAngle();

	for (int i = 0; i < mBoundingCount; ++i) {
		center_x = pos.x + offset * sinf(angle);
		center_y = pos.y + offset * cosf(angle);
		distance = CommonFunction::CalcDistance(bullet_pos.x, bullet_pos.y, center_x, center_y);
		if (distance <= center_distance) {
			return true;
		}
		offset -= mBoundingRadius * 2;
	}

	return false;
}

void GameFish::catchFish(WORD view_chair_id, SCORE score)
{

	int angle = 0; 
	if (view_chair_id<3)
		angle +=M_PI;

	if (mStatus == CHAIN_LOCK)
	{
		if(mGreenLight)
		{
			mGreenLight->removeFromParentAndCleanup(true);
			mGreenLight = nullptr;
		}
	}
	else
	{
		mScoreLable = LabelAtlas::create("0","common/players/catch_num.png",44,64,'0');
		mScoreLable->setString(String::createWithFormat("%lld",score)->getCString());
		Vec2 pos;
		Vec2::add(mFishAction->getPosition(),Vec2(-mScoreLable->getContentSize().width/2,cos(angle)*mScoreLable->getContentSize().height/2),&pos);
		mScoreLable->setPosition(pos);
		if (view_chair_id < 3)
		{
			mScoreLable->setRotation(180);
		}
		mScoreLable->setLocalZOrder(100);
		addChild(mScoreLable);
		mScoreLable->retain();

		mActionScore = ActionScore::create(pos,view_chair_id,String::createWithFormat("%lld",score)->getCString());
		mActionScore->retain();

	}

	mStatus = DEAD;
	//位置不对，下一帧才调整过来
	if(mFishDead)
	{
		//特别注意，在子类的设置可见
		mFishDead->play();
	}

	//播放死亡音效
	//SoundManager::playMusicEffect(CommonFunction::getEffectName("fishdiescore.mp3").c_str());
	SoundManager::playFishEffect("fishdiescore.mp3");
}

void GameFish::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

//闪电处理
void GameFish::onChainPro(GameScene* scene,Vec2 chain_start_pos,Vec2 fish_end_pos,WORD view_chair_id,int fishIndex)
{
	mViewChairId = view_chair_id;

	mScene = scene;

	mChainStartPos = chain_start_pos;
	
	mFishEndPos = fish_end_pos;

	Vector<FiniteTimeAction*> actions;
	DelayTime* delay = DelayTime::create(2.0f*fishIndex);
	actions.pushBack(delay);
	CallFuncN* call = CallFuncN::create(this,callfuncN_selector(GameFish::onStartScale));
	actions.pushBack(call);
	
	runAction(Sequence::create(actions));
}

void GameFish::onStartScale(Node* pSender)
{
	Vec2 pos = mFishAction->getPosition();

	mGreenLight = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_CHAIN));
	mGreenLight->setPosition(pos);
	mGreenLight->setScale(2.0f);
	mScene->addChain(mGreenLight,mViewChairId); 

	mScaleAction  = ActionScaleTo::create(0.5f,2.0f,2.0f,mScale);
	mScaleAction->retain();
	mScaleAction->start();

	Vec2 delta;
	Vec2::subtract(pos,mChainStartPos,&delta);
	float chain_len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	float rote_angle = 0.f;
	float scale = 1.f;
	float angle = 0.0f;

	mChainAnimation = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByName("LightChain"));
	mChainAnimation->retain();

	if (chain_len > 0) {
		if (delta.y > 0) {
			rote_angle = -std::acos(delta.x / chain_len);
		} else {
			rote_angle = std::acos(delta.x / chain_len);
		}
		angle = CommonFunction::CalcAngle(pos.x,pos.y,mChainStartPos.x,mChainStartPos.y);
		scale = chain_len / mChainAnimation->getFrameWidth();
	}

	delta.x = mChainStartPos.x + (chain_len / 2) * std::sin(angle);
	delta.y = mChainStartPos.y + (chain_len / 2) * std::cos(angle);
	mChainAnimation->render(delta,CC_RADIANS_TO_DEGREES(rote_angle),scale);
	mChainAnimation->play();//动画先播放起来

	
	mScene->addChain(mChainAnimation,mViewChairId);


	Vec2 cicle_pos ;
	cicle_pos.x = mChainStartPos.x + (chain_len - mGreenLight->getContentSize().width/2 ) * std::sin(angle);
	cicle_pos.y = mChainStartPos.y + (chain_len - mGreenLight->getContentSize().width/2 ) * std::cos(angle);
	mCircleSp = Sprite::create("dntg2/images/fish/ScoreCircle.png");
	//circleSp->setScale(2.0f);
	mCircleSp->setPosition(Vec2(cicle_pos.x,cicle_pos.y));

	mScene->addChild(mCircleSp);

	DelayTime* delay = DelayTime::create(0.2f);
	CallFuncN* call = CallFuncN::create(this,callfuncN_selector(GameFish::onReadyToMov));
	

	runAction(Sequence::createWithTwoActions(delay,call));
}

void GameFish::chainFish()
{
	stopAllActions();
	if (mGreenLight)
	{
		mGreenLight->setVisible(false);
	}
	if (mChainAnimation)
	{
		mChainAnimation->setVisible(false);
	}
}

void GameFish::onReadyToMov(Node* pSender)
{
	Vec2 pos = mFishAction->getPosition();

	float distance = std::sqrt((mFishEndPos.x - pos.x) * (mFishEndPos.x - pos.x) + (mFishEndPos.y - pos.y) * (mFishEndPos.y - pos.y));
	float speed = distance/1.0f;

	ActionFishMove* action = ActionFishMoveLinear::create(speed,pos,mFishEndPos);
	set_action_fish(action);

	//开始向目标靠近
	//mStatus = CHAIN_LOCK;
	set_pause(false);
}

//重新设置鱼的行走路线对象
void GameFish::set_action_fish(ActionFishMove* action)
{
	if (mFishAction)
	{
		CC_SAFE_RELEASE_NULL(mFishAction);
	}
	mFishAction = action;
	mFishAction->retain();
}

//重新设置鱼的游走动画对象
void GameFish::set_action_walk(GameAnimation* walk)
{
	if (mFishwalking)
	{
		CC_SAFE_RELEASE_NULL(mFishwalking);
	}
	mFishwalking = walk;
	mFishwalking->retain();
}


Vec2 GameFish::getEndPos()
{
	return mFishEndPos;
}
//////////////////////////////////////////////////////////////////////////特殊鱼类
//神仙船类
GameFishShip::GameFishShip()
{
	mAnimationDic = __Dictionary::create();
	mDeadAnimationDic = __Dictionary::create();
	mAnimationDic->retain();
	mDeadAnimationDic->retain();
}

GameFishShip::~GameFishShip()
{
	CC_SAFE_RELEASE_NULL(mAnimationDic);
	CC_SAFE_RELEASE_NULL(mDeadAnimationDic);
}

GameFishShip* GameFishShip::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishShip* fish = new GameFishShip();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishShip::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	float kScale = 0.5f;
	//初始化自己特有的精灵 
	GameAnimation* pAnima = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_JINSHA));
	pAnima->setScale(kScale);
	addChild(pAnima);
	mAnimationDic->setObject(pAnima,"js");
	pAnima =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_BIANFUYU));
	pAnima->setScale(kScale);
	addChild(pAnima);                           
	mAnimationDic->setObject(pAnima,"bf");
	pAnima =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_KONGQUEYU));
	pAnima->setScale(kScale);
	addChild(pAnima);
	mAnimationDic->setObject(pAnima,"kq1");

	pAnima =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_KONGQUEYU));
	pAnima->setScale(kScale);
	addChild(pAnima);
	mAnimationDic->setObject(pAnima,"kq2");



	DictElement* pElement = NULL;
	CCDICT_FOREACH(mAnimationDic,pElement)
	{
		GameAnimation* pAnima = (GameAnimation*)pElement->getObject();
		pAnima->play();
	}

	//各种死亡动画
	pAnima = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_JINSHA,false));
	pAnima->setScale(kScale);
	pAnima->setVisible(false);
	addChild(pAnima);
	mDeadAnimationDic->setObject(pAnima,"js");
	pAnima =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_BIANFUYU,false));
	pAnima->setScale(kScale);
	pAnima->setVisible(false);
	addChild(pAnima);                           
	mDeadAnimationDic->setObject(pAnima,"bf");
	pAnima =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_KONGQUEYU,false));
	pAnima->setScale(kScale);
	pAnima->setVisible(false);
	addChild(pAnima);
	mDeadAnimationDic->setObject(pAnima,"kq1");

	pAnima =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_KONGQUEYU,false));
	pAnima->setScale(kScale);
	pAnima->setVisible(false);
	addChild(pAnima);
	mDeadAnimationDic->setObject(pAnima,"kq2");
	return true;

}

void GameFishShip::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		mFishwalking->update(delta_time);

		DictElement* pElement = NULL;
		CCDICT_FOREACH(mAnimationDic,pElement)
		{
			GameAnimation* pAnima = (GameAnimation*)pElement->getObject();
			pAnima->update(delta_time);
		}
		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		//船的划动动画照样播放
		mFishwalking->update(delta_time);
		bool isCanDestory = true;
		DictElement* pElement = NULL;
		CCDICT_FOREACH(mDeadAnimationDic,pElement)
		{
			GameAnimation* pAnima = (GameAnimation*)pElement->getObject();
			pAnima->update(delta_time);
			if (pAnima->IsPlaying())
			{
				isCanDestory = false;
			}
			if (!pAnima->isVisible())
			{
				pAnima->setVisible(true);
			}
		}

		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				//所有子鱼的死亡动画播放完成
				if(isCanDestory)
				{
					DelayTime* delay = DelayTime::create(0.5f);
					FadeOut*   fo    = FadeOut::create(0.5f);
					Spawn* span = Spawn::createWithTwoActions(delay,fo);
					CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishShip::removeScore));
					mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
				}
			
			}
		}
	}

}

void GameFishShip::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishShip::Render()
{
	__Dictionary* dic  = nullptr;
	Vec2 pos = mFishAction->getPosition();
	float tempAngle = CC_RADIANS_TO_DEGREES(mFishAction->getAngle());
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		dic = mAnimationDic;
	}else if (mStatus == DEAD)
	{
		dic = mDeadAnimationDic;
	}
	if(mStatus == MOVE || mStatus == CHAIN_LOCK || mStatus == DEAD)
	{
		mFishwalking->render(mFishAction->getPosition(),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		float radius = mFishwalking->getFrameWidth() / 2;
		const float kScale =0.5;
		GameAnimation* jiasha = (GameAnimation*)dic->objectForKey("js");
		GameAnimation* bianfuyu = (GameAnimation*)dic->objectForKey("bf");
		GameAnimation* kongqueyu1 = (GameAnimation*)dic->objectForKey("kq1");
		GameAnimation* kongqueyu2 = (GameAnimation*)dic->objectForKey("kq2");

		float radius1 =jiasha->getFrameWidth()* kScale / 2;
		float radius2 = bianfuyu->getFrameWidth()* kScale / 2;
		float radius3 = kongqueyu1->getFrameWidth() * kScale / 2;
		float radius4 =kongqueyu1->getFrameHeight() * kScale;
		float offset = radius - radius1;
		float center_x, center_y;
		float angle = mFishAction->getAngle();
		center_x = pos.x + offset * cosf(angle);
		center_y = pos.y - offset * sinf(angle);
		jiasha->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),kScale);
		
		offset -= radius1 + radius2;
		center_x = pos.x + offset * cosf(angle);
		center_y = pos.y - offset * sinf(angle);
		bianfuyu->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),kScale);
		offset -= radius2 + radius3;
		center_x = pos.x + offset * cosf(angle);
		center_y = pos.y - offset * sinf(angle);
		kongqueyu1->render(Vec2(center_x + cosf(angle - M_PI_2) * radius4, center_y - sinf(angle - M_PI_2) * radius4), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),kScale);
		kongqueyu2->render(Vec2(center_x + cosf(angle + M_PI_2) * radius4, center_y - sinf(angle + M_PI_2) * radius4), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),kScale);
	}

	if (mStatus == DEAD)
		RenderScoreNum();
}

void GameFishShip::catchFish(WORD view_chair_id, SCORE score)
{
	GameFish::catchFish(view_chair_id,score);
	DictElement* pElement = NULL;
	CCDICT_FOREACH(mDeadAnimationDic,pElement)
	{
		GameAnimation* pAnima = (GameAnimation*)pElement->getObject();
		//pAnima->setVisible(true);
		pAnima->play();
	}
	CCDICT_FOREACH(mAnimationDic,pElement)
	{
		GameAnimation* pAnima = (GameAnimation*)pElement->getObject();
		pAnima->setVisible(false);
	}

	//SoundManager::playMusicEffect(CommonFunction::getEffectName("fishdiescore.mp3").c_str());
	SoundManager::playFishEffect("fishdiescore.mp3");
}

//////////////////////////////////////////////////////////////////////////

GameFishYuqun::GameFishYuqun()
{
	mAnimationArray = __Array::create();
	mDeadAnimationArray = __Array::create();
	mAnimationArray->retain();
	mDeadAnimationArray->retain();
}

GameFishYuqun::~GameFishYuqun()
{
	CC_SAFE_RELEASE_NULL(mAnimationArray);
	CC_SAFE_RELEASE_NULL(mDeadAnimationArray);
}

GameFishYuqun* GameFishYuqun::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishYuqun* fish = new GameFishYuqun();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishYuqun::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	for (int i=0; i<4 ;i++)
	{
		Sprite* fishBg = Sprite::create("dntg2/images/fish/yuqunquan.png");
		mRadius = fishBg->getContentSize().width;
		fishBg->runAction(RepeatForever::create(RotateBy::create(0.1f,-45)));
		//for 初始位置
		fishBg->setPosition(action->getPosition());
		addChild(fishBg,0,YUQUNQUANBGTAG+i);

		GameAnimation* pAnima = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag));
		fishBg->setLocalZOrder(pAnima->getLocalZOrder()-1);
		pAnima->setPosition(action->getPosition());
		addChild(pAnima);

		pAnima->play();
		mAnimationArray->addObject(pAnima);


		GameAnimation* pDeadAnima = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag,false));
		addChild(pDeadAnima);
		pDeadAnima->setVisible(false);
		mDeadAnimationArray->addObject(pDeadAnima);

	}
	return true;

}

void GameFishYuqun::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);

		cocos2d::Ref* pElement = NULL;
		CCARRAY_FOREACH(mAnimationArray,pElement)
		{
			GameAnimation* pAnima = (GameAnimation*)pElement;
			pAnima->update(delta_time);
		}
		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		GameAnimation* pAciton = nullptr;
		bool isCanDestory = true;
		for (int i =0 ; i < 4; i++)
		{
			pAciton = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			if(pAciton->isVisible())
				pAciton->setVisible(false);
			pAciton = (GameAnimation*)mDeadAnimationArray->getObjectAtIndex(i);
			if(!pAciton->isVisible())
			{
				pAciton->setVisible(true);
				pAciton->play();
			}
			//死亡动画是否播放完
			if (pAciton->IsPlaying())
			{
				isCanDestory = false;
			}
			pAciton->update(delta_time);
		}

		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				if(isCanDestory)
				{
					DelayTime* delay = DelayTime::create(0.5f);
					FadeOut*   fo    = FadeOut::create(0.5f);
					Spawn* span = Spawn::createWithTwoActions(delay,fo);
					CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishYuqun::removeScore));
					mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
				}
			}
		}

		
	}
}

void GameFishYuqun::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishYuqun::Render()
{
	__Array* cur = mAnimationArray;
	if (mStatus == DEAD)
	{
		cur = mDeadAnimationArray;
	}

	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK || mStatus == DEAD) {
		float radius = mRadius / 2;
		float offset = radius * 4;
		float center_x, center_y;
		float angle = mFishAction->getAngle() + M_PI_2;
		for (int i = 0; i < 4; ++i) {
			GameAnimation* pAnima = (GameAnimation*)cur->getObjectAtIndex(i);
			center_x = pos.x + offset * cosf(angle);
			center_y = pos.y - offset * sinf(angle);
			pAnima->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));

			Sprite* bg = (Sprite*)getChildByTag(YUQUNQUANBGTAG+i);
			bg->setPosition(Vec2(center_x, center_y));
			offset -= radius * 2;
		}
	}

	if (mStatus == DEAD)
	{
		RenderScoreNum();
	}
}

bool GameFishYuqun::BulletHitTest(GameBullet* bullet) {
	if (mStatus != MOVE)
		return false;

	Vec2 bullet_pos = bullet->getPosition();
	float center_distance = mBoundingRadius + bullet->getBulletRadius();
	float distance;
	Vec2 pos = mFishAction->getPosition();
	float offset = 0;
	if (mBoundingCount > 1)
		offset = mBoundingCount * mBoundingRadius;
	float center_x, center_y;
	float angle = mFishAction->getAngle() + M_PI_2;
	for (int i = 0; i < mBoundingCount; ++i) {
		center_x = pos.x + offset * cosf(angle);
		center_y = pos.y + offset * sinf(angle);
		distance = CommonFunction::CalcDistance(bullet_pos.x, bullet_pos.y, center_x, center_y);
		if (distance <= center_distance) {
			return true;
		}
		offset -= mBoundingRadius * 2;
	}

	return false;
}



//////////////////////////////////////////////////////////////////////////
//大闹天宫
GameFishDntg::GameFishDntg()
{
	mInnerFishWalking = nullptr;
	mInnerFishDead  = nullptr;
}

GameFishDntg::~GameFishDntg()
{
	if(mInnerFishWalking)
		CC_SAFE_RELEASE_NULL(mInnerFishWalking);
	if(mInnerFishDead)
		CC_SAFE_RELEASE_NULL(mInnerFishDead);
}

GameFishDntg* GameFishDntg::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishDntg* fish = new GameFishDntg();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishDntg::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	Sprite* fishBg = Sprite::create("dntg2/images/fish/dntgquan.png");
	fishBg->runAction(RepeatForever::create(RotateBy::create(0.15f,-30)));
	fishBg->setPosition(action->getPosition());
	addChild(fishBg,0,YUQUNQUANBGTAG);

	//内部鱼的游走动画对象
	mInnerFishWalking = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag));
	fishBg->setLocalZOrder(mInnerFishWalking->getLocalZOrder()-1);
	//for 初始化位置不对
	mInnerFishWalking->setPosition(action->getPosition());

	addChild(mInnerFishWalking);

	CC_SAFE_RETAIN(mInnerFishWalking);
	mInnerFishWalking->play();

	AnimationInfo* dead_info = ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag,false);
	if (dead_info)
	{
		mInnerFishDead = GameAnimation::create(dead_info);
	}
	else
	{
		mInnerFishDead = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag));
	}
	addChild(mInnerFishDead);
	mInnerFishDead->setVisible(false);
	CC_SAFE_RETAIN(mInnerFishDead);

	return true;
}

void GameFishDntg::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		mInnerFishWalking->update(delta_time);

		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		if (mInnerFishDead->isVisible())
		{
			mInnerFishDead->setVisible(true);
			mInnerFishDead->play();
			mInnerFishWalking->setVisible(false);
		}
		mInnerFishDead->update(delta_time);
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				DelayTime* delay = DelayTime::create(0.5f);
				FadeOut*   fo    = FadeOut::create(0.5f);
				Spawn* span = Spawn::createWithTwoActions(delay,fo);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishDntg::removeScore));
				mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
			}
		}
	}
}

void GameFishDntg::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishDntg::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		Sprite* bgQuan = (Sprite*)getChildByTag(YUQUNQUANBGTAG);
		bgQuan->setPosition(pos);
		mInnerFishWalking->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
	}else if(mStatus == DEAD)
	{
		mInnerFishDead->render(pos,mFishAction->getAngle());
		RenderScoreNum();
	}
}

//////////////////////////////////////////////////////////////////////////
//一箭双雕
GameFishYjsd::GameFishYjsd()
{
	mAnimationArray = __Array::create();
	mDeadArray = __Array::create();
	mAnimationArray->retain();
	mDeadArray->retain();
	mRadius =0;
}

GameFishYjsd::~GameFishYjsd()
{
	CC_SAFE_RELEASE_NULL(mAnimationArray);
	CC_SAFE_RELEASE_NULL(mDeadArray);
}

GameFishYjsd* GameFishYjsd::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishYjsd* fish = new GameFishYjsd();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishYjsd::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	Sprite* fishBg = Sprite::create("dntg2/images/fish/yjsdquan.png");
	fishBg->runAction(RepeatForever::create(RotateBy::create(0.15f,-30)));
	mRadius = fishBg->getContentSize().width;
	fishBg->setPosition(action->getPosition());
	addChild(fishBg,0,YUQUNQUANBGTAG);

	GameAnimation* fish1_walk = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kYjsdSubFish[tag][0]));
	fishBg->setLocalZOrder(fish1_walk->getLocalZOrder()-1);
	//for 初始化位置不对
	fish1_walk->setPosition(action->getPosition());
	addChild(fish1_walk);
	mAnimationArray->addObject(fish1_walk);

	GameAnimation* fish2_walk = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kYjsdSubFish[tag][1]));
	//for 初始化位置不对
	fish2_walk->setPosition(action->getPosition());
	addChild(fish2_walk);
	mAnimationArray->addObject(fish2_walk);

	for (int i =0; i < mAnimationArray->count();i++)
	{
		GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
		pAction->play();
	}

	GameAnimation* dead_anima1 =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kYjsdSubFish[tag][0],false));
	dead_anima1->setVisible(false);
	addChild(dead_anima1);
	mDeadArray->addObject(dead_anima1);

	GameAnimation* dead_anima2 =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kYjsdSubFish[tag][1],false));
	dead_anima2->setVisible(false);
	addChild(dead_anima2);
	mDeadArray->addObject(dead_anima2);
	return true;

}

void GameFishYjsd::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		//子鱼的游走动画刷新
		for (int i =0; i < mAnimationArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->update(delta_time);
		}

		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		int i =0;
		GameAnimation* pAction = nullptr;
		for (i ;i < mAnimationArray->count();i++)
		{
			pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			if (pAction->isVisible())
			{
				pAction->setVisible(false);
			}
		}

		for (i=0 ;i < mDeadArray->count();i++)
		{
			pAction = (GameAnimation*)mDeadArray->getObjectAtIndex(i);
			if (!pAction->isVisible())
			{
				pAction->setVisible(true);
				pAction->play();
			}
			pAction->update(delta_time);
		}
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				DelayTime* delay = DelayTime::create(0.5f);
				FadeOut*   fo    = FadeOut::create(0.5f);
				Spawn* span = Spawn::createWithTwoActions(delay,fo);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishYjsd::removeScore));
				mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
			}
		}
	}
}

void GameFishYjsd::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishYjsd::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		Sprite* bgQuan = (Sprite*)getChildByTag(YUQUNQUANBGTAG);
		bgQuan->setPosition(pos);
		//每条鱼的位置刷新
		float radius = mRadius / 4;
		float offset = radius;
		float center_x, center_y;
		float angle = mFishAction->getAngle() + M_PI_2;
		for (int i =0; i < mAnimationArray->count();i++)
		{
			center_x = pos.x + offset * cosf(angle);
			center_y = pos.y - offset * sinf(angle);
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->render(Vec2(center_x,center_y),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
			offset -= radius * 2;
		}
	}else if(mStatus == DEAD)
	{
		float radius = mRadius / 4;
		float offset = radius;
		float center_x, center_y;
		float angle = mFishAction->getAngle() + M_PI_2;
		for (int i =0; i < mDeadArray->count();i++)
		{
			center_x = pos.x + offset * cosf(angle);
			center_y = pos.y - offset * sinf(angle);
			GameAnimation* pAction = (GameAnimation*)mDeadArray->getObjectAtIndex(i);
			pAction->render(Vec2(center_x,center_y),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
			offset -= radius * 2;
		}
		RenderScoreNum();
	}
}

//////////////////////////////////////////////////////////////////////////
//一石三鸟
GameFishYssn::GameFishYssn()
{
	mAnimationArray = __Array::create();
	mDeadArray = __Array::create();
	mAnimationArray->retain();
	mDeadArray->retain();
	mRadius =0;
}

GameFishYssn::~GameFishYssn()
{
	CC_SAFE_RELEASE_NULL(mAnimationArray);
	CC_SAFE_RELEASE_NULL(mDeadArray);
}

GameFishYssn* GameFishYssn::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishYssn* fish = new GameFishYssn();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishYssn::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	Sprite* fishBg = Sprite::create("dntg2/images/fish/yssnquan.png");
	fishBg->runAction(RepeatForever::create(RotateBy::create(0.15f,-30)));
	mRadius = fishBg->getContentSize().width;
	fishBg->setPosition(action->getPosition());
	addChild(fishBg,-1,YUQUNQUANBGTAG);


	for(int i = 0 ; i < 3 ;i++)
	{
		GameAnimation* fish1_walk = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kYssnSubFish[tag][i]));
		addChild(fish1_walk);
		//for 初始化位置不对
		fish1_walk->setPosition(action->getPosition());
		mAnimationArray->addObject(fish1_walk);

		GameAnimation* fish1_dead = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kYssnSubFish[tag][i],false));
		addChild(fish1_dead);
		fish1_dead->setVisible(false);
		mDeadArray->addObject(fish1_dead);
	}

	for (int i =0; i < mAnimationArray->count();i++)
	{
		GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
		pAction->play();
	}
	return true;
}

void GameFishYssn::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		//子鱼的游走动画刷新
		for (int i =0; i < mAnimationArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->update(delta_time);
		}

		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		for (int i = 0 ; i < 3;i++)
		{
			GameAnimation* walk = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			if (walk->isVisible())
			{
				walk->setVisible(false);
			}
			GameAnimation* dead = (GameAnimation*)mDeadArray->getObjectAtIndex(i);
			if (!dead->isVisible())
			{
				dead->setVisible(true);
				dead->play();
			}
			dead->update(delta_time);
		}
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				DelayTime* delay = DelayTime::create(0.5f);
				FadeOut*   fo    = FadeOut::create(0.5f);
				Spawn* span = Spawn::createWithTwoActions(delay,fo);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishYssn::removeScore));
				mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
			}
		}

	}
}

void GameFishYssn::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishYssn::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		Sprite* bgQuan = (Sprite*)getChildByTag(YUQUNQUANBGTAG);
		bgQuan->setPosition(pos);
		//每条鱼的位置刷新
		float radius = mRadius / 4;
		float offset = radius;
		float center_x, center_y;
		float angle = mFishAction->getAngle();
		for (int i =0; i < mAnimationArray->count();i++)
		{
			center_x = pos.x + offset * cosf(angle);
			center_y = pos.y - offset * sinf(angle);
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->render(Vec2(center_x,center_y),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
			offset -= radius * 2;
			if(i==1)//第三条鱼位置特别处理
			{
				offset = radius;
				angle = mFishAction->getAngle() + M_PI_2;
			}
		}
	}else if(mStatus == DEAD)
	{
		float radius = mRadius / 4;
		float offset = radius;
		float center_x, center_y;
		float angle = mFishAction->getAngle();
		for (int i =0; i < mDeadArray->count();i++)
		{
			center_x = pos.x + offset * cosf(angle);
			center_y = pos.y - offset * sinf(angle);
			GameAnimation* pAction = (GameAnimation*)mDeadArray->getObjectAtIndex(i);
			pAction->render(Vec2(center_x,center_y),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
			offset -= radius * 2;
			if(i==1)//第三条鱼位置特别处理
			{
				offset = radius;
				angle = mFishAction->getAngle() + M_PI_2;
			}
		}
		RenderScoreNum();
	}
}

//////////////////////////////////////////////////////////////////////////
//全家福
GameFishQjf::GameFishQjf()
{
	mAnimationArray = __Array::create();
	mDeadArray = __Array::create();
	mAnimationArray->retain();
	mDeadArray->retain();
	mRadius =0;
}

GameFishQjf::~GameFishQjf()
{
	CC_SAFE_RELEASE_NULL(mAnimationArray);
	CC_SAFE_RELEASE_NULL(mDeadArray);
}

GameFishQjf* GameFishQjf::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishQjf* fish = new GameFishQjf();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishQjf::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	int fishtag = YUQUNQUANBGTAG;
	Sprite* fishBg = Sprite::create("dntg2/images/fish/qjfquan.png");
	fishBg->runAction(RepeatForever::create(RotateBy::create(0.15f,-30)));
	mRadius = fishBg->getContentSize().width;
	fishBg->setPosition(action->getPosition());
	addChild(fishBg,0,fishtag);

	fishtag++;
	//小里圈背景
	for (int j=0;j<5 ;j++)
	{
		Sprite* innerBg = Sprite::create("dntg2/images/fish/qjfdiquan.png");
		mInnerWidht = innerBg->getContentSize().width;
		mInnerHeight = innerBg->getContentSize().height;
		mScale = mRadius/(mInnerWidht*3);
		innerBg->setScale(mScale);
		innerBg->runAction(RepeatForever::create(RotateBy::create(0.15f,-45)));
		//for 初始位置不对
		innerBg->setPosition(action->getPosition());
		addChild(innerBg,0,fishtag);
		fishtag++;
	}
	//每条鱼的游走动画
	for (int i = 0 ; i < 10 ; i++)
	{
		GameAnimation* fish_walk = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kQuanJiaFu[i]));
		fish_walk->setScale(mScale);
		//for 初始化位置不对
		fish_walk->setPosition(action->getPosition());
		addChild(fish_walk);
		mAnimationArray->addObject(fish_walk);

		GameAnimation* fish_dead = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)kQuanJiaFu[i],false));
		fish_dead->setScale(mScale);
		fish_dead->setVisible(false);
		addChild(fish_dead);
		mDeadArray->addObject(fish_dead);
	}
	
	

	for (int i =0; i < mAnimationArray->count();i++)
	{
		GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
		pAction->play();
	}

	return true;

}

void GameFishQjf::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		//子鱼的游走动画刷新
		for (int i =0; i < mAnimationArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->update(delta_time);
		}

		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		//子鱼的死亡动画播放
		bool isCanDestory = true;
		for (int i =0; i < mDeadArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mDeadArray->getObjectAtIndex(i);
			pAction->update(delta_time);
			if (pAction->IsPlaying())
			{
				isCanDestory = false;
			}
		}
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				if(isCanDestory)
				{
					DelayTime* delay = DelayTime::create(0.5f);
					FadeOut*   fo    = FadeOut::create(0.5f);
					Spawn* span = Spawn::createWithTwoActions(delay,fo);
					CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishQjf::removeScore));
					mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
				}
			}	
		}
		
	}
}

void GameFishQjf::removeScore(Node* pNode)
{
	pNode->removeFromParent();
	mStatus = DESTROY;
}

GameAnimation* GameFishQjf::getFishAnimationByIndex(int index)
{
	GameAnimation* pAnima = nullptr;
	if(!isDealDead)
	{
		pAnima = (GameAnimation*)mAnimationArray->getObjectAtIndex(index);
	}
	else
	{
		pAnima = (GameAnimation*)mDeadArray->getObjectAtIndex(index);
		if (pAnima && !pAnima->isVisible())
		{
			pAnima->setVisible(true);
			pAnima->play();
		}
	}

	if (pAnima)
	{
		return pAnima;
	}
	return nullptr;
}


void GameFishQjf::Render()
{
	int tag=0;
	Vec2 pos = mFishAction->getPosition();
	isDealDead = false;
	if (mStatus == DEAD)
	{
		isDealDead = true;
	}

	tag= YUQUNQUANBGTAG+1;
	Sprite* bgQuan = (Sprite*)getChildByTag(YUQUNQUANBGTAG);
	bgQuan->setPosition(pos);
		
	Sprite* innerQuan = (Sprite*)getChildByTag(tag++);
	innerQuan->setPosition(pos);
	GameAnimation* fish1 = getFishAnimationByIndex(0);
	fish1->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));

	Vec2 temp_pos;
	float radius = mScale * mInnerWidht / 4;
	float offset = getFishAnimationByIndex(1)->getFrameWidth() / 2 + getFishAnimationByIndex(2)->getFrameWidth() / 2;
	float center_x, center_y;
	float angle =mFishAction->getAngle() + M_PI_2;
	center_x = pos.x + radius * mScale * cosf(angle);
	center_y = pos.y - radius * mScale * sinf(angle);
	getFishAnimationByIndex(1)->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));

	int index = 2;
	// 左上
	temp_pos.x = pos.x + mInnerWidht * mScale * cosf(mFishAction->getAngle() - M_PI_2 - M_PI_4);
	temp_pos.y = pos.y + mInnerHeight * mScale * sinf(mFishAction->getAngle() - M_PI_2 - M_PI_4);
	innerQuan = (Sprite*)getChildByTag(tag++);
	innerQuan->setPosition(temp_pos);

	offset = radius;
	for (int i = 0; i < 2; ++i) {
		center_x = temp_pos.x + offset * cosf(angle);
		center_y = temp_pos.y - offset * sinf(angle);
		getFishAnimationByIndex(i+index)->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		offset -= radius * 2;
	}
	index += 2;
	// 右上
	temp_pos.x = pos.x + mInnerWidht * mScale * cosf(mFishAction->getAngle() - M_PI_4);
	temp_pos.y = pos.y + mInnerHeight * mScale * sinf(mFishAction->getAngle() - M_PI_4);
	innerQuan = (Sprite*)getChildByTag(tag++);
	innerQuan->setPosition(temp_pos);
	offset = radius;
	for (int i = 0; i < 2; ++i) {
		center_x = temp_pos.x + offset * cosf(angle);
		center_y = temp_pos.y - offset * sinf(angle);
		getFishAnimationByIndex(i+index)->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		offset -= radius * 2;
	}
	index += 2;
	// 右下
	temp_pos.x = pos.x + mInnerWidht * mScale * cosf(mFishAction->getAngle() + M_PI_4);
	temp_pos.y = pos.y +mInnerHeight * mScale * sinf(mFishAction->getAngle() + M_PI_4);
	innerQuan = (Sprite*)getChildByTag(tag++);
	innerQuan->setPosition(temp_pos);
	offset = radius;
	for (int i = 0; i < 2; ++i) {
		center_x = temp_pos.x + offset * cosf(angle);
		center_y = temp_pos.y - offset * sinf(angle);
		getFishAnimationByIndex(i+index)->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		offset -= radius * 2;
	}
	index += 2;
	// 左下
	temp_pos.x = pos.x + mInnerWidht * mScale * cosf(mFishAction->getAngle() + M_PI_2 + M_PI_4);
	temp_pos.y = pos.y + mInnerHeight* mScale * sinf(mFishAction->getAngle() + M_PI_2 + M_PI_4);
	innerQuan = (Sprite*)getChildByTag(tag++);
	innerQuan->setPosition(temp_pos);
	offset = radius;
	for (int i = 0; i < 2; ++i) {
		center_x = temp_pos.x + offset * cosf(angle);
		center_y = temp_pos.y - offset * sinf(angle);
		getFishAnimationByIndex(i+index)->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		offset -= radius * 2;
	}

	
	if(isDealDead)
	{
		for (int i = 0 ; i < 10 ; i++ )
		{
			GameAnimation* pChildFishAnima = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			if (pChildFishAnima && pChildFishAnima->isVisible())
			{
				pChildFishAnima->setVisible(false);
			}
		}

		RenderScoreNum();
		
	}
}

//////////////////////////////////////////////////////////////////////////
//闪电鱼
GameFishChain::GameFishChain()
{
	mAnimationArray = __Array::create();
	mDeadArray = __Array::create();
	mAnimationArray->retain();
	mDeadArray->retain();
	mScale = 0.f;
	mScene = nullptr;

	mChainAnimation = nullptr;
	mScaleAction = nullptr;
	greenLight = nullptr;
	mIsRotating = false;
	mSubFishDead = nullptr;
	mViewChairId = -1;
	mCircleSp = nullptr;
}

GameFishChain::~GameFishChain()
{
	CC_SAFE_RELEASE_NULL(mAnimationArray);
	CC_SAFE_RELEASE_NULL(mDeadArray);
	if (mChainAnimation)
		CC_SAFE_RELEASE_NULL(mChainAnimation);
	if (mScaleAction)
		CC_SAFE_RELEASE_NULL(mScaleAction);
	if (mSubFishDead)
		CC_SAFE_RELEASE_NULL(mSubFishDead);
	if (mCircleSp)
	{
		mCircleSp->removeFromParent();
		mCircleSp = nullptr;
	}
}

GameFishChain* GameFishChain::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishChain* fish = new GameFishChain();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishChain::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;
	
	GameAnimation* fish1_walk = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag));
	fish1_walk->setLocalZOrder(mFishwalking->getLocalZOrder()-1);
	//for 初始化位置不对
	fish1_walk->setPosition(action->getPosition());

	addChild(fish1_walk);
	mAnimationArray->addObject(fish1_walk);

	//dead
	AnimationInfo* dead_info = ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)tag,false);
	if (dead_info)
	{
		mSubFishDead = GameAnimation::create(dead_info);
		addChild(mSubFishDead,5);
		mSubFishDead->setVisible(false);
		CC_SAFE_RETAIN(mSubFishDead);
	}

	float scale  =  MAX(fish1_walk->getFrameWidth()*10.0,fish1_walk->getFrameHeight()*10.0) / mFishwalking->getFrameWidth();
	mFishwalking->setScale(scale/10.0);
	mScale = scale/10.0;
	for (int i =0; i < mAnimationArray->count();i++)
	{
		GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
		pAction->play();
	}


	return true;

}

void GameFishChain::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause)
			mFishAction->step(delta_time);
		mFishwalking->update(delta_time);
		//子鱼的游走动画刷新
		for (int i =0; i < mAnimationArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->update(delta_time);
			if(mStatus == CHAIN_LOCK && mFishAction->isDone())
			{
				if(! mIsRotating)
				{
					RotateBy* rb = RotateBy::create(0.1f,15);
					pAction->runAction(RepeatForever::create(rb));
					
					mIsRotating = true;

					//发送消息
					__Dictionary* dic = __Dictionary::create();
					dic->setObject(__Integer::create(mViewChairId),"chairId");
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(FishArriveMsg,dic);
				}
			}

		}
		//闪电链
		if (mChainAnimation)
			mChainAnimation->update(delta_time);

		//green light
		if (greenLight)
		{
			greenLight->update(delta_time);
			mScaleAction->step(delta_time);
		}

		if (mFishAction->isDone())
		{
			if(mStatus == MOVE)
				mStatus = DESTROY;
			else if (mStatus == CHAIN_LOCK && mChainAnimation)
			{
				mChainAnimation->removeFromParentAndCleanup(true);
				mChainAnimation = nullptr;

				mCircleSp->removeFromParent();
				mCircleSp = nullptr;

				mFishwalking->reset(ResourcesManager::getInstance()->getAnimationInfoByName("BlueLight"));
				RotateBy* rb = RotateBy::create(0.1f,15);
				mFishwalking->runAction(RepeatForever::create(rb));

			}
		}
	} else if (mStatus == DEAD) {
		if(mSubFishDead)
		{
			mSubFishDead->update(delta_time);
			if (!mSubFishDead->IsPlaying())//死亡动画播放完后删除
			{
				mStatus = DESTROY ;
			}
		}
	}
}


void GameFishChain::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		mFishwalking->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),mScale);
		for (int i =0; i < mAnimationArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		}

		Vec2 delta;
		float angle = 0.0f;
		if(mChainAnimation)
		{
			//当前鱼的位置
			pos = mFishwalking->getPosition();
			
			Vec2::subtract(mChainStartPos,pos,&delta);
			float chain_len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			float rote_angle = 0.f;
			float scale = 1.f;
			if (chain_len > 0) {
				if (delta.y > 0) {
					rote_angle = -std::acos(delta.x / chain_len);
				} else {
					rote_angle = std::acos(delta.x / chain_len);
				}
				angle =  CommonFunction::CalcAngle(pos.x,pos.y,mChainStartPos.x,mChainStartPos.y);
				scale = chain_len / mChainAnimation->getFrameWidth();
			}
			delta.x = mChainStartPos.x + (chain_len / 2) * std::sin(angle);
			delta.y = mChainStartPos.y + (chain_len / 2) * std::cos(angle);

			mChainAnimation->render(delta,CC_RADIANS_TO_DEGREES(rote_angle),scale);
		}

		if (greenLight)
		{
			Vec2 green_scale = mScaleAction->getCurScale();
			greenLight->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()),green_scale.x);
			
			if (mScaleAction->isDone())
			{
				greenLight->removeFromParentAndCleanup(true);
				greenLight = nullptr;
			}
		}
		//太极圆
		if (mCircleSp)
		{
			Vec2::subtract(mChainStartPos,pos,&delta);
			float len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			angle =  CommonFunction::CalcAngle(pos.x,pos.y,mChainStartPos.x,mChainStartPos.y);
			Vec2 circle_pos;
			circle_pos.x = mChainStartPos.x + (len - (mFishwalking->getContentSize().width/2 - mCircleSp->getContentSize().width/2)*mScaleAction->getCurScale().x ) * std::sin(angle);
			circle_pos.y = mChainStartPos.y + (len - (mFishwalking->getContentSize().width/2 - mCircleSp->getContentSize().width/2)*mScaleAction->getCurScale().x) * std::cos(angle);
			mCircleSp->setPosition(circle_pos);
			mCircleSp->setScale(mScaleAction->getCurScale().x);
			mCircleSp->setRotation(CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		}
	
	}
	else if(mStatus == DEAD)
	{
		//隐藏背后的闪电圈
		if (mFishwalking->isVisible())
			mFishwalking->setVisible(false);
		//隐藏里面的子鱼的游走精灵动象
		for (int i =0; i < mAnimationArray->count();i++)
		{
			GameAnimation* pAction = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			pAction->setVisible(false);
		}
		//设置死亡精灵的位置及旋转角度
		if(mSubFishDead)
		{
			if(!mSubFishDead->isVisible()) mSubFishDead->setVisible(true);
			mSubFishDead->render(mFishAction->getPosition(),CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		}
	}
}

void GameFishChain::onChainPro(GameScene* scene,Vec2 chain_start_pos,Vec2 fish_end_pos,WORD view_chair_id,int fishIndex)
{

	mViewChairId = view_chair_id;
	 
	setLocalZOrder(1000);

	mScene = scene;
	Vec2 pos = mFishAction->getPosition();
	greenLight = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_CHAIN));
	greenLight->setPosition(pos);
	greenLight->setScale(2.0f);
	mScene->addChain(greenLight,mViewChairId);

	mScaleAction  = ActionScaleTo::create(0.5f,2.0f,2.0f,mScale);
	mScaleAction->retain();
	mScaleAction->start();
	
	mChainStartPos = chain_start_pos;

	mFishEndPos = fish_end_pos;

	//float distance = std::sqrt((mFishEndPos.x - pos.x) * (mFishEndPos.x - pos.x) + (mFishEndPos.y - pos.y) * (mFishEndPos.y - pos.y));
	//float speed = distance/1.0f;
	//ActionFishMove* action = ActionFishMoveLinear::create(speed,pos,mFishEndPos);
	//set_action_fish(action);

	Vec2 delta;
	Vec2::subtract(pos,chain_start_pos,&delta);
	float chain_len = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	float rote_angle = 0.f;
	float scale = 1.f;
	float angle = 0.0f;

	mChainAnimation = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByName("LightChain"));
	mChainAnimation->retain();

	if (chain_len > 0) {
		if (delta.y > 0) {
		rote_angle = -std::acos(delta.x / chain_len);
		} else {
		rote_angle = std::acos(delta.x / chain_len);
		}
		angle = CommonFunction::CalcAngle(pos.x,pos.y,chain_start_pos.x,chain_start_pos.y);
		scale = chain_len / mChainAnimation->getFrameWidth();
	}

	delta.x = chain_start_pos.x + (chain_len / 2) * std::sin(angle);
	delta.y = chain_start_pos.y + (chain_len / 2) * std::cos(angle);
	mChainAnimation->render(delta,CC_RADIANS_TO_DEGREES(rote_angle),scale);

	mChainAnimation->play();

	scene->addChain(mChainAnimation,mViewChairId);

	Vec2 cicle_pos ;
	cicle_pos.x = mChainStartPos.x + (chain_len - greenLight->getContentSize().width/2 ) * std::sin(angle);
	cicle_pos.y = mChainStartPos.y + (chain_len - greenLight->getContentSize().width/2 ) * std::cos(angle);
	mCircleSp = Sprite::create("dntg2/images/fish/ScoreCircle.png");
	mCircleSp->setScale(2.0f);
	mCircleSp->setPosition(Vec2(cicle_pos.x,cicle_pos.y));

	mScene->addChild(mCircleSp);

	Vector<FiniteTimeAction*> actions;
	DelayTime* delay = DelayTime::create(0.5f);
	actions.pushBack(delay);
	CallFuncN* call = CallFuncN::create(this,callfuncN_selector(GameFishChain::onStartMov));
	actions.pushBack(call);

	runAction(Sequence::create(actions));
	
}

//开始移动
void GameFishChain::onStartMov(Node* pSender)
{
	Vec2 pos = mFishAction->getPosition();

	float distance = std::sqrt((mFishEndPos.x - pos.x) * (mFishEndPos.x - pos.x) + (mFishEndPos.y - pos.y) * (mFishEndPos.y - pos.y));
	float speed = distance/1.0f;
	ActionFishMove* action = ActionFishMoveLinear::create(speed,pos,mFishEndPos);
	set_action_fish(action);

	//mStatus = CHAIN_LOCK;
	set_pause(false);
}



Vec2 GameFishChain::getEndPos()
{
	return mFishEndPos;
}


void GameFishChain::chainFish()
{
	stopAllActions();
	if(mChainAnimation)
		mChainAnimation->setVisible(false);
	if (greenLight)
		greenLight->setVisible(false);

}

//////////////////////////////////////////////////////////////////////////
//炼丹炉
GameFishBglu::GameFishBglu()
{
	mFontAnima = nullptr;
	mDeadAnima = nullptr;
}

GameFishBglu::~GameFishBglu()
{
	CC_SAFE_RELEASE_NULL(mFontAnima);
	CC_SAFE_RELEASE_NULL(mDeadAnima);
}

GameFishBglu* GameFishBglu::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishBglu* fish = new GameFishBglu();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishBglu::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;
	return true;

}

void GameFishBglu::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		mFishwalking->update(delta_time);
		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				DelayTime* delay = DelayTime::create(0.5f);
				FadeOut*   fo    = FadeOut::create(0.5f);
				Spawn* span = Spawn::createWithTwoActions(delay,fo);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishBglu::removeScore));
				mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
			}
		}	
	}
}

void GameFishBglu::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}


void GameFishBglu::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		mFishwalking->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
	}
	else if(mStatus == DEAD)
	{
		if(mFishwalking->isVisible())
			mFishwalking->setVisible(false);
		//Vec2 center_pos;
		//center_pos.x = kScreenWidth/2;
		//center_pos.y = kScreenHeight/2;
		//mFontAnima->render(center_pos,0);
		//mDeadAnima->render(center_pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		RenderScoreNum();
	}
}

void GameFishBglu::catchFish(WORD view_chair_id, SCORE score)
{
	GameFish::catchFish(view_chair_id,score);
	__Dictionary * dic = __Dictionary::create();
	dic->setObject(__Integer::create(fish_id()),"fishId");
	dic->setObject(__Integer::create(view_chair_id),"chairId");
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(dealBgLuEffectMsg,dic);
}
//////////////////////////////////////////////////////////////////////////
//李逵鱼群
GameFishYuqunForLK::GameFishYuqunForLK()
{
	mAnimationArray = __Array::create();
	mDeadAnimationArray = __Array::create();
	mAnimationArray->retain();
	mDeadAnimationArray->retain();
	mFishCnt = 0;
}

GameFishYuqunForLK::~GameFishYuqunForLK()
{
	CC_SAFE_RELEASE_NULL(mAnimationArray);
	CC_SAFE_RELEASE_NULL(mDeadAnimationArray);
}

GameFishYuqunForLK* GameFishYuqunForLK::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishYuqunForLK* fish = new GameFishYuqunForLK();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishYuqunForLK::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	FishKind fish_kind_temp = FISH_KIND_4;
	mFishCnt = 3;
	if (fish_kind == FISH_KIND_25_27)
	{
		if(tag == 0)
			fish_kind_temp = FISH_KIND_4;
		else if(tag == 1)
			fish_kind_temp = FISH_KIND_5;
		else
			fish_kind_temp = FISH_KIND_7;
		
	}
	else
	{
		if(tag == 0)
			fish_kind_temp = FISH_KIND_6;
		else if(tag == 1)
			fish_kind_temp = FISH_KIND_8;
		else
			fish_kind_temp = FISH_KIND_10;
		mFishCnt = 4;
	}

	for (int i=0; i<mFishCnt ;i++)
	{
		Sprite* fishBg = Sprite::create("GameResources/lkpy/fish/halo.png");
		mRadius = fishBg->getContentSize().width;
		fishBg->runAction(RepeatForever::create(RotateBy::create(0.4f,-40)));
		//for 初始位置
		fishBg->setPosition(action->getPosition());
		addChild(fishBg,0,YUQUNQUANBGTAG+i);

		GameAnimation* pAnima = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)fish_kind_temp));
		fishBg->setLocalZOrder(pAnima->getLocalZOrder()-1);
		pAnima->setPosition(action->getPosition());
		addChild(pAnima);

		pAnima->play();
		mAnimationArray->addObject(pAnima);


		GameAnimation* pDeadAnima = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)fish_kind_temp,false));
		addChild(pDeadAnima);
		pDeadAnima->setVisible(false);
		mDeadAnimationArray->addObject(pDeadAnima);

	}
	return true;

}

void GameFishYuqunForLK::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);

		cocos2d::Ref* pElement = NULL;
		CCARRAY_FOREACH(mAnimationArray,pElement)
		{
			GameAnimation* pAnima = (GameAnimation*)pElement;
			pAnima->update(delta_time);
		}
		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		GameAnimation* pAciton = nullptr;
		bool isCanDestory = true;
		for (int i =0 ; i < mFishCnt; i++)
		{
			pAciton = (GameAnimation*)mAnimationArray->getObjectAtIndex(i);
			if(pAciton->isVisible())
				pAciton->setVisible(false);
			pAciton = (GameAnimation*)mDeadAnimationArray->getObjectAtIndex(i);
			if(!pAciton->isVisible())
			{
				pAciton->setVisible(true);
				pAciton->play();
			}
			//死亡动画是否播放完
			if (pAciton->IsPlaying())
			{
				isCanDestory = false;
			}
			pAciton->update(delta_time);
		}

		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				if(isCanDestory)
				{
					DelayTime* delay = DelayTime::create(0.5f);
					FadeOut*   fo    = FadeOut::create(0.5f);
					Spawn* span = Spawn::createWithTwoActions(delay,fo);
					CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishYuqun::removeScore));
					mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
				}
			}
		}


	}
}

void GameFishYuqunForLK::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishYuqunForLK::Render()
{
	__Array* cur = mAnimationArray;
	if (mStatus == DEAD)
	{
		cur = mDeadAnimationArray;
	}

	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK || mStatus == DEAD) {
		float radius = mRadius / 2;
		float offset = radius * 4;
		float center_x, center_y;
		float angle = mFishAction->getAngle() + M_PI_2;
		for (int i = 0; i <mFishCnt; ++i) {
			GameAnimation* pAnima = (GameAnimation*)cur->getObjectAtIndex(i);
			center_x = pos.x + offset * cosf(angle);
			center_y = pos.y - offset * sinf(angle);
			pAnima->render(Vec2(center_x, center_y), CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));

			Sprite* bg = (Sprite*)getChildByTag(YUQUNQUANBGTAG+i);
			bg->setPosition(Vec2(center_x, center_y));
			offset -= radius * 2;
		}
	}

	if (mStatus == DEAD)
	{
		RenderScoreNum();
	}
}

bool GameFishYuqunForLK::BulletHitTest(GameBullet* bullet) {
	if (mStatus != MOVE)
		return false;

	Vec2 bullet_pos = bullet->getPosition();
	float center_distance = mBoundingRadius + bullet->getBulletRadius();
	float distance;
	Vec2 pos = mFishAction->getPosition();
	float offset = 0;
	if (mBoundingCount > 1)
		offset = mBoundingCount * mBoundingRadius;
	float center_x, center_y;
	float angle = mFishAction->getAngle() + M_PI_2;
	for (int i = 0; i < mBoundingCount; ++i) {
		center_x = pos.x + offset * cosf(angle);
		center_y = pos.y + offset * sinf(angle);
		distance = CommonFunction::CalcDistance(bullet_pos.x, bullet_pos.y, center_x, center_y);
		if (distance <= center_distance) {
			return true;
		}
		offset -= mBoundingRadius * 2;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
//李逵
GameFishLK::GameFishLK()
{
	mulripleLable = nullptr;
}

GameFishLK::~GameFishLK()
{
}

GameFishLK* GameFishLK::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishLK* fish = new GameFishLK();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishLK::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;
	fish_mulriple_ = 40;

	return true;
}

void GameFishLK::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		mFishwalking->update(delta_time);

		//移动路线已走完，销毁
		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		
		if (mFishwalking->isVisible())
			mFishwalking->setVisible(false);
		if (!mFishDead->isVisible())
		{
			mFishDead->setVisible(true);
		}
		mFishDead->update(delta_time);
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				DelayTime* delay = DelayTime::create(0.5f);
				FadeOut*   fo    = FadeOut::create(0.5f);
				Spawn* span = Spawn::createWithTwoActions(delay,fo);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishLK::removeScore));
				mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
			}
		}
		//等死亡动画播完，再销毁
		if (!mFishDead->IsPlaying())
		{
			mStatus = DESTROY;
		}
	}
}

void GameFishLK::removeScore(Node* pSender)
{
	pSender->removeFromParent();
}

void GameFishLK::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		mFishwalking->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		RenderMulriple(pos, CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
	}else if(mStatus == DEAD)
	{
		mFishDead->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
		RenderScoreNum();
	}
}

void GameFishLK::RenderMulriple(Vec2 pos,float degree)
{
	float center_x, center_y;
	float angle = mFishAction->getAngle();
	if (!mulripleLable)
	{
		mulripleLable=  LabelAtlas::create("0","GameResources/lkpy/score_num.png",33,38,'0');
		center_x = pos.x + mFishwalking->getFrameWidth()/2 * cosf(angle);
		center_y = pos.y - mFishwalking->getFrameHeight()/2 * sinf(angle);
	
		mulripleLable->setPosition(Vec2(center_x,center_y));
		mulripleLable->setAnchorPoint(Vec2(0.2,0.5));
		mulripleLable->setString(numberToString(fish_mulriple_));
		mulripleLable->setRotation(degree);

		addChild(mulripleLable);
	}
	else
	{
		center_y = pos.y + mFishwalking->getFrameHeight()/2 * cosf(angle);
		center_x = pos.x + mFishwalking->getFrameHeight()/2 * sinf(angle);
		mulripleLable->setPosition(Vec2(center_x,center_y));
		mulripleLable->setString(numberToString(fish_mulriple_));
		mulripleLable->setRotation(degree);
	}
}
//////////////////////////////////////////////////////////////////////////
//李逵中的鱼王
GameFishKing::GameFishKing()
{
	mInnerFishWalking = nullptr;
	mInnerFishDead  = nullptr;
}

GameFishKing::~GameFishKing()
{
	if(mInnerFishWalking)
		CC_SAFE_RELEASE_NULL(mInnerFishWalking);
	if(mInnerFishDead)
		CC_SAFE_RELEASE_NULL(mInnerFishDead);
}

GameFishKing* GameFishKing::create(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	GameFishKing* fish = new GameFishKing();
	if(fish && fish->init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
	{

		fish->autorelease();
		return fish;
	}
	CC_SAFE_DELETE(fish);
	return nullptr;
}

bool GameFishKing::init(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
{
	if (!GameFish::init(fish_kind,fish_id,tag,bounding_radius,bounding_count,action))
		return false;

	std::string imageName = "";
	int innerFishTag ;
	if (SessionManager::shareInstance()->getGameKind() == GameKindLkpy)
	{
		imageName = "GameResources/lkpy/fish/halo.png";
		innerFishTag = FISH_KIND_1 + tag;
	}else if(SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
	{
		imageName = "GameResources/jcpy/fish/halo.png";
		innerFishTag = FISH_KIND_JC1 + tag; 
	}
	Sprite* fishBg = Sprite::create(imageName);
	fishBg->runAction(RepeatForever::create(RotateBy::create(0.3f,-45)));
	fishBg->setPosition(action->getPosition());
	addChild(fishBg,0,YUQUNQUANBGTAG);

	//内部鱼的游走动画对象
	mInnerFishWalking = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)(innerFishTag)));
	fishBg->setLocalZOrder(mInnerFishWalking->getLocalZOrder()-1);
	//for 初始化位置不对
	mInnerFishWalking->setPosition(action->getPosition());

	addChild(mInnerFishWalking);

	CC_SAFE_RETAIN(mInnerFishWalking);
	mInnerFishWalking->play();

	//AnimationInfo* dead_info = ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)(FISH_KIND_1 + tag),false);
	AnimationInfo* dead_info = ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)(innerFishTag),false);
	if (dead_info)
	{
		mInnerFishDead = GameAnimation::create(dead_info);
	}
	else
	{
		mInnerFishDead = GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind((FishKind)(innerFishTag)));
	}
	addChild(mInnerFishDead);
	mInnerFishDead->setVisible(false);
	CC_SAFE_RETAIN(mInnerFishDead);

	return true;
}

void GameFishKing::Update(float delta_time)
{
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) {
		if (!mBpause || mStatus == CHAIN_LOCK)
			mFishAction->step(delta_time);
		mInnerFishWalking->update(delta_time);

		if (mFishAction->isDone() && mStatus == MOVE)
			mStatus = DESTROY;
	} else if (mStatus == DEAD) {
		if (mInnerFishDead->isVisible())
		{
			mInnerFishDead->setVisible(true);
			mInnerFishDead->play();
			mInnerFishWalking->setVisible(false);
		}
		mInnerFishDead->update(delta_time);
		//产生数字
		if(mActionScore && mScoreLable) 
		{
			if (!mActionScore->isDone())
			{
				mActionScore->step(delta_time);
			}
			else
			{
				DelayTime* delay = DelayTime::create(0.5f);
				FadeOut*   fo    = FadeOut::create(0.5f);
				Spawn* span = Spawn::createWithTwoActions(delay,fo);
				CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameFishKing::removeScore));
				mScoreLable->runAction(Sequence::createWithTwoActions(span,call));
			}
		}
	}
}

void GameFishKing::removeScore(Node* pSender)
{
	pSender->removeFromParent();
	mStatus = DESTROY;
}

void GameFishKing::Render()
{
	Vec2 pos = mFishAction->getPosition();
	if (mStatus == MOVE || mStatus == CHAIN_LOCK) 
	{
		Sprite* bgQuan = (Sprite*)getChildByTag(YUQUNQUANBGTAG);
		bgQuan->setPosition(pos);
		mInnerFishWalking->render(pos,CC_RADIANS_TO_DEGREES(mFishAction->getAngle()));
	}else if(mStatus == DEAD)
	{
		mInnerFishDead->render(pos,mFishAction->getAngle());
		RenderScoreNum();
	}
}

//////////////////////////////////////////////////////////////////////////

GameFishManager::GameFishManager()
{
	mFishVector = __Array::create();
	CC_SAFE_RETAIN(mFishVector);
}

GameFishManager::~GameFishManager()
{
  freeAllFish();
  CC_SAFE_RELEASE_NULL(mFishVector);
}

 GameFishManager* GameFishManager::sharedInstance()
 {
	 if(__instance == nullptr)
		__instance =  new GameFishManager;
	 return __instance;
 }

 void GameFishManager::purgeInstance()
 {
	 CC_SAFE_DELETE(__instance);
 }

 GameFish* GameFishManager::creatFish(FishKind fish_kind, int fish_id, int tag, float bounding_radius, int bounding_count, ActionFishMove* action)
 {
    GameFish* fish = NULL;
	int gameKind = SessionManager::shareInstance()->getGameKind();
	switch (gameKind)
	{
	case GameKindDntg:
		{
			if (fish_kind == FISH_BGLU) {
				fish = GameFishBglu::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_SHENXIANCHUAN) {
				fish =  GameFishShip::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_YUQUN) {
				fish = GameFishYuqun::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_DNTG) {
				fish =  GameFishDntg::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_YJSD) {
				fish =  GameFishYjsd::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_YSSN) {
				fish = GameFishYssn::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_QJF) {
				fish = GameFishQjf::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			} else if (fish_kind == FISH_CHAIN) {
				fish =GameFishChain::create(fish_kind, fish_id, tag, kChainFishRadius[tag], 1, action);
			} else {
				fish = GameFish::create(fish_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
		}
		break;
	case GameKindLkpy:
		{
			int temp_kind = fish_kind + FISH_KIND_1 ;
			if(temp_kind == FISH_KIND_21)
			{
				fish =  GameFishLK::create((FishKind)temp_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
			else if (temp_kind == FISH_KIND_25_27 || temp_kind == FISH_KIND_28_30) {
				fish =  GameFishYuqunForLK::create((FishKind)temp_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
			else if (temp_kind == FISH_KIND_31_40) {
				fish =  GameFishKing::create((FishKind)temp_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
			else {
				fish = GameFish::create((FishKind)temp_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
		}
		break;
	case  GameKindJcpy:
		{
			int temp_kind = fish_kind + FISH_KIND_JC1 ;
			if (temp_kind == FISH_KIND_JCKING) 
			{
				fish =  GameFishKing::create((FishKind)temp_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
			else 
			{
				fish = GameFish::create((FishKind)temp_kind, fish_id, tag, bounding_radius, bounding_count, action);
			}
		}
		break;
	}
	 mFishVector->addObject(fish);
	 return fish;
 }


void GameFishManager::freeAllFish()
{
	GameFish* fish = NULL;
	int fishCnt = mFishVector->count();
	for(int i=0;i< fishCnt;i++)
	{
		GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		if(fish)
		{
			fish->removeFromParent();
		}
	}
	mFishVector->removeAllObjects();
}

void GameFishManager::Update(float dt)
{
  GameFish* fish = NULL;
  if(mFishVector->count()>0)
  {
	  for(int i=0;i<mFishVector->count();i++)
	  {
		  GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		  if(fish->status() == GameFish::DESTROY)
		  {
			  fish->removeFromParent();
			  //被捕捉到，或游到终点，从数组中移出
			  mFishVector->removeObjectAtIndex(i,true);
		  }
		  else
		  {
			  fish->Update(dt);
		  }
	  }
  }
}

void GameFishManager::Render()
{
	GameFish* fish = NULL;
	for(int i=0;i<mFishVector->count();i++)
	{
		GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		if(fish)
		{
			fish->Render();
		}
	}
}


bool GameFishManager::InsideScreen(GameFish* fish)
{
	Vec2 position = fish->position();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (position.x >= 0 && position.x <= visibleSize.width && position.y >= 0 && position.y <= visibleSize.height)
		return true;
	return false;
}


bool GameFishManager::BulletHitTest(GameBullet* bullet)
{
	GameFish* fish = NULL;
	for(int i=0;i< mFishVector->count();i++)
	{
		GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		if(fish)
		{
			 if(fish->BulletHitTest(bullet))
				 return true;
		}
	}
	return false;
}

GameFish* GameFishManager::getFish(int fish_id)
{
	GameFish* fish = NULL;
	for(int i=0;i< mFishVector->count();i++)
	{
		GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		if(fish->fish_id() == fish_id)
		{
			return fish;
		}
	}
	return nullptr;
}

GameFish* GameFishManager::catchFish(int chairId,FishKind fish_kind ,int fish_id,SCORE fish_score)
{
	GameFish* fish = getFish(fish_id);
	if (!fish)
		return nullptr;
	fish->catchFish(chairId,fish_score);
	return fish;
}

void GameFishManager::fishPause(bool swith,float elapsed)
{
	GameFish* fish = NULL;
	for(int i=0;i< mFishVector->count();i++)
	{
		GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		if (fish && fish->status() != GameFish::CHAIN_LOCK)
		{
			fish->set_pause(swith);
			fish->action_fish()->setElapsed(MAX(fish->action_fish()->getElapsed()-elapsed,0));
		}
	}
}

//李逵专用
void GameFishManager::modifyLkMulriple(int fish_id, int mulriple)
{
	GameFish* fish = NULL;
	for(int i=0;i< mFishVector->count();i++)
	{
		GameFish* fish = (GameFish*)mFishVector->getObjectAtIndex(i);
		if (fish && fish->fish_id() == fish_id)
		{
			if (fish->fish_kind() == FISH_KIND_21)
			{
				fish->SetFishMulriple(mulriple);
			}
		}
	}
}