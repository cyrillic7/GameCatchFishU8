#include "GameGiftCovertWidget.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Message.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "CommonFunction.h"
#include "GameServiceClientManager.h"
#include "GameHallBankWidget.h"
#include "Model/OnlineUserModel.h"
#include "SessionManager.h"
#include "GameForgetWidget.h"
#include "Widgets/GameHallBagWidget.h"

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  BtnOkTag  398
#define  TextFieldInputTag 400

GameGiftCovertWidget::GameGiftCovertWidget()
{
	mPwdText = nullptr;
}

GameGiftCovertWidget::~GameGiftCovertWidget()
{

}

std::string GameGiftCovertWidget::getWidgetJsonName()
{
	return "giftCovertWidget";
}

void GameGiftCovertWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameGiftCovertWidget::onEnter()
{
	TNWidget::onEnter();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateSuccessMsg, CC_CALLBACK_1(GameGiftCovertWidget::receiveOperateSuccessMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameGiftCovertWidget::receiveOperateFailureMsg, this)), this);
}

bool GameGiftCovertWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}



	return true;
}
void GameGiftCovertWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	
	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameGiftCovertWidget::onBack, this));

	Button* btn_OK = static_cast<Button*>(popBg->getChildByTag(BtnOkTag));
	btn_OK->addTouchEventListener(CC_CALLBACK_2(GameGiftCovertWidget::onOk, this));

	mPwdText = static_cast<TextField*>(popBg->getChildByTag(TextFieldInputTag));

	mPwdEdit = ui::EditBox::create(mPwdText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mPwdEdit->setPosition(Vec2(mPwdText->getContentSize().width/2,mPwdText->getContentSize().height/2));
	mPwdEdit->setFontSize(mPwdText->getFontSize());
	mPwdEdit->setFontColor(mPwdText->getColor());//设置文字颜色

	mPwdEdit->setPlaceHolder(mPwdText->getPlaceHolder().c_str());//点位符
	mPwdEdit->setPlaceholderFontSize(mPwdText->getFontSize());
	mPwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mPwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mPwdEdit->setText(mPwdText->getStringValue().c_str());
	mPwdEdit->setTag(mPwdText->getTag());
	
	mPwdText->addChild(mPwdEdit, 10);


}


void GameGiftCovertWidget::receiveOperateSuccessMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_OperateSuccess* sInfo = (CMD_GP_OperateSuccess*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this, CommonFunction::GBKToUTF8("查看背包"), CommonFunction::GBKToUTF8(sInfo->szDescribeString)));
}

void GameGiftCovertWidget::receiveOperateFailureMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_OperateFailure* failureInfo = (CMD_GP_OperateFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(failureInfo->szDescribeString)));
}

void GameGiftCovertWidget::onBack(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameGiftCovertWidget::onOk(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if(strlen(mPwdEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("兑换码不能为空!")));
			return;
		}
		showLoading();
	}
}



void GameGiftCovertWidget::okCallback()
{
	removeFromParent();
	//直接跳到背包界面
	ModalViewManager::sharedInstance()->showWidget(GameHallBagWidget::create());
}

void GameGiftCovertWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameGiftCovertWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}