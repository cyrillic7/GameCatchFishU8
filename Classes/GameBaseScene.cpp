#include "GameBaseScene.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "MsgDefine/CMD_GameServer.h"
#include "Common/ModalViewManager.h"
#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "LoginScene.h"
#include "Common/ExitWidget.h"
#include "Common/CommonLoadingWidget.h"
#include "GameScene.h"
#include "Common/pystring.h"
#include "SessionManager.h"

// game base scene 的子类需要加载UI的时候，接受该消息
#define GameBaseSceneNeedLoadUI "GameBaseSceneNeedLoadUI"

// game base scene 的子类需要重新进入房间的时候，接受该消息
#define GameBaseSceneNeedReenterRoom "GameBaseSceneNeedReenterRoom"

//打印loading时的日志
//#define PrintLoadingLog        printTestLog("-----------[ Loading... ] => [ zone: %s, function: %s login: %d]", m_gameName.c_str(), __FUNCTION__, m_serviceClient->getIsLogin());

//资源异步并发加载数
#define ResourceLoaderConcurrentCount       5

GameBaseScene::GameBaseScene()
	: m_serviceClient(NULL)
    , m_gameName("")
    , currentRoom("")
    , m_alreadyLoadedUI(false)
    , m_inGame(false)
	, m_isResourceLoaded(false)
	, bNeedRelogin(false)
	,m_resourceLoader(nullptr)
{
}

GameBaseScene::~GameBaseScene() {
	log("~GameBaseScene");
	CC_SAFE_RELEASE(m_serviceClient);
	log("~GameBaseScene end");
	if (bNeedRelogin)
	{
		okCallback();
	}

}

void GameBaseScene::onEnter() {
    Scene::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginTaskServerMsg, CC_CALLBACK_1(GameBaseScene::onloginTaskServer, this)), this);
    
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginAtOtherPlace, CC_CALLBACK_1(GameBaseScene::onloginAtOtherPlace, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ExitGameMsg, CC_CALLBACK_1(GameBaseScene::onExitGame, this)), this);

	//比赛开始消息
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(matchStartMsg, CC_CALLBACK_1(GameBaseScene::receiveMatchStartMsg, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginRealRoomSuccess, CC_CALLBACK_1(GameBaseScene::loginMatchRoomSuccessRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginRealRoomFailure, CC_CALLBACK_1(GameBaseScene::loginMatchRoomFailureRsp, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(EnterMatchRoomMsg, CC_CALLBACK_1(GameBaseScene::onEnterMatch, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(RemoveGameSoketMsg, CC_CALLBACK_1(GameBaseScene::ReceiveRemoveGameSoketMsg, this)), this);
	

	//ModalViewManager::sharedInstance()->removeAllWidgets(true);

    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginGameSucceedMessage, CC_CALLBACK_1(GameBaseScene::receiveLoginSucceedMessage, this)), this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoginGameFailedMessage, CC_CALLBACK_1(GameBaseScene::receiveLoginFailMessage, this)), this);

    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ConnectionLostMessage, CC_CALLBACK_1(GameBaseScene::handleConnectionLost, this)), this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ConnectionStartRetryMessage, CC_CALLBACK_1(GameBaseScene::receiveRetryConnectionMessage, this)), this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ConnectionResumeMessage, CC_CALLBACK_1(GameBaseScene::handleConnectionResume, this)), this);

	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ManualRetryConnectionMessage, CC_CALLBACK_1(GameBaseScene::receiveRetryConnectionMessage, this)), this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(CancelConnectionMessage, CC_CALLBACK_1(GameBaseScene::receiveCancelConnectionMessage, this)), this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GameServiceErrorOrSuccessResponseMessage, CC_CALLBACK_1(GameBaseScene::receiveErrorOrSucceedExtensionMessage, this)), this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(LoadingWidgetDidCloseMessage, CC_CALLBACK_1(GameBaseScene::receiveCloseLoadingWidgetMessage, this)), this);
	
	// 加载必须的资源
	loadResourceNeed();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameBaseScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	

}

void GameBaseScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {  
	switch(keyCode)     
	{            	
		case EventKeyboard::KeyCode::KEY_ESCAPE:  //监听返回键 
			{
				if(!SessionManager::shareInstance()->isInGame())
				{
					ModalViewManager::sharedInstance()->showWidget(ExitWidget::create());
				}else if (SessionManager::shareInstance()->isInGame())//检测是否处于长按状态 
				{
					Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(KeyEscapeMsg);
				}
				//Director::getInstance()->end();      
			}
			break;           
		case EventKeyboard::KeyCode::KEY_MENU:    //监听menu键          
			break;    
	} 
}

void GameBaseScene::onExit() {
    _eventDispatcher->removeEventListenersForTarget(this);

	if (nullptr != m_resourceLoader) {
		m_resourceLoader->purge();
	}

    Scene::onExit();
}

