#include "Common/ResourceLoader.h"
#include "GameServiceClientManager.h"
#include "Model/LoginUserModel.h"
#include "SessionManager.h"
#include "GameSmallScene.h"
#include "Common/AlertWidget.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"
#include "Common/CommonLoadingWidget.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "MsgDefine/CMD_GameServer.h"
#include "GameScene.h"
#include "GameMainScene.h"
#include "Message.h"
#include "SoundManager.h"
#include "Common/TNVisibleRect.h"

#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

#define  ImageBgTag 4
#define  ImageJcpyTag 6
#define  ImageLkpyTag 7
#define  ImageDntgTag 8

const Position kBubblePos[5] = {
	{ 256.5f,256.f }, { 512.5f, 665.f }, { 1188.5f, 256.f},
	{ 256.5f, 665.5f }, {808.5f, 665.f }
};


GameSmallScene::GameSmallScene()
{
	mBubbleArray = __Array::create();
	CC_SAFE_RETAIN(mBubbleArray);
	mRoomLevel = -1;
	mSelectGameKind = -1;
	mLoading_forJc = nullptr;
	mLoading_forLk = nullptr;
}

GameSmallScene::~GameSmallScene()
{
	CC_SAFE_RELEASE_NULL(mBubbleArray);
}

 GameSmallScene* GameSmallScene::create(int roomLevel)
{
	GameSmallScene* pScene = new GameSmallScene();
	if (pScene && pScene->init(roomLevel))
	{
		pScene->autorelease();
		return pScene;
	}
	else
	{
		delete pScene;
		pScene = nullptr;
		return nullptr;
	}
}

bool GameSmallScene::init(int roomLevel)
{
	if (!GameBaseScene::init()) {
		return false;
	}
	mRoomLevel = roomLevel;

	std::string path = FileUtils::getInstance()->getWritablePath() + GameResourcePackStoragePath;
	ResourcePackManager::sharedInstance()->createSaveDirectory(path.c_str());
	ResourcePackManager::sharedInstance()->start();

	return true;
}

