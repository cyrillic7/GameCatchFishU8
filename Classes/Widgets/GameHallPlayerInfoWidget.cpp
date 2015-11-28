#include "GameHallPlayerInfoWidget.h"
#include "GameBindPhoneWidget.h"
#include "Common/ModalViewManager.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"
#include "CommonFunction.h"
#include "GameHallShopWidget.h"
#include "Common/ModalViewManager.h"
#include "GameHallChargeWidget.h"
#include "GameServiceClientManager.h"
#include "Common/AlertWidget.h"
#include "Message.h"
#include "MsgDefine/CMD_LogonServer.h"

#define  ImagePopbgTag    18
#define  BtnCloseTag	  42 
#define  ImageFaceTag     435
#define  BtnBindPhoneTag  436
#define  ImageAwardTag    476
#define  BtnCovertTag     481

#define  ImageNickBgTag   450
#define  TextFieldNickTag  451
#define  ImageCheckBoxManTag   455
#define  ImageCheckBoxWomanTag 461

#define  LabelUserIdTag  462
#define  ImageGoldBgTag  464
#define  ImageJiangJuanBg  476
#define  ImageIngotBg    2008
#define  LabelScoreTag  471
#define  BtnScoreTag    472
#define  LabelIngotTag  2009
#define  BtnIngotTag    2010
#define  LabelJiangJuanTag 477

#define ImageHeadIconbg 2434

#define  ImageHasBindTag 4897

GameHallPlayerInfoWidget::GameHallPlayerInfoWidget()
{
	mIsWoman = true;
	mUserIdText = nullptr;
	mScoreText = nullptr;
	mIngotText = nullptr;
	mJiangJuanText = nullptr;
	mHeadIcon = nullptr;
}

GameHallPlayerInfoWidget::~GameHallPlayerInfoWidget()
{

}

GameHallPlayerInfoWidget* GameHallPlayerInfoWidget::create()
{
	GameHallPlayerInfoWidget* pInfoWidget = new GameHallPlayerInfoWidget();
	if (pInfoWidget && pInfoWidget->init())
	{
		pInfoWidget->autorelease();
		return pInfoWidget;
	}
	else
	{
		delete pInfoWidget;
		pInfoWidget = nullptr;
		return nullptr;
	}
}

bool GameHallPlayerInfoWidget::init()
{
	if (!TNWidget::init())
		return false;


	return true;
}

void GameHallPlayerInfoWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateSuccessMsg, CC_CALLBACK_1(GameHallPlayerInfoWidget::receiveSuccessResult, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameHallPlayerInfoWidget::receiveFailureResult, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(treasureInfoMsg, CC_CALLBACK_1(GameHallPlayerInfoWidget::refreshTreasureData, this)), this);
	
	//获取财富消息
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendTreasureRequest();
}

void GameHallPlayerInfoWidget::onExit()
{
	TNWidget::onExit();
}

std::string GameHallPlayerInfoWidget::getWidgetJsonName()
{
	return "userInfo";
}

void GameHallPlayerInfoWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	//绑定按钮事件
	OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::onBack, this));

	Button* btn_bindPhone = static_cast<Button*>(popBg->getChildByTag(BtnBindPhoneTag));
	btn_bindPhone->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::onClickBindPhone, this));
	OnlineUserModel* user = SessionManager::shareInstance()->getUser();
	
	ImageView* has_bind = static_cast<ImageView*>(popBg->getChildByTag(ImageHasBindTag));
	if ( user->getPhone().length() > 0 && strcmp(user->getPhone().c_str()," ") !=0  )
	{
		btn_bindPhone->setVisible(false);
		has_bind->setVisible(true);
	}
	else
	{
		has_bind->setVisible(false);
	}

	mCheckBoxMan = static_cast<ImageView*>(popBg->getChildByTag(ImageCheckBoxManTag));
	mCheckBoxMan->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::onSelectMan, this));
	mCheckBoxMan->setTouchSize(Size(80,80));

	mCheckBoxWoman = static_cast<ImageView*>(popBg->getChildByTag(ImageCheckBoxWomanTag));
	mCheckBoxWoman->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::onSelectWoman, this));
	mCheckBoxWoman->setTouchSize(Size(80,80));
	//covert兑换
	ImageView* imageAward = static_cast<ImageView*>(popBg->getChildByTag(ImageAwardTag));
	Button* btnCovert = static_cast<Button*>(imageAward->getChildByTag(BtnCovertTag));
	btnCovert->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::onClickConvert, this));

	mHeadIcon = static_cast<ImageView*>(popBg->getChildByTag(ImageFaceTag));
	if (pUser->getUserSex() == 0 )
	{
		mHeadIcon->loadTexture(__String::createWithFormat("head/head%d.png",pUser->getFaceId())->getCString(),ui::Widget::TextureResType::LOCAL);
	}
	else
	{
		mHeadIcon->loadTexture(__String::createWithFormat("head/head%d.png",pUser->getFaceId())->getCString(),ui::Widget::TextureResType::LOCAL);
		mCheckBoxMan->loadTexture("checkbox_sex_on.png",Widget::TextureResType::PLIST);
		mCheckBoxWoman->loadTexture("checkbox_sex.png",Widget::TextureResType::PLIST);
		mIsWoman = false;
	}

	ImageView* headBg = static_cast<ImageView*>(popBg->getChildByTag(ImageHeadIconbg));
	float scale = headBg->getContentSize().width*10.0f/mHeadIcon->getContentSize().width;
	mHeadIcon->setScale(scale/10.f);

	mTextFieldNickName = static_cast<Text*>(popBg->getChildByTag(TextFieldNickTag));
	mTextFieldNickName->setString(CommonFunction::GBKToUTF8(pUser->getNickname().c_str()));

	mUserIdText = static_cast<Text*>(popBg->getChildByTag(LabelUserIdTag));
	mUserIdText->setString(numberToString(pUser->getGameId()));

	ImageView* goldBg = static_cast<ImageView*>(popBg->getChildByTag(ImageGoldBgTag));
	mScoreText = static_cast<Text*>(goldBg->getChildByTag(LabelScoreTag));
	mScoreText->setString(__String::createWithFormat("%lld",pUser->getUserScore())->getCString());

	ImageView* toCharge = static_cast<ImageView*>(goldBg->getChildByTag(BtnScoreTag));
	toCharge->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::gotoCharge, this));
	toCharge->setTouchSize(Size(80,80));

	ImageView* ingotBg = static_cast<ImageView*>(popBg->getChildByTag(ImageIngotBg));
	mIngotText = static_cast<Text*>(ingotBg->getChildByTag(LabelIngotTag));
	mIngotText->setString(numberToString(pUser->getUserIngot()));

	toCharge = static_cast<ImageView*>(ingotBg->getChildByTag(BtnIngotTag));
	toCharge->addTouchEventListener(CC_CALLBACK_2(GameHallPlayerInfoWidget::gotoCharge, this));
	toCharge->setTouchSize(Size(80,80));

	ImageView* jianjuanBg = static_cast<ImageView*>(popBg->getChildByTag(ImageJiangJuanBg));
	mJiangJuanText = static_cast<Text*>(jianjuanBg->getChildByTag(LabelJiangJuanTag));
	mJiangJuanText->setString(numberToString(pUser->getUserJiangJuan()));


}

