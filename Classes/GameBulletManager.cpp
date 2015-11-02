#include "GameBulletManager.h"
#include "GameFishManager.h"
#include "SoundManager.h"
#include "CommonFunction.h"
#include "SessionManager.h"

static GameBulletManager* __instance = nullptr;

GameBullet::GameBullet()
{
	mBulletKind = BULLET_2_NORMAL;
	mBoudingRadius =0;
	mFirerChairId = -1;
	mBulletSpeed = 0;
	mBulletId = 0;
	mBulletAction = nullptr;
	mStatus = MOVE;
	mBulletWidth =0;
	mBulletHeight =0;
	mReuseFlag = false;
}

GameBullet::~GameBullet()
{
	CC_SAFE_RELEASE_NULL(mBulletAction);
}

GameBullet* GameBullet::create(BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed)
{
	GameBullet* pBullet = new GameBullet();
	if (pBullet && pBullet->init(bullet_kind,bounding_radius,firer_view_chair_id,bullet_speed))
	{
		pBullet->autorelease();
		return pBullet;
	}
	CC_SAFE_DELETE(pBullet);
	return nullptr;
}

bool GameBullet::init(BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed)
{
	SpriteFrame* frame =nullptr;
	if (bullet_kind >= BULLET_2_DOUBLE)
	{
		String* imageName = String::createWithFormat("bullet%d_double_%d.png",bullet_kind-1,1);
		frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
	}
	else
	{
		String* imageName  = nullptr;
		if(SessionManager::shareInstance()->getGameKind() != GameKindJcpy)
			 imageName = String::createWithFormat("bullet%d_norm%d.png",bullet_kind+2,firer_view_chair_id+1);
		else
			 imageName = String::createWithFormat("bullet%d_norm%d_%d.png",bullet_kind+2,firer_view_chair_id+1,1);
		frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
	}

	if (!Sprite::initWithSpriteFrame(frame))
		return false;

	mBulletKind = bullet_kind;
	mBoudingRadius = bounding_radius;
	mFirerChairId = firer_view_chair_id;
	mBulletSpeed = bullet_speed;

	String* pathStr = nullptr;
	std::string  dir = "";
	int gameKindId = SessionManager::shareInstance()->getGameKind();
	bool isDouble = false;
	if (mBulletKind >= BULLET_2_DOUBLE)
	{
		isDouble =true;
	}

	if(gameKindId == GameKindJcpy ||(gameKindId !=GameKindJcpy && isDouble))
	{

		Vector<SpriteFrame*> frames;
		for (int i=0;i<2;i++)
		{
			String* imageName = nullptr;
			if(isDouble)
				 imageName = String::createWithFormat("bullet%d_double_%d.png",mBulletKind-1,i+1);
			else
				 imageName = String::createWithFormat("bullet%d_norm%d_%d.png",mBulletKind+2,mFirerChairId+1,i+1);

		    frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
			frames.pushBack(frame);
		}
		Animation* anima = Animation::createWithSpriteFrames(frames,0.14);
		Animate* anit = Animate::create(anima);
		runAction(RepeatForever::create(anit));
	}
	else
	{
		//String* imageName = String::createWithFormat("bullet%d_norm%d.png",mBulletKind+2,mFirerChairId+1);
		//frame= SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());

		/*	Sprite* imgBullet = Sprite::create(imageName->getCString());
		mBulletWidth = imgBullet->getContentSize().width;
		mBulletHeight = imgBullet->getContentSize().height;
		SpriteFrame* frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(0,0,mBulletWidth,mBulletHeight));
		setDisplayFrame(frame);*/
	}
	return true;
}

