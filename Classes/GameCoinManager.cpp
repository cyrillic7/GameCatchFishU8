
#include "GameCoinManager.h"
#include "CommonFunction.h"
#include "GameScene.h"
#include "SessionManager.h"

const float kCoinMoveDistance = 10.f * 30 * 4 /3;

static CoinManager* __instance = nullptr;

CoinManager::CoinManager() {
 mCoinWidth = 0;
 mGoldCoinWidth = 0;
}

CoinManager::~CoinManager() {
 
}



CoinManager* CoinManager::shareInstance()
{
	if (__instance == nullptr)
	{
		__instance = new CoinManager;
		__instance->init();
	}

	return __instance;
}

void CoinManager::purgeInstance() {
	CC_SAFE_DELETE(__instance);
}

void CoinManager::init()
{

}

void CoinManager::BuildCoins(const Vec2& fish_pos, float fish_angle, const Vec2& turret_pos, SCORE score, FishKind fish_kind,GameScene* scene) {
  const int kCoinCountEnum[] = { 0, 100, 1000, 10000, 100000 };
  const int kCointCount[] = { 2, 3, 4, 5, 6 };//{ 1, 2, 3, 4, 5 };
  float coin_width;

  bool isGoldCoinFrame = false;

  Sprite* coinSp = Sprite::createWithSpriteFrameName("yinbi_1.png");
  mCoinWidth = coinSp->getContentSize().width;
  coinSp = Sprite::createWithSpriteFrameName("jinbi_1.png");
  mGoldCoinWidth =  coinSp->getContentSize().width;

  switch (SessionManager::shareInstance()->getGameKind())
  {
  case GameKindDntg:
	  {
		  if (fish_kind < FISH_XIAOCIYU) {
			  coin_width = mCoinWidth;
		  } else {
			  coin_width = mGoldCoinWidth;
			  isGoldCoinFrame = true;
		  }
	  }
	  break;
  case GameKindLkpy:
	  {
		  if (fish_kind < FISH_KIND_7) {
			  coin_width = mCoinWidth;
		  } else {
			  coin_width = mGoldCoinWidth;
			  isGoldCoinFrame = true;
		  }
	  }
	  break;
  case GameKindJcpy:
	  {
		  if (fish_kind < FISH_KIND_JC7) {
			  coin_width = mCoinWidth;
		  } else {
			  coin_width = mGoldCoinWidth;
			  isGoldCoinFrame = true;
		  }
	  }
	
  }
  
  int coin_count = 0;
  for (int i = sizeof(kCoinCountEnum)/sizeof(int) - 1; i >= 0; --i) {
    if (score >= kCoinCountEnum[i]) {
      if (i == sizeof(kCoinCountEnum)/sizeof(int) - 1) {
        //coin_count = kCointCount[i] + int((score - kCoinCountEnum[i]) / 10000);
		coin_count = kCointCount[i] + int((score - kCoinCountEnum[i]) /10000);
        coin_count = MIN(20, coin_count);
      } else {
        coin_count = kCointCount[i];
      }
      break;
    }
  }
 
  float angle = CommonFunction::CalcAngle(fish_pos.x,fish_pos.y,turret_pos.x,turret_pos.y);
  float radius = CommonFunction::CalcDistance(turret_pos.x, turret_pos.y, fish_pos.x, fish_pos.y);

  Vec2 center = Vec2(turret_pos.x, turret_pos.y);
  if (coin_count > 10) {
	  center.x = turret_pos.x + coin_width * sinf(angle);
	  center.y = turret_pos.y + coin_width * cosf(angle);
	  radius = (10 - 1) * coin_width / 2;
  }

	//当前鱼与炮台的夹角
	Vec2 start;
	for (int i = 0; i < coin_count; ++i) {
		if (radius < 0.f) {
			start.x = center.x + radius * sinf(angle);
			start.y = center.y + radius * cosf(angle);
		} else {
			start.x = center.x + radius * sinf(angle);
			start.y = center.y + radius * cosf(angle);
		}
		float distance = CommonFunction::CalcDistance(turret_pos.x, turret_pos.y, start.x, start.y);
    
		Sprite* coin =nullptr;
		if(isGoldCoinFrame)
			coin = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("jinbi_1.png"));
		else
			coin = Sprite::createWithSpriteFrame(SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("yinbi_1.png"));
		coin->setPosition(start);
		coin->setRotation(CC_RADIANS_TO_DEGREES(angle+M_PI_2));
		scene->addCoin(coin);
		MoveTo* mt = MoveTo::create(distance / kCoinMoveDistance,turret_pos);
  
		SpriteFrame* frame = nullptr;
		Animation* anima = nullptr;
		Vector<SpriteFrame*>  frames;
		for (int i=0;i<12;i++)
		{
			String* imageName = nullptr;
			if(isGoldCoinFrame)
				imageName = String::createWithFormat("jinbi_%d.png",i+1);
			else
				imageName = String::createWithFormat("yinbi_%d.png",i+1);
			frame= SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imageName->getCString());
			if (frame)
			{
				frames.pushBack(frame);
			}
		}
		anima = Animation::createWithSpriteFrames(frames,0.05f);

		//if(!isGoldCoinFrame)
		//{
		//	//初始化银币帧序列
		//	
		//	for (int i=0;i<12;i++)
		//	{
		//		frame= SpriteFrame::create("common/players/yinbi.png",CCRectMake(i*mCoinWidth,0,mCoinWidth,mCoinWidth));
		//		if (frame)
		//		{
		//			frames.pushBack(frame);
		//		}
		//	}
		//	anima = Animation::createWithSpriteFrames(frames,0.05f);
		//}
		//
		//else
		//{
		//	//初始化金币帧序列
		//	for (int i=0;i<12;i++)
		//	{
		//		frame = SpriteFrame::create("common/players/jinbi.png",CCRectMake(i*mGoldCoinWidth,0,mGoldCoinWidth,mGoldCoinWidth));
		//		if (frame)
		//		{
		//			frames.pushBack(frame);
		//		}
		//	}
		//	anima =  Animation::createWithSpriteFrames(frames,0.05f);
		//}
		
		Animate* anit = Animate::create(anima);
		coin->runAction(RepeatForever::create(anit));
		RemoveSelf* rs = RemoveSelf::create(true);
		Sequence* seq = Sequence::createWithTwoActions(mt,rs);
		coin->runAction(seq);
  
		radius -= coin_width;
		if (i == 10) {
			center.x = turret_pos.x + coin_width * sinf(angle);
			center.y = turret_pos.y + coin_width * cosf(angle);
			radius = (coin_count - 10 - 1) * coin_width / 2;
		}
	}
}
