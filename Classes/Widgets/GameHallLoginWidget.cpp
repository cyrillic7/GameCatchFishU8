#include "GameHallLoginWidget.h"
#include "SessionManager.h"
#include "GameServiceClientManager.h"
#include "Model/LoginUserModel.h"
#include "Common/AlertWidget.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"
#include "Common/CommonLoadingWidget.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "GameMainScene.h"
#include "GameHallRegistWidget.h"
#include "GameInputAccoutWidget.h"
#include "Common/pystring.h"

#define  popBgTag 18
#define  BtnCloseTag 42
#define  TextFeildAccoutTag 400
#define  TextFeildPwdTag    409 
#define  BtnRegisterTag     1424
#define  BtnLoginTag        1432

#define  BtnForgetPwdTag    3597

#define BtnAccountListTag  8270
#define ImageAccountTag    8271
#define ListAccountTag     8272


AcountItemWiget::AcountItemWiget()
{
	m_acount = "";
	m_pwd = "";
}

AcountItemWiget::~AcountItemWiget()
{

}

AcountItemWiget* AcountItemWiget::create(const char* acount,int width)
{
	AcountItemWiget* pWidget = new AcountItemWiget();
	if (pWidget&& pWidget->init(acount,width))
	{
		pWidget->autorelease();
		return pWidget;
	}
	delete pWidget;
	pWidget = nullptr;
	return nullptr;
}

bool AcountItemWiget::init(const char* acount,int width)
{
	if(!Layout::init())
		return false;
	setContentSize(Size(width,50));


	std::string info = acount;
	int pos = info.find('&');
	if (pos > 0 )
	{
		m_acount = pystring::slice(info,0,pos);
		m_pwd  = pystring::slice(info,pos+1,info.length());
	}
	

    m_shade= LayerColor::create();
	m_shade->setColor(ccc3(0,0,0));
	m_shade->setOpacity(180);
	m_shade->setPosition(Vec2(0,0));
	m_shade->setVisible(false);
	addChild(m_shade);

	Text* acountText =  Text::create();
	acountText->setText(m_acount);
	acountText->setFontSize(30);
	acountText->setTextColor(ccc4(0,0,0,255));
	acountText->setAnchorPoint(Vec2(0.5, 0.5));
	acountText->setPosition(Vec2(acountText->getContentSize().width/2+5,getContentSize().height/2));
	addChild(acountText);

	Button* btn_del = Button::create();
	btn_del->loadTextureNormal("btn_delete.png",ui::Widget::TextureResType::LOCAL);
	btn_del->loadTexturePressed("btn_delete.png",ui::Widget::TextureResType::LOCAL);
	btn_del->setPosition(Vec2(getContentSize().width - btn_del->getContentSize().width/2 -10,getContentSize().height/2));
	btn_del->addTouchEventListener(CC_CALLBACK_2(AcountItemWiget::onClickDelete,this));
	addChild(btn_del);

	return true;
}

void AcountItemWiget::onClickDelete(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		__Array* accounts = SessionManager::shareInstance()->getAccountList();
		int index = SessionManager::shareInstance()->getIndexByAccount(m_acount);
		if(index !=-1)
		{
			accounts->removeObjectAtIndex(index);
			//重新保存文件
			SessionManager::shareInstance()->writeAccountFile();
		}
		removeFromParent();

	}
}

//////////////////////////////////////////////////////////////////////////

GameHallLoginWidget::GameHallLoginWidget()
{
	mAccountEdit = nullptr;
	mPwdEdit = nullptr;
	mCurAccountItem = nullptr;
}

GameHallLoginWidget::~GameHallLoginWidget()
{

}

bool GameHallLoginWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}

	return true;
}

void GameHallLoginWidget::onEnter()
{
	TNWidget::onEnter();

	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginSuccessMsg, CC_CALLBACK_1(GameHallLoginWidget::LoginSuccessRsp, this)), this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginFaildMsg, CC_CALLBACK_1(GameHallLoginWidget::LoginFaildRsp, this)), this);


}

void GameHallLoginWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}

