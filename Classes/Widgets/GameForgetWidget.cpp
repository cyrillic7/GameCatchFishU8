#include "GameForgetWidget.h"
#include "Message.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"
#include "GameServiceClientManager.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameHallLoginWidget.h"
#include "GameEnterBank.h"

#define  ImagePopbgTag 18
#define  BtnCloseTag   42 
#define  BtnGetSecurityCodeTag 504
#define  BtnBindTag    511

#define  TextPhoneTag 400
#define  TextPwdTag   409
#define  TextCodeTag  415

GameForgetWidget::GameForgetWidget()
{
	mPwdType = -1;
	mPwdText = nullptr;
	mCodeText = nullptr;
	mTimeOutImage = nullptr;
	mTimeOut = 60;
	mTimeOutLabel = nullptr;
	m_account = "";
}

GameForgetWidget::~GameForgetWidget()
{

}
GameForgetWidget* GameForgetWidget::create(int pwdType,char* phone)
{
	GameForgetWidget* pWidget = new GameForgetWidget();
	if (pWidget && pWidget->init(pwdType,phone))
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
bool GameForgetWidget::init(int pwdType,char* phone)
{
	if(!TNWidget::init())
		return false;

	mPwdType = pwdType;
	m_phone = phone;
	return true;
}

std::string GameForgetWidget::getWidgetJsonName()
{
	return "forgetPwdWidget";
}
void GameForgetWidget::onEnter()
{
	TNWidget::onEnter();
	//绑定消息
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(setPassRspMsg, CC_CALLBACK_1(GameForgetWidget::receiveModifyPwdRsp, this)), this);
	schedule(schedule_selector(GameForgetWidget::onTimeOut),1.0);
}

void GameForgetWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	unschedule(schedule_selector(GameForgetWidget::onTimeOut));
	TNWidget::onExit();
}

void GameForgetWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//绑定相关按钮事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameForgetWidget::onBack, this));

	//绑定
	Button* btn_bind =  static_cast<Button*>(popBg->getChildByTag(BtnBindTag));
	btn_bind->addTouchEventListener(CC_CALLBACK_2(GameForgetWidget::onClickModify, this));
	//获取验证码
	btn_get =  static_cast<Button*>(popBg->getChildByTag(BtnGetSecurityCodeTag));
	btn_get->addTouchEventListener(CC_CALLBACK_2(GameForgetWidget::onClickGetSecurityCode, this));

	mTimeOutImage = ImageView::create();
	mTimeOutImage->loadTexture("yzm_time.png");
	mTimeOutImage->setPosition(btn_get->getPosition());

	mTimeOutLabel = Text::create();
	mTimeOutLabel->setFontSize(20);
	mTimeOutLabel->setColor(ccc3(100,100,100));
	mTimeOutLabel->setPosition(Vec2(mTimeOutImage->getContentSize().width/2 ,mTimeOutImage->getContentSize().height/2));
	mTimeOutImage->addChild(mTimeOutLabel);
	mTimeOutImage->setVisible(false);
	mTimeOutImage->setScale(0.9);
	popBg->addChild(mTimeOutImage);

	Text* mPhoneText = static_cast<Text*>(popBg->getChildByTag(TextPhoneTag));
	OnlineUserModel* user = SessionManager::shareInstance()->getUser();
	if (user->getPhone().length() == 0 ||(strcmp(user->getPhone().c_str()," ") == 0))
		mPhoneText->setText(m_phone);
	else
		mPhoneText->setText(user->getPhone());

	mPwdText = static_cast<TextField*>(popBg->getChildByTag(TextPwdTag));

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


	mCodeText = static_cast<TextField*>(popBg->getChildByTag(TextCodeTag));

	mCodeEdit = ui::EditBox::create(mCodeText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mCodeEdit->setPosition(Vec2(mCodeText->getContentSize().width/2,mCodeText->getContentSize().height/2));
	mCodeEdit->setFontSize(mCodeText->getFontSize());
	mCodeEdit->setFontColor(mCodeText->getColor());//设置文字颜色

	mCodeEdit->setPlaceHolder(mCodeText->getPlaceHolder().c_str());//点位符
	mCodeEdit->setPlaceholderFontSize(mCodeText->getFontSize());
	mCodeEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	mCodeEdit->setText(mCodeText->getStringValue().c_str());
	mCodeEdit->setTag(mCodeText->getTag());
	mCodeText->addChild(mCodeEdit, 10);
}