GameBaseScene* GameBaseScene::create() {
    GameBaseScene *w = new GameBaseScene;
    if (w && w->init()) {
        w->autorelease();
        return w;
    }
    CC_SAFE_DELETE(w);
    return NULL;
}

bool GameBaseScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	////登录, 大厅是自己调用登录接口
	//if (0 == strcmp(m_gameName.c_str(), XwZoneName)) {
	//	m_serviceClient->Connect("125.88.145.41",8100);
	//}

	return true;
}

void GameBaseScene::setServiceClient(GameServiceClient *client) {
	if (client != m_serviceClient) {
		CC_SAFE_RELEASE_NULL(m_serviceClient);
		m_serviceClient = client;
		m_serviceClient->retain();
	}
}

bool GameBaseScene::reenterRoom() {
    return false;
}

void GameBaseScene::setCurrentRoom(std::string room) {
    currentRoom = room;
}

std::string GameBaseScene::getCurrentRoom() {
    return currentRoom;
}

void GameBaseScene::setInGame(bool inGame) {
    m_inGame = inGame;
}

bool GameBaseScene::getInGame() const {
    return m_inGame;
}

__Array *GameBaseScene::getResourceNeedLoad() {
	__Array* a = __Array::create();

	return a;
}

void GameBaseScene::updateProgress(int resourceLeftToLoad)
{

}


void GameBaseScene::loadResourceNeed() {
	__Array* r = getResourceNeedLoad();

	if (NULL != r && r->count() > 0) {
		if (NULL == m_resourceLoader) {
			m_resourceLoader = ResourceLoader::create(this);
			CC_SAFE_RETAIN(m_resourceLoader);
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
			m_resourceLoader->setConcurrentCount(ResourceLoaderConcurrentCount);
			CCLOG("ResourceLoaderConcurrentCount = %d ",ResourceLoaderConcurrentCount);
#endif
		}
		m_resourceLoader->loadResourceAsync(r);
	} else {
		actionAfterLoadResource();
	}
}

void GameBaseScene::oneResourceLoaded(ResourceDefinition *d, int resourceLeftToLoad) {
	if (resourceLeftToLoad <= 0) {
		actionAfterLoadResource();
	}
	CCLOG("res %s is load sucess",d->getPath().c_str());
	//todo 进度条的进度更新
	updateProgress(resourceLeftToLoad);
	
}

void GameBaseScene::actionAfterLoadResource() {
	//资源加载结束
	m_isResourceLoaded = true;
}

void GameBaseScene::onloginTaskServer(EventCustom* evt)
{
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(taskClient);
	if(c == NULL)
	{
		m->registerServiceClient(taskClient, false,TASK_SORKET_INDEX);
	    c = m->serviceClientForName(taskClient);
	}
	//bool ret = c->Connect(SessionManager::shareInstance()->getTaskServer().c_str(),SessionManager::shareInstance()->getTaskPort());
	//if (ret)
	{
		c->loginTaskServer();
	}
}

void GameBaseScene::onloginAtOtherPlace(EventCustom* evt)
{
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(taskClient);
	if(c && c->isConnect() )
	{
		CMD_GL_SystemMessage* info = (CMD_GL_SystemMessage*)evt->getUserData();
		AlertWidget* pWidget = AlertWidget::create(this,"",CommonFunction::GBKToUTF8(info->szString));
		pWidget->setName(WIDGET_CANNOT_REMVOVE_FLAG);
		ModalViewManager::sharedInstance()->showWidget(pWidget);
		SessionManager::shareInstance()->clearMsgArray();
		SessionManager::shareInstance()->clearMatchRooms();
		bNeedRelogin = true;
	}

}

void GameBaseScene::receiveMatchStartMsg(EventCustom* evt)
{

	ModalViewManager::sharedInstance()->removeAllWidgets();

	CMD_GL_MatchServer * pMatchServer =(CMD_GL_MatchServer *)evt->getUserData();
	SessionManager::shareInstance()->setMatchStart(true);
	//保存当前房间比赛唯 一标识 供继续比赛使用
	MatchSerialModel* pSerialModel = SessionManager::shareInstance()->getRoomSerial();
	pSerialModel->setKindID(pMatchServer->MatchSerial.dwKindID);
	pSerialModel->setMatchInfoID(pMatchServer->MatchSerial.dwMatchInfoID);
	pSerialModel->setMatchType(pMatchServer->MatchSerial.dwMatchType);
	pSerialModel->setFullPlayerNum(pMatchServer->MatchSerial.dwFullPlayerNum);
	std::string sort_str = __String::createWithFormat("%lld",pMatchServer->MatchSerial.dwMatchInfoID)->getCString();
	std::string id_str = pystring::slice(sort_str,0,2);
	pSerialModel->setSortId(atoi(id_str.c_str()));
	//在普通游戏房间提示
	if (SessionManager::shareInstance()->isInGame())
	{

		MatchInfoModel* pMatchInfo = SessionManager::shareInstance()->getMatchInfoByID(pSerialModel->getMatchInfoID());
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(MatchIsBeingStartMsg,pMatchInfo);
		
		SessionManager::shareInstance()->setMatchSever(pMatchServer->szServerIP);
		SessionManager::shareInstance()->setMatchPort(pMatchServer->dwServerPort);
	}
	else
	{
		//创建SORKET
		createGameSorket();
		//连接服务器
		connectGameServer(pMatchServer->szServerIP,pMatchServer->dwServerPort);

		__Array* matchRooms = SessionManager::shareInstance()->getMatchRooms();
		for ( int i = 0 ; i < matchRooms->count();i++)
		{
			MatchInfoModel* pInfoModel = (MatchInfoModel*)matchRooms->getObjectAtIndex(i);
			if (pInfoModel->getSerialModel()->getMatchInfoID() == pMatchServer->MatchSerial.dwMatchInfoID )
			{
				pInfoModel->setsignUpStatus(SignUpStatus::NoSignUp);
				break;
			}
		}
	}
}