void GameSmallScene::onEnter()
{
	GameBaseScene::onEnter();
	ResourcePackManager::sharedInstance()->setDelegate(this);
	SoundManager::playMusicBackground("room_bgmusic.m4a",true);

	//绑定签到返消息
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginRealRoomSuccess, CC_CALLBACK_1(GameSmallScene::loginRealRoomSuccessRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(loginRealRoomFailure, CC_CALLBACK_1(GameSmallScene::loginRealRoomFailureRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(connectGameServerFaildMsg, CC_CALLBACK_1(GameSmallScene::receiveConnectFailMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(downLoadReponseMsg, CC_CALLBACK_1(GameSmallScene::receiveDownLoadInfo, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(treasureInfoMsg, CC_CALLBACK_1(GameSmallScene::refreshTreasureData, this)), this);
	loadUI();

}

void GameSmallScene::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	ResourcePackManager::sharedInstance()->setDelegate(NULL);
	GameBaseScene::onExit();	
}

char* GameSmallScene::getWidgetJsonName()
{
	return "smallHallScene";
}

void GameSmallScene::loadUI()
{
	log("GameSmallScene::loadUI()");
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_mainWidget = CommonFunction::getWidgetFromCocostudioFile(getWidgetJsonName());
	m_mainWidget->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	m_mainWidget->setAnchorPoint(Vec2(0.5,0.5));
	addChild(m_mainWidget);
	
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Splash/AnimationTitleLogon.ExportJson");  
	Armature* armature = Armature::create("AnimationTitleLogon");
	armature->setAnchorPoint(ccp(0.5, 0.5));
	armature->setPosition(Vec2(kScreenWidth/2, kScreenHeight- 60));
	armature->getAnimation()->play("Animation1",1.5f,1);
	this->addChild(armature);

	//绑定相关事件
	ImageView* bg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageBgTag));

	ResourcePackManager *man = ResourcePackManager::sharedInstance();	

	//金蝉
	ImageView* imageJc = static_cast<ImageView*>(bg->getChildByTag(ImageJcpyTag));
	imageJc->addTouchEventListener(CC_CALLBACK_2(GameSmallScene::onClickJcpy,this));

	mLoading_forJc = SmallLoadingProgressWidget::create(false);
	mLoading_forJc->setPosition(Vec2(imageJc->getContentSize().width/2,imageJc->getContentSize().height/2));
	mLoading_forJc->setVisible(man->isInDownPackList("jcpy"));
	mLoading_forJc->setProgress(0);
	mLoading_forJc->setZOrder(5);
	imageJc->addChild(mLoading_forJc);

	//李逵
	ImageView* imageLk = static_cast<ImageView*>(bg->getChildByTag(ImageLkpyTag));
	imageLk->addTouchEventListener(CC_CALLBACK_2(GameSmallScene::onClickLkpy,this));

	mLoading_forLk = SmallLoadingProgressWidget::create(false);
	mLoading_forLk->setPosition(Vec2(imageLk->getContentSize().width/2,imageLk->getContentSize().height/2));
	mLoading_forLk->setVisible(man->isInDownPackList("lkpy"));
	mLoading_forLk->setProgress(0);
	mLoading_forLk->setZOrder(5);
	imageLk->addChild(mLoading_forLk);
	//悟空
	ImageView* imageWk = static_cast<ImageView*>(bg->getChildByTag(ImageDntgTag));
	imageWk->addTouchEventListener(CC_CALLBACK_2(GameSmallScene::onClickDntg,this));


	ImageView* imageBack = ImageView::create();
	imageBack->setTouchEnabled(true);
	imageBack->loadTexture("back.png");
	imageBack->setPosition(Vec2( imageBack->getContentSize().width/2 + 100, kScreenHeight - imageBack->getContentSize().height/2 - 100));
	imageBack->setTouchSize(Size(100,100));
	addChild(imageBack);

	Sprite* qipaoSp = Sprite::create();
	qipaoSp->setPosition(Vec2(35,30));
	imageBack->addChild(qipaoSp);
	std::vector<std::string> imageNames;
	std::string path = "";
	for (int i = 0 ; i < 6;i++)
	{
		if (i < 4)
			path = "qipao"+numberToString(i+1)+".png";
		else
			path = "qipao"+numberToString(4-i%3)+".png";

		imageNames.push_back(path);
	}

	Animate* pAnima = CommonFunction::createFrameAnimate(imageNames,0.2);
	qipaoSp->runAction(RepeatForever::create(pAnima));

	imageBack->addTouchEventListener(CC_CALLBACK_2(GameSmallScene::onClickBack,this));
	//起定时器
	//Director::getInstance()->getScheduler()->schedule(schedule_selector(GameSmallScene::update), this,1/30.f, false);

}

void GameSmallScene::onClickBack(Ref* pSender,ui::Widget::TouchEventType type)
{

	if(type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);

	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->replaceScene(MainScene::create());
	}
}


void GameSmallScene::onClickJcpy(Ref* pSender,ui::Widget::TouchEventType type)
{

	if(type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);

	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
#if 1
		ResourcePackManager *man = ResourcePackManager::sharedInstance();	
		if (!man->isResourceDownloaded("jcpy"))
		{

			ResourcePack* pack = man->getCurrentDownloadingResourcePack();
			if (pack && (strcmp("jcpy",pack->getName().c_str()) == 0  || strcmp("lkpy",pack->getName().c_str()) == 0))
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("正在下载，请稍候！")));
				return;
			}
			else
			{
					std::string netType = CommonFunction::getNetWorkType();
					log("net type = %s",netType.c_str());

					if (strcmp(netType.c_str(),"null") ==0)
					{
#if CC_TARGET_PLATFORM  != CC_PLATFORM_WIN32
						ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("当前没有网络，请查看您的网络！")));
						return;
#else
						GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendDownLoadUrlRequest(GameKindJcpy);
						showLoading();
						return;
#endif
					}

					if (strcmp(netType.c_str(),"wifi") ==0)
					{
						GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendDownLoadUrlRequest(GameKindJcpy);
						showLoading();
					}
					else
					{
						__String* desc = __String::createWithFormat("%s%s%s",CommonFunction::GBKToUTF8("您当前正处于").c_str(),netType.c_str(),CommonFunction::GBKToUTF8("网络下，建议用WIFI进行下载，是否继续？").c_str());
						ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",desc->getCString(),true));
					}

					/*ResourcePack* pack = man->getCurrentDownloadingResourcePack();
					if (pack && strcmp("jcpy",pack->getName().c_str()) == 0)
					{
					ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("正在下载，请稍候！")));
					}
					else
					{
					GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendDownLoadUrlRequest(GameKindJcpy);
					showLoading();
					}*/
				   return;
			}
		}
