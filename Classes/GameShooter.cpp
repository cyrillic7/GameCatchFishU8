#include "GameShooter.h"
#include "GameBulletManager.h"
#include "CommonFunction.h"
#include "Message.h"
#include "GameJettonManager.h"
#include "SessionManager.h"

// 后座偏移量
const float kBackblowExcusion = 8.0f;


GameShooter::GameShooter()
{
	mAnger = 0;
	mViewChairId = -1;
	mGunSp = nullptr;
	mFireSp = nullptr;
	mScoreLable = nullptr;
	mBulletMulriple = 0;
	mMulripleLable = nullptr;
	mScore = 0;

	mBtnPlus = nullptr;
	mBtnSub = nullptr;
	mMinMulriple = 0;
	mMaxMulriple = 0;
	mIsMatchRoom = false;
}

GameShooter::~GameShooter()
{
	CC_SAFE_RELEASE_NULL(mGunSp);
	CC_SAFE_RELEASE_NULL(mFireSp);
	CC_SAFE_RELEASE_NULL(mScoreLable);
}

GameShooter* GameShooter::create(WORD myView_chair_id,BulletKind bKind,float angle,DWORD bulletMulriple,bool isMe)
{
	GameShooter* pShooter = new GameShooter();
	if (pShooter && pShooter->init(myView_chair_id,bKind,angle,bulletMulriple,isMe))
	{
		pShooter->autorelease();
		return pShooter;
	}
	CC_SAFE_DELETE(pShooter);
	return nullptr;
}

