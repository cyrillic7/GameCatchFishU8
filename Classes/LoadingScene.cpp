#include "LoadingScene.h"
#include "SessionManager.h"
#include "CommonFunction.h"

LoadingScene::LoadingScene()
{
	mProgressBar = nullptr;
	mProgressIcon = nullptr;
}

LoadingScene::~LoadingScene()
{
	CC_SAFE_RELEASE_NULL(mProgressBar);
	CC_SAFE_RELEASE_NULL(mProgressIcon);
}

bool LoadingScene::init()
{
	if(!Scene::init())
		return false;

	return true;
}

void LoadingScene::onEnter()
{
	Scene::onEnter();

	loadUI();
}


void LoadingScene::onExit()
{
	Scene::onExit();
}

void LoadingScene::loadUI()
{
	int gameKind = SessionManager::shareInstance()->getGameKind();
	Sprite* bg = nullptr;
	switch (gameKind)
	{
	case GameKindDntg:
		bg = Sprite::create("loading/background.jpg");
		break;
	case GameKindLkpy:
		bg = Sprite::create("loading/background_lk.jpg");
		break;
	case GameKindJcpy:
		bg = Sprite::create("loading/background_jc.jpg");
		break;
	}
	
	bg->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2));
	addChild(bg);

	Sprite* progressBg = nullptr;
	if (gameKind == GameKindDntg)
	{
		progressBg = Sprite::create("loading/progress_bar_dntg_bg.png");
	}
	else
	{
		progressBg = Sprite::create("loading/progress_bar_bg.png");
	}
	progressBg->setPosition(Vec2(bg->getContentSize().width/2,100));
	addChild(progressBg);

	Sprite* progress =nullptr;
	if (gameKind == GameKindDntg)
	{
		progress = Sprite::create("loading/progress_bar_dntg.png");
	}
	else
	{
		progress = Sprite::create("loading/progress_bar.png");
	}
	mProgressBar = ProgressTimer::create(progress);
	mProgressBar->setPosition(progressBg->getPosition());
	mProgressBar->setPercentage(0);
	mProgressBar->setMidpoint(ccp(0, 0.5));       
	mProgressBar->setBarChangeRate(ccp(1, 0));
	mProgressBar->setType(ProgressTimer::Type::BAR);
	mProgressBar->retain();
	addChild(mProgressBar);

	if (gameKind == GameKindLkpy)
	{
		mProgressIcon = Sprite::create("loading/progress_cur_lk.png");
		mProgressIcon->setPosition(mProgressIcon->getContentSize().width/2,mProgressBar->getContentSize().height/2);
		mProgressIcon->retain();
		mProgressBar->addChild(mProgressIcon);
	}else if(gameKind == GameKindDntg)
	{
		mProgressIcon = Sprite::create("loading/progress_cur.png");
		mProgressIcon->setPosition(mProgressIcon->getContentSize().width/2,mProgressBar->getContentSize().height/2);
		mProgressIcon->retain();
		mProgressBar->addChild(mProgressIcon);
	}
	else if(gameKind== GameKindJcpy)
	{
		mProgressIcon = Sprite::create("loading/progress_cur_jc.png");
		mProgressIcon->setPosition(mProgressIcon->getContentSize().width/2,mProgressBar->getContentSize().height/2);
		mProgressIcon->retain();
		mProgressBar->addChild(mProgressIcon);

		std::vector<std::string> names;
		names.push_back("loading/progress_cur_jc.png");
		names.push_back("loading/progress_cur_jc2.png");

		Animate* pAction = CommonFunction::createFrameAnimate(names,0.2f);
		mProgressIcon->runAction(RepeatForever::create(pAction));
	}
}

void LoadingScene::setPercent(int persent)
{
	log("LoadingScene::setPercent = %d",persent);
	mProgressBar->setPercentage(persent);
	if (persent >= 100)
	{
		_eventDispatcher->dispatchCustomEvent(RemoveLoadingMsg);
		removeFromParent();
	}
	else
	{
		if (mProgressIcon)
		{
			mProgressIcon->setPosition(Vec2(persent*mProgressBar->getContentSize().width/100 - mProgressIcon->getContentSize().width/2 + 30 ,mProgressBar->getContentSize().height/2));
		}
	}
}