#endif
		__Array* pRooms = SessionManager::shareInstance()->getAllRoomLevels();
		if (pRooms == nullptr || pRooms->count() == 0)
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("房间列表为空，请返回到大厅重新进入!")));
		else
		{
			createGameSorket();
			RoomLevelInfo* info = SessionManager::shareInstance()->getLevelInfoByRoomLevel(GameKindJcpy,mRoomLevel);
			if(info)
			{
				connectGameServer(info);
				mSelectGameKind = GameKindJcpy;
			}
			else
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("游戏尚未上线,请稍候!")));
			}
		}
	}
}

void GameSmallScene::onClickLkpy(Ref* pSender,ui::Widget::TouchEventType type)
{
	if(type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);

	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		ResourcePackManager *man = ResourcePackManager::sharedInstance();
#if 1
		if (!man->isResourceDownloaded("lkpy"))
		{
			ResourcePack* pack = man->getCurrentDownloadingResourcePack();
			if (pack && (strcmp("jcpy",pack->getName().c_str()) == 0  || strcmp("lkpy",pack->getName().c_str()) == 0))
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("正在下载，请稍候！")));
				return;
			}
			else
			{
				std::string netType = CommonFunction::getNetWorkType();
				log("net type = %s",netType.c_str());
				if (strcmp(netType.c_str(),"null") ==0)
				{
#if CC_TARGET_PLATFORM  != CC_PLATFORM_WIN32
					ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("当前没有网络，请查看您的网络！")));
					return;
#else
					GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendDownLoadUrlRequest(GameKindLkpy);
					showLoading();
					return;
#endif
				}

				if (strcmp(netType.c_str(),"wifi") ==0)
				{
					GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendDownLoadUrlRequest(GameKindLkpy);
					showLoading();
				}
				else
				{
					__String* desc = __String::createWithFormat("%s%s%s",CommonFunction::GBKToUTF8("您当前正处于").c_str(),netType.c_str(),CommonFunction::GBKToUTF8("网络下，建议用WIFI进行下载，是否继续？").c_str());
					ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",desc->getCString(),true));
				}
			
			}
			return;
		}
#endif
		__Array* pRooms = SessionManager::shareInstance()->getAllRoomLevels();
		if (pRooms == nullptr || pRooms->count() == 0)
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("房间列表为空，请返回到大厅重新进入!")));
		else
		{
			createGameSorket();
			RoomLevelInfo* info = SessionManager::shareInstance()->getLevelInfoByRoomLevel(GameKindLkpy,mRoomLevel);
			if(info)
			{
				connectGameServer(info);
				mSelectGameKind = GameKindLkpy;
			}
			else
			{
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("游戏尚未上线,请稍候!")));
			}
		}
	}
}

void GameSmallScene::okCallback()
{
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendDownLoadUrlRequest(mSelectGameKind);
	showLoading();
}

void GameSmallScene::onClickDntg(Ref* pSender,ui::Widget::TouchEventType type)
{
	if(type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);

	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		__Array* pRooms = SessionManager::shareInstance()->getAllRoomLevels();
		if (pRooms == nullptr || pRooms->count() == 0)
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("房间列表为空，请返回到大厅重新进入!")));
		else
		{
			createGameSorket();
			RoomLevelInfo* info = SessionManager::shareInstance()->getLevelInfoByRoomLevel(GameKindDntg,mRoomLevel);
			if(!info)
				ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("没有该等级房间")));
			else
			{
				connectGameServer(info);
				mSelectGameKind = GameKindDntg;
			}
		}
	}
}

void GameSmallScene::loginRealRoomSuccessRsp(EventCustom* evt)
{
	removeLoading();
	SessionManager::shareInstance()->setGameKind(mSelectGameKind);
	//进入游戏之前加载鱼的游走动画和死亡动画数据
	ResourcesManager::getInstance()->loadResourceFile(mSelectGameKind);
	ResourcesManager::getInstance()->loadGuideFile(mSelectGameKind);
	SessionManager::shareInstance()->setInGame(true);
	Director::sharedDirector()->replaceScene(GameScene::create(mRoomLevel,mSelectGameKind));
}

void GameSmallScene::loginRealRoomFailureRsp(EventCustom* evt)
{
	removeLoading();
	CMD_GR_LogonFailure* errorCode = (CMD_GR_LogonFailure*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(errorCode->szDescribeString)));
}

//连接失败
void GameSmallScene::receiveConnectFailMsg(EventCustom* evt)
{
	removeLoading();
	AlertWidget* pWidget = AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("连接出错,请查看您的网络!"));
	ModalViewManager::sharedInstance()->showWidget(pWidget);
}