bool GameShooter::init(WORD myView_chair_id,BulletKind bKind,float angle,DWORD bulletMulriple,bool isMe)
{
	String* pathStr = nullptr;
	if (!Layer::init())
		return false;
	mViewChairId = myView_chair_id;
	mBulletKind  = bKind;
	mAnger       = angle;

	
	pathStr = String::createWithFormat("%s/players/turret%d.png",getResDir().c_str(),myView_chair_id);
	//炮架
	Sprite* turret = Sprite::create(pathStr->getCString());
	addChild(turret);
	
	//炮
	if (mBulletKind >= BULLET_2_DOUBLE)
	{
		pathStr = String::createWithFormat("%s/players/gun%02d_double.png",getResDir().c_str(),mBulletKind - BULLET_2_DOUBLE + 2);
	}
	else
	{
		pathStr = String::createWithFormat("%s/players/gun%02d_normal.png",getResDir().c_str(),mBulletKind + 2);
	}
	mGunSp  = Sprite::create(pathStr->getCString());
	mGunSp->setRotation(CC_RADIANS_TO_DEGREES(mAnger));
	mGunSp->retain();
	addChild(mGunSp);
	//炮台
	pathStr = String::createWithFormat("%s/players/turret_front.png",getResDir().c_str());
	Sprite* tower = Sprite::create(pathStr->getCString());
	tower->setPosition(Vec2(0,-tower->getContentSize().height/2*std::cos(mAnger)));
	tower->setRotation(CC_RADIANS_TO_DEGREES(mAnger));
	addChild(tower);



	//分数背景
	mScoreBg = Sprite::create(String::createWithFormat("%s/players/money_back%d.png",getResDir().c_str(),mViewChairId)->getCString());
	mScoreBg->setPosition(Vec2((tower->getContentSize().width + mScoreBg->getContentSize().width/2+20)*std::cos(mAnger),0));
	mScoreBg->setRotation(CC_RADIANS_TO_DEGREES(mAnger));
	addChild(mScoreBg);


	pathStr = String::createWithFormat("%s/players/money_num.png",getResDir().c_str());
	if (SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
		mScoreLable = LabelAtlas::create("0",pathStr->getCString(),19,23,'0');
	else
		mScoreLable = LabelAtlas::create("0",pathStr->getCString(),24,28,'0');
	mScoreLable->setPosition(mScoreBg->getPosition());
	mScoreLable->setAnchorPoint(Vec2(0.5,0.5));
	mScoreLable->setRotation(CC_RADIANS_TO_DEGREES(mAnger));
	mScoreLable->retain();
	addChild(mScoreLable);


	//炮力
	mMulripleLable = LabelAtlas::create("0","common/players/turret_lv_num.png",18,22,'0');
	mMulripleLable->setString(numberToString(bulletMulriple));
	mMulripleLable->setPosition(Vec2(0,-20*std::cos(mAnger)));
	mMulripleLable->setAnchorPoint(Vec2(0.5,0.5));
	mMulripleLable->setRotation(CC_RADIANS_TO_DEGREES(mAnger));
	addChild(mMulripleLable);


	//add btn
	mIsMatchRoom = isMe;
	if(isMe)
	{
		 
		mBtnPlus = Button::create();
		mBtnPlus->loadTextureNormal("btn_plus.png",ui::Widget::TextureResType::LOCAL);
		mBtnPlus->loadTexturePressed("btn_plus_on.png",ui::Widget::TextureResType::LOCAL);
	
		mBtnPlus->setPosition(Vec2(tower->getContentSize().width-20,0));
		if (SessionManager::shareInstance()->getGameKind() == GameKindDntg)
		{
			mBtnPlus->setPosition(Vec2(tower->getContentSize().width-25,0));
		}
		mBtnPlus->setScale(0.8f);
		mBtnPlus->addTouchEventListener(CC_CALLBACK_2(GameShooter::onClickPlus,this));
		mBtnPlus->setTouchSize(Size(80,80));
		addChild(mBtnPlus);

		//sub btn
		mBtnSub  = Button::create();
		mBtnSub->loadTextureNormal("btn_sub.png",ui::Widget::TextureResType::LOCAL);
		mBtnSub->setPosition(Vec2(-tower->getContentSize().width+20,0));
		if (SessionManager::shareInstance()->getGameKind() == GameKindDntg)
		{
			mBtnSub->setPosition(Vec2(-tower->getContentSize().width+25,0));
		}
		mBtnSub->setScale(0.8f);
		mBtnSub->loadTexturePressed("btn_sub_on.png",ui::Widget::TextureResType::LOCAL);
		mBtnSub->addTouchEventListener(CC_CALLBACK_2(GameShooter::onClickSub,this));
		mBtnSub->setTouchSize(Size(80,80));
		addChild(mBtnSub);
	}


	
	return true;
}

void GameShooter::onClickPlus(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (ui::Widget::TouchEventType::ENDED == eventType)
	{
		bool isMaxFalg = false;
		switch (mShootGrade)
		{
		case GameShooter::Grade_100:
			if (mBulletMulriple < mMaxMulriple)
			{
				mBulletMulriple +=10;
			}
			break;
		case GameShooter::Grade_1000:
			if (mBulletMulriple < mMaxMulriple)
			{
				mBulletMulriple +=100;	
			}
			break;
		case GameShooter::Grade_10000:
			if (mBulletMulriple < 10000)
			{
				mBulletMulriple +=1000;
			}
			break;
		}
		if (mBulletMulriple >= mMaxMulriple)
		{
			mBulletMulriple = mMaxMulriple;
			isMaxFalg = true;
		}
		setBulletMulriple(mBulletMulriple);
		Button* pButton = (Button*)pSender;
		if (isMaxFalg)
		{
			pButton->setTouchEnabled(false);
			CommonFunction::addGray(pButton);
		}
		else
		{
			if (!mBtnSub->isTouchEnabled())
			{
				mBtnSub->setTouchEnabled(true);
				CommonFunction::removeGray(mBtnSub);
			}
		}
	}
}

void GameShooter::onClickSub(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (ui::Widget::TouchEventType::ENDED == eventType)
	{
		bool isMinFalg = false;
		switch (mShootGrade)
		{
		case GameShooter::Grade_100:
			if (mBulletMulriple  > mMinMulriple)
			{
				mBulletMulriple -=10;
			}
			break;
		case GameShooter::Grade_1000:
			if (mBulletMulriple  >mMinMulriple)
			{
				mBulletMulriple -=100;
			}
			break;
		case GameShooter::Grade_10000:
			if (mBulletMulriple  > mMinMulriple)
			{
				mBulletMulriple -=1000;
			}
			break;
		}
		if (mBulletMulriple <= mMinMulriple)
		{
			mBulletMulriple = mMinMulriple;
			isMinFalg = true;
		}
		setBulletMulriple(mBulletMulriple);
		Button* pButton = (Button*)pSender;
		if (isMinFalg)
		{
			pButton->setTouchEnabled(false);
			CommonFunction::addGray(pButton);
		}
		else
		{
			if (!mBtnPlus->isTouchEnabled())
			{
				mBtnPlus->setTouchEnabled(true);
				CommonFunction::removeGray(mBtnPlus);
			}
		}
	}
}

std::string GameShooter::getResDir()
{
	std::string dir = "";
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
	return dir;
}
void GameShooter::switchGun(BulletKind kind)
{
	String* pathStr = nullptr;
	mBulletKind  = kind;
	//炮
	if (mBulletKind >= BULLET_2_DOUBLE)
	{
		pathStr = String::createWithFormat("%s/players/gun%02d_double.png",getResDir().c_str(),mBulletKind - BULLET_2_DOUBLE + 2);
	}
	else
	{
		pathStr = String::createWithFormat("%s/players/gun%02d_normal.png",getResDir().c_str(),mBulletKind + 2);
	}
	Sprite* sp = Sprite::create(pathStr->getCString());
	SpriteFrame* frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(0,0,sp->getContentSize().width,sp->getContentSize().height));
	mGunSp->setDisplayFrame(frame);
}