std::string GameHallLoginWidget::getWidgetJsonName()
{
	return "accountLoginWidget";
}

void GameHallLoginWidget::loadUI()
{
	//初始化界面相关数据，及事件绑定
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(popBgTag));

	m_mainWidget->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onClickPanel,this));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onBack, this));

	LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	loginModel->setAccountType(accountType::accountNormal);
	mFieldAccount = static_cast<TextField*>(popBg->getChildByTag(TextFeildAccoutTag));
	mFieldAccount->setText("");
	mFieldAccount->setVisible(false);

	mAccountEdit= ui::EditBox::create(mFieldAccount->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	//mAccountEdit->setPosition(Vec2(mFieldAccount->getContentSize().width/2,mFieldAccount->getContentSize().height/2));
	mAccountEdit->setAnchorPoint(Vec2(0.5,0.5));
	mAccountEdit->setPosition(Vec2(mFieldAccount->getPositionX()+mFieldAccount->getContentSize().width/2,mFieldAccount->getPositionY()));
	mAccountEdit->setFontSize(mFieldAccount->getFontSize());
	mAccountEdit->setFontColor(mFieldAccount->getColor());//设置文字颜色

	mAccountEdit->setPlaceHolder(mFieldAccount->getPlaceHolder().c_str());//点位符
	mAccountEdit->setPlaceholderFontSize(mFieldAccount->getFontSize());
	mAccountEdit->setText(loginModel->getAccount().c_str());
	mAccountEdit->setTag(mFieldAccount->getTag());
	mAccountEdit->setDelegate(this);
	popBg->addChild(mAccountEdit, 10);


	mFieldPwd = static_cast<TextField*>(popBg->getChildByTag(TextFeildPwdTag));
	mFieldPwd->setText("");
	//mFieldPwd->setText(loginModel->getPassword());
	//mFieldPwd->setEnabled(false);

	mPwdEdit = ui::EditBox::create(mFieldPwd->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mPwdEdit->setPosition(Vec2(mFieldPwd->getContentSize().width/2,mFieldPwd->getContentSize().height/2));
	mPwdEdit->setFontSize(mFieldPwd->getFontSize());
	mPwdEdit->setFontColor(mFieldPwd->getColor());//设置文字颜色

	mPwdEdit->setPlaceHolder(mFieldPwd->getPlaceHolder().c_str());//点位符
	mPwdEdit->setPlaceholderFontSize(mFieldPwd->getFontSize());
	mPwdEdit->setInputFlag(ui::EditBox::InputFlag::PASSWORD);
	mPwdEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	mPwdEdit->setText(loginModel->getPassword().c_str());
	mPwdEdit->setTag(mFieldPwd->getTag());
	mPwdEdit->setDelegate(this);
	mFieldPwd->addChild(mPwdEdit, 10);

	Button* btn_register = static_cast<Button*>(popBg->getChildByTag(BtnRegisterTag));
	btn_register->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onRegister, this));


	Button* btn_login = static_cast<Button*>(popBg->getChildByTag(BtnLoginTag));
	btn_login->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onLogin, this));


	Button* btn_forget = static_cast<Button*>(popBg->getChildByTag(BtnForgetPwdTag));
	btn_forget->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onForgetPwd, this));

	//帐户下拉列表

	Button* btn_accountList = static_cast<Button*>(popBg->getChildByTag(BtnAccountListTag));
	btn_accountList->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onShowAccountList, this));

	mAccountBg = static_cast<ImageView*>(popBg->getChildByTag(ImageAccountTag));
	mAccountBg->setVisible(false);
	mAccountList = static_cast<ListView*>(mAccountBg->getChildByTag(ListAccountTag));
	mAccountList->setBounceEnabled(true);

}

void GameHallLoginWidget::editBoxEditingDidBegin(ui::EditBox *editBox)    
{    
	auto editbox = (ui::EditBox*)editBox;  
	if (editBox->getTag() == TextFeildAccoutTag)// 帐号改变
	{
		mPwdEdit->setText("");
	}
}