void GameBullet::reInit(BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed)
{
	stopAllActions();
	mReuseFlag = true;
	mStatus = MOVE;
	mBulletKind = bullet_kind;
	mBoudingRadius = bounding_radius;
	mFirerChairId = firer_view_chair_id;
	mBulletSpeed = bullet_speed;

	String* pathStr = nullptr;
	std::string  dir = "";
	int gameKindId = SessionManager::shareInstance()->getGameKind();
	switch (gameKindId)
	{
	case GameKindDntg:
		{
			dir = "dntg2/images";
		}
		break;
	case GameKindLkpy:
		{
			dir = "GameResources/lkpy";
		}
		break;
	case GameKindJcpy:
		{
			dir = "GameResources/jcpy";
		}
		break;
	}
	bool isDouble = false;
	if (mBulletKind >= BULLET_2_DOUBLE)
	{
		isDouble =true;
		pathStr = String::createWithFormat("%s/bullet/bullet%d_double.png",dir.c_str(),mBulletKind-1);
	}
	else
	{
		pathStr = String::createWithFormat("%s/bullet/bullet%d_norm%d.png",dir.c_str(),mBulletKind+2,mFirerChairId+1);
	}


	if(gameKindId == GameKindJcpy ||(gameKindId !=GameKindJcpy && isDouble))
	{
		Sprite* imgBullet = Sprite::create(pathStr->getCString());
		mBulletWidth = imgBullet->getContentSize().width/2;
		mBulletHeight = imgBullet->getContentSize().height;

		Vector<SpriteFrame*> frames;
		for (int i=0;i<2;i++)
		{
			SpriteFrame* frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(i*mBulletWidth,0,mBulletWidth,mBulletHeight));
			frames.pushBack(frame);
			if(i==0 && frame)
				setDisplayFrame(frame);
		}
		Animation* anima = Animation::createWithSpriteFrames(frames,0.14);
		Animate* anit = Animate::create(anima);
		runAction(RepeatForever::create(anit));
	}
	else
	{
		Sprite* imgBullet = Sprite::create(pathStr->getCString());
		mBulletWidth = imgBullet->getContentSize().width;
		mBulletHeight = imgBullet->getContentSize().height;
		SpriteFrame* frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(0,0,mBulletWidth,mBulletHeight));
		setDisplayFrame(frame);
	}
}

void GameBullet::update(float delta)
{
	if (mStatus ==MOVE )
	{
		mBulletAction->step(delta);
	}
}

void GameBullet::render() {
	Vec2 pos = mBulletAction->getPosition();
	if (mStatus == MOVE) {
		setPosition(pos);
		setRotation(CC_RADIANS_TO_DEGREES(mBulletAction->getAngle()));
	}
}

//爆炸效果
void GameBullet::Explode()
{
	mStatus = EXPLODE;
	stopAllActions();
	String* pathStr = nullptr;
	bool isDouble = false;
	if (mBulletKind >= BULLET_2_DOUBLE)
		isDouble = true;

	/*if(isDouble)
		pathStr = String::createWithFormat("common/bullet/double_explode.png");
	else
		pathStr = String::createWithFormat("common/bullet/explode.png");

	Vector<SpriteFrame*> frames;
	if (isDouble)
	{
		for (int i=0;i<2;i++)
		{
			SpriteFrame* frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(i*259,0,259,259));
			frames.pushBack(frame);
		}
	}
	else
	{
		for (int i=0;i<2;i++)
		{
			for (int j=0;j<5;j++)
			{
				SpriteFrame* frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(j*360,i*360,360,360));
				frames.pushBack(frame);
			}
		}
	}*/

	
	Vector<SpriteFrame*> frames;
	String* imageName = nullptr;

	if (isDouble)
	{
		for (int i=0;i<2;i++)
		{
			imageName =  String::createWithFormat("double_explode_%d.png",i+1);
			SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
			frames.pushBack(frame);
		}
	}
	else
	{
		for (int i=0;i<10;i++)
		{
			imageName = String::createWithFormat("explode%d.png",i+1);
			SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
			frames.pushBack(frame);
		}
	}
	Vector<FiniteTimeAction*> actions;
	float interval = 1.0f/60.f;
	Animation* anima = Animation::createWithSpriteFrames(frames,interval);
	Animate* anit = Animate::create(anima);
	actions.pushBack(anit);
	FadeOut* fo = FadeOut::create(0.7f);
	actions.pushBack(fo);
	//RemoveSelf* rs = RemoveSelf::create();
	CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(GameBullet::freeBullet));
	actions.pushBack(pCall);
	runAction(Sequence::create(actions));

	//播放音效
	//SoundManager::playFishEffect(CommonFunction::getEffectName("bullet_baozha.mp3").c_str(),0.5);

}

void GameBullet::freeBullet(Node* node)
{
	mStatus = DESTROY; 
}

void GameBullet::setBulletAction(ActionBulletMove* action)
{
	if (mBulletAction && mBulletAction != action)
	{
		CC_SAFE_RELEASE_NULL(mBulletAction);
	}
	mBulletAction = action;
	mBulletAction->retain();

	//设置子弹的角度 modify at 2015 -9 - 26
	setRotation(CC_RADIANS_TO_DEGREES(mBulletAction->getAngle()));
}


