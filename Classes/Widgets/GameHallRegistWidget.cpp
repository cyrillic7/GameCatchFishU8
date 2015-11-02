#include "GameHallRegistWidget.h"
#include "Common/ModalViewManager.h"
#include "Common/AlertWidget.h"
#include "CommonFunction.h"
#include "GameServiceClientManager.h"
#include "Common/CommonLoadingWidget.h"
#include "GameMainScene.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "SessionManager.h"
#include "CommonFunction.h"

#define  popBgTag 18
#define  BtnCloseTag 42

#define  TextFieldAccoutTag 400
#define  TextFieldPwdTag  409
#define  TextFieldPwdAgainTag 1463

#define  BtnRegisterTag     1432


GameHallRegisterWidget::GameHallRegisterWidget()
{

}

GameHallRegisterWidget::~GameHallRegisterWidget()
{

}

bool GameHallRegisterWidget::init()
{
	if(!TNWidget::init())
		return false;


	return true;
}

std::string GameHallRegisterWidget::getWidgetJsonName()
{
	return "registerWidget";
}

void GameHallRegisterWidget::onEnter()
{
	TNWidget::onEnter();
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginSuccessMsg, CC_CALLBACK_1(GameHallRegisterWidget::LoginSuccessRsp, this)), this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginFaildMsg, CC_CALLBACK_1(GameHallRegisterWidget::LoginFaildRsp, this)), this);

}

void GameHallRegisterWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallRegisterWidget::loadUI()
{

	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//绑定相关事件

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallRegisterWidget::onBack, this));


	mFieldAccount = static_cast<TextField*>(popBg->getChildByTag(TextFieldAccoutTag));

	mAccountEdit= ui::EditBox::create(mFieldAccount->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mAccountEdit->setPosition(Vec2(mFieldAccount->getContentSize().width/2,mFieldAccount->getContentSize().height/2));
	mAccountEdit->setFontSize(mFieldAccount->getFontSize());
	mAccountEdit->setFontColor(mFieldAccount->getColor());//设置文字颜色

	mAccountEdit->setPlaceHolder(mFieldAccount->getPlaceHolder().c_str());//点位符
	mAccountEdit->setPlaceholderFontSize(mFieldAccount->getFontSize());
	mAccountEdit->setText(mFieldAccount->getStringValue().c_str());
	mAccountEdit->setTag(mFieldAccount->getTag());
	mFieldAccount->addChild(mAccountEdit, 10);

	mFieldNick = static_cast<TextField*>(popBg->getChildByTag(TextFieldPwdTag));
	mNickEdit = ui::EditBox::create(mFieldNick->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mNickEdit->setPosition(Vec2(mFieldNick->getContentSize().width/2,mFieldNick->getContentSize().height/2));
	mNickEdit->setFontSize(mFieldNick->getFontSize());
	mNickEdit->setFontColor(mFieldNick->getColor());//设置文字颜色

	mNickEdit->setPlaceHolder(mFieldNick->getPlaceHolder().c_str());//点位符
	mNickEdit->setPlaceholderFontSize(mFieldNick->getFontSize());
	mNickEdit->setText(mFieldNick->getStringValue().c_str());
	mNickEdit->setTag(mFieldNick->getTag());
	mFieldNick->addChild(mNickEdit, 10);



	mFieldPwd = static_cast<TextField*>(popBg->getChildByTag(TextFieldPwdAgainTag));

	mPwdEdit = ui::EditBox::create(mFieldPwd->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mPwdEdit->setPosition(Vec2(mFieldPwd->getContentSize().width/2,mFieldPwd->getContentSize().height/2));
	mPwdEdit->setFontSize(mFieldPwd->getFontSize());
	mPwdEdit->setFontColor(mFieldPwd->getColor());//设置文字颜色

	mPwdEdit->setPlaceHolder(mFieldPwd->getPlaceHolder().c_str());//点位符
	mPwdEdit->setPlaceholderFontSize(mFieldPwd->getFontSize());
	mPwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mPwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mPwdEdit->setText(mFieldPwd->getStringValue().c_str());
	mPwdEdit->setTag(mFieldPwd->getTag());
	mFieldPwd->addChild(mPwdEdit, 10);
	
	Button* btn_register = static_cast<Button*>(popBg->getChildByTag(BtnRegisterTag));
	btn_register->addTouchEventListener(CC_CALLBACK_2(GameHallRegisterWidget::onRegister, this));

}

void GameHallRegisterWidget::onRegister(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		if (checkData())
		{
			__Dictionary* dic = __Dictionary::create();
			dic->setObject(__String::create(mAccountEdit->getText()),"account");
			dic->setObject(__String::create(mNickEdit->getText()),"nick");
			dic->setObject(__String::create(mPwdEdit->getText()),"password");
			
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(accountRegisterMsg,dic);
			removeFromParent();
			//GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->onRegister((char*)mAccountEdit->getText(),(char*)(CommonFunction::UTF8TOGBK((mNickEdit->getText()))),(char*)mPwdEdit->getText());
			//showLoading();
		}
	}
}

void GameHallRegisterWidget::onBack(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

bool GameHallRegisterWidget::checkData()
{
	bool ret = true;

	std::string account = mAccountEdit->getText();
	if (account.length() <=0)
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("帐号不能为空!")));
		return false;
	}
	else
	{
		if (CommonFunction::isDigit2(account) || CommonFunction::isAllChniese(account))
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("帐号非法!")));
			return false;
		}
	}

	std::string nick = mNickEdit->getText();
	if (nick.length() <=0)
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("昵称不能为空!")));
		return false ;
	}

	std::string pwd = mPwdEdit->getText();
	if (pwd.length() <=0)
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码不能为空!")));
		return false ;
	}
	else
	{
		if (CommonFunction::isDigit2(pwd))
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码不能为全为数字!")));
			return false ;
		}
	}
	
	return ret;
}

void GameHallRegisterWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		CommonLoadingWidget* loading = CommonLoadingWidget::create(visibleSize);
		loading->setTips("请稍候...");
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
		addChild(loading, 5);
	}
}

void GameHallRegisterWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}
void GameHallRegisterWidget::LoginSuccessRsp(EventCustom* evt)
{
	removeLoading();
	LoginUserModel *user = SessionManager::shareInstance()->getLoginModel();
	user->setAccount(mAccountEdit->getText());
	user->setPassword(mPwdEdit->getText());
	 
	SessionManager::shareInstance()->flush();
	//写到帐户下拉列表
	__Array* accounts = SessionManager::shareInstance()->getAccountList();
	if(SessionManager::shareInstance()->isNotLogin(mAccountEdit->getText()))
	{
		accounts->addObject(__String::createWithFormat("%s&%s",mAccountEdit->getText(),mPwdEdit->getText()));
	}


	SessionManager::shareInstance()->writeAccountFile();
	SessionManager::shareInstance()->readAssignInfo();
	SessionManager::shareInstance()->loadReadMsgInfo();
	removeFromParent();
	Director::sharedDirector()->replaceScene(MainScene::create());
}

void GameHallRegisterWidget::LoginFaildRsp(EventCustom* evt)
{
	removeLoading();
}
