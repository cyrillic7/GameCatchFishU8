#include "GameBindPhoneWidget.h"
#include "Message.h"
#include "CommonFunction.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "GameServiceClientManager.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"

#define  ImagePopbgTag 18
#define  BtnCloseTag   42 
#define  BtnGetSecurityCodeTag 504
#define  BtnBindTag    511

#define  TextPhoneTag 400
#define  TextPwdTag   409
#define  TextCodeTag  415


GameBindPhoneWidget::GameBindPhoneWidget()
{
	mPhoneText = nullptr;
	mPwdText = nullptr;
	mCodeText = nullptr;
	mTimeOut = 60;
	mTimeOutImage = nullptr;
	mTimeOutLabel = nullptr;
	btn_get= nullptr;
	mEdit1 = nullptr;
}

GameBindPhoneWidget::~GameBindPhoneWidget()
{

}

bool GameBindPhoneWidget::init()
{
	if(!TNWidget::init())
		return false;


	return true;
}

std::string GameBindPhoneWidget::getWidgetJsonName()
{
	return "bindPhoneWidget";
}
void GameBindPhoneWidget::onEnter()
{
	TNWidget::onEnter();
	//绑定签到返消息
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(bindPhoneRspMsg, CC_CALLBACK_1(GameBindPhoneWidget::receiveBindPhoneRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getCaptchaRspMsg, CC_CALLBACK_1(GameBindPhoneWidget::receiveGetCaptchaRsp, this)), this);

	schedule(schedule_selector(GameBindPhoneWidget::onTimeOut),1.0);
}

void GameBindPhoneWidget::onExit()
{
	unschedule(schedule_selector(GameBindPhoneWidget::onTimeOut));
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameBindPhoneWidget::loadUI()
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
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameBindPhoneWidget::onBack, this));

	//绑定
	Button* btn_bind =  static_cast<Button*>(popBg->getChildByTag(BtnBindTag));
	btn_bind->addTouchEventListener(CC_CALLBACK_2(GameBindPhoneWidget::onClickBind, this));
	//获取验证码
	btn_get =  static_cast<Button*>(popBg->getChildByTag(BtnGetSecurityCodeTag));
	btn_get->addTouchEventListener(CC_CALLBACK_2(GameBindPhoneWidget::onClickGetSecurityCode, this));

	mTimeOutImage = ImageView::create();
	mTimeOutImage->loadTexture("yzm_time.png");
	mTimeOutImage->setPosition(btn_get->getPosition());

	mTimeOutLabel = Text::create();
	mTimeOutLabel->setFontSize(20);
	mTimeOutLabel->setColor(ccc3(100,100,100));
	mTimeOutLabel->setPosition(Vec2(mTimeOutImage->getContentSize().width/2 ,mTimeOutImage->getContentSize().height/2));
	mTimeOutImage->addChild(mTimeOutLabel);
	mTimeOutImage->setVisible(false);
	popBg->addChild(mTimeOutImage);

	mPhoneText = static_cast<TextField*>(popBg->getChildByTag(TextPhoneTag));

	mEdit1= ui::EditBox::create(mPhoneText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mEdit1->setPosition(Vec2(mPhoneText->getContentSize().width/2,mPhoneText->getContentSize().height/2));
	mEdit1->setFontSize(mPhoneText->getFontSize());
	mEdit1->setFontColor(mPhoneText->getColor());//设置文字颜色

	mEdit1->setPlaceHolder(mPhoneText->getPlaceHolder().c_str());//点位符
	mEdit1->setPlaceholderFontSize(mPhoneText->getFontSize());
	mEdit1->setInputMode(ui::EditBox::InputMode::NUMERIC);
	mEdit1->setMaxLength(mPhoneText->getMaxLength());

	mPhoneText->addChild(mEdit1);
	mCodeText = static_cast<TextField*>(popBg->getChildByTag(TextCodeTag));
	initCode();
}

void GameBindPhoneWidget::initCode()
{
	ui::EditBox*pCodeEdit= ui::EditBox::create(mCodeText->getContentSize(), ui::Scale9Sprite::create("edit_box.png"));
	pCodeEdit->setPosition(Vec2(mCodeText->getContentSize().width/2,mCodeText->getContentSize().height/2));
	pCodeEdit->setFontSize(mCodeText->getFontSize());
	pCodeEdit->setFontColor(mCodeText->getColor());//
	pCodeEdit->setPlaceHolder(mCodeText->getPlaceHolder().c_str());//
	pCodeEdit->setPlaceholderFontSize(mCodeText->getFontSize());
	pCodeEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);

	mCodeText->addChild(pCodeEdit,10,999);
	//mCodeEdit  = (ui::EditBox*)mCodeText->getChildByTag(999);
}

void GameBindPhoneWidget::onClickBind(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		//发送绑定消息
		ui::EditBox*pCodeEdit = (ui::EditBox*)mCodeText->getChildByTag(999);
		if (strlen(pCodeEdit->getText())== 0 )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("验证码不能为空!")));
			return;
		}

		int code = atoi(pCodeEdit->getText());
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendBindPhoneRequest(code);
		
	}
}

void GameBindPhoneWidget::onClickGetSecurityCode(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		//发送获取验证码消息
		if (strlen(mEdit1->getText())== 0 )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("手机号不能为空!")));
			return;
		}

		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendGetVerifyCode(mEdit1->getText());
		btn_get->setVisible(false);
		mTimeOutImage->setVisible(true);

		mTimeOut = 60;
		__String* price_str =  __String::createWithFormat("%s(%ds)",CommonFunction::GBKToUTF8("重新获取").c_str(),mTimeOut);
		mTimeOutLabel->setText(price_str->getCString());
		
	}
}

void GameBindPhoneWidget::onTimeOut(float dt)
{
	if (mTimeOutImage->isVisible())
	{
		mTimeOut-- ;
		__String* price_str = __String::createWithFormat("%s(%ds)",CommonFunction::GBKToUTF8("重新获取").c_str(),mTimeOut);
		log("price_str1111 =%s",price_str->getCString());
		//log("price_str<><><><><> =%s",CommonFunction::GBKToUTF8(price_str->getCString()));
		mTimeOutLabel->setText(price_str->getCString());
	
		if (!mTimeOutLabel->isVisible())
		{
			mTimeOutLabel->setVisible(true);
		}
		if ( mTimeOut<=0)
		{
			mTimeOutImage->setVisible(false);
			btn_get->setVisible(true);
			mTimeOut = 60;
		}
	}
}

void GameBindPhoneWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParentAndCleanup(false);
	}
}

void GameBindPhoneWidget::receiveBindPhoneRsp(EventCustom* evt)
{
	CMD_GP_CaptchaRet* ret = (CMD_GP_CaptchaRet*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
	//绑定成功
	if (ret->lResultCode == 0)
	{
		OnlineUserModel* user = SessionManager::shareInstance()->getUser();
		user->setPhone(mEdit1->getText());
		mTimeOutImage->setVisible(false);
		btn_get->setVisible(true);
		mTimeOut = 60;
		////验证码置空
		ui::EditBox*pCodeEdit = (ui::EditBox*)mCodeText->getChildByTag(999);
		pCodeEdit->setText("");
		removeFromParent();
	}
}

void GameBindPhoneWidget::receiveGetCaptchaRsp(EventCustom* evt)
{
	CMD_GP_CaptchaRet* ret = (CMD_GP_CaptchaRet*)evt->getUserData();
	if (ret->lResultCode !=0)
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
		mTimeOutImage->setVisible(false);
		btn_get->setVisible(true);
		mTimeOut = 60;
	}
}
