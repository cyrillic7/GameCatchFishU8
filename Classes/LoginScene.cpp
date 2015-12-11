
#include "LoginScene.h"
#include "LoginLayer.h"
#include "Common/ResourceLoader.h"
#include "GameServiceClientManager.h"
#include "Model/LoginUserModel.h"
#include "SessionManager.h"
#include "GameMainScene.h"
#include "Common/AlertWidget.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"
#include "Common/CommonLoadingWidget.h"
#include "Widgets/GameHallLoginWidget.h"
#include "SoundManager.h"
#include "crypto/CCCrypto.h"
#include "Widgets/GameUpdateWidget.h"
#include "Common/pystring.h"
#include "Common/Statistics.h"

LoginScene::LoginScene()
{
	m_gameName = XwZoneName;
	m_Account = "";
	m_Password = "";
	m_token = "";
	mbQQLogin = false;
	mBRegister = false;
	mbFastLogin = false;
	mRegisterInfo = nullptr;
	memset(&mQucikAccout,0,sizeof(mQucikAccout));
	mUpateInfo = nullptr;
}

LoginScene::~LoginScene()
{
	CC_SAFE_RELEASE_NULL(mRegisterInfo);;
	if (mUpateInfo)
	{
		CC_SAFE_RELEASE_NULL(mUpateInfo);
	}
}

LoginScene* LoginScene::create(__Dictionary* pUpdateInfo)
{
	log("LoginScene ::create");
	LoginScene* pScene = new LoginScene();
	if (pScene&& pScene->init(pUpdateInfo))
	{
		pScene->autorelease();
		return pScene;
	}
	delete pScene;
	pScene = nullptr;
	return pScene;
}

bool LoginScene::init(__Dictionary* pUpdateInfo)
{
	if (!GameBaseScene::init()) {
		return false;
	}
	if(pUpdateInfo)
	{
		mUpateInfo = pUpdateInfo;
		mUpateInfo->retain();
	}

	return true;
}