//////////////////////////////////////////////////////////////////////////

GameBulletManager::GameBulletManager()
{
	mBulletArray = __Array::create();
	mBulletArray->retain();
}

GameBulletManager::~GameBulletManager()
{
	 FreeAllBullet();
	 CC_SAFE_RELEASE_NULL(mBulletArray);
}

GameBulletManager* GameBulletManager::shareInstance()
{
	if (__instance == nullptr)
	{
		__instance = new GameBulletManager();
	}
	return __instance;
}

void GameBulletManager::purgeInstance()
{
	CC_SAFE_RELEASE_NULL(__instance);
}

bool GameBulletManager::init()
{
	return true;
}

void GameBulletManager::update(float delta_time)
{
	GameBullet::BulletStatus status_;
	for (int i=0 ; i < mBulletArray->count();i++)
	{
		GameBullet* pBullet = (GameBullet*)mBulletArray->getObjectAtIndex(i);
		status_ = pBullet->getStatus();
		if(status_ == GameBullet::DESTROY)
		{
			pBullet->removeFromParent();
			mBulletArray->removeObjectAtIndex(i,false);
		}
		else
		{
			pBullet->update(delta_time);
			if(status_ == GameBullet::MOVE && GameFishManager::sharedInstance()->BulletHitTest(pBullet))
			{
				pBullet->Explode();
			}
		}
	}
}

void GameBulletManager::render()
{
	for (int i=0 ; i < mBulletArray->count();i++)
	{
		GameBullet* pBullet = (GameBullet*)mBulletArray->getObjectAtIndex(i);
		if(pBullet->getStatus() == GameBullet::MOVE)
			pBullet->render();
	}
}



bool GameBulletManager::FreeBullet(GameBullet* bullet) {
	
	for (int i=0 ; i < mBulletArray->count();i++)
	{
		GameBullet* pBullet = (GameBullet*)mBulletArray->getObjectAtIndex(i);
		if(pBullet == bullet)
		{
			mBulletArray->removeObjectAtIndex(i,false);
			pBullet->removeFromParent();
			return true;
		}
	}
	return false;
}

void GameBulletManager::FreeAllBullet() {
	for (int i=0 ; i < mBulletArray->count();i++)
	{
		GameBullet* pBullet = (GameBullet*)mBulletArray->getObjectAtIndex(i);
		if(pBullet )
		{
			mBulletArray->removeObjectAtIndex(i,false);
			pBullet->removeFromParent();
		}
	}

}

GameBullet* GameBulletManager::GetBullet(DWORD bullet_id) {
	
	for (int i=0 ; i < mBulletArray->count();i++)
	{
		GameBullet* pBullet = (GameBullet*)mBulletArray->getObjectAtIndex(i);
		if(pBullet->getBulletID() == bullet_id)
		{
			return pBullet;
		}
	}
	return NULL;
}


void GameBulletManager::Explode(DWORD bullet_id) {
	for (int i=0 ; i < mBulletArray->count();i++)
	{
		GameBullet* pBullet = (GameBullet*)mBulletArray->getObjectAtIndex(i);
		if(pBullet->getBulletID() == bullet_id && pBullet->getStatus() == GameBullet::MOVE)
		{
			pBullet->Explode();
		}
	}
}

GameBullet* GameBulletManager::Fire(const Vec2& fire_pos, float angle, BulletKind bullet_kind, float bounding_radius, WORD firer_view_chair_id, float bullet_speed, int lock_fish_id, ActionInterval* action_fish_move) {
	assert(firer_view_chair_id < GAME_PLAYER);
	if (firer_view_chair_id >= GAME_PLAYER)
		return NULL;
	GameBullet* bullet = GameBullet::create(bullet_kind, bounding_radius, firer_view_chair_id, bullet_speed);

	ActionBulletMove* action_bullet = NULL;
	
	action_bullet = ActionBulletMove::create(fire_pos, angle, bullet_speed);
	//初始化初始位置
	bullet->setPosition(action_bullet->getPosition());
	bullet->setRotation(angle);
	bullet->setBulletAction(action_bullet);

	mBulletArray->addObject(bullet);
	
	return bullet;
}

float GameBulletManager::BulletMoveDuration(const Vec2& start, const Vec2& end, float bullet_speed) {
	Vec2 delta = end - start;
	float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	return length / bullet_speed;
}


