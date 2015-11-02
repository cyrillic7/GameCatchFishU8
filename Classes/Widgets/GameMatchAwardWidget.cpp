#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "GameMatchAwardWidget.h"
#include "GameMatchScene.h"
#include "Message.h"

#define  ImagePopTag 18
#define  PanelLoseTag 13371
#define  PanelWinTag   13791
#define  LableLoseNameTag 13772
#define  LabelLoseRankTag  13775
#define  LableWinNameTag 13793
#define  LabelWinRankTag   13795

#define  imageWinAwardBgTag 13798
#define  BtnContinueTag 13800
#define  BtnReturnTag  13801

//定时赛结算界面定义
#define  LabelHighScoreTag  14278
#define  LabelCurScoreTag    14284
#define  LabelRankTag   14286
#define  LabelTryCountTag 14288


GameMatchAwardWidget::GameMatchAwardWidget()
{
}

GameMatchAwardWidget::~GameMatchAwardWidget()
{

}

std::string GameMatchAwardWidget::getWidgetJsonName()
{
	return "matchResultWidget";
}

void GameMatchAwardWidget::onExit()
{
	TNWidget::onExit();
}

void GameMatchAwardWidget::onEnter()
{
	TNWidget::onEnter();
	scheduleOnce(schedule_selector(GameMatchAwardWidget::ExitSelf),10.0f);
}

GameMatchAwardWidget* GameMatchAwardWidget::create(CMD_GR_MatchResult *pResult)
{
	GameMatchAwardWidget* pAwardWidget = new GameMatchAwardWidget();
	if (pAwardWidget && pAwardWidget->init(pResult))
	{
		pAwardWidget->autorelease();
		return pAwardWidget;
	}

	delete pAwardWidget;
	pAwardWidget =nullptr;
	return pAwardWidget;
}