void GameSmallScene::receiveDownLoadInfo(EventCustom* evt)
{
	removeLoading();
	CMD_GP_DownloadResUrl* info = (CMD_GP_DownloadResUrl*)evt->getUserData();
	ResourcePackManager *man = ResourcePackManager::sharedInstance();

	ResourcePack* pack = ResourcePack::create();

	int gameKind = info->dwKindID;
	if (gameKind == GameKindLkpy)
	{
		pack->setName("lkpy");
		pack->setTag("lkpyTag");
		//for md5
		//pack->setDownloadPath("http://download.qicainiu.com/download/Mobile/GameCatchFish/lkpy.zip");//URL 地址
	}else if(gameKind == GameKindJcpy)
	{
		pack->setName("jcpy");
		pack->setTag("jcpyTag");
		//for md5
		//pack->setDownloadPath("http://download.qicainiu.com/download/Mobile/GameCatchFish/jcpy.zip");//URL 地址
	}
	pack->setVersion(numberToString(info->dwVersion));

	pack->setCount(1);

	pack->setDownloadPath(info->szResUrl);//URL 地址

	pack->setVerifyCode(info->szMd5);//MD5签证码

	pack->setStoragePath(GameResourcePackStoragePath);

	pack->setPakcageSize(info->dwPakageSize);
	if (!man->isResourcePackDownloaded(pack))
	{
		man->addResourcePack(pack);//添加下载任务
	}
}

void GameSmallScene::createGameSorket()
{
	//初始化SOCKET
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(XwZoneDntg);
	//释放上一个SOKET 创建新的
	if (c)
	{
		m->removeServiceClient(XwZoneDntg);
	}
	//NEW SOKET
	m->registerServiceClient(XwZoneDntg, false,GAMEROOM_SORKET_INDEX);
	c = m->serviceClientForName(XwZoneDntg);
}

void GameSmallScene::connectGameServer(RoomLevelInfo* info)
{
	GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	GameServiceClient* c = m->serviceClientForName(XwZoneDntg);

	bool ret = c->Connect(info->getRoomServerAddr().c_str(),info->getServerPort());

	//连接成功
	if(ret)
	{
		showLoading();
		c->loginRealRoom(info);
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("游戏服务器异常，请联系客服!")));
	}
}

void GameSmallScene::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}

void GameSmallScene::creatBubble(BubbleType type, Vec2 pos)
{
	/*BubbleItem* pItem = BubbleItem::create(CommonFunction::fgRangeRand(0,360),CommonFunction::fgRangeRand(30,50),pos,type);
	pItem->setPosition(pos);

	mBubbleArray->addObject(pItem);*/

}

void GameSmallScene::update(float dt)
{
	/*int i = 0,j=0;
	for (i = 0; i < mBubbleArray->count(); i++)
	{
	BubbleItem* pItem = (BubbleItem*)mBubbleArray->getObjectAtIndex(i);
	pItem->update(dt);
	}
	for (i = 0 ; i < mBubbleArray->count();i++)
	{
	BubbleItem* pItem = (BubbleItem*)mBubbleArray->getObjectAtIndex(i);
	for (j = i +1 ; j < mBubbleArray->count();j++)
	{
	BubbleItem* pItem2 = (BubbleItem*)mBubbleArray->getObjectAtIndex(j);
	float distance = CommonFunction::CalcDistance(pItem->getPositionX(),pItem->getPositionY(),pItem2->getPositionX(),pItem2->getPositionY()) ;
	if ( distance <= pItem->getContentSize().width/2 + pItem2->getContentSize().width/2 - 10 )
	{
	float intervel = (pItem->getContentSize().width/2 + pItem2->getContentSize().width/2 - distance)/2;
	pItem->getAction()->reverseX();
	pItem->getAction()->reverseY();

	Vec2 nextPos;
	Vec2::add(pItem->getPosition(),Vec2(pItem->getAction()->getDX(),pItem->getAction()->getDY()),&nextPos);
	pItem->setPosition(nextPos);

	pItem2->getAction()->reverseX();
	pItem2->getAction()->reverseY();

	Vec2::add(pItem2->getPosition(),Vec2(pItem->getAction()->getDX(),pItem->getAction()->getDY()),&nextPos);
	pItem2->setPosition(nextPos);
	}
	}
	}*/
}


void GameSmallScene::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameSmallScene::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		getChildByTag(COMMON_LOADING_TAG)->removeFromParentAndCleanup(false);
	}
}



void GameSmallScene::onSuccess(ResourcePack *pack) {
	if (NULL != pack ) {
		log("GameSmallScene::onSuccess");
		onProgress(pack,-1);//下载成功，隐藏加载控件
	}
}