void GameHallPlayerInfoWidget::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}

void GameHallPlayerInfoWidget::onClickBindPhone(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype ==ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameBindPhoneWidget::create());
	}
}

void GameHallPlayerInfoWidget::onClickConvert(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype ==ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallShopWidget::create());
	}

}

void GameHallPlayerInfoWidget::gotoCharge(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype ==ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
		Widget* pWidget = (Widget*) pSender;
		if (pWidget->getTag() == BtnScoreTag)
		{
			ModalViewManager::sharedInstance()->showWidget(GameHallChargeWidget::create(tab_gold));
		}
		else
		{
			ModalViewManager::sharedInstance()->showWidget(GameHallChargeWidget::create(tab_ingot));
		}
		
	}

}

void GameHallPlayerInfoWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
	}
}

void GameHallPlayerInfoWidget::onSelectMan(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mIsWoman)
		{
			mCheckBoxMan->loadTexture("checkbox_sex_on.png",Widget::TextureResType::PLIST);
			mCheckBoxWoman->loadTexture("checkbox_sex.png",Widget::TextureResType::PLIST);
			mIsWoman = false;
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyIndividual(true);
		}
	}

}

void GameHallPlayerInfoWidget::onSelectWoman(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (!mIsWoman)
		{
			mCheckBoxMan->loadTexture("checkbox_sex.png",Widget::TextureResType::PLIST);
			mCheckBoxWoman->loadTexture("checkbox_sex_on.png",Widget::TextureResType::PLIST);
			mIsWoman = true;
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyIndividual(false);
		}
		
	}

}

void GameHallPlayerInfoWidget::receiveSuccessResult(EventCustom* evt)
{
	CMD_GP_OperateSuccess* ret = (CMD_GP_OperateSuccess*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
	OnlineUserModel* user = SessionManager::shareInstance()->getUser();
	int faceId = user->getFaceId();
	if (faceId >4 && faceId < 9)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyFaceId(faceId);
	}
	if (mIsWoman)
	{
		faceId = rand()%5 ;
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyFaceId(faceId);
	}else
	{
		faceId = 10 + rand()%5 ;
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendModifyFaceId(faceId);
	}
    user->setFaceId(faceId);
	mHeadIcon->loadTexture(__String::createWithFormat("head/head%d.png",user->getFaceId())->getCString(),ui::Widget::TextureResType::LOCAL);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateHeadMsg);
}

void GameHallPlayerInfoWidget::receiveFailureResult(EventCustom* evt)
{
	CMD_GP_OperateFailure* ret = (CMD_GP_OperateFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
}

void GameHallPlayerInfoWidget::refreshTreasureData(EventCustom* evt)
{
	CMD_GP_UserTreasure* treasureInfo = (CMD_GP_UserTreasure*)evt->getUserData();
	mScoreText->setString(numberToString(treasureInfo->lScore));
	mJiangJuanText->setString(numberToString(treasureInfo->lLottery));
	mIngotText->setString(numberToString(treasureInfo->lIngotScore));
}