#include "GameInputAccoutWidget.h"
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

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  BtnOkTag  398
#define  TextFieldInputTag 400

GameInputAccoutWidget::GameInputAccoutWidget()
{
	mAccountText = nullptr;
}

GameInputAccoutWidget::~GameInputAccoutWidget()
{

}

std::string GameInputAccoutWidget::getWidgetJsonName()
{
	return "forgetLoginPwdWidget";
}

void GameInputAccoutWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameInputAccoutWidget::onEnter()
{
	TNWidget::onEnter();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateSuccessMsg, CC_CALLBACK_1(GameInputAccoutWidget::receiveOperateSuccessMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameInputAccoutWidget::receiveOperateFailureMsg, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(checkAccountRspMsg, CC_CALLBACK_1(GameInputAccoutWidget::receiveCheckAccountRspMsg, this)), this);
}

bool GameInputAccoutWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}



	return true;
}
void GameInputAccoutWidget::loadUI()
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
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameInputAccoutWidget::onBack, this));

	Button* btn_next = static_cast<Button*>(popBg->getChildByTag(BtnOkTag));
	btn_next->addTouchEventListener(CC_CALLBACK_2(GameInputAccoutWidget::onOk, this));

	mAccountText = static_cast<TextField*>(popBg->getChildByTag(TextFieldInputTag));

	mAccountEdit = ui::EditBox::create(mAccountText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mAccountEdit->setPosition(Vec2(mAccountText->getContentSize().width/2,mAccountText->getContentSize().height/2));
	mAccountEdit->setFontSize(mAccountText->getFontSize());
	mAccountEdit->setFontColor(mAccountText->getColor());//设置文字颜色

	mAccountEdit->setPlaceHolder(mAccountText->getPlaceHolder().c_str());//点位符
	mAccountEdit->setPlaceholderFontSize(mAccountText->getFontSize());
	mAccountEdit->setText(mAccountText->getStringValue().c_str());
	mAccountEdit->setTag(mAccountText->getTag());
	mAccountText->addChild(mAccountEdit, 10);


}


void GameInputAccoutWidget::receiveOperateSuccessMsg(EventCustom* evt)
{
	removeLoading();
	removeFromParent();
}

void GameInputAccoutWidget::receiveOperateFailureMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_OperateFailure* failureInfo = (CMD_GP_OperateFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(failureInfo->szDescribeString)));
}

void GameInputAccoutWidget::receiveCheckAccountRspMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_AccountsRet* ret = (CMD_GP_AccountsRet*)evt->getUserData();
	if (ret->lResultCode == 0)//成功跳转
	{
		
		SessionManager::shareInstance()->setUserId(ret->dwUserID);
		GameForgetWidget* pWidget = GameForgetWidget::create(0,ret->szPhone);
		pWidget->setAccount(mAccountEdit->getText());
		ModalViewManager::sharedInstance()->showWidget(pWidget);

		removeFromParent();
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
	}
}

void GameInputAccoutWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameInputAccoutWidget::onOk(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (strlen(mAccountEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("帐号不能为空!")));
			return;
		}
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendEnsureAccoutRequest(mAccountEdit->getText());
		showLoading();
	}
}


void GameInputAccoutWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameInputAccoutWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}