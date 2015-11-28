#include "GameHallFirstCharge.h"
#include "Message.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "GameHallMsgItem.h"
#include "CommonFunction.h"
#include "GameHallChargeItem.h"
#include "GameHallVipPower.h"
#include "Common/ModalViewManager.h"
#include "Common/AlertWidget.h"

#define  ImagePopbgTag 18
#define  BtnCloseTag   42 
#define  BtnChargeTag  55

#define  ImageScoreTag 99
#define  ImageSendScoreTag 104

GameHallFirstChargeWidget::GameHallFirstChargeWidget()
{
	mCurOrderNumber = "";
}

GameHallFirstChargeWidget::~GameHallFirstChargeWidget()
{

}

bool GameHallFirstChargeWidget::init()
{
	if (!TNWidget::init())
		return false;




	return true;
}

std::string GameHallFirstChargeWidget::getWidgetJsonName()
{
	return "firstChargeScene";
}

void GameHallFirstChargeWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallFirstChargeWidget::onBack, this));

	Button* btn_charge = static_cast<Button*>(popBg->getChildByTag(BtnChargeTag));
	btn_charge->addTouchEventListener(CC_CALLBACK_2(GameHallFirstChargeWidget::onClickCharge, this));

	ImageView* score = static_cast<ImageView*>(popBg->getChildByTag(ImageScoreTag));

	LabelAtlas* labelScore =  LabelAtlas::create("0:","award_effect_number.png",36,35,'0');
	labelScore->setString(__String::create("10")->getCString());
	labelScore->setPosition(Vec2(score->getPositionX()-80,score->getPositionY() - score->getContentSize().height));


	ImageView* bit = ImageView::create();
	bit->loadTexture("font_ingot.png",ui::Widget::TextureResType::LOCAL);
	bit->setPosition(Vec2(labelScore->getPositionX()  -35+labelScore->getContentSize().width*3/22+bit->getContentSize().width/2,labelScore->getContentSize().height/2));
	labelScore->addChild(bit);
	
	popBg->addChild(labelScore);
	Sprite* qipao = Sprite::create("charge_qipao1.png");
	qipao->setPosition(Vec2(score->getContentSize().width/2,score->getContentSize().height/2-20));
	score->addChild(qipao);

	std::vector<std::string> imageNames;
	std::string path  = "";
	for (int i = 0 ; i < 4;i++ )
	{
		if(i < 3)
			path= "charge_qipao" + numberToString(i+1) + ".png";
		else
			path= "charge_qipao" + numberToString(2-i%3) + ".png";
		imageNames.push_back(path);
	}


	Animate* pAction = CommonFunction::createFrameAnimate(imageNames,0.25f);
	qipao->runAction(RepeatForever::create(pAction));

	ImageView* send_score = static_cast<ImageView*>(popBg->getChildByTag(ImageSendScoreTag));
	//赠送
	LabelAtlas* labelScore2 =  LabelAtlas::create("0:","award_effect_number.png",36,35,'0');
	labelScore2->setString(__String::create("5")->getCString());
	labelScore2->setPosition(Vec2(send_score->getPositionX()-50,score->getPositionY() - score->getContentSize().height));


    ImageView* bit2= ImageView::create();
	bit2->loadTexture("font_ingot.png",ui::Widget::TextureResType::LOCAL);
	bit2->setPosition(Vec2(40+bit->getContentSize().width/2,labelScore2->getContentSize().height/2));
	labelScore2->addChild(bit2);

	popBg->addChild(labelScore2);

	Sprite* qipao2 = Sprite::create();
	qipao2->setPosition(Vec2(send_score->getContentSize().width/2,score->getContentSize().height/2-20));
	send_score->addChild(qipao2);
	
	pAction = CommonFunction::createFrameAnimate(imageNames,0.14f);
	qipao2->runAction(RepeatForever::create(pAction));

	Sprite* spShayu = Sprite::create("shayu0.png");
	spShayu->setPosition(Vec2(spShayu->getContentSize().width/2-5,popBg->getContentSize().height/2 + 160));
	popBg->addChild(spShayu);
	std::vector<std::string> shayu;
	for (int i = 0 ; i < 4;i++ )
	{
		if(i < 3)
			path= "shayu" + numberToString(i) + ".png";
		else
			path= "shayu" + numberToString(1) + ".png";
		shayu.push_back(path);
	}
	pAction = CommonFunction::createFrameAnimate(shayu,0.2f);
	spShayu->runAction(RepeatForever::create(pAction));
}



void GameHallFirstChargeWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(RechargeOrderRspMsg, CC_CALLBACK_1(GameHallFirstChargeWidget::receiveRechargeOrderRspMsg, this)), this);

}


void GameHallFirstChargeWidget::onExit()
{
	TNWidget::onExit();
	_eventDispatcher->removeEventListenersForTarget(this);
}

void GameHallFirstChargeWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallFirstChargeWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}



void GameHallFirstChargeWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}


void GameHallFirstChargeWidget::onClickCharge(Ref* pSender,ui::Widget::TouchEventType type)
{

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		mCurOrderNumber = CommonFunction::getCurOrderNo();
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendRechargeOrder(10,mCurOrderNumber.c_str(),true);
		showLoading();
		//CommonFunction::callPay();
	}
}


void GameHallFirstChargeWidget::receiveRechargeOrderRspMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_RechargeOrderLog* orderLog = (CMD_GP_RechargeOrderLog*)evt->getUserData();
	if (orderLog->dwRet == 0)	{
		__String *sAction = __String::createWithFormat("{\"name\":\"ingot\",\"desc\":\"first recharge\",\"price\":\"%d\",\"tradeNo\":\"%s\"}", 10,mCurOrderNumber.c_str());
		CommonFunction::callPay(sAction->getCString());
	}
	else if (orderLog->dwRet == 10)//已充值过
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(hasFirstRechargeMsg);
		removeFromParent();
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(orderLog->szDescribeString)));
	}
}