void LoginScene::onEnter()
{
	GameBaseScene::onEnter();

	SoundManager::playMusicBackground("login_bgmusic.m4a",true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginSuccessMsg, CC_CALLBACK_1(LoginScene::LoginSuccessRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginFaildMsg, CC_CALLBACK_1(LoginScene::LoginFaildRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(RequestFastLoginNotification, CC_CALLBACK_1(LoginScene::sendfastLogin, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(qucikLoginByAccount, CC_CALLBACK_1(LoginScene::onFastLogin, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(qqLoginMsg, CC_CALLBACK_1(LoginScene::QQLogin, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(accountLoginMsg, CC_CALLBACK_1(LoginScene::AccountLogin, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(QucikLoginRspMsg,CC_CALLBACK_1(LoginScene::FastLoginRsp,this)),this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(accountRegisterMsg,CC_CALLBACK_1(LoginScene::AccountRegister,this)),this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(UpdateMsg,CC_CALLBACK_1(LoginScene::goToUpdate,this)),this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(CancelUpdateMsg,CC_CALLBACK_1(LoginScene::CancelUpdate,this)),this);

	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(netWorkValidMsg, CC_CALLBACK_1(LoginScene::netWorkIsValid, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(wxLoginMsg, CC_CALLBACK_1(LoginScene::wxLogin, this)), this);

	TextureCache::sharedTextureCache()->addImage("login_bg.png");

	std::string imageName = "";
	std::vector<std::string> images;
	

	// 注册 [SmartFox client], 有可能复用已经存在的。
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	if (!m->serviceClientForName(m_gameName.c_str()))
	{
		m->registerServiceClient(m_gameName.c_str(), false,FRAME_SORKET_INDEX);
	}
	
	GameServiceClient* c = m->serviceClientForName(m_gameName.c_str());

	// 当前client设置成大厅的client
	GameServiceClientManager::sharedInstance()->setCurrentServiceClientName(m_gameName.c_str());

	// 因为可能复用已经存在的，所以恢复响应
	c->setNeedPassToResponseHandler(true);
	setServiceClient(c);

	loadUI();
	log("onEnter end");
}

void LoginScene::onEnterTransitionDidFinish()
{
	if(mUpateInfo)
	{
		bool isNeedUpdate = ((__Bool*)mUpateInfo->objectForKey("needUpdate"))->getValue();
		if (isNeedUpdate)
		{
			std::string content = ((__String*)mUpateInfo->objectForKey("updateDesc"))->getCString();
			ModalViewManager::sharedInstance()->showWidget(UpdateWidget::create(content.c_str()));
		}
	}
}

void LoginScene::goToUpdate(EventCustom* evt)
{
	__String* url = (__String*)mUpateInfo->objectForKey("updateUrl");
	CommonFunction::updateGame(url->getCString());
}

void LoginScene::CancelUpdate(EventCustom* evt)
{
	int updateType = ((__Integer*)mUpateInfo->objectForKey("updateType"))->getValue();
	if (updateType ==1) //强制更新取消退出游戏
	{
		Director::sharedDirector()->end();
	}
}

__Array* LoginScene::getResourceNeedLoad()
{
	__Array* a  = GameBaseScene::getResourceNeedLoad();

	__String* imageName = nullptr;
	ResourceDefinition* d = nullptr;
	for (int i = 0 ; i < 10 ; i++)
	{
		imageName = __String::createWithFormat("common/commonloading/loading_logo_%d.png",i+1);
		d = ResourceDefinition::create(imageName->getCString(), ResourceTypeImage);
	}
	return a;
}

void LoginScene::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	GameBaseScene::onExit();	
}

void LoginScene::loadUI()
{
	auto layer = LoginLayer::create();
	this->addChild(layer);
}

void LoginScene::showLoading()
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

void LoginScene::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void LoginScene::sendfastLogin(EventCustom* evt)
{
	std::string uuid = CommonFunction::getUUID();
	std::string Bit32UUid = Crypto::MD5String((void*)uuid.c_str(), strlen(uuid.c_str()));
	Bit32UUid = "mobile" + pystring::slice(Bit32UUid,0,26);
	if (!m_serviceClient->Connect(SessionManager::shareInstance()->getLoginAddr().c_str(), 8100))
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr, "", CommonFunction::GBKToUTF8("连接失败，请查看您的网络!")));
	}
	else
	{
		showLoading();
		m_serviceClient->sendQucikLogin(Bit32UUid.c_str());
	}
	
	
	/*LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
	if (loginModel->getAccount().length() <=0 || loginModel->getPassword().length() <=0)
	{
	ModalViewManager::sharedInstance()->showWidget(GameHallLoginWidget::create());
	}
	else
	{
	m_serviceClient->onLogin(loginModel->getAccount().c_str(),loginModel->getPassword().c_str());
	showLoading();
	}*/
}

void LoginScene::onFastLogin(EventCustom* evt)
{
	LoginUserModel*  model = SessionManager::shareInstance()->getLoginModel();
	//mbFastLogin = true;
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(m_gameName.c_str());
	
	if (!c->Connect(SessionManager::shareInstance()->getLoginAddr().c_str(), 8100))
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr, "", CommonFunction::GBKToUTF8("连接失败，请查看您的网络!")));
	}
	else
	{
		c->onLogin(model->getQucikAccount().c_str(), model->getQucikPwd().c_str());
		showLoading();
	}
	
}

void LoginScene::FastLoginRsp(EventCustom* evt)
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(m_gameName.c_str());

	CMD_MB_Quick_Logon_Success* accountInfo = (CMD_MB_Quick_Logon_Success*)evt->getUserData();
	LoginUserModel*  model = SessionManager::shareInstance()->getLoginModel();
	log("FastLoginRsp account= %s",accountInfo->szAccounts);
	if(accountInfo->lResultCode == 0)
	{
		mbFastLogin = true;
		memcpy(&mQucikAccout,accountInfo,sizeof(CMD_MB_Quick_Logon_Success));
		c->onLogin(accountInfo->szAccounts,accountInfo->szLogonPass);
	}
	else
	{
		removeLoading();
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(accountInfo->szDescribeString)));
	}
}

void LoginScene::QQLogin(EventCustom* evt)
{
	mbQQLogin = true;
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	m_Account  = ((__String*)info->objectForKey("account"))->getCString();
	m_Password = ((__String*)info->objectForKey("password"))->getCString();
	///for网页退回来死机问题
	scheduleOnce(SEL_SCHEDULE(&LoginScene::logonGameByQQ),0.5);
}

void LoginScene::wxLogin(EventCustom* evt)
{
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	///for网页退回来死机问题
	m_token = ((__String*)info->objectForKey("token"))->getCString();
	scheduleOnce(SEL_SCHEDULE(&LoginScene::logonGameByWx), 0.5);
}

void LoginScene::logonGameByWx(float dt)
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(m_gameName.c_str());
	showLoading();
	log("[ logonGameByWx ] => [ m_token: %s ]", m_token.c_str());
	c->onWxLogin(m_token.c_str());

}