void GameSmallScene::onError(ResourcePack *pack, TNAssetsManagerActionCode errorCode) {
	if (NULL != pack ) {
		
		onProgress(pack,-1);//下载失败，隐藏加载控件
		switch (errorCode) {
            case kCreateFileError:
                log("create file fail");
                break;
            case kInvalidAssetData:
                log("invalid asset data");
                break;
            case kDownload:
            {
                ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(NULL,"",CommonFunction::GBKToUTF8("资源包下载失败!")));
            }
                break;
            case kVerifyPackage:
            {
                ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(NULL,"",CommonFunction::GBKToUTF8("资源包验证错误!")));
            }
                break;
            case kUncompress:
            {
                ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(NULL,"",CommonFunction::GBKToUTF8("资源包解压失败!")));
            }
                break;
        }
	}
}

void GameSmallScene::onProgress(ResourcePack *pack, int progress) {
	if (NULL != pack ) {

		log("GameSmallScene::onProgress zone name = %s, present = %d",pack->getName().c_str(),progress);
		SmallLoadingProgressWidget* loadingWidget = nullptr;
		if (strcmp(pack->getName().c_str(),"jcpy") == 0)
		{
			loadingWidget = mLoading_forJc;
		}else if (strcmp(pack->getName().c_str(),"lkpy") == 0)
		{
			loadingWidget = mLoading_forLk;
		}
		if (progress >= 0 && progress <= 100) {
			loadingWidget->setVisible(true);
			loadingWidget->setProgress(progress);
		} else {
			loadingWidget->setVisible(false);
			loadingWidget->setProgress(0);
		}

	}
}

void GameSmallScene::refreshTreasureData(EventCustom* evt)
{
	CMD_GP_UserTreasure* treasureInfo = (CMD_GP_UserTreasure*)evt->getUserData();
	OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
	pUser->setUserScore(treasureInfo->lScore);
	pUser->setUserIngot(treasureInfo->lIngotScore);
	pUser->setUserInsure(treasureInfo->lInsureScore);
	pUser->setUserJiangJuan(treasureInfo->lLottery);
}

//bool GameSmallScene::checkGameDownloaded(const char *zone) {
//	if (NULL != zone) {
//		__Dictionary *packMap = SessionManager::sharedManager()->getGameResourceMap();
//		if (NULL != packMap) {
//			ResourcePackManager *man = ResourcePackManager::sharedInstance();
//			ResourcePack *pack = (ResourcePack*)packMap->objectForKey(zone);
//			return man->isResourcePackDownloaded(pack);
//		}
//	}
//	return false;
//}
//////////////////////////////////////////////////////////////////////////

BubbleItem::BubbleItem()
{
	mAction = nullptr;
}

BubbleItem::~BubbleItem()
{
	CC_SAFE_RELEASE_NULL(mAction);
}

BubbleItem* BubbleItem::create(float angle,float speed,Vec2 pos,BubbleType Bubbletype )
{
	BubbleItem* pItem = new BubbleItem();
	if (pItem && pItem->init(angle,speed,pos,Bubbletype))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		delete pItem;
		pItem = nullptr;
		return nullptr;
	}
}

bool BubbleItem::init(float angle,float speed,Vec2 pos,BubbleType Bubbletype )
{
	std::string imageName = "";
	if (Bubbletype == BubbleNormal)
	{
		int i = rand()%3;
		imageName = __String::createWithFormat("bubble%d.png",i+1)->getCString();
	}
	else if(Bubbletype == BubbleDntg)
	{
		imageName = "dntg.png";
	}
	else if(Bubbletype == BubbleLkpy)
	{
		imageName = "lkpy.png";
	}else if(Bubbletype == BubbleJcpy)
	{
		imageName = "jcpy.png";
	}

	if (!Sprite::initWithFile(imageName))
	{
		return false;
	}
	
	mAction = ActionBubbletMove::create(pos,angle,speed,CommonFunction::fgRangeRand(5,10),float(getContentSize().width/2.0f));
	CC_SAFE_RETAIN(mAction);

	RotateBy* rb = RotateBy::create(2.5f,30);
	runAction(RepeatForever::create(rb));

	return true;
}


void BubbleItem::update(float delta)
{
	if (mAction)
	{
		mAction->step(delta);
		Vec2 pos = mAction->getPosition();
		this->setPosition(pos);
	}
}

void BubbleItem::render()
{
	if (mAction)
	{
		
	}
}





