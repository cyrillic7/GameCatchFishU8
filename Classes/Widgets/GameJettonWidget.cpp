#include "GameJettonWidget.h"

const int kMaxLayerCount = 62;
const int kBaseScore = 10000;

GameJettonWidget::GameJettonWidget()
{
	mScore = 0;
	mIndex = 0;
	mTickCount = 0;
	m_isMov = false;
	mCurLayCnt = 0;
}

GameJettonWidget::~GameJettonWidget()
{
	stopAllActions();
}

GameJettonWidget* GameJettonWidget::create(int score,int index)
{
	GameJettonWidget* pWidget = new GameJettonWidget();
	if (pWidget && pWidget->init(score,index))
	{
		pWidget->autorelease();
		return pWidget;
	}
	else
	{
		delete pWidget;
		pWidget = nullptr;
		return nullptr;
	}
}

bool GameJettonWidget::init(int score,int index)
{
	if(!Node::init())
		return false;
	mScore = score;
	mIndex = index;
	mMaxLayCnt = 1+mScore/kBaseScore;
	if (mMaxLayCnt > kMaxLayerCount)
	{
		mMaxLayCnt = kMaxLayerCount;
	}

	Rect rc = calJettonRect();
	SpriteFrame* frame = SpriteFrame::create("common/players/jetton.png",rc);
	mJettonSp = Sprite::createWithSpriteFrame(frame);
	mJettonSp->setAnchorPoint(Vec2(0.5,0));
	addChild(mJettonSp);


	return true;
}

Rect GameJettonWidget::calJettonRect()
{
	Sprite* tmp = Sprite::create("common/players/jetton.png");
	/*int laycnt =1+mScore/kBaseScore ;

	if (laycnt > kMaxLayerCount)
	{
	laycnt = kMaxLayerCount;
	}*/
	return Rect(0,0,tmp->getContentSize().width,tmp->getContentSize().height*mCurLayCnt/kMaxLayerCount);
}

void GameJettonWidget::Render()
{
	if (mCurLayCnt < mMaxLayCnt )
	{
		mCurLayCnt++;
		bool isDrawScore = false;
		if (mCurLayCnt >= mMaxLayCnt)
		{
			mCurLayCnt = mMaxLayCnt;
			isDrawScore = true;
		}

		Rect rc = calJettonRect();
		SpriteFrame* frame = SpriteFrame::create("common/players/jetton.png",rc);
		mJettonSp->setDisplayFrame(frame);

		if (isDrawScore)
		{
			RendScoreLable();
		}
	}
}

void GameJettonWidget::RendScoreLable()
{
	Rect rc = calJettonRect();
	Sprite* numberBg = nullptr;
	if (mIndex == 0 )
	{
		numberBg = Sprite::create("common/players/jetton_bgc1.png");
	}
	else 
	{
		numberBg = Sprite::create("common/players/jetton_bgc2.png");
	}
	numberBg->setPosition(Vec2(rc.size.width/2,(rc.size.height+numberBg->getContentSize().height/2)));

	mJettonSp->addChild(numberBg);

	LabelAtlas * scoreLable = LabelAtlas::create("0","common/players/jetton_num.png",12,17,'0');
	scoreLable->setPosition(numberBg->getPosition());
	scoreLable->setAnchorPoint(Vec2(0.5,0.5));
	scoreLable->setString(__String::createWithFormat("%d",mScore)->getCString());
	mJettonSp->addChild(scoreLable);

	float scalex = scoreLable->getContentSize().width*10.0/numberBg->getContentSize().width;

	numberBg->setScaleX(scalex/10.f);
}