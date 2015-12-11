
#include "GameMatchManager.h"
#include "CommonFunction.h"
#include "GameScene.h"
#include "SessionManager.h"



#define  timeMatchPos 150
#define  forverMatchPost 100
#define  TextInterval 2

GameMatchManager::GameMatchManager()
{
	mDelegate = nullptr;
	mData = nullptr;
	mTimeMatchNode = nullptr;
	mTimeNode = nullptr;
	mForeverMatchNode = nullptr;

	mRankText = nullptr;
	mCurScoreText= nullptr;
	mMaxScoreText= nullptr;
	mTotalScoreText= nullptr;
	mPalyCountText= nullptr;
	mMatchTimeText= nullptr;

	mTimeSpan = -1;
}

GameMatchManager::~GameMatchManager()
{
	
}

GameMatchManager* GameMatchManager::create(GameScene* pDelegate)
{
	GameMatchManager* pManager = new GameMatchManager();
	if (pManager && pManager->init(pDelegate))
	{
		pManager->autorelease();
		return pManager;
	}

	delete pManager;
	pManager = nullptr;
	return nullptr;
}

bool GameMatchManager::init(GameScene* pDelegate)
{
	mDelegate = pDelegate;

	
	return true;
}

void GameMatchManager::onEnter()
{
	Node::onEnter();
	//启定时器
	schedule(schedule_selector(GameMatchManager::updateMatchRoom), 0.5f);
}

void GameMatchManager::onExit()
{
	unschedule(schedule_selector(GameMatchManager::updateMatchRoom));
	Node::onExit();
}