void LoginScene::logonGameByQQ(float dt)
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(m_gameName.c_str());
	showLoading();
	c->onLogin(m_Account.c_str(),m_Password.c_str());
	
}

void LoginScene::AccountLogin(EventCustom* evt)
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(m_gameName.c_str());
	LoginUserModel *userModel = SessionManager::shareInstance()->getLoginModel();
	//showLoading();
	c->onLogin(userModel->getAccount().c_str(),userModel->getPassword().c_str());
	
}

void LoginScene::AccountRegister(EventCustom* evt)
{
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(m_gameName.c_str());
	__Dictionary* infoDic = (__Dictionary*)evt->getUserData();
	__String* pAccount = (__String*)infoDic->objectForKey("account");
	__String* pNick = (__String*)infoDic->objectForKey("nick");
	__String* pPwd = (__String*)infoDic->objectForKey("password");
	
	if (mRegisterInfo != infoDic)
	{
		CC_SAFE_RELEASE_NULL(mRegisterInfo);
		mRegisterInfo = infoDic;
		mRegisterInfo->retain();
	}
	mBRegister = true;
	//showLoading();
	c->onRegister((char*)pAccount->getCString(),(char*)(CommonFunction::UTF8TOGBK((pNick->getCString()))).c_str(),(char*)pPwd->getCString());
	
}


void LoginScene::LoginSuccessRsp(EventCustom* evt)
{
	removeLoading();
	LoginUserModel *userModel = SessionManager::shareInstance()->getLoginModel();
	log("LoginSuccessRsp AccountType  =  %d",(int)userModel->getAccountType());
	if (mbQQLogin)
	{
		userModel->setAccount(m_Account);
		userModel->setPassword(m_Password);
		userModel->setAccountType(accountType::accountQQ);
		//SessionManager::shareInstance()->flush();
	}
	else
	{
		//注册的帐号
		if(mBRegister)
		{
			__String* pAccount = (__String*)mRegisterInfo->objectForKey("account");
			__String* pPwd = (__String*)mRegisterInfo->objectForKey("password");
			userModel->setAccount(pAccount->getCString());
			userModel->setPassword(pPwd->getCString());
			userModel->setAccountType(accountType::accountNormal);
			//注册统计
			Statistics::shareInstance()->sendStatisticsData(Statistics::S_REGISTER);
		}
		else if(mbFastLogin)
		{
			if (strcmp(userModel->getQucikAccount().c_str(),mQucikAccout.szAccounts) !=0  || strcmp(userModel->getQucikPwd().c_str(),mQucikAccout.szLogonPass) == 0)
			{
				userModel->setQucikPwd(mQucikAccout.szLogonPass);
				userModel->setQucikAccount(mQucikAccout.szAccounts);
			}
			userModel->setAccountType(accountType::accountMachineID);
		}

		if(userModel->getAccountType() == accountType::accountNormal)
		{
			__Array* accounts = SessionManager::shareInstance()->getAccountList();
			if(SessionManager::shareInstance()->isNotLogin(userModel->getAccount().c_str()))
			{
				accounts->addObject(__String::createWithFormat("%s&%s",userModel->getAccount().c_str(),userModel->getPassword().c_str()));
				SessionManager::shareInstance()->writeAccountFile();
			}

			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(closeLoginDialogMsg);
		}
		SessionManager::shareInstance()->flush();
	}

	//登录统计
	Statistics::shareInstance()->sendStatisticsData(Statistics::S_ACCOUNT_LOGON);

	SessionManager::shareInstance()->readAssignInfo();
	SessionManager::shareInstance()->loadReadMsgInfo();
	Director::sharedDirector()->replaceScene(MainScene::create());

}

void LoginScene::LoginFaildRsp(EventCustom* evt)
{
	mbQQLogin =false;
	mbFastLogin = false;
	mBRegister = false;
	removeLoading();
	CMD_MB_LogonFailure* errerLog = (CMD_MB_LogonFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(errerLog->szDescribeString)));
}

void LoginScene::netWorkIsValid(EventCustom* evt)
{
	removeLoading();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr, "", CommonFunction::GBKToUTF8("连接失败，请查看您的网络!")));
}