bool GameMatchAwardWidget::init(CMD_GR_MatchResult *pResult)
{
	if (!TNWidget::init())
	{
		return false;
	}

	memcpy(&m_awardInfo,pResult,sizeof(CMD_GR_MatchResult));

	return true;
}
void GameMatchAwardWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	
	//for test
	//int k = 2;
	//switch (k)
	//{
	//case  1:
	//	m_awardInfo.cbResultType = 1;
	//	m_awardInfo.dwGold = 1000;
	//	break;
	//case 2:
	//	m_awardInfo.cbResultType = 1;
	//	m_awardInfo.dwGold = 0;
	//	m_awardInfo.dwMedal = 100;
	//	m_awardInfo.dwCansaijuan = 3;
	//	break;
	//case 3:
	//	m_awardInfo.cbResultType = 1;
	//	m_awardInfo.dwGold = 1000;
	//	m_awardInfo.dwMedal = 10;
	//	m_awardInfo.dwCansaijuan = 1;
	//	break;
	//default:
	//	break;
	//}
	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	Layout*  pLosePanel = static_cast<Layout*>(popBg->getChildByTag(PanelLoseTag));
	Layout*  pWinPanel= static_cast<Layout*>(popBg->getChildByTag(PanelWinTag));

	if (m_awardInfo.cbResultType == 0)//未达到奖励条件
	{
		pLosePanel->setVisible(true);
		pWinPanel->setVisible(false);

		Text* pName = static_cast<Text*>(pLosePanel->getChildByTag(LableLoseNameTag));
		pName->setText(CommonFunction::GBKToUTF8(m_awardInfo.szName));

		TextAtlas* pRankAtlas = static_cast<TextAtlas*>(pLosePanel->getChildByTag(LabelLoseRankTag));
		std::string rank = __String::createWithFormat("%d",m_awardInfo.dwRankLast)->getCString();
		pRankAtlas->setString(rank);

	}else if (m_awardInfo.cbResultType == 1)
	{
		pLosePanel->setVisible(false);
		pWinPanel->setVisible(true);

		Text* pName = static_cast<Text*>(pWinPanel->getChildByTag(LableWinNameTag));
		pName->setText(CommonFunction::GBKToUTF8(m_awardInfo.szName));

		TextAtlas* pRankAtlas = static_cast<TextAtlas*>(pWinPanel->getChildByTag(LabelWinRankTag));
		pRankAtlas->setString(numberToString(m_awardInfo.dwRankLast));

		ImageView* awardBg = static_cast<ImageView*>(pWinPanel->getChildByTag(imageWinAwardBgTag));
		int award_cnt = 0;
		bool has_gold = false, has_lottery = false, has_cansaijuang = false;
		if (m_awardInfo.dwGold >0)
		{
			award_cnt++;
			has_gold = true;
		}
		if (m_awardInfo.dwCansaijuan >0)
		{
			award_cnt++;
			has_cansaijuang = true;
		}
		if (m_awardInfo.dwMedal > 0 )//奖劵
		{
			award_cnt++;
			has_lottery = true;
		}
		switch (award_cnt)
		{
		case  1:
			{
				ImageView* icon = ImageView::create();
				ImageView* font_text= ImageView::create();
				int  award_cnt = 0;
				if (has_gold)
				{	
					icon->loadTexture("match/u_icon_gold.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_gold.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwGold;
				}else if (has_lottery)
				{
					icon->loadTexture("match/u_icon_lottery.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_lottery.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwMedal;
				}else if (has_cansaijuang)
				{
					icon->loadTexture("match/u_icon_ticket.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_ticket.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwCansaijuan;
				}
				icon->setPosition(Vec2(awardBg->getContentSize().width/2 - 250,awardBg->getContentSize().height/2));
				font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
				awardBg->addChild(icon);
				awardBg->addChild(font_text);

				ImageView* image_plus = ImageView::create();
				image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
				image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
				awardBg->addChild(image_plus);

				int bit = 0;
				int tmp = award_cnt;
				while (tmp)
				{
					tmp /=10;
					bit++;
				}
				LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
				label_count->setString(numberToString(award_cnt));
				label_count->setAnchorPoint(Vec2(0,0.5));
				label_count->setScaleX(0.5);
				label_count->setScaleY(0.6);
				label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
				awardBg->addChild(label_count);
			}
			break;
		case  2:
			{
				ImageView* icon =nullptr;
				ImageView* font_text= nullptr;
				int  award_cnt = 0;
				if (has_gold)
				{	
					icon = ImageView::create();
					font_text = ImageView::create();
					icon->loadTexture("match/u_icon_gold.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_gold.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwGold;

					icon->setPosition(Vec2(awardBg->getContentSize().width/2 - 250,awardBg->getContentSize().height/2));
					font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(icon);
					awardBg->addChild(font_text);

					ImageView* image_plus = ImageView::create();
					image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
					image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(image_plus);

					LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
					label_count->setString(numberToString(award_cnt));
					label_count->setAnchorPoint(Vec2(0,0.5));
					label_count->setScaleX(0.5);
					label_count->setScaleY(0.6);
					label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
					awardBg->addChild(label_count);
				}
				
			
				if (has_lottery)
				{	
					icon = ImageView::create();
					font_text = ImageView::create();
					icon->loadTexture("match/u_icon_lottery.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_lottery.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwMedal;

					int dis;
					if (has_gold)
					{
						dis = 50;
					}else
					{
						dis = 250;
					}
					icon->setPosition(Vec2(awardBg->getContentSize().width/2 - dis,awardBg->getContentSize().height/2));
					font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(icon);
					awardBg->addChild(font_text);

					ImageView* image_plus = ImageView::create();
					image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
					image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(image_plus);

					LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
					label_count->setString(numberToString(award_cnt));
					label_count->setAnchorPoint(Vec2(0,0.5));
					label_count->setScaleX(0.5);
					label_count->setScaleY(0.6);
					label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
					awardBg->addChild(label_count);
				}

				if (has_cansaijuang)
				{	
					icon = ImageView::create();
					font_text = ImageView::create();
					icon->loadTexture("match/u_icon_ticket.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_ticket.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwCansaijuan;


					icon->setPosition(Vec2(awardBg->getContentSize().width/2 - 50,awardBg->getContentSize().height/2));
					font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(icon);
					awardBg->addChild(font_text);

					ImageView* image_plus = ImageView::create();
					image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
					image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(image_plus);

					LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
					label_count->setString(numberToString(award_cnt));
					label_count->setAnchorPoint(Vec2(0,0.5));
					label_count->setScaleX(0.5);
					label_count->setScaleY(0.6);
					label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
					awardBg->addChild(label_count);
				}
			}
			break;
		case  3:
			{
				ImageView* icon = nullptr;
				ImageView* font_text= nullptr;
				int  award_cnt = 0;
				if (has_gold)
				{	
					icon = ImageView::create();
					font_text = ImageView::create();
					icon->loadTexture("match/u_icon_gold.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_gold.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwGold;

					icon->setPosition(Vec2(awardBg->getContentSize().width/2 - 250,awardBg->getContentSize().height/2));
					font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(icon);
					awardBg->addChild(font_text);

					ImageView* image_plus = ImageView::create();
					image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
					image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(image_plus);

					LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
					label_count->setString(numberToString(award_cnt));
					label_count->setAnchorPoint(Vec2(0,0.5));
					label_count->setScaleX(0.5);
					label_count->setScaleY(0.6);
					label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
					awardBg->addChild(label_count);
				}


				if (has_lottery)
				{	
					icon = ImageView::create();
					font_text = ImageView::create();
					icon->loadTexture("match/u_icon_lottery.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_lottery.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwMedal;

					icon->setPosition(Vec2(awardBg->getContentSize().width/2 - 50,awardBg->getContentSize().height/2));
					font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(icon);
					awardBg->addChild(font_text);

					ImageView* image_plus = ImageView::create();
					image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
					image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(image_plus);

				
					LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
					label_count->setString(numberToString(award_cnt));
					label_count->setAnchorPoint(Vec2(0,0.5));
					label_count->setScaleX(0.5);
					label_count->setScaleY(0.6);
					label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
					awardBg->addChild(label_count);
				}

				if (has_cansaijuang)
				{	
					icon = ImageView::create();
					font_text = ImageView::create();
					icon->loadTexture("match/u_icon_ticket.png",ui::Widget::TextureResType::LOCAL);
					font_text->loadTexture("match/u_text_ticket.png",ui::Widget::TextureResType::LOCAL);
					award_cnt = m_awardInfo.dwCansaijuan;

					icon->setPosition(Vec2(awardBg->getContentSize().width/2 +130,awardBg->getContentSize().height/2));
					font_text->setPosition(Vec2(icon->getPositionX() + icon->getContentSize().width/2 + font_text->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(icon);
					awardBg->addChild(font_text);

					ImageView* image_plus = ImageView::create();
					image_plus->loadTexture("match/match_plus.png",ui::Widget::TextureResType::LOCAL);
					image_plus->setPosition(Vec2(font_text->getPositionX() + font_text->getContentSize().width/2 + image_plus->getContentSize().width/2 + 5,awardBg->getContentSize().height/2));
					awardBg->addChild(image_plus);


					LabelAtlas* label_count = LabelAtlas::create("0:","match/match_rank_num.png",27,35,'0');
					label_count->setString(numberToString(award_cnt));
					label_count->setAnchorPoint(Vec2(0,0.5));
					label_count->setScaleX(0.5);
					label_count->setScaleY(0.6);
					label_count->setPosition(Vec2(image_plus->getPositionX() +image_plus->getContentSize().width/2,awardBg->getContentSize().height/2));
					awardBg->addChild(label_count);
				}
			}
			break;
		}
	}

	Button* btn_back = static_cast<Button*>(popBg->getChildByTag(BtnReturnTag));
	btn_back->addTouchEventListener(CC_CALLBACK_2(GameMatchAwardWidget::onBack, this));

	Button* btn_continue = static_cast<Button*>(popBg->getChildByTag(BtnContinueTag));
	btn_continue->addTouchEventListener(CC_CALLBACK_2(GameMatchAwardWidget::onContinue, this));

}


void GameMatchAwardWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		Director::sharedDirector()->replaceScene(GameMatchScene::create());
	}
}

void GameMatchAwardWidget::ExitSelf(float dt)
{
	removeFromParent();
	Director::sharedDirector()->replaceScene(GameMatchScene::create());
}

void GameMatchAwardWidget::onContinue(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		Director::sharedDirector()->replaceScene(GameMatchScene::create(true));
	}
}

//////////////////////////////////////////////////////////////////////////
GameMatchNextTryWidget::GameMatchNextTryWidget()
{
	mModeInfo = nullptr;
}

GameMatchNextTryWidget::~GameMatchNextTryWidget()
{
	CC_SAFE_RELEASE_NULL(mModeInfo);
}

GameMatchNextTryWidget* GameMatchNextTryWidget::create(GameMatchModel *pModel)
{
	GameMatchNextTryWidget* pWidget = new GameMatchNextTryWidget();
	if (pWidget && pWidget->init(pModel))
	{
		pWidget->autorelease();
		return pWidget;
	}

	delete pWidget;
	pWidget = nullptr;
	return pWidget;
}

bool GameMatchNextTryWidget::init(GameMatchModel *pModel)
{
	if (!TNWidget::init())
	{
		return false;
	}

	mModeInfo = pModel;
	mModeInfo->retain();

	return true;
}

void GameMatchNextTryWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	mHighScore = static_cast<TextAtlas*>(popBg->getChildByTag(LabelHighScoreTag));
	mHighScore->setString(numberToString(mModeInfo->getMyMaxScore()));

	mCurScore = static_cast<TextAtlas*>(popBg->getChildByTag(LabelCurScoreTag));
	mCurScore->setString(numberToString(mModeInfo->getMyCurScore()));

	mRank = static_cast<TextAtlas*>(popBg->getChildByTag(LabelRankTag));
	mRank->setString(numberToString(mModeInfo->getSelfRank()));

	TextAtlas* pTryCount = static_cast<TextAtlas*>(popBg->getChildByTag(LabelTryCountTag));
	pTryCount->setString(numberToString(mModeInfo->getAllPlayCount() - mModeInfo->getPlayCount()));

	Button* btn_back = static_cast<Button*>(popBg->getChildByTag(BtnReturnTag));
	btn_back->addTouchEventListener(CC_CALLBACK_2(GameMatchNextTryWidget::onBack, this));

	Button* btn_try = static_cast<Button*>(popBg->getChildByTag(BtnContinueTag));
	if (mModeInfo->getBContinue())
	{
		btn_try->addTouchEventListener(CC_CALLBACK_2(GameMatchNextTryWidget::onTryAgain, this));
	}
	else
	{
		btn_try->setColor(ccc3(100,100,100));
	}
	
}

std::string GameMatchNextTryWidget::getWidgetJsonName()
{
	return "matchTimeResultWidget";
}

void GameMatchNextTryWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameMatchNextTryWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateMatchRankMsg, CC_CALLBACK_1(GameMatchNextTryWidget::updateRankMsg, this)), this);
}

//返回
void GameMatchNextTryWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		Director::sharedDirector()->replaceScene(GameMatchScene::create());
	}
}

//重新挑战
void GameMatchNextTryWidget::onTryAgain(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(MatchTryAgainMsg);
	}
}

void GameMatchNextTryWidget::updateRankMsg(EventCustom* evt)
{
	GameMatchModel* pMode = (GameMatchModel*)evt->getUserData();
	mRank->setString(numberToString(pMode->getSelfRank()));
	mHighScore->setString(numberToString(pMode->getMyMaxScore()));
	mCurScore->setString(numberToString(pMode->getMyCurScore()));
}