void GameMatchManager::updateMatchRoom(float dt)
{
	mData = mDelegate->getGameMatchModel();
	std::string text_str = "";
	if (mData->getMatchType() == 0  )//循环赛
	{

		if (!mForeverMatchNode)
		{
			mForeverMatchNode = Node::create();
			mForeverMatchNode->setPosition(Vec2(kScreenWidth - 100 ,kScreenHeight -200));
			mDelegate->addChild(mForeverMatchNode,999);

			ui::Scale9Sprite* bg = ui::Scale9Sprite::create("match/game_math_bg.png");
			bg->setContentSize(Size(200,122));
			mForeverMatchNode->addChild(bg);

			Text* pTitle = nullptr;
			pTitle = Text::create();
			pTitle->setFontSize(24);
			pTitle->setColor(ccc3(255,255,255));
			pTitle->setAnchorPoint(Vec2(0.5,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8(mData->getMatchTitle().c_str()));
			pTitle->setPosition(Vec2(bg->getContentSize().width/2 ,bg->getContentSize().height - pTitle->getContentSize().height/2 - 2));
			bg->addChild(pTitle);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("当前排名:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 40));
			bg->addChild(pTitle);

			mRankText = Text::create();
			mRankText->setFontSize(20);
			mRankText->setColor(ccc3(255,255,0));
			mRankText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%d/%d",mData->getSelfRank(),mData->getHaveAllUser())->getCString();
			mRankText->setText(text_str);
			mRankText->setPosition(Vec2(100,bg->getContentSize().height - 40));
			bg->addChild(mRankText);

			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("个人积分:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 62));
			bg->addChild(pTitle);

			mCurScoreText = Text::create();
			mCurScoreText->setFontSize(20);
			mCurScoreText->setColor(ccc3(255,255,0));
			mCurScoreText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%lld",mData->getMyCurScore())->getCString();
			mCurScoreText->setText(text_str);
			mCurScoreText->setPosition(Vec2(100,bg->getContentSize().height - 62));
			bg->addChild(mCurScoreText);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("最高分 :"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 84));
			bg->addChild(pTitle);

			mMaxScoreText = Text::create();
			mMaxScoreText->setFontSize(20);
			mMaxScoreText->setColor(ccc3(255,255,0));
			mMaxScoreText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%lld",mData->getTotalMaxScore())->getCString();
			mMaxScoreText->setText(text_str);
			mMaxScoreText->setPosition(Vec2(100,bg->getContentSize().height - 84));
			bg->addChild(mMaxScoreText);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("剩余时间:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 106));
			bg->addChild(pTitle);


			mMatchTimeText = Text::create();
			mMatchTimeText->setFontSize(20);
			mMatchTimeText->setColor(ccc3(255,255,0));
			mMatchTimeText->setAnchorPoint(Vec2(0,0.5));
			mMatchTimeText->setText(CommonFunction::covertTimeSpanStr(mData->getMatchTime()));
			mMatchTimeText->setPosition(Vec2(100,bg->getContentSize().height - 106));
			bg->addChild(mMatchTimeText);
		}
		else
		{
			text_str = __String::createWithFormat("%d/%d",mData->getSelfRank(),mData->getHaveAllUser())->getCString();
			mRankText->setText(text_str);

			text_str = __String::createWithFormat("%lld",mData->getMyCurScore())->getCString();
			mCurScoreText->setText(text_str);

			text_str = __String::createWithFormat("%lld",mData->getTotalMaxScore())->getCString();
			mMaxScoreText->setText(text_str);

			mMatchTimeText->setText(CommonFunction::covertTimeSpanStr(mData->getMatchTime()));
		}
	}
	else//定时赛
	{

		if (!mTimeMatchNode)
		{
			mTimeMatchNode = Node::create();
			mTimeMatchNode->setPosition(Vec2(kScreenWidth - 140 ,kScreenHeight -220));
			mDelegate->addChild(mTimeMatchNode,999);

			ui::Scale9Sprite* bg = ui::Scale9Sprite::create("match/game_math_bg.png");
			bg->setContentSize(Size(280,170));
			mTimeMatchNode->addChild(bg);


			Text* pTitle = nullptr;
			pTitle = Text::create();
			pTitle->setFontSize(24);
			pTitle->setColor(ccc3(255,255,255));
			pTitle->setAnchorPoint(Vec2(0.5,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8(mData->getMatchTitle().c_str()));
			pTitle->setPosition(Vec2(bg->getContentSize().width/2 ,bg->getContentSize().height - 3 - pTitle->getContentSize().height/2));
			bg->addChild(pTitle);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("当前排名:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 40));
			bg->addChild(pTitle);

			mRankText = Text::create();
			mRankText->setFontSize(20);
			mRankText->setColor(ccc3(255,255,0));
			mRankText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%d/%d",mData->getSelfRank(),mData->getHaveAllUser())->getCString();
			mRankText->setText(text_str);
			mRankText->setPosition(Vec2(timeMatchPos,bg->getContentSize().height - 40));
			bg->addChild(mRankText);

			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("个人当前积分:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 62));
			bg->addChild(pTitle);

			mCurScoreText = Text::create();
			mCurScoreText->setFontSize(20);
			mCurScoreText->setColor(ccc3(255,255,0));
			mCurScoreText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%lld",mData->getMyCurScore())->getCString();
			mCurScoreText->setText(text_str);
			mCurScoreText->setPosition(Vec2(timeMatchPos,bg->getContentSize().height - 62));
			bg->addChild(mCurScoreText);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("个人最高积分:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 84));
			bg->addChild(pTitle);

			mMaxScoreText = Text::create();
			mMaxScoreText->setFontSize(20);
			mMaxScoreText->setColor(ccc3(255,255,0));
			mMaxScoreText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%lld",mData->getMyMaxScore())->getCString();
			mMaxScoreText->setText(text_str);
			mMaxScoreText->setPosition(Vec2(timeMatchPos,bg->getContentSize().height - 84));
			bg->addChild(mMaxScoreText);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("本场最高积分 :"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 106));
			bg->addChild(pTitle);

			mTotalScoreText = Text::create();
			mTotalScoreText->setFontSize(20);
			mTotalScoreText->setColor(ccc3(255,255,0));
			mTotalScoreText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%lld",mData->getTotalMaxScore())->getCString();
			mTotalScoreText->setText(text_str);
			mTotalScoreText->setPosition(Vec2(timeMatchPos,bg->getContentSize().height - 106));
			bg->addChild(mTotalScoreText);

			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("挑战次数:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 128));
			bg->addChild(pTitle);

			mPalyCountText = Text::create();
			mPalyCountText->setFontSize(20);
			mPalyCountText->setColor(ccc3(255,255,0));
			mPalyCountText->setAnchorPoint(Vec2(0,0.5));
			text_str = __String::createWithFormat("%d/%d",mData->getPlayCount(),mData->getAllPlayCount())->getCString();
			mPalyCountText->setText(text_str);
			mPalyCountText->setPosition(Vec2(timeMatchPos,bg->getContentSize().height - 128));
			bg->addChild(mPalyCountText);


			pTitle = Text::create();
			pTitle->setFontSize(20);
			pTitle->setColor(ccc3(255,255,0));
			pTitle->setAnchorPoint(Vec2(0,0.5));
			pTitle->setText(CommonFunction::GBKToUTF8("剩余时间:"));
			pTitle->setPosition(Vec2(10,bg->getContentSize().height - 150));
			bg->addChild(pTitle);


			mMatchTimeText = Text::create();
			mMatchTimeText->setFontSize(20);
			mMatchTimeText->setColor(ccc3(255,255,0));
			mMatchTimeText->setAnchorPoint(Vec2(0,0.5));
			mMatchTimeText->setText(CommonFunction::covertTimeSpanStr(mData->getMatchTime()));
			mMatchTimeText->setPosition(Vec2(timeMatchPos,bg->getContentSize().height - 150));
			bg->addChild(mMatchTimeText);
		}
		else
		{
			text_str = __String::createWithFormat("%d/%d",mData->getSelfRank(),mData->getHaveAllUser())->getCString();
			mRankText->setText(text_str);

			text_str = __String::createWithFormat("%lld",mData->getMyCurScore())->getCString();
			mCurScoreText->setText(text_str);

			text_str = __String::createWithFormat("%lld",mData->getMyMaxScore())->getCString();
			mMaxScoreText->setText(text_str);

			text_str = __String::createWithFormat("%lld",mData->getTotalMaxScore())->getCString();
			mTotalScoreText->setText(text_str);

			text_str = __String::createWithFormat("%d/%d",mData->getPlayCount(),mData->getAllPlayCount())->getCString();
			mPalyCountText->setText(text_str);

			mMatchTimeText->setText(CommonFunction::covertTimeSpanStr(mData->getMatchTime()));
		}
	}

	//开赛或结束倒计时
	if (!mTimeNode)
	{
		mTimeNode = Node::create();

		mTimeNode->setPosition(Vec2(kScreenWidth/2 ,kScreenHeight/2));
		mDelegate->addChild(mTimeNode,999);

		ui::Scale9Sprite* bg = ui::Scale9Sprite::create("match/game_math_bg.png");
		bg->setContentSize(Size(372,170));
		mTimeNode->addChild(bg);

		mStartOrEnd = ImageView::create();
		mStartOrEnd->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height - 45));
		bg->addChild(mStartOrEnd);

		mStartOrEndTimeLable =  LabelAtlas::create("1:","match_time_num.png",22,24,'0');
		mStartOrEndTimeLable->setAnchorPoint(Vec2(0.5,0.5));
		mStartOrEndTimeLable->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2-20));
		mStartOrEndTimeLable->setVisible(false);
		bg->addChild(mStartOrEndTimeLable);
		
		mRedTimeLable =  LabelAtlas::create("0","match_red_num.png",44,64,'0');
		mRedTimeLable->setAnchorPoint(Vec2(0.5,0.5));
		mRedTimeLable->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2-20));
		mRedTimeLable->setVisible(false);
		bg->addChild(mRedTimeLable);

	}

	if (!mTimeNode->isVisible())
	{
		mTimeNode->setVisible(true);
	}

	if(mData->getCurMatchStatus() == MG_SIGNUPING)
	{
		mStartOrEnd->loadTexture("match/image_match_ready.png",ui::Widget::TextureResType::LOCAL);
	}
	else if(mData->getCurMatchStatus() == MG_MATCHING)
	{
		mStartOrEnd->loadTexture("match/image_match_end.png",ui::Widget::TextureResType::LOCAL);
	}

	if (mData->getCurMatchStatus() == MG_SIGNUPING)
	{
		if (mData->getMatchTime() <=10)
		{
			mStartOrEndTimeLable->setVisible(false);
			mRedTimeLable->setString(numberToString(mData->getMatchTime()));
			if (!mRedTimeLable->isVisible())
			{
				mRedTimeLable->setVisible(true);
			}
			if (mData->getMatchTime() == 0)
			{
				mTimeNode->setVisible(false);
			}
		}
		else
		{
			mRedTimeLable->setVisible(false);
			mStartOrEndTimeLable->setString(CommonFunction::covertTimeSpanStr(mData->getMatchTime()));
			if (!mStartOrEndTimeLable->isVisible())
			{
				mStartOrEndTimeLable->setVisible(true);
			}
		}
	}else if (mData->getCurMatchStatus() == MG_MATCHING)
	{
		if (mData->getMatchTime() <=10)
		{
			mRedTimeLable->setString(numberToString(mData->getMatchTime()));
			if (!mRedTimeLable->isVisible())
			{
				mRedTimeLable->setVisible(true);
			}

			if (mData->getMatchTime() == 0)
			{
				mTimeNode->setVisible(false);
			}
		}
		else
		{
			mTimeNode->setVisible(false);
		}
	}

}