void GameBaseScene::createGameSorket()
{
	//初始化SOCKET
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(XwZoneDntg);

	if(c)
	{
		GameServiceClientManager::sharedInstance()->removeServiceClient(XwZoneDntg);
	}
	//创建
	m->registerServiceClient(XwZoneDntg, false,GAMEROOM_SORKET_INDEX);	
}

void GameBaseScene::connectGameServer(const char* ip ,int port)
{
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(XwZoneDntg);

	bool ret = c->Connect(ip,port);
	//连接成功
	if(ret)
	{
		showLoading();
		c->loginMatchRoom();
	}
	else
	{
		removeLoading();
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("游戏服务器异常，请联系客服!")));
	}
}

void GameBaseScene::loginMatchRoomSuccessRsp(EventCustom* evt)
{
	//普通捕鱼进入游戏不处理
	if (!SessionManager::shareInstance()->getMatchStart()) return;
	removeLoading();
	SessionManager::shareInstance()->setGameKind(GameKindDntg);
	//进入游戏之前加载鱼的游走动画和死亡动画数据
	ResourcesManager::getInstance()->loadResourceFile(GameKindDntg);
	ResourcesManager::getInstance()->loadGuideFile(GameKindDntg);
	SessionManager::shareInstance()->setInGame(true);
	SessionManager::shareInstance()->setMatchStart(false);
	Director::sharedDirector()->replaceScene(GameScene::create(RoomLevel_2,GameKindDntg,true));
}

void GameBaseScene::loginMatchRoomFailureRsp(EventCustom* evt)
{
	//普通捕鱼进入游戏不处理
	if (!SessionManager::shareInstance()->getMatchStart()) return;
	removeLoading();
	CMD_GR_LogonFailure* errorCode = (CMD_GR_LogonFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(errorCode->szDescribeString)));
}

void GameBaseScene::onEnterMatch(EventCustom* evt)
{
	//创建SORKET
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(XwZoneDntg);
	//清除上次的SOKET 
	if(c)
	{
		c->closeSoket();
		m->removeServiceClient(XwZoneDntg);
	}
	else
	{
		//创建新的
		m->registerServiceClient(XwZoneDntg, false,GAMEROOM_SORKET_INDEX);
		c = m->serviceClientForName(XwZoneDntg);
	}
	
	//连接服务器
	connectGameServer(SessionManager::shareInstance()->getMatchSever().c_str(),SessionManager::shareInstance()->getMatchPort());

	MatchSerialModel* pSerialModel = SessionManager::shareInstance()->getRoomSerial();
	__Array* matchRooms = SessionManager::shareInstance()->getMatchRooms();
	for ( int i = 0 ; i < matchRooms->count();i++)
	{
		MatchInfoModel* pInfoModel = (MatchInfoModel*)matchRooms->getObjectAtIndex(i);
		if (pInfoModel->getSerialModel()->getMatchInfoID() == pSerialModel->getMatchInfoID() )
		{
			pInfoModel->setsignUpStatus(SignUpStatus::NoSignUp);
			break;
		}
	}
}

void GameBaseScene::okCallback()
{
	GameServiceClientManager::sharedInstance()->removeServiceClient(taskClient);
	ModalViewManager::sharedInstance()->removeAllWidgets();
	SessionManager::shareInstance()->setInGame(false);
	Director::sharedDirector()->replaceScene(LoginScene::create());
}

void GameBaseScene::onExitGame(EventCustom* evt)
{
	Director::getInstance()->end();  
}

void GameBaseScene::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameBaseScene::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		getChildByTag(COMMON_LOADING_TAG)->removeFromParentAndCleanup(false);
	}
}

void GameBaseScene::ReceiveRemoveGameSoketMsg(EventCustom* evt)
{
	GameServiceClientManager::sharedInstance()->removeServiceClient(XwZoneDntg);
}