void GameForgetWidget::onClickModify(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (strlen(mPwdEdit->getText()) <=0)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("新密码不能为空!")));
			return;
		}
		else
		{
			if (CommonFunction::isDigit2(mPwdEdit->getText()))
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码不能全为数字!")));
				return;
			}
				
		}
		//TO DO发送绑定消息
		if (strlen(mCodeEdit->getText()) <=0 )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("验证码不能为空!")));
			return;
		}
		int code = atoi(mCodeEdit->getText());
		//GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendBindPhoneRequest(code);
		if (mPwdType == 0 )//修改登录密码
		{
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendSetPassRequest(atoi(mCodeEdit->getText()),mPwdEdit->getText());
		}
		else if(mPwdType == 1)//修改银行密码
		{
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendSetInsuerPass(atoi(mCodeEdit->getText()),mPwdEdit->getText());
		}
		showLoading();
	}
}

void GameForgetWidget::onClickGetSecurityCode(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
	
		if (mPwdType == 0 )//修改登录密码
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendGetVerifyCodeByUid();
		else if(mPwdType == 1 )
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendInsureVerifyRequest();

		btn_get->setVisible(false);
		mTimeOutImage->setVisible(true);
		__String* price_str =  __String::createWithFormat("%s(%ds)",CommonFunction::GBKToUTF8("重新获取").c_str(),mTimeOut);
		mTimeOutLabel->setText(price_str->getCString());
	}
}





void GameForgetWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
	}
}

void GameForgetWidget::receiveModifyPwdRsp(EventCustom* evt)
{
	removeLoading();
	//置位
	mTimeOutImage->setVisible(false);
	btn_get->setVisible(true);
	CMD_GP_CaptchaRet* ret = (CMD_GP_CaptchaRet*)evt->getUserData();
	
	if(ret->lResultCode == 0)
	{
		if (mPwdType == 1)//设置银行密码
		{
			SessionManager::shareInstance()->setInsurePass(mPwdEdit->getText());
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
			//回到输入银行密码界面 //to do 
			//removeFromParent();
		}
		else if(mPwdType == 0)//保存登录密码
		{
			//保存用户登录密码
			LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
			loginModel->setPassword(mPwdEdit->getText());
			loginModel->setAccount(m_account);
			//保存到本地
			loginModel->flush();
			//改变密码成功后，更新到帐号文件中去
			__Array* accouts = SessionManager::shareInstance()->getAccountList();
			int index = SessionManager::shareInstance()->getIndexByAccount(loginModel->getAccount());
			if (index >=0)
			{
				accouts->removeObjectAtIndex(index);
				accouts->addObject(__String::createWithFormat("%s&%s",loginModel->getAccount().c_str(),mPwdEdit->getText()));
				//重新保存
				SessionManager::shareInstance()->writeAccountFile();
			}
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
			//跳转到登录界面
			//ModalViewManager::sharedInstance()->showWidget(GameHallLoginWidget::create());
		}
	}

}

void GameForgetWidget::okCallback()
{
	if (mPwdType == 0)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallLoginWidget::create());
	}
	else if(mPwdType ==1)
	{
		ModalViewManager::sharedInstance()->showWidget(GameEnterBank::create());
	}
	removeFromParent();
}

void GameForgetWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameForgetWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameForgetWidget::onTimeOut(float dt)
{
	if (mTimeOutImage->isVisible())
	{
		mTimeOut-- ;
		__String* price_str =   __String::createWithFormat("%s(%ds)",CommonFunction::GBKToUTF8("重新获取").c_str(),mTimeOut);
		mTimeOutLabel->setText(price_str->getCString());
		if ( mTimeOut<=0)
		{
			mTimeOutImage->setVisible(false);
			btn_get->setVisible(true);
			mTimeOut = 60;
		}
	}
}

void GameForgetWidget::setAccount(const char* account)
{
	m_account = account;
}