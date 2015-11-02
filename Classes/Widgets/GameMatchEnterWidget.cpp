#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "GameMatchEnterWidget.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "Common/pystring.h"
#include "Message.h"

#define ImagePopTag 18
#define  ImageTimeBgTag 13326
#define  LableMatchTimeTag 13331
#define  LableLeftTimeTag 13333
#define  ImageSignUpPlayersBg 13334
#define  LableSignUpPlayers   13337
#define  TextMatchNameTag 13804
#define  BtnSignUpTag  13338
#define  LabelSignFeeTag    13341
#define  ListViewAwardTag 13344
#define  BtnCloseTag  13345

GameMatchEnterWidget::GameMatchEnterWidget()
{
	mData = nullptr;
	mSpanTime = 0;
}

GameMatchEnterWidget::~GameMatchEnterWidget()
{
	CC_SAFE_RELEASE_NULL(mData);
}

std::string GameMatchEnterWidget::getWidgetJsonName()
{
	return "matchEnterWidget";
}

void GameMatchEnterWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameMatchEnterWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(matchCountDownMsg, CC_CALLBACK_1(GameMatchEnterWidget::receiveMatchCountDownMsg, this)), this);

	//起定时器每三秒刷新比赛人数
	schedule(schedule_selector(GameMatchEnterWidget::updateLeftTime),1.0f);
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
	c->sendCountDownRequest(mData);
}

GameMatchEnterWidget* GameMatchEnterWidget::create(MatchInfoModel* info)
{
	GameMatchEnterWidget* pWidget = new GameMatchEnterWidget();
	if (pWidget && pWidget->init(info))
	{
		pWidget->autorelease();
		return pWidget;
	}
	delete pWidget;
	pWidget = nullptr;
	return nullptr;
}

