#include "GameEnterBank.h"
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
#include "GameBindPhoneWidget.h"

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  BtnOkTag  398
#define  TextFieldInputTag 400
#define  BtnForgetTag      410

GameEnterBank::GameEnterBank()
{
	mPwdText = nullptr;
}

GameEnterBank::~GameEnterBank()
{

}

std::string GameEnterBank::getWidgetJsonName()
{
	return "bankInputMM";
}

void GameEnterBank::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameEnterBank::onEnter()
{
	TNWidget::onEnter();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateSuccessMsg, CC_CALLBACK_1(GameEnterBank::receiveOperateSuccessMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameEnterBank::receiveOperateFailureMsg, this)), this);
}

bool GameEnterBank::init()
{
	if (!TNWidget::init())
	{
		return false;
	}



	return true;
}
void GameEnterBank::loadUI()
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
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameEnterBank::onBack, this));

	Button* btn_OK = static_cast<Button*>(popBg->getChildByTag(BtnOkTag));
	btn_OK->addTouchEventListener(CC_CALLBACK_2(GameEnterBank::onOk, this));

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


	Button* btn_forget = static_cast<Button*>(popBg->getChildByTag(BtnForgetTag));
	btn_forget->addTouchEventListener(CC_CALLBACK_2(GameEnterBank::forgetPwd, this));

}


void GameEnterBank::receiveOperateSuccessMsg(EventCustom* evt)
{
	removeLoading();
	SessionManager::shareInstance()->setInsurePass(mPwdEdit->getText());
	removeFromParent();
	ModalViewManager::sharedInstance()->showWidget(GameHallBankWidget::create());

}

void GameEnterBank::receiveOperateFailureMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_OperateFailure* failureInfo = (CMD_GP_OperateFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(failureInfo->szDescribeString)));
}

void GameEnterBank::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameEnterBank::onOk(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		if(strlen(mPwdEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码不能为空!")));
			return;
		}
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendVerifyInsurePass(mPwdEdit->getText());
		showLoading();
	}
}

void GameEnterBank::forgetPwd(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		OnlineUserModel* user = SessionManager::shareInstance()->getUser();
		if (user->getPhone().length() == 0 || strcmp(user->getPhone().c_str()," ") == 0 )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8("您还未绑定手机,请先绑定手机!")));
		}
		else
		{
			removeFromParent();
			ModalViewManager::sharedInstance()->showWidget(GameForgetWidget::create(1,(char*)user->getPhone().c_str()));
		}
	}
}

void GameEnterBank::okCallback()
{
	removeFromParent();
	//直接跳到绑定手机界面
	ModalViewManager::sharedInstance()->showWidget(GameBindPhoneWidget::create());
}

void GameEnterBank::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameEnterBank::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}