void GameShooter::setAngle(float angle)
{
	mAnger= CC_RADIANS_TO_DEGREES(angle);
	mGunSp->setRotation(mAnger);
	//及时改变FIRE 的位置和角度
	if (mFireSp)
	{
		int fireDistanceX = 60 ,fireDistanceY = 60;
		if (SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
		{
			fireDistanceX = 40 ;
			fireDistanceY = 40;
		}
		mFireSp->setPosition(Vec2(fireDistanceX * std::sin(CC_DEGREES_TO_RADIANS(mAnger)),fireDistanceY * std::cos(CC_DEGREES_TO_RADIANS(mAnger))));
		mFireSp->setRotation(mAnger);
	}
}

void GameShooter::setScore(SCORE score)
{
	mScore = score;
	mScoreLable->setString(__String::createWithFormat("%lld",score)->getCString());
	if (mScoreLable->getContentSize().width > mScoreBg->getContentSize().width-10)
	{
		float scalex = (mScoreBg->getContentSize().width-10)*10.0f/mScoreLable->getContentSize().width;
		mScoreLable->setScaleX(scalex/10.f);
	}
	
}

void GameShooter::addScore(SCORE score,bool coin_effect,bool bingo_effect)
{
	mScore +=score;
	if (CommonFunction::SwitchViewChairID(mViewChairId) == SessionManager::shareInstance()->getChairId())
	{
		if (mIsMatchRoom)//比赛房间除外
		{
			OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
			pUser->setUserScore(mScore);
		}
	}

	mScoreLable->setString(__String::createWithFormat("%lld",mScore)->getCString());
	if (mScoreLable->getContentSize().width > mScoreBg->getContentSize().width-10)
	{
		float scalex = (mScoreBg->getContentSize().width-10)*10.f/mScoreLable->getContentSize().width;
		mScoreLable->setScaleX(scalex/10.f);
	}
	if (coin_effect)
	{
		GameJettonManager::shareInstance()->addJetton(mViewChairId,score);
	}

	if (bingo_effect)
	{
		if(score>0)
		{
			__Dictionary* dic = __Dictionary::create();
			dic->setObject(__Integer::create(mViewChairId),"chair_id");
			dic->setObject(__String::createWithFormat("%lld",score),"score");
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(DealBingoEffectMsg,dic);
		}
	}
}

void GameShooter::fire()
{
	mGunSp->stopAllActions();
	Vec2 startPos = mGunSp->getPosition();
	Vec2 endPos =  Vec2(startPos.x - (kBackblowExcusion) * std::sin(CC_DEGREES_TO_RADIANS(mAnger)),startPos.y - (kBackblowExcusion)*std::cos(CC_DEGREES_TO_RADIANS(mAnger)));
	Vector<FiniteTimeAction*> actions;
	MoveTo* mt = MoveTo::create(0.05f,endPos);
	actions.pushBack(mt);
	DelayTime* delay = DelayTime::create(0.1f);
	actions.pushBack(delay);
	MoveTo* back = MoveTo::create(0.05f,Vec2(0,0));
	actions.pushBack(back);
	//CallFuncN* dealFire = CallFuncN::create(this,callfuncN_selector(GameShooter::dealFire));
	//actions.pushBack(dealFire);
	mGunSp->runAction(Sequence::create(actions));

	dealFire(mGunSp);
}

void GameShooter::dealFire(cocos2d::Node* pNode)
{
	if (!mFireSp)
	{
		mFireSp = Sprite::create();
		mFireSp->retain();
		addChild(mFireSp);
	}
	else
	{
		if (!mFireSp->isVisible())
		{
			mFireSp->setVisible(true);
		}
	}
	mFireSp->stopAllActions();
	int fireDistanceX = 60 ,fireDistanceY = 60;
	if (SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
	{
		fireDistanceX = 40 ;
		fireDistanceY = 40;
	}
	mFireSp->setPosition(Vec2(fireDistanceX * std::sin(CC_DEGREES_TO_RADIANS(mAnger)),fireDistanceY* std::cos(CC_DEGREES_TO_RADIANS(mAnger))));
	mFireSp->setRotation(mAnger);
	

	Vector<SpriteFrame*> frames;
	int col = 9;
	int row = 1;
	for(int i=0;i<col;i++)
	{
		for(int j=0;j<row;j++)
		{
			if(i*row+j < 9 )
			{
				SpriteFrame* frame = SpriteFrame::create("common/players/gun_fire.png",CCRectMake(i*140,j*140,140,140));
				if(i==0 && j==0)
				{
					mFireSp->setSpriteFrame(frame);
				}
				if (NULL != frame) {
					frames.pushBack(frame);
				}
			}
		}
	}

	Vector<FiniteTimeAction*> actions;
	//DelayTime* dt = DelayTime::create(0.02f);
	//actions.pushBack(dt);
	Animation* anima = Animation::createWithSpriteFrames(frames,0.3f/9);
	Animate* anit = Animate::create(anima);
	actions.pushBack(anit);
	CallFuncN* call = CallFuncN::create(this,callfuncN_selector(GameShooter::FireDealOver));
	actions.pushBack(call);
	mFireSp->runAction(Sequence::create(actions));
}

//开火结束
void GameShooter::FireDealOver(Node* pSender)
{
	mFireSp->setVisible(false);
}

void GameShooter::dealBingoEffect(SCORE score)
{

	Node* bingoNode = Node::create();

	Sprite* bingoSp = Sprite::create();
	Animate* anim = CommonFunction::createFrameAnimate("common/players/bingo.png",0.14f,2,5,212,212);
	
	LabelAtlas* LabelScore = LabelAtlas::create("0","common/players/bingo_num.png",40,48,'0');

	LabelScore->setString(__String::createWithFormat("%lld",score)->getCString());
	LabelScore->setAnchorPoint(Vec2(0.5f,0.5f));
	

	Vector<FiniteTimeAction*> labelactions;
	RotateTo* rb = RotateTo::create(0.4f,40);
	
	RotateTo* rb2 = RotateTo::create(0.8f, - 80);

	labelactions.pushBack(DelayTime::create(5.0f));
	labelactions.pushBack(FadeOut::create(1.5f));
	labelactions.pushBack(RemoveSelf::create(true));

	LabelScore->runAction(RepeatForever::create(Sequence::createWithTwoActions(rb,rb2)));
	LabelScore->runAction(Sequence::create(labelactions));

	bingoSp->runAction(RepeatForever::create(anim));

	Vector<FiniteTimeAction*> actions;
	DelayTime* delay = DelayTime::create(5.0f);
	actions.pushBack(delay);
	FadeOut*  fo     = FadeOut::create(1.5f);
	actions.pushBack((fo));
	CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameShooter::removeBingo));
	actions.pushBack(call);
	bingoSp->runAction(Sequence::create(actions));

	int dis =0;
	if (mViewChairId < 3 )
	{
		dis = -150;
	}
	else
	{
		dis = 150;
	}

	bingoNode->addChild(bingoSp);
	bingoNode->addChild(LabelScore);

	if (mViewChairId < 3)
		bingoNode->setRotation(CC_RADIANS_TO_DEGREES(M_PI));
	bingoNode->setPosition(Vec2(0,dis));
	addChild(bingoNode);

}


void GameShooter::removeBingo(Node* pSender)
{
	Node* parent = pSender->getParent();
	parent->removeFromParent();

}

void GameShooter::setBulletMulriple(int power)
{
	mMulripleLable->setString(numberToString(power));
	mBulletMulriple = power;
}

void GameShooter::setShootGrade(int roomLevel)
{
	switch (roomLevel)
	{
	case  RoomLevel_0:
		mShootGrade = Grade_100;
		break;
	case  RoomLevel_1:
		mShootGrade = Grade_1000;
		break;
	case  RoomLevel_2:
		mShootGrade = Grade_10000;
		break;
	}
}

void GameShooter::setMulripleArea(int min,int max)
{
	mMinMulriple = min;
	mMaxMulriple = max;
}