bool GameMatchEnterWidget::init(MatchInfoModel* info)
{
	if (!TNWidget::init())
	{
		return false;
	}
	mData = info;
	mData->retain();

	return true;
}
void GameMatchEnterWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	
	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	Text*  pMatchName = static_cast<Text*>(popBg->getChildByTag(TextMatchNameTag));
	pMatchName->setText(CommonFunction::GBKToUTF8(mData->getMatchRemark().c_str()));

	ImageView* pTimeBg  = static_cast<ImageView*>(popBg->getChildByTag(ImageTimeBgTag));
	Text* pMatchTime =  static_cast<Text*>(pTimeBg->getChildByTag(LableMatchTimeTag));

	//开赛时间
	SYSTEMTIME curtime = mData->getMatchTime();
	std::string time_str = __String::createWithFormat("%d-%d",curtime.wMonth,curtime.wDay)->getCString();
	if (strcmp(time_str.c_str(),SessionManager::shareInstance()->getMatchTime().c_str()) == 0 )//今天
	{
		__String* content= __String::createWithFormat("%s %02d:%02d",CommonFunction::GBKToUTF8("今天").c_str(),curtime.wHour,curtime.wMinute);
		pMatchTime->setText(content->getCString());
	}
	else
	{
		__String* content= __String::createWithFormat("%d-%d %02d:%02d",curtime.wMonth,curtime.wDay,curtime.wHour,curtime.wMinute);
		pMatchTime->setText(content->getCString());
	}

	mLeftTime = static_cast<Text*>(pTimeBg->getChildByTag(LableLeftTimeTag));
	mLeftTime->setString(CommonFunction::covertTimeSpanStr(mData->getSpaceTime()));

	ImageView* signUpBg = static_cast<ImageView*>(popBg->getChildByTag(ImageSignUpPlayersBg));

	mSignUpPlays =  static_cast<Text*>(signUpBg->getChildByTag(LableSignUpPlayers));
	mSignUpPlays->setString(numberToString(mData->getSignUpPlayerNum()));

	Button* btn_signUp =  static_cast<Button*>(popBg->getChildByTag(BtnSignUpTag));
	btn_signUp->addTouchEventListener(CC_CALLBACK_2(GameMatchEnterWidget::onClickSignUp, this));

	Text*  fee_text =  static_cast<Text*>(btn_signUp->getChildByTag(LabelSignFeeTag));
	__String* signStr =nullptr;
	if (mData->getSignUpTicket() > 0)
	{
		signStr = __String::createWithFormat("(%d%s)",mData->getSignUpTicket(),CommonFunction::GBKToUTF8("参赛劵").c_str());
		fee_text->setText(signStr->getCString());
	}
	else if(mData->getSignUpScore() > 0)
	{
		signStr = __String::createWithFormat("(%lld%s)",mData->getSignUpScore(),CommonFunction::GBKToUTF8("金币").c_str());
		fee_text->setText(signStr->getCString());
	}
	bool isFreeCharge = false;//免费
	if ( nullptr == signStr)
	{
		isFreeCharge = true;
		fee_text->setText(CommonFunction::GBKToUTF8("(免费报名)"));
	}

	//奖励列表
	ListView* pAwardList = static_cast<ListView*>(popBg->getChildByTag(ListViewAwardTag));
	__Array* awards = mData->getAwardArray();
	for ( int i = 0 ; i < awards->count(); i++)
	{
		MatchAwardModel* pAwardModel = (MatchAwardModel*)awards->getObjectAtIndex(i);
		if (pAwardModel->getBeginRank() == 0)
		{
			continue;
		}
		std::string str_award = "";
		Text* awardText = Text::create();
		awardText->setTextColor(ccc4(160,70,40,255));
		awardText->setFontSize(20);
		if (pAwardModel->getEndRank() != pAwardModel->getBeginRank())
		{
			str_award = __String::createWithFormat("%s%d-%d%s   ",CommonFunction::GBKToUTF8("第").c_str(),pAwardModel->getBeginRank(),pAwardModel->getEndRank(),CommonFunction::GBKToUTF8("名").c_str())->getCString();
		}
		else
		{
			str_award = __String::createWithFormat("%s%d%s   ",CommonFunction::GBKToUTF8("第").c_str(),pAwardModel->getBeginRank(),CommonFunction::GBKToUTF8("名").c_str())->getCString();
		}
		
		if (pAwardModel->getAward1() > 0)
		{
			str_award += __String::createWithFormat("%d%s+",pAwardModel->getAward1(),pAwardModel->getAwardDesc1().c_str())->getCString();
		}
		if (pAwardModel->getAward2() > 0)
		{
			str_award += __String::createWithFormat("%d%s+",pAwardModel->getAward2(),pAwardModel->getAwardDesc2().c_str())->getCString();
		}
		if (pAwardModel->getAward3() > 0)
		{
			str_award += __String::createWithFormat("%d%s+",pAwardModel->getAward3(),pAwardModel->getAwardDesc3().c_str())->getCString();
		}

		str_award = pystring::slice(str_award,0,str_award.length() -1);
		awardText->setString(str_award);
		pAwardList->pushBackCustomItem(awardText);
	}

	if (awards->count() >=7)
	{
		pAwardList->setBounceEnabled(true);
	}

	
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameMatchEnterWidget::onBack, this));
}

void GameMatchEnterWidget::updateLeftTime(float dt)
{
	if (mSpanTime-- >0)
	{
		mLeftTime->setString(CommonFunction::covertTimeSpanStr(mSpanTime));
	}
}

void GameMatchEnterWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}


void GameMatchEnterWidget::onClickSignUp(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(toSignUpNotice,mData);
		removeFromParent();
	}
		
}


void GameMatchEnterWidget::receiveMatchCountDownMsg(EventCustom* evt)
{
	CMD_GL_MatchCountDown* pInfo = (CMD_GL_MatchCountDown*)evt->getUserData();
	if (mData->getSerialModel()->getMatchInfoID() == pInfo->MatchSerial.dwMatchInfoID)
	{
		mSpanTime = pInfo->dwCountDown;
	}
	
}