void GameHallLoginWidget::editBoxEditingDidEnd(ui::EditBox *editBox)    
{    
	auto editbox = (ui::EditBox*)editBox;  
	if (editBox->getTag() == TextFeildAccoutTag)// 编辑帐号后，隐藏下拉框
	{
		mPwdEdit->setText("");
	}
} 

void GameHallLoginWidget::editBoxReturn(ui::EditBox *editBox)    
{ 
	auto editbox = (ui::EditBox*)editBox;  
	if (editBox->getTag() == TextFeildAccoutTag)//编辑帐号后，隐藏下拉框
	{
		if(mAccountBg->isVisible())
			mAccountBg->setVisible(false);
	}
}    
void GameHallLoginWidget::editBoxTextChanged(ui::EditBox *editBox,const std::string &text)    
{     
}  

bool GameHallLoginWidget::checkData(loginDataType type)
{
	bool ret = true;
	switch (type)
	{
	case Type_Password:
		{
			std::string pwd = mPwdEdit->getText();
			if (pwd.length() <=0)
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("密码不能为空!")));
				return false ;
			}
		}
		break;
	case Type_Accout:
		{
			std::string account = mAccountEdit->getText();
			if (account.length() <=0)
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("帐号不能为空!")));
				return false;
			}
		}
		break;
	}

	return ret;
}

void GameHallLoginWidget::onLogin(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		if(checkData(Type_Accout) && checkData(Type_Password))
		{
			//先缓存，登录成功后再写入本地文件
			LoginUserModel *user = SessionManager::shareInstance()->getLoginModel();
			user->setAccount(mAccountEdit->getText());
			user->setPassword(mPwdEdit->getText());
			user->setAccountType(accountType::accountNormal);
			
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(accountLoginMsg);
			removeFromParent();
			//GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->onLogin(mAccountEdit->getText(),mPwdEdit->getText());
			//showLoading();
			
		}
		
	
	}
}

void GameHallLoginWidget::onRegister(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallRegisterWidget::create());
		removeFromParent();
	}
}

void GameHallLoginWidget::onBack(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallLoginWidget::showLoading()
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

void GameHallLoginWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallLoginWidget::LoginSuccessRsp(EventCustom* evt)
{
	removeLoading();
	LoginUserModel *user = SessionManager::shareInstance()->getLoginModel();
	user->setAccount(mAccountEdit->getText());
	user->setPassword(mPwdEdit->getText());

	SessionManager::shareInstance()->flush();

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

void GameHallLoginWidget::LoginFaildRsp(EventCustom* evt)
{
	removeLoading();
//	CMD_MB_LogonFailure* errerLog = (CMD_MB_LogonFailure*)evt->getUserData();
//	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(errerLog->szDescribeString)));
}


void GameHallLoginWidget::onForgetPwd(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		
			removeFromParent();
			ModalViewManager::sharedInstance()->showWidget(GameInputAccoutWidget::create());
		
	}
}

void GameHallLoginWidget::onShowAccountList(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		mAccountBg->setVisible(!mAccountBg->isVisible());
		if (mAccountBg->isVisible())
		{
			mAccountList->removeAllItems();
			//加载帐号列表
			__Array* pInfo = SessionManager::shareInstance()->getAccountList();
			for (int i= 0; i < pInfo->count();i++)
			{
				std::string account = ((__String*)pInfo->getObjectAtIndex(i))->getCString();
				AcountItemWiget* pItem = AcountItemWiget::create(account.c_str(),mAccountList->getContentSize().width);
				pItem->setTouchEnabled(true);
				pItem->addTouchEventListener(CC_CALLBACK_2(GameHallLoginWidget::onSelectAccout,this));
				mAccountList->pushBackCustomItem(pItem);
			}
		}
	}
}

void GameHallLoginWidget::onSelectAccout(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	AcountItemWiget* pItem = (AcountItemWiget*)pSender;
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
	
		mAccountEdit->setText(pItem->getAccount().c_str());
		mAccountBg->setVisible(false);
		//以前帐号的密码清空
		mPwdEdit->setText(pItem->getPwd().c_str());
	
	}
}


void GameHallLoginWidget::onClickPanel(Ref* pSender,ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		mAccountBg->setVisible(false);
	}
}