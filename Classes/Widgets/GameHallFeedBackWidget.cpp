#include "GameHallFeedBackWidget.h"
#include "CommonFunction.h"
#include "GameServiceClientManager.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "CommonFunction.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Message.h"

#define  ImagePopBg 18
#define  BtnColseTag 42
#define  BtnSendTag 55

#define  ImageTextBgTag  9778
#define  TextFieldTextTag 9781

#define  IamgeQQTag   9780
#define  TextFieldQQTag 9782



GameHallFeedBackWidget::GameHallFeedBackWidget()
{
	mContentText = nullptr;
	mQQText = nullptr;
}

GameHallFeedBackWidget::~GameHallFeedBackWidget()
{

}


bool GameHallFeedBackWidget::init()
{
	if(!TNWidget::init())
		return false;

	return true;
}

void GameHallFeedBackWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(feedBackMsg, CC_CALLBACK_1(GameHallFeedBackWidget::receiveFeedBackMsg, this)), this);
}

void GameHallFeedBackWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

std::string GameHallFeedBackWidget::getWidgetJsonName()
{
	return "feedbackWidget";
}

void GameHallFeedBackWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBg));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnColseTag));

	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallFeedBackWidget::onClose,this));



	Button* btn_sendGood = static_cast<Button*>(popBg->getChildByTag(BtnSendTag));

	btn_sendGood->addTouchEventListener(CC_CALLBACK_2(GameHallFeedBackWidget::onSend,this));

	ImageView* textBg = static_cast<ImageView*>(popBg->getChildByTag(ImageTextBgTag));
	mContentText =  static_cast<TextField*>(textBg->getChildByTag(TextFieldTextTag));

	//mContentEdit = ui::EditBox::create(backText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	//mContentEdit->setPosition(Vec2(backText->getContentSize().width/2,backText->getContentSize().height/2));
	//mContentEdit->setFontSize(backText->getFontSize());
	//mContentEdit->setFontColor(backText->getColor());//设置文字颜色

	//mContentEdit->setPlaceHolder(backText->getPlaceHolder().c_str());//点位符
	//mContentEdit->setPlaceholderFontSize(backText->getFontSize());
	//mContentEdit->setText(backText->getStringValue().c_str());
	//
	//backText->addChild(mContentEdit, 10);


	ImageView* textQQBg = static_cast<ImageView*>(popBg->getChildByTag(IamgeQQTag));
	mQQText =  static_cast<TextField*>(textQQBg->getChildByTag(TextFieldQQTag));

//	mQQEdit = ui::EditBox::create(backQQ->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
//	mQQEdit->setPosition(Vec2(backQQ->getContentSize().width/2,backQQ->getContentSize().height/2));
//	mQQEdit->setFontSize(backQQ->getFontSize());
//	mQQEdit->setFontColor(backQQ->getColor());//设置文字颜色
//
//	mQQEdit->setPlaceHolder(backQQ->getPlaceHolder().c_str());//点位符
//	mQQEdit->setPlaceholderFontSize(backQQ->getFontSize());
//	mQQEdit->setText(backQQ->getStringValue().c_str());
//
//	backQQ->addChild(mQQEdit, 10);
}

void GameHallFeedBackWidget::onClose(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallFeedBackWidget::onSend(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (mContentText->getString().length() <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("反馈内容不能为空!")));
			return;
		}
		if (mQQText->getString().length() <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("联系方式不能为空!")));
			return;
		}
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendFeedBackRequest(mContentText->getString().c_str(),mQQText->getString().c_str());
		showLoading();
	}
}

void GameHallFeedBackWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallFeedBackWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallFeedBackWidget::receiveFeedBackMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_FeedbackLog* log =(CMD_GP_FeedbackLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(log->szDescribeString)));
	if (log->dwRet == 0)
	{
		mContentText->setText("");
		mQQText->setText("");
	}
}

