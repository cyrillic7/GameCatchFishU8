#include "GameScene.h"
#include "SessionManager.h"
#include "GameServiceClientManager.h"
#include "MsgDefine/CMD_GameServer.h"
#include "CommonFunction.h"
#include "GameBulletManager.h"
#include "GameCoinManager.h"
#include "GameTimerManager.h"
#include "GameChainManager.h"
#include "SoundManager.h"
#include "Widgets/GameJettonWidget.h"
#include "Widgets/GameMatchAwardWidget.h"
#include "GameJettonManager.h"
#include "Widgets/BackgroundGoUpParticle.h"
#include "GameMainScene.h"
#include "GameSmallScene.h"
#include "GameMatchScene.h"
#include "Common/ModalViewManager.h"
#include "audio/include/AudioEngine.h"


// 开炮间隔0.4秒
const DWORD kFireInterval = 200;
// 闪电标志位置
const float kChainFlagRotateRadius = 128.f;
//参生气泡间隔
const float kBubbleInterval = 5.f;
//闪电标记TAG 
#define  kChainFlagTag  10018
//bingo标记TAG
#define  KBinGoFalgTag  11018
//bglu 标记TAG
#define  KBgluTag       12018
//爆击标记TAG 
#define  KBaojiTag      13018
//fengyintag
#define  KFengyinTag    14018
//公共提示框背景TAG
#define KCommonBagTag   15018
//系统提示消息TAG
#define  KCommonTipTag  16018

#define  kWaveInterval  0.06666666666666666666666667f//0.0333333333333333f

#define  outTipShowInterval 4.0f//

GameScene::GameScene()
{
	next_scene_kind = SCENE_COUNT;//默认值
	bubble_elapsed_ = 0.0f;
	mLoadResourseCount = 0;
	mCurBgIndex = 0 ;
	mDealCnt  = 0;
	bSend = false;
	mScoreIsChange = true;
	memset(&mGameConfig,0,sizeof(mGameConfig));

	is_special_scene_ = false;
	mSpcialSceneBeganTime = 0;
	mLastFireTime = 0;

	myLayer = Layer::create();
	myLayer->retain();
	myLayer->setTouchEnabled(true);

	mBgLayer = Layer::create();
	mBgLayer->retain();
	addChild(mBgLayer);

	EventListenerTouchOneByOne* TouchListenr = EventListenerTouchOneByOne::create();
	//回调函数绑定
	TouchListenr->onTouchBegan= CC_CALLBACK_2(GameScene::onTouchBegan, this);
	TouchListenr->onTouchMoved= CC_CALLBACK_2(GameScene::onTouchMoved, this);
	TouchListenr->onTouchEnded= CC_CALLBACK_2(GameScene::onTouchEnded, this);
	//添加监听器到事件分发器中
	_eventDispatcher->addEventListenerWithSceneGraphPriority(TouchListenr, this);
	addChild(myLayer);

	mPlayerManager = nullptr;
	mLoadingWidget = nullptr;
	mCanShooter = false;
	mCursorSp = nullptr;
	mSceneFishManager = nullptr;
	mBgSp = nullptr;
	mIceImage = nullptr;
	mWaveIndex = 1;
	mWaves = NULL;

	//切换切景时新场景里的WAVE 数据
	mNextSceneWaves = __Array::create();
	mNextSceneWaves->retain();
	mWaves = __Array::create();
	mWaves->retain();

	//消息数组
	mMsgArray =__Array::create();
	mMsgArray->retain();

	//粒子数组
	mParticlesArray = __Array::create();
	mParticlesArray->retain();

	mWaveInterval = 0.f;

	//加载音效资源
	preLoadSoundRes();

	//加载资源页面
	showLoading();

	mKickOutLabel = nullptr;
	//子弹批节点
	mBulletBatchNode = nullptr;
	//金币批节点
	mCoinBatchNode = nullptr;
	//向导控件
	mGuideWidget = nullptr;

	mRoomLevel = -1;
	mGameKind  = -1;

	mTipOutFish = ImageView::create();
	mTipOutFish->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2 + 150));
	addChild(mTipOutFish);
	mTipOutFish->setVisible(false);

	mIsMatchFlag = false;
	//比赛房间信息
	mGameMatchInfo = nullptr;
	mMatchManager = nullptr;

	mbEnterMatch = false;
	bShowAward = false;
	//设置SORKET
	GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(XwZoneDntg); 
	setServiceClient(c);
}
GameScene::~GameScene()
{
	CC_SAFE_RELEASE_NULL(myLayer);
	CC_SAFE_RELEASE_NULL(mBgLayer);
	CC_SAFE_RELEASE_NULL(mPlayerManager);
	CC_SAFE_RELEASE_NULL(me);
	CC_SAFE_RELEASE_NULL(mCursorSp);
	CC_SAFE_RELEASE_NULL(mSceneFishManager);
	CC_SAFE_RELEASE_NULL(mWaves);
	CC_SAFE_RELEASE_NULL(mNextSceneWaves);
	CC_SAFE_RELEASE_NULL(mMsgArray);
	CC_SAFE_RELEASE_NULL(mParticlesArray);
	CC_SAFE_RELEASE_NULL(mGameMatchInfo);
	CC_SAFE_RELEASE_NULL(mMatchManager);

	GameFishManager::sharedInstance()->purgeInstance();
	GameChainManager::shareInstance()->purgeInstance();
	GameJettonManager::shareInstance()->purgeInstance();
	GameBulletManager::shareInstance()->purgeInstance();
	CoinManager::shareInstance()->purgeInstance();
	SoundManager::stopMusicBackground();

	SessionManager::shareInstance()->clearGamePlayers();
	//modify 收到站立包时，清除SOKET
	GameServiceClientManager::sharedInstance()->removeServiceClient(m_gameName.c_str());
	
	GameServiceClientManager::sharedInstance()->setCurrentServiceClientName(XwZoneName);

	SessionManager::shareInstance()->setChairId(-1);
	SessionManager::shareInstance()->setGameKind(0);
	SessionManager::shareInstance()->setInGame(false);
	SessionManager::shareInstance()->setHasFinish(false);

	//从游戏中退出，如果分数有变化，刷新玩家财富
	if (mScoreIsChange)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendTreasureRequest();
	}

	//清除声音缓存
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	int i = 0;
	if(mGameKind == GameKindDntg)
	{
		for ( i = 0 ; i <MusicCount;i++)
		{
			SoundManager::unloadEffect(__String::createWithFormat("audios/%s.mp3",GameMusic[i])->getCString());
		}

	
	}
	else if(mGameKind == GameKindLkpy )
	{
		for (i = 0 ; i <MusicLkCount;i++)
		{
			SoundManager::unloadEffect(__String::createWithFormat("GameResources/audios_lk/%s.mp3",GameMusicForLk[i])->getCString());
		}
	}
	else if(mGameKind == GameKindJcpy )
	{
		for (i = 0 ; i <MusicLkCount;i++)
		{
			SoundManager::unloadEffect(__String::createWithFormat("GameResources/audios_jc/%s.mp3",GameMusicForLk[i])->getCString());
		}
	}
	
#endif
	//清除动画配置数据
	ResourcesManager::getInstance()->clearAllResource();
}

void GameScene::onExit()
{
	stopAllActions();

	//停止定时器
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
	//清除缓存里所有不再用到的图片
	Director::getInstance()->purgeCachedData();
	switch (mGameKind)
	{
	case  GameKindDntg:
		SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("bullet_dntg.plist");
		break;
	case  GameKindJcpy:
		{
			SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("bullet_jcby.plist");
		}
		break;
	case GameKindLkpy:
		{
			SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("bullet_lkpy.plist");
		}
		break;
	}
	
	GameBaseScene::onExit();
}


void GameScene::onEnter()
{
	log("GameScene::onEnter()");
	GameBaseScene::onEnter();

	//先加载声音资源
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(CreateNewFishMsg, CC_CALLBACK_1(GameScene::receiveSubDistributeFish, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GameStatusMsg, CC_CALLBACK_1(GameScene::receiveGameStausMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GameSceneMsg, CC_CALLBACK_1(GameScene::receiveGameSceneMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GetUserStatusMsg, CC_CALLBACK_1(GameScene::receiveUserEnterMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(PlayerToSeperaterMsg, CC_CALLBACK_1(GameScene::receivePlayerToSeperaterMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(UserFireMsg, CC_CALLBACK_1(GameScene::receiveUserFireMsgMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(CatchFishGroupMsg, CC_CALLBACK_1(GameScene::receiveCatchFishMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ExchangeFishScoreMsg, CC_CALLBACK_1(GameScene::receiveExchangeScoreMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(TimerSyncMsg, CC_CALLBACK_1(GameScene::receiveTimerSyncMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(RemoveLoadingMsg, CC_CALLBACK_1(GameScene::receiveRemoveLoadingMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ChangeSceneMsg, CC_CALLBACK_1(GameScene::receiveChangeSceneMsg, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ForceTimerSyncMsg, CC_CALLBACK_1(GameScene::receiveForceTimerSyncMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(SubSceneFishMsg, CC_CALLBACK_1(GameScene::receiveSubSceneFishMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(BulletDoubleTimeoutMsg, CC_CALLBACK_1(GameScene::receiveBulletDoubleTimeoutMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(CatchChainMsg, CC_CALLBACK_1(GameScene::receiveCatchChainMsg, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(FishArriveMsg, CC_CALLBACK_1(GameScene::receiveFishArriveMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(DealBingoEffectMsg, CC_CALLBACK_1(GameScene::receiveDealBingoEffectMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(DealJettonEffectMsg, CC_CALLBACK_1(GameScene::receiveDealJettonEffectMsg, this)), this);
	


	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(dealBgLuEffectMsg, CC_CALLBACK_1(GameScene::receiveDealBgluEffectMsg, this)), this);

	//系统消息
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(SystemMessage, CC_CALLBACK_1(GameScene::receiveSystemMsg, this)), this);
	//修改李逵倍数消息
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(ModifyLkMulriple, CC_CALLBACK_1(GameScene::receiveModifyLkMulriple, this)), this);

	//网络断开连接
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GameSorketCloseMsg, CC_CALLBACK_1(GameScene::receiveGameSorketCloseMsg, this)), this);
	//按到返回键处理
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(KeyEscapeMsg, CC_CALLBACK_1(GameScene::receiveKeyEscapeMsg, this)), this);

	if(mIsMatchFlag)
	{
		//比赛相关
		_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateMatchStatusMsg, CC_CALLBACK_1(GameScene::receiveUpdateMatchStatusMsg, this)), this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateMatchInfoMsg, CC_CALLBACK_1(GameScene::receiveUpdateMatchInfoMsg, this)), this);
	}


	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(matchAwardMsg, CC_CALLBACK_1(GameScene::receiveMatchAwardMsg, this)), this);
	//MatchTryAgainMsg
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(MatchTryAgainMsg, CC_CALLBACK_1(GameScene::toSendTryAgainMatch, this)), this);
	//showMatchTryWidget
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(showMatchTryMsg, CC_CALLBACK_1(GameScene::showMatchTryAgainMsg, this)), this);
	//MatchIsBeingStartMsg
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(MatchIsBeingStartMsg, CC_CALLBACK_1(GameScene::receiveMatchIsBeingStartMsg, this)), this);
}

void GameScene::showLoading()
{
	mLoadingWidget = LoadingScene::create();
	mLoadingWidget->retain();
	addChild(mLoadingWidget);
}


//加载完成
void GameScene::receiveRemoveLoadingMsg(EventCustom* event)
{
	if (m_isResourceLoaded && !m_alreadyLoadedUI) {
		loadUI();
		SoundManager::playMusicBackground(getMusicBackgroudName(1).c_str(),true);
		if (mIsMatchFlag)
		{
			m_serviceClient->sendGetGameMatchConfig();
		}
		else
		{
			m_serviceClient->sendGetGameConfig();
		}

		if (mIsMatchFlag)
		{
			mMatchManager = GameMatchManager::create(this);
			mMatchManager->retain();

			addChild(mMatchManager);
		}
	}
}

std::string GameScene::getMusicBackgroudName(int index)
{
	std::string name ="";
	if (mGameKind == GameKindDntg)
	{
		name = __String::createWithFormat("audios/bg_%02d.mp3",index)->getCString();
	}else if(mGameKind == GameKindLkpy )
	{
		name = __String::createWithFormat("GameResources/audios_lk/bg_%02d.mp3",index)->getCString();
	}
	else if(mGameKind == GameKindJcpy )
	{
		name = __String::createWithFormat("GameResources/audios_jc/bg_%02d.mp3",index)->getCString();
	}

	return name;
}



void GameScene::loadUI()
{
	GameBaseScene::loadUI();
	loadBg();

	//SessionManager::shareInstance()->setInGame(true);

	//加载之前就在桌子的玩家UI
	__Array* players = SessionManager::shareInstance()->getGamePlayers();
	for (int i = 0 ; i < players->count(); i ++)
	{
		UserModel* pModel = (UserModel*)players->getObjectAtIndex(i);
		if (!mPlayerManager->playerIsExist(pModel->getChairId()))
		{
			log("has enter chair id =%d\n",pModel->getChairId());
			mPlayerManager->addPlayer(pModel->getChairId());
		}
	}
	SessionManager::shareInstance()->setHasFinish(true);
	//起定时器
	Director::getInstance()->getScheduler()->schedule(schedule_selector(GameScene::update), this,1.0f/60, false);
}

void GameScene::updateProgress(int resourceLeftToLoad)
{
	mLoadingWidget->setPercent(100 - resourceLeftToLoad*100/mLoadResourseCount);
}

//提前加载一些大图片
__Array * GameScene::getResourceNeedLoad()
{
	__Array* a = GameBaseScene::getResourceNeedLoad();
	ResourceDefinition* d = NULL;
	String* pathStr = nullptr;
	switch (mGameKind)
	{
	case GameKindDntg:
		{
			for (int i = 0 ; i <= FISH_BGLU;i++)
			{
				pathStr= String::createWithFormat("dntg2/images/fish/%s.png",CommonFunction::fish_name_with_status(FishKind(i),"walking"));

				d = ResourceDefinition::create(pathStr->getCString(), ResourceTypeImage);
				a->addObject(d);

				if(i != FISH_FOSHOU && i != FISH_SHENXIANCHUAN && i != FISH_BGLU )
				{
					pathStr = String::createWithFormat("dntg2/images/fish/%s.png",CommonFunction::fish_name_with_status(FishKind(i),"dying"));
					d = ResourceDefinition::create(pathStr->getCString(), ResourceTypeImage);
					a->addObject(d);
				}

			}
			d = ResourceDefinition::create("dntg2/images/fish/LChainFlag.png", ResourceTypeImage);
			a->addObject(d);

			d = ResourceDefinition::create("dntg2/images/fish/LightChain.png", ResourceTypeImage);
			a->addObject(d);

			////加载地图
			int k = 0;
			for (k=0;k<3;k++)
			{
				d = ResourceDefinition::create(__String::createWithFormat("dntg2/images/map/map%d.jpg",k)->getCString(), ResourceTypeImage);
				a->addObject(d);
			}
			//加载波浪
		/*	for (k=0;k<32;k++)
			{
				d = ResourceDefinition::create(__String::createWithFormat("map/Wave_000_0%02d.png",k)->getCString(), ResourceTypeImage);
				a->addObject(d);
			}*/

			//d = ResourceDefinition::create("particles/dingshen0.plist", ResourceTypePlist);
			//a->addObject(d);

			//封印图片
			//d = ResourceDefinition::create("ice.png", ResourceTypeImage);
			//a->addObject(d);
			//提前加载子弹
			SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bullet_dntg.plist");
			mBulletBatchNode = SpriteBatchNode::create("bullet_dntg.png",1000);
			myLayer->addChild(mBulletBatchNode,2);
		}
		break;
	case GameKindLkpy :
		{
			////加载地图
			int k = 0;
			for (k=0;k<3;k++)
			{
				d = ResourceDefinition::create(__String::createWithFormat("%s/map/map%d.jpg",CommonFunction::getResDir().c_str(),k)->getCString(), ResourceTypeImage);
				a->addObject(d);
			}
			//加载波浪
			/*	for (k=0;k<32;k++)
			{
			d = ResourceDefinition::create(__String::createWithFormat("map/Wave_000_0%02d.png",k)->getCString(), ResourceTypeImage);
			a->addObject(d);
			}*/

			d = ResourceDefinition::create("particles/dingshen0.plist", ResourceTypePlist);
			a->addObject(d);

			//封印图片
			d = ResourceDefinition::create("ice.png", ResourceTypeImage);
			a->addObject(d);

			for (int i = FISH_KIND_1 ; i < FISH_KIND_25_27;i++)
			{
				pathStr= String::createWithFormat("%s/fish/%s.png",CommonFunction::getResDir().c_str(),CommonFunction::fish_name_with_status(FishKind(i),"walking"));
				d = ResourceDefinition::create(pathStr->getCString(), ResourceTypeImage);
				a->addObject(d);

				
				pathStr = String::createWithFormat("%s/fish/%s.png",CommonFunction::getResDir().c_str(),CommonFunction::fish_name_with_status(FishKind(i),"dying"));
				d = ResourceDefinition::create(pathStr->getCString(), ResourceTypeImage);
				a->addObject(d);
				
			}
			//提前加载子弹
			//pathStr = String::createWithFormat("%s/bullet/bullet_lkpy.plist",CommonFunction::getResDir().c_str());
			SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bullet_lkpy.plist");
			mBulletBatchNode = SpriteBatchNode::create( "bullet_lkpy.png",1000);
			myLayer->addChild(mBulletBatchNode,2);
			//mBulletBatchNode->retain();
		}
		break;
	case GameKindJcpy :
		{
			////加载地图
			int k = 0;
			for (k=0;k<3;k++)
			{
				d = ResourceDefinition::create(__String::createWithFormat("%s/map/map%d.jpg",CommonFunction::getResDir().c_str(),k)->getCString(), ResourceTypeImage);
				a->addObject(d);
			}
			//加载波浪
			/*for (k=0;k<32;k++)
			{
				d = ResourceDefinition::create(__String::createWithFormat("map/Wave_000_0%02d.png",k)->getCString(), ResourceTypeImage);
				a->addObject(d);
			}*/

			d = ResourceDefinition::create("particles/dingshen0.plist", ResourceTypePlist);
			a->addObject(d);

			

			for (int i = FISH_KIND_JC1 ; i < FISH_KIND_JCKING;i++)
			{
				AnimationInfo* info = ResourcesManager::getInstance()->getAnimationInfoByKind(FishKind(i));
				pathStr = __String::createWithFormat("%s/fish/%s_walking.png",CommonFunction::getResDir().c_str(),info->getImageName().c_str());
				d = ResourceDefinition::create(pathStr->getCString(), ResourceTypeImage);
				a->addObject(d);

				info = ResourcesManager::getInstance()->getAnimationInfoByKind(FishKind(i),false);
				if(info)
				{
					pathStr = __String::createWithFormat("%s/fish/%s_dying.png",CommonFunction::getResDir().c_str(),info->getImageName().c_str());
					d = ResourceDefinition::create(pathStr->getCString(), ResourceTypeImage);
					a->addObject(d);
				}
			
			}
			//提前加载子弹
			//pathStr = String::createWithFormat("%s/bullet/bullet_jcby.plist",CommonFunction::getResDir().c_str());
			SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bullet_jcby.plist");
			mBulletBatchNode = SpriteBatchNode::create( "bullet_jcby.png",1000);
			myLayer->addChild(mBulletBatchNode,2);
			//mBulletBatchNode->retain();
		}
		break;
	}
	
	////金币
	//SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gamecoin.plist");
	//mCoinBatchNode = SpriteBatchNode::create("gamecoin.png",600);
	//addChild(mCoinBatchNode,1);

	d = ResourceDefinition::create("gamecoin.plist", ResourceTypePlist);
	a->addObject(d);

	//向导图片
	d = ResourceDefinition::create("gameguide.plist", ResourceTypePlist);
	a->addObject(d);

	mLoadResourseCount = a->count();
	return a;
}

GameScene* GameScene::create(int level,int gameKind,bool isMatch)
{
	GameScene* pScene = new GameScene();
	if (pScene && pScene->init(level,gameKind,isMatch))
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
bool GameScene::init(int level,int gameKind,bool isMatch)
{
	if (!GameBaseScene::init())
		return false;
	mIsMatchFlag = isMatch;
	mRoomLevel = level;
	mGameKind  = gameKind;
	SessionManager::shareInstance()->setGameKind(mGameKind);
	m_gameName = XwZoneDntg;
	me = nullptr;
	mBlongPressed = false;

	if (mIsMatchFlag)
	{
		mGameMatchInfo = GameMatchModel::create();
		mGameMatchInfo->retain();
	}
	mPlayerManager  = GamePlayerManager::create(this);
	mPlayerManager->retain();

	////初始化SOCKET
	//GameServiceClientManager *m = GameServiceClientManager::sharedInstance();
	//m->registerServiceClient(m_gameName.c_str(), false,GAMEROOM_SORKET_INDEX);
	//GameServiceClient* c = m->serviceClientForName(m_gameName.c_str());
	////
	//GameServiceClientManager::sharedInstance()->setCurrentServiceClientName(m_gameName.c_str());
	//setServiceClient(c);
	return  true;

}


void GameScene::shakeScreen()
{
	Shake* pShake = Shake::create(1.0f,3.0f);
	CallFuncN* call = CallFuncN::create(this,callfuncN_selector(GameScene::reset));
	mBgLayer->runAction(Sequence::createWithTwoActions(pShake,call));
}

void GameScene::reset(Node* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//背景层复位
	mBgLayer->setPosition(Vec2(0,0));
}

void GameScene::loadBg()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	__String* mapName = __String::createWithFormat("%s/map/map0.jpg",CommonFunction::getResDir().c_str());
	mBgSp = Sprite::create(mapName->getCString());

	mBgLayer->addChild(mBgSp);
	mBgSp->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	
	//水波
	//createWave(myLayer);
	
	//鼠标
	mCursorSp = Sprite::create("common/players/Cursor.png");
	mCursorSp->setVisible(false);
	mCursorSp->retain();
	addChild(mCursorSp);

	//返回按钮
	ui::Button* btn_back = ui::Button::create();
	btn_back->loadTextureNormal("back.png",ui::Widget::TextureResType::LOCAL);
	btn_back->loadTexturePressed("back.png",ui::Widget::TextureResType::LOCAL);
	btn_back->setPosition(Vec2(btn_back->getContentSize().width/2+ 30,kScreenHeight - btn_back->getContentSize().height/2 - 70));
	
	addChild(btn_back);
	btn_back->addTouchEventListener(CC_CALLBACK_2(GameScene::onBack, this));

	Sprite* qipaoSp = Sprite::create();
	qipaoSp->setPosition(Vec2(35,30));
	btn_back->addChild(qipaoSp);
	std::vector<std::string> imageNames;
	std::string path = "";
	int i  = 0;
	for (i= 0 ; i < 6;i++)
	{
		if (i < 4)
			path = "qipao"+numberToString(i+1)+".png";
		else
			path = "qipao"+numberToString(4-i%3)+".png";

		imageNames.push_back(path);
	}

	Animate* pAnima = CommonFunction::createFrameAnimate(imageNames,0.2);
	qipaoSp->runAction(RepeatForever::create(pAnima));


	//详情按钮
	ui::Button* btn_guide = ui::Button::create();
	btn_guide->loadTextureNormal("btn_guide.png",ui::Widget::TextureResType::PLIST);
	btn_guide->loadTexturePressed("btn_guide.png",ui::Widget::TextureResType::PLIST);
	btn_guide->setPosition(Vec2( kScreenWidth - btn_guide->getContentSize().width/2 - 30,kScreenHeight - btn_guide->getContentSize().height/2 - 70));

	addChild(btn_guide);
	btn_guide->addTouchEventListener(CC_CALLBACK_2(GameScene::onClickGuide, this));

	Sprite* qipaoSp2 = Sprite::create();
	qipaoSp2->setPosition(Vec2(btn_guide->getContentSize().width/2,30));
	btn_guide->addChild(qipaoSp2);
	imageNames.clear();
    path = "";
	for (i = 0 ; i < 6;i++)
	{
		if (i < 4)
			path = "qipao"+numberToString(i+1)+".png";
		else
			path = "qipao"+numberToString(4-i%3)+".png";

		imageNames.push_back(path);
	}
	qipaoSp2->runAction(DelayTime::create(0.2f));
	pAnima = CommonFunction::createFrameAnimate(imageNames,0.3);
	qipaoSp2->runAction(RepeatForever::create(pAnima));


	Sprite* title= Sprite::create(__String::createWithFormat("%s/static/frame_top.png",CommonFunction::getResDir().c_str())->getCString());
	title->setPosition(Vec2(kScreenWidth/2,kScreenHeight - title->getContentSize().height/2));
	myLayer->addChild(title,501);

	//金币批处理结点
	Sprite* coinSp = Sprite::createWithSpriteFrameName("yinbi_1.png");
	if (coinSp)
	{
		int k = 0;
	}
	mCoinBatchNode = SpriteBatchNode::create("gamecoin.png",600);
	addChild(mCoinBatchNode,1);
}

void GameScene::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//干掉游戏SORKET 直接退出
		m_serviceClient->sendStandUp();
		
		DelayTime* delay = DelayTime::create(5.0f);
		CallFuncN* callBack =CallFuncN::create(this,callfuncN_selector(GameScene::standupOverTime));
		runAction(Sequence::createWithTwoActions(delay,callBack));
		//exitGameRoom();	
	}
}


void GameScene::onClickGuide(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		   if(!mGuideWidget)
		   {
			   mGuideWidget = GameGuideWidget::create();
			   mGuideWidget->setPosition(Vec2(0,0));
			   addChild(mGuideWidget,999);
		   }
		   else
		   {
			   
			   mGuideWidget->setVisible(!mGuideWidget->isVisible());
		   }
	}
}

void GameScene::update(float dt)
{
	mWaveInterval +=dt;
	/*if (mWaves && mWaveInterval >= kWaveInterval)
	{
		mWaveInterval -=kWaveInterval;
		cocos2d::Ref* pElement = NULL;
		for (int i = 0 ; i < mWaves->count();i++)
		{
			Sprite* sp = (Sprite*)mWaves->getObjectAtIndex(i);
			if(sp)
			{
				Texture2D* texture= TextureCache::sharedTextureCache()->textureForKey(__String::createWithFormat("map/Wave_000_0%02d.png",mWaveIndex)->getCString());

				SpriteFrame* frame = SpriteFrame::createWithTexture(texture,CCRectMake(0,0,256,256));
				sp->setSpriteFrame(frame);
				BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
				sp->setBlendFunc(blendFunc);

				
			}
		}
		//切换场景特别处理
		if (mNextSceneWaves->count() > 0 )
		{
			CCARRAY_FOREACH(mNextSceneWaves,pElement)
			{
				Sprite* sp = (Sprite*)pElement;
				Texture2D* texture= TextureCache::sharedTextureCache()->textureForKey(__String::createWithFormat("map/Wave_000_0%02d.png",mWaveIndex)->getCString());


				SpriteFrame* frame = SpriteFrame::createWithTexture(texture,CCRectMake(0,0,256,256));
				sp->setSpriteFrame(frame);
				BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
				sp->setBlendFunc(blendFunc);
			}
		}
		mWaveIndex++;
		if (mWaveIndex >=31)
		{
			mWaveIndex = 0;
		}
	}*/
	
	

	//每三秒与服务器同步一次
	TimerManager::shareInstance()->Update(dt);
	if (TimerManager::shareInstance()->sync_elapsed() >= 3.0f && m_serviceClient) {
		TimerManager::shareInstance()->Reset();
		m_serviceClient->SendTimerSync();
	}

	GameFishManager::sharedInstance()->Update(dt);
	GameBulletManager::shareInstance()->update(dt);
	GameChainManager::shareInstance()->update(dt);
	GameJettonManager::shareInstance()->Update(dt);
	if (mBlongPressed)
	{
		DWORD tick = SessionManager::shareInstance()->getCurrentTime();
		if(!is_special_scene_ && next_scene_kind == SCENE_COUNT && tick - mLastFireTime > kFireInterval)
		{
			mLastFireTime = tick;
			onBulletFire();	
		}
	}

	bubble_elapsed_ += dt;
	if (bubble_elapsed_ >= kBubbleInterval)
	{
		bubble_elapsed_ -= kBubbleInterval;
		HaloParticle* p = HaloParticle::create("qipao3.png");
		Vec2 pos;
		pos.x = CommonFunction::fgRangeRand(100,kScreenWidth-100);
		pos.y = CommonFunction::fgRangeRand(100,kScreenHeight-100);
		p->setPosition(pos);
		addChild(p);
	}

	if (is_special_scene_) {
		DWORD tick = SessionManager::shareInstance()->getTimeStamp();
		
		if (tick - mSpcialSceneBeganTime >= special_scene_waited_time_) {
			special_scene_waited_time_ = 0.f;
			is_special_scene_ = false;
			// 弹出公共提示
			popCommonTip(__String::create("特殊鱼阵结束, 您可以正常打鱼了!"));
		}
	}
	

	//空闲超120000秒退出
	DWORD now = SessionManager::shareInstance()->getCurrentTime();
	if (mLastFireTime !=0 &&now - mLastFireTime > 120000 && m_serviceClient && (!bSend) && !mIsMatchFlag)
	{
		m_serviceClient->sendStandUp();
		DelayTime* delay = DelayTime::create(10.0f);
		CallFuncN* callBack =CallFuncN::create(this,callfuncN_selector(GameScene::standupOverTime));
		runAction(Sequence::createWithTwoActions(delay,callBack));
		bSend = true;
	}
	//空闲提示
	RenderKickoutTips();
}

void GameScene::render(Renderer* renderer)
{
	Scene::render(renderer);
	GameFishManager::sharedInstance()->Render();
	GameBulletManager::shareInstance()->render();
	GameChainManager::shareInstance()->render();
	GameJettonManager::shareInstance()->Render();
}


void GameScene::receiveSubDistributeFish(EventCustom* event)
{
	DWORD elasped;
	bool timer_sync = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	ActionFishMove* action = nullptr;
	__Array* fishArray = (__Array*)event->getUserData();
	if (fishArray->count()> 0 )
	{
		for (int i=0; i < fishArray->count();i++)
		{
			__Dictionary* fishInfoDic = (__Dictionary*)fishArray->getObjectAtIndex(i);
			FishKind kid = (FishKind)((__Integer*)fishInfoDic->objectForKey("fishKind"))->getValue();
			int fishId = ((__Integer*)fishInfoDic->objectForKey("fishId"))->getValue();
			int fishTag = ((__Integer*)fishInfoDic->objectForKey("tag"))->getValue();
			
			PointArray* ptArray = (PointArray*)fishInfoDic->objectForKey("ptArray");
			switch (ptArray->count())
			{
			case 2:
				{
					action = ActionFishMoveLinear::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1));
				}
				break;
			case 3:
				{
					action = ActionFishMoveBezier::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1),ptArray->getControlPointAtIndex(2));
				}
				break;
			case 4:
				{
					action = ActionFishMoveBezier::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1),ptArray->getControlPointAtIndex(2),ptArray->getControlPointAtIndex(3));
				}
				break;
			}

			if(SessionManager::shareInstance()->getGameKind() == GameKindDntg)
			{
				//佛手特别处理
				if (kid == FISH_FOSHOU)
				{
					action = ActionFishMoveFoshou::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1));
				}
			}
			

			DWORD  tick = ((__Integer*)fishInfoDic->objectForKey("tick"))->getValue();
			elasped = TimerManager::shareInstance()->GetDelayTick(tick);
			if (elasped >= kMaxDiffrence) {
				elasped = 0;
				timer_sync = true;
			}
			// TODO:服务器和本地时间同步
			action->step((float)elasped / 1000.f);

		
			
			GameFish* fish = GameFishManager::sharedInstance()->creatFish(kid,fishId,fishTag,mGameConfig.fish_bounding_radius[kid],mGameConfig.fish_bounding_count[kid],action);
			if (fish)
				myLayer->addChild(fish);
			
			showOutTip(kid);
				
		}

		if (timer_sync)
		{
			m_serviceClient->SendTimerSync();
		}
	}
}

void GameScene::showOutTip(FishKind kid)
{
	int temp_kind =0;

	bool isShow = true;
	switch (mGameKind)
	{
	case GameKindDntg:
		{
			if (kid == FISH_SWK)//swk
			{
				mTipOutFish->loadTexture("dntg2/images/static/out_swk.png",ui::Widget::TextureResType::LOCAL);
			}else if(kid == FISH_YUWANGDADI)//ywdd
			{
				mTipOutFish->loadTexture("dntg2/images/static/out_ywdd.png",ui::Widget::TextureResType::LOCAL);
			}else if(kid == FISH_XIAOJINLONG)//金龙
			{
				mTipOutFish->loadTexture("common/static/out_jinlong.png",ui::Widget::TextureResType::LOCAL);
			}else if (kid == FISH_BAWANGJING)//鲨鱼
			{
				mTipOutFish->loadTexture("common/static/out_jinsha.png",ui::Widget::TextureResType::LOCAL);
			}
			else if (kid == FISH_XIAOQINGLONG)//青龙
			{
				mTipOutFish->loadTexture("dntg2/images/static/out_ql.png",ui::Widget::TextureResType::LOCAL);
			}
			else if (kid == FISH_XIAOYINLONG)//银龙
			{
				mTipOutFish->loadTexture("dntg2/images/static/out_yl.png",ui::Widget::TextureResType::LOCAL);
			}
			else
			{
				isShow = false;
			}
		}
		break;
	case GameKindLkpy:
		{
			temp_kind = kid + FISH_KIND_1 ;
			if (temp_kind == FISH_KIND_21 )//lk
			{
				mTipOutFish->loadTexture("GameResources/lkpy/static/out_lk.png",ui::Widget::TextureResType::LOCAL);
			}else if(temp_kind == FISH_KIND_JC20)//QQ
			{
				mTipOutFish->loadTexture("GameResources/lkpy/static/out_qq.png",ui::Widget::TextureResType::LOCAL);
			}else if(temp_kind == FISH_KIND_18)//金龙
			{
				mTipOutFish->loadTexture("common/static/out_jinlong.png",ui::Widget::TextureResType::LOCAL);
			}else if (temp_kind == FISH_KIND_19)//鲨鱼
			{
				mTipOutFish->loadTexture("common/static/out_jinsha.png",ui::Widget::TextureResType::LOCAL);
			}else
			{
				isShow =false;
			}
		}
		break;
	case GameKindJcpy:
		{
			temp_kind = kid + FISH_KIND_JC1 ;
			if (temp_kind == FISH_KIND_JC22 )//jc
			{
				mTipOutFish->loadTexture("GameResources/jcpy/static/out_jc.png",ui::Widget::TextureResType::LOCAL);
			}else if(temp_kind == FISH_KIND_JC19)//蓝豚
			{
				mTipOutFish->loadTexture("GameResources/jcpy/static/out_lantun.png",ui::Widget::TextureResType::LOCAL);
			}else if(temp_kind == FISH_KIND_JC18)//金龙
			{
				mTipOutFish->loadTexture("common/static/out_jinlong.png",ui::Widget::TextureResType::LOCAL);
			}else if (temp_kind == FISH_KIND_JC21)//鲨鱼
			{
				mTipOutFish->loadTexture("common/static/out_jinsha.png",ui::Widget::TextureResType::LOCAL);
			}
			else
			{
				isShow = false;
			}
		}
		break;
	}

	if(isShow)
	{
		mTipOutFish->stopAllActions();
		if (!mTipOutFish->isVisible())
			mTipOutFish->setVisible(true);
		DelayTime* dt = DelayTime::create(1.5f);
		CallFuncN* callback =CallFuncN::create(this,callfuncN_selector(GameScene::hideOutTip));
		mTipOutFish->runAction(Sequence::createWithTwoActions(dt,callback));
	}
	
}

void GameScene::hideOutTip(Node* pSender)
{
	pSender->setVisible(false);
}

void GameScene::receiveGameSceneMsg(EventCustom* event)
{	
	CMD_S_GameStatus* gamestatus = static_cast<CMD_S_GameStatus*>(event->getUserData());

	memcpy(&mGameConfig, &gamestatus->game_config, sizeof(mGameConfig));

	//modify by 7 -9
	//createShooter();
	mCanShooter = true;

	is_special_scene_ = gamestatus->is_special_scene;
	special_scene_waited_time_ = gamestatus->special_sceene_waited_time;

	if (is_special_scene_) {
		//处理网络延时
		special_scene_waited_time_ -= (float)(TimerManager::shareInstance()->GetDelayTick(gamestatus->tick_count)) / 1000.f;
		wchar_t msg[256];
		mSpcialSceneBeganTime = SessionManager::shareInstance()->getTimeStamp();
		//to do 
		popCommonTip(__String::createWithFormat("游戏正在进行特殊鱼阵, 请等待%d秒.",(int)special_scene_waited_time_));

	}
	mSceneFishManager = SceneFishManager::create(myLayer,&mGameConfig);
	mSceneFishManager->retain();
	//保存当前时间
	mLastFireTime = SessionManager::shareInstance()->getCurrentTime();

	//刷新其他玩家状态
	BulletKind bullet_kind = calBulletKind();
	
	for (WORD i = 0; i < GAME_PLAYER; ++i) {
		GamePlayer* player = mPlayerManager->getPlayerByChairId(i);
		if(player)
		{
			
			if (mIsMatchFlag)
			{
				//比赛房间
				player->getShooter()->setScore(gamestatus->fish_score[i]);
			}
			else
			{
				//自已不用再设置分数 
				if (player->getChairID() != SessionManager::shareInstance()->getChairId())
				{
					player->getShooter()->setScore(gamestatus->fish_score[i]);
				}
			}
		
			player->setBulletMulriple(mGameConfig.min_bullet_multiple);
			player->getShooter()->setMulripleArea(mGameConfig.min_bullet_multiple,mGameConfig.max_bullet_multiple);
			player->switchGun(bullet_kind);
		}
	}
}

BulletKind GameScene::calBulletKind()
{
	BulletKind bullet_kind;
	if (mGameConfig.min_bullet_multiple <= 100) {
		bullet_kind = BULLET_2_NORMAL;
	} else if (mGameConfig.min_bullet_multiple > 100 && mGameConfig.min_bullet_multiple <= 1000) {
		bullet_kind = BULLET_3_NORMAL;
	} else if (mGameConfig.min_bullet_multiple > 1000) {
		bullet_kind = BULLET_4_NORMAL;
	}
	return bullet_kind;
}

void GameScene::receiveGameStausMsg(EventCustom* event)
{
	__Dictionary* dic = (__Dictionary*)event->getUserData();
	__Integer* type = (__Integer*)dic->objectForKey("gameStatus");
	mGameStaus = type->getValue();
	__Bool* isLook = (__Bool*)dic->objectForKey("isLookOn");
	mIsLookOn = isLook->getValue();

}

void GameScene::receiveUserFireMsgMsg(EventCustom* event)
{
	__Dictionary* dic = (__Dictionary*)event->getUserData();
	int chairId = ((__Integer*)(dic->objectForKey("chairId")))->getValue();
	if (chairId == SessionManager::shareInstance()->getChairId()) {
		if (!mBulletQueue.empty()) {
			GameBullet* bullet = mBulletQueue.front();
			mBulletQueue.pop_front();
			int bulletId = ((__Integer*)(dic->objectForKey("bulletId")))->getValue();
			bullet->setBulletID(bulletId);
		}
	} else {
		//WORD view_chair_id = SwitchViewChairID(user_fire->chair_id);
		SCORE score = ((__Integer*)(dic->objectForKey("mulriple")))->getValue();
		mPlayerManager->AddScore(chairId, -score, false, false);

		float angle = ((__Float*)(dic->objectForKey("angle")))->getValue();
		if (chairId < 3) {
			angle -= M_PI;
		}
		if (!IsSameSide(chairId, SessionManager::shareInstance()->getChairId()))
			angle += M_PI;

		GamePlayer* curPlayer = mPlayerManager->getPlayerByChairId(chairId);
		if (!curPlayer)
			return ;
		curPlayer->setBulletMulriple(score);
	
		bool isDouble = ((__Bool*)(dic->objectForKey("isDouble")))->getValue();
		BulletKind bullet_kind;
		if (score <= 100) {
			bullet_kind = isDouble ? BULLET_2_DOUBLE : BULLET_2_NORMAL;
		} else if (score > 100 && score <= 1000) {
			bullet_kind = isDouble ? BULLET_3_DOUBLE : BULLET_3_NORMAL;
		} else if (score > 1000) {
			bullet_kind = isDouble ? BULLET_4_DOUBLE : BULLET_4_NORMAL;
		}
		curPlayer->getShooter()->setAngle(angle);
		curPlayer->switchGun(bullet_kind);
		curPlayer->fire();

		//播放开火音效
		//if (isDouble) {
		//	SoundManager::playFishEffect("sound_LZP_fire.mp3",0.3f);
		//	
		//} else {
		//	SoundManager::playFishEffect("fire.mp3",0.3f);
		//}

		Vec2 turret_pos = GetTurretPosition(SwitchViewChairID(chairId));
		//生成子弹
		GameBullet* pBullet = GameBulletManager::shareInstance()->Fire(turret_pos,angle,bullet_kind,
			mGameConfig.bullet_bounding_radius[bullet_kind],
			SwitchViewChairID(chairId),
			mGameConfig.bullet_speed[bullet_kind]);
		DWORD tick = ((__Integer*)(dic->objectForKey("tick")))->getValue();
		
		//同步位置
		pBullet->getBulletAction()->step(static_cast<float>(TimerManager::shareInstance()->GetDelayTick(tick)) / 1000.f);
		
		int b_id = ((__Integer*)dic->objectForKey("bulletId"))->getValue();
		pBullet->setBulletID(b_id);

		//批处理修改 9 -11
		//myLayer->addChild(pBullet,2);
		mBulletBatchNode->addChild(pBullet);
	}

}

void GameScene::receiveCatchFishMsg(EventCustom* event)
{
	  __Array* catchFishGroup = (__Array*)event->getUserData();
	  __Dictionary* fishDic = (__Dictionary*)catchFishGroup->getObjectAtIndex(0);
	  __Array* catchFish = (__Array*)fishDic->objectForKey("catchFish");
	  __Dictionary* firstFishDic = (__Dictionary*)catchFish->getObjectAtIndex(0);
	  int _link_fish_id = ((__Integer*)(firstFishDic->objectForKey("linkFishId")))->getValue();
	  int _kind = ((__Integer*)(firstFishDic->objectForKey("fishKind")))->getValue();
	  bool same_kind_fish = false;
	  bool bomb = false;
	  switch (SessionManager::shareInstance()->getGameKind())
	  {
	  case GameKindDntg:
		  {
			  if (_link_fish_id == -1 && ((FishKind)_kind >= FISH_DNTG && (FishKind)_kind <=FISH_QJF))
				  same_kind_fish = true;
			  if (_link_fish_id == -1 && ((FishKind)_kind == FISH_FOSHOU || (FishKind)_kind ==FISH_BGLU))
				  bomb = true;
		  }
		  break;
	  case GameKindLkpy:
		  {
			  if (_link_fish_id == -1 && (FishKind)_kind == FISH_KIND_31_40 )
				  same_kind_fish = true;
			  if (_link_fish_id == -1 && (FishKind)_kind == FISH_KIND_24 )
				  bomb = true;
		 } 
		  break;
	  case GameKindJcpy:
		  {
			  if (_link_fish_id == -1 && (FishKind)_kind == FISH_KIND_JCKING )
				  same_kind_fish = true;
		  }
		  break;
	  }
	 
	  DWORD bullet_id = ((__Integer*)fishDic->objectForKey("bulletId"))->getValue();
	  SCORE total_score = 0;
	  if (bomb)
		total_score = atoi(((__String*)firstFishDic->objectForKey("fishScore"))->getCString());

	  int chairId = ((__Integer*)fishDic->objectForKey("chairId"))->getValue();

	  WORD view_chair_id = SwitchViewChairID(chairId);
	  if (same_kind_fish)
		  GameChainManager::shareInstance()->SameChainFinish(view_chair_id);
	  std::vector<__Dictionary*> same_catch_fish;
	  int tick = 0;
	  for (int i =0;i<catchFishGroup->count();i++)
	  {
		  fishDic = (__Dictionary*)catchFishGroup->getObjectAtIndex(i);
		  catchFish = (__Array*)fishDic->objectForKey("catchFish");
		  chairId = ((__Integer*)fishDic->objectForKey("chairId"))->getValue();
		  tick = ((__Integer*)fishDic->objectForKey("tick"))->getValue();
		  for (int k=0;k <catchFish->count();k++)
		  {
		 
			  __Dictionary* infoDic = (__Dictionary*)catchFish->getObjectAtIndex(k);
			  bool isDouble = ((__Bool*)infoDic->objectForKey("isDouble"))->getValue();
			  if (isDouble)
			  {
				  int bullet_mulriple = mPlayerManager->getPlayerByChairId(chairId)->getBulletMulriple();
				  BulletKind bullet_kind;
				  if (bullet_mulriple < 100) {
					  bullet_kind = BULLET_2_DOUBLE;
				  } else if (bullet_mulriple >= 100 && bullet_mulriple < 1000) {
					  bullet_kind = BULLET_3_DOUBLE;
				  } else if (bullet_mulriple >= 1000) {
					  bullet_kind = BULLET_4_DOUBLE;
				  }
				  //触发双倍，则换成双倍的炮
				  mPlayerManager->getPlayerByChairId(chairId)->switchGun(bullet_kind);
				  // 播放换炮的音乐
				  //SoundManager::playMusicEffect(CommonFunction::getEffectName("sound_change_gun.mp3").c_str());
				  SoundManager::playFishEffect("sound_change_gun.mp3");
			  }
			  int link_fish_id = ((__Integer*)infoDic->objectForKey("linkFishId"))->getValue();
			  if (link_fish_id == -1) {
				   //特别处理，客户端还未检测到碰撞，服务端已发送捕鱼的消息
				  GameBulletManager::shareInstance()->Explode(bullet_id);
			  }
			  if (!same_kind_fish)
			  {
					int fish_kind = ((__Integer*)infoDic->objectForKey("fishKind"))->getValue();
					int fish_id = ((__Integer*)infoDic->objectForKey("fishId"))->getValue();
				  
					int fish_score = atoi(((__String*)infoDic->objectForKey("fishScore"))->getCString());
			
					GameFish* fish = nullptr;
					bool bingo = false;
				  
					int gameKind = SessionManager::shareInstance()->getGameKind();
					//分游戏进行处理
					if(gameKind == GameKindDntg)
					{
						fish = GameFishManager::sharedInstance()->catchFish(view_chair_id,(FishKind)fish_kind,fish_id,fish_score);
						if(!fish)
							continue;
						if (fish->fish_kind() >= FISH_JINCHAN && link_fish_id == -1)
							bingo = true;
						if (!bomb)
						{
							if (mIsMatchFlag&&mGameMatchInfo->getMatchType() == 1)//定时赛
							{

							}
							else
								mPlayerManager->AddScore(chairId,fish_score,true,bingo);
						}
						
						if (fish->fish_kind() == FISH_FOSHOU)
							DealBaoJiEffect(SwitchViewChairID(chairId));
						if (link_fish_id == -1) 
						{
							if (fish->fish_kind() == FISH_BGLU) 
							{
								//SoundManager::bagualu(fish->tag());
								if (fish->tag() == 0) 
								{
									float interval = TimerManager::shareInstance()->GetDelayTick(tick) / 1000.f;
									if (interval < kFishPauseTime) 
									{
										stopAllActions();
										DelayTime* delay = DelayTime::create(kFishPauseTime - interval);
										CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(GameScene::fishPauseOver));
										runAction(Sequence::createWithTwoActions(delay,callback));
										GameFishManager::sharedInstance()->fishPause(true, interval);
									}
								}
							} else if (fish->fish_kind() == FISH_FOSHOU) {
								//SoundManager::playMusicEffect("audios/qpzd_yuyin.mp3");
								SoundManager::playFishEffect("qpzd_yuyin.mp3");

							} else if (fish->fish_kind() == FISH_SWK) {
								//SoundManager::playMusicEffect("audios/sunwukong.mp3");
								SoundManager::playFishEffect("sunwukong.mp3");
							} else if (fish->fish_kind() == FISH_YUWANGDADI) {
								//SoundManager::playMusicEffect("audios/yuhuangdadi.mp3");
								SoundManager::playFishEffect("yuhuangdadi.mp3");
							} else if (fish->fish_kind() >= FISH_HAIGUI && fish->fish_kind() < FISH_CHAIN) {
								SoundManager::highScore();
							}
							ExecuteEffect(fish);
							CoinManager::shareInstance()->BuildCoins(fish->action_fish()->getPosition(),fish->action_fish()->getAngle(),GetTurretPosition(view_chair_id),fish_score,(FishKind)fish_kind,this);
						}
					}
					else if(gameKind == GameKindLkpy)
					{
						
						fish = GameFishManager::sharedInstance()->catchFish(view_chair_id,(FishKind)(fish_kind+FISH_KIND_1),fish_id,fish_score);
						if(!fish)
							continue;
						if (fish->fish_kind() >= FISH_KIND_20 && link_fish_id == -1)
							bingo = true;
						if (!bomb)
						{
							mPlayerManager->AddScore(chairId,fish_score,true,bingo);
						}
						if (link_fish_id == -1) 
						{
							//李逵中的定屏
							if (fish->fish_kind()  == FISH_KIND_22) 
							{
								float interval = TimerManager::shareInstance()->GetDelayTick(tick) / 1000.f;
								if (interval < kFishPauseTime) 
								{
									stopAllActions();
									DelayTime* delay = DelayTime::create(kFishPauseTime - interval);
									CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(GameScene::fishPauseOver));
									runAction(Sequence::createWithTwoActions(delay,callback));
									GameFishManager::sharedInstance()->fishPause(true, interval);
								}
								  
							} 
							ExecuteEffect(fish);
							CoinManager::shareInstance()->BuildCoins(fish->action_fish()->getPosition(),fish->action_fish()->getAngle(),GetTurretPosition(view_chair_id),fish_score,(FishKind)(fish->fish_kind()),this);
						}
					}else if(gameKind == GameKindJcpy)
					{
						fish = GameFishManager::sharedInstance()->catchFish(view_chair_id,(FishKind)(fish_kind+FISH_KIND_JC1),fish_id,fish_score);
						if(!fish)
							continue;
						if (fish->fish_kind() >= FISH_KIND_JC20 && link_fish_id == -1)
							bingo = true;
						if (!bomb)
						{
							mPlayerManager->AddScore(chairId,fish_score,true,bingo);
						}
						if (link_fish_id == -1) 
						{
							ExecuteEffect(fish);
							CoinManager::shareInstance()->BuildCoins(fish->action_fish()->getPosition(),fish->action_fish()->getAngle(),GetTurretPosition(view_chair_id),fish_score,(FishKind)(fish->fish_kind()),this);
						}
					}
			  }
			  else
			  {
				  same_catch_fish.push_back(infoDic);
			  } 

		 }
		
	  }

	  if (same_kind_fish) {
		  //在切换场景过程中有闪电过程直接完成
		  if (next_scene_kind != SCENE_COUNT) {
		  		GameChainManager::shareInstance()->SameChainFinish(view_chair_id);
		  } else {
				GameChainManager::shareInstance()->SameKindChainFish(view_chair_id, same_catch_fish,this);
		  }
	  }

	  if (bomb) {
		  if (mIsMatchFlag&&mGameMatchInfo->getMatchType() == 1)//定时赛
		  {

		  }
		  else
				mPlayerManager->AddScore(chairId, total_score, true, true);
	  }

}

void GameScene::ExecuteEffect(GameFish* fish) {

	int gameKind = SessionManager::shareInstance()->getGameKind();
	if(gameKind == GameKindDntg)
	{
		if (fish->fish_kind() < FISH_JINCHAN)
			return;

		Vec2 pos = fish->position();
		if (fish->fish_kind() == FISH_BGLU) {
			if (fish->tag() == 0)
				dealPauseStartEffect();
			else
				dealbombEffect(pos);
		}else{
			dealbombEffect(pos);
		}
	}else if(gameKind == GameKindLkpy)
	{

		 SoundManager::fish(fish->fish_kind());
		 if (fish->fish_kind() < FISH_KIND_16)
				return;

		 if(fish->fish_kind() == FISH_KIND_22)
		 {
			 dealPauseStartEffect();
		 }else if ( fish->fish_kind() != FISH_KIND_31_40)
		 {
			 //播放粒子效果
			 dealbombEffect(fish->position());
			 //播放爆炸音效
			 //SoundManager::playMusicEffect(CommonFunction::getEffectName("bomb_baozha.mp3").c_str());
			 SoundManager::playFishEffect("bomb_baozha.mp3");
		 }	
	}else
	{
		SoundManager::fish(fish->fish_kind());
		if (fish->fish_kind() < FISH_KIND_JC16)
			return;
		if(fish->fish_kind() != FISH_KIND_JCKING)
		{
			//播放粒子效果
			dealbombEffect(fish->position());
			//播放爆炸音效
			//SoundManager::playMusicEffect(CommonFunction::getEffectName("bomb_baozha.mp3").c_str());
			SoundManager::playFishEffect("bomb_baozha.mp3");
		}
	}
	
}
void GameScene::fishPauseOver(Node* pSender)
{
	GameFishManager::sharedInstance()->fishPause(false);
	dealPauseEndEffect();
}

void GameScene::receiveChangeSceneMsg(EventCustom* event)
{
	CMD_S_SwitchScene* switch_scene = static_cast<CMD_S_SwitchScene*>(event->getUserData());

	for (WORD i = 0; i < GAME_PLAYER; ++i) {
		GameChainManager::shareInstance()->chainFinish(i);
	}
	next_scene_kind  = switch_scene->next_scene;
	DWORD interval = TimerManager::shareInstance()->GetDelayTick(switch_scene->tick_count);
	float elasped = MIN(static_cast<float>(interval) / 1000.0f, kTideActionDuration);

	mCurBgIndex = (mCurBgIndex+1)%3;
	//客户端与服务器时间同步
	changeScenePro(kTideActionDuration - elasped);

	//SoundManager::playMusicEffect(CommonFunction::getEffectName("change_bg.mp3").c_str());
	SoundManager::playFishEffect("change_bg.mp3");

}

//修改李逵倍数
void GameScene::receiveModifyLkMulriple(EventCustom* event)
{
	CMD_S_HitFishLK* info = (CMD_S_HitFishLK*)event->getUserData();
	GameFishManager::sharedInstance()->modifyLkMulriple(info->fish_id,info->fish_mulriple);
}



void GameScene::changeScenePro(float dt)
{
	
	__String* bgName = __String::createWithFormat("%s/map/map%d.jpg",CommonFunction::getResDir().c_str(),mCurBgIndex);
	Sprite* mov_bg = Sprite::create(bgName->getCString());
	
	mov_bg->setPosition(Vec2(kScreenWidth*3/2,kScreenHeight/2));
	//myLayer->addChild(mov_bg);
	myLayer->addChild(mov_bg,1);

	//取掉波纹 2015-08-28
	//createWave(mov_bg,true);

	Sprite* tide_sp = Sprite::create();
	Vector<SpriteFrame*> frames;
	for (int i =0 ;i < 2;i++)
	{
		SpriteFrame* frame = SpriteFrame::create("map/tide.png",CCRectMake(i*346,0,346,1080));
		if (i == 0)
		{
			tide_sp->setDisplayFrame(frame);
		}
		frames.pushBack(frame);
	}
	Animation* anima = Animation::createWithSpriteFrames(frames,0.18f);
	tide_sp->runAction(RepeatForever::create(Animate::create(anima)));
	tide_sp->setPosition(Vec2(tide_sp->getContentSize().width/2 - 50 ,tide_sp->getContentSize().height/2));
	mov_bg->addChild(tide_sp,1);

	/*if(mGameKind == GameKindDntg)
	{
		Sprite* logo = Sprite::create("dntg2/images/map/logo.png");
		logo->setPosition(Vec2(logo->getContentSize().width/2-50,kScreenHeight/2));
		mov_bg->addChild(logo,1);
	}*/
	

	MoveTo* mt =MoveTo::create(dt,Vec2(kScreenWidth/2,kScreenHeight/2));
	CallFuncN* mov_end = CallFuncN::create(this,callfuncN_selector(GameScene::changeSceneOver));
	mov_bg->runAction(Sequence::createWithTwoActions(mt,mov_end));

}

void GameScene::createWave(Node* parent,bool isNextScene)
{
	Sprite* waveSp = nullptr;
	std::vector<std::string> imageNames;
	int i = 0, j = 0;
	for (i= 0; i < 32;i++)
	{
		imageNames.push_back(__String::createWithFormat("map/Wave_000_0%02d.png",i)->getCString());
	}
	__Array* waves = nullptr;
	if (!isNextScene)
	{
		waves = mWaves;
	}
	else
	{
		waves = mNextSceneWaves;
	}

	int dis_x = 0;
	if (isNextScene)
	{
		dis_x = 0;
	}
	for (i = 0 ;i < 1+ kScreenWidth/256;i++ )
	{
		for ( j = 0; j < 1+kScreenHeight/256; j++)
		{
			waveSp = Sprite::create("map/Wave_000_000.png");
			
			BlendFunc blendFunc = {GL_SRC_ALPHA,GL_ONE};
			waveSp->setBlendFunc(blendFunc);
			
			waveSp->setPosition(Vec2(dis_x + waveSp->getContentSize().width/2 +i*waveSp->getContentSize().width ,waveSp->getContentSize().height/2 + j*waveSp->getContentSize().height));
			parent->addChild(waveSp,1);
			waves->addObject(waveSp);
			
		}
	}
}

void GameScene::changeSceneOver(Node* pSender)
{
	//切换背景音乐
	//SoundManager::playMusicBackground(__String::createWithFormat("audios/bg_%02d.mp3",mCurBgIndex)->getCString(),true);
	SoundManager::playMusicBackground(getMusicBackgroudName(mCurBgIndex).c_str(),true);

	mNextSceneWaves->removeAllObjects();

	pSender->removeFromParent();
	__String* mapName = __String::createWithFormat("%s/map/map%d.jpg",CommonFunction::getResDir().c_str(),mCurBgIndex);
	mBgSp->setDisplayFrame(SpriteFrame::create(mapName->getCString(),CCRectMake(0,0,kScreenWidth,kScreenHeight)));

	GameFishManager::sharedInstance()->freeAllFish();
	mSceneFishManager->BuildSceneFish(next_scene_kind, SessionManager::shareInstance()->getChairId());

	next_scene_kind = SCENE_COUNT;
}

void GameScene::receiveUserEnterMsg(EventCustom* event)
{
	UserModel* pUser = (UserModel*)event->getUserData();
	log("GameScene receiveUserEnter111111 chair id =%d",pUser->getChairId());
	if (!mPlayerManager->playerIsExist(pUser->getChairId()))
	{
		mPlayerManager->addPlayer(pUser->getChairId());
	}
}

void GameScene::receiveExchangeScoreMsg(EventCustom* event)
{
	CMD_S_ExchangeFishScore* exchange_fishscore = static_cast<CMD_S_ExchangeFishScore*>(event->getUserData());
	mPlayerManager->exchangeScore(exchange_fishscore->chair_id,exchange_fishscore->exchange_fish_score);

	//SoundManager::playMusicEffect(CommonFunction::getEffectName("inscore.mp3").c_str());
	SoundManager::playFishEffect("inscore.mp3");
}

void  GameScene::receiveTimerSyncMsg(EventCustom* event) {
	CMD_S_TimerSync* timer_sync = static_cast<CMD_S_TimerSync*>(event->getUserData());
	TimerManager::shareInstance()->TimerSync(timer_sync->client_tick, timer_sync->server_tick);
}

void GameScene::receivePlayerToSeperaterMsg(EventCustom* event)
{
	UserModel* pUser = (UserModel*)event->getUserData();
	 mPlayerManager->removePlayer(pUser->getChairId());
	 if (pUser->getUserId() == SessionManager::shareInstance()->getUserId())
	 {
		 if(mIsMatchFlag)
		 {
			 //在比赛过程中要发退赛请求
			m_serviceClient->sendLeaveMatchRequest();
			if (!bShowAward)//中途退出
			{
				exitGameRoom();
			}
		 }
		 else
			exitGameRoom();
	 }
}


bool GameScene::onTouchBegan(Touch* pTouch, Event* pEvent)
{
	if(!mCanShooter || SessionManager::shareInstance()->getChairId() < 0 || SessionManager::shareInstance()->getChairId() > GAME_PLAYER)
		return false;
	//隐藏百科
	if (mGuideWidget && mGuideWidget->isVisible())
	{
		mGuideWidget->setVisible(false);
	}
	DWORD tick = SessionManager::shareInstance()->getCurrentTime();
	Vec2 curPt = pTouch->getLocation();
	
	if(!mCursorSp->isVisible())
		mCursorSp->setVisible(true);
	mCursorSp->setPosition(curPt);
	
	WORD me_chair_id = SessionManager::shareInstance()->getChairId();
	WORD chairId = SwitchViewChairID(me_chair_id);
	Vec2 turret_pos = GetTurretPosition(chairId);
	mAngle = CommonFunction::CalcAngle(curPt.x,curPt.y,turret_pos.x,turret_pos.y);
	
	me->getShooter()->setAngle(mAngle);
	if(!is_special_scene_ && next_scene_kind == SCENE_COUNT && tick - mLastFireTime >= kFireInterval)
	{
		mLastFireTime = tick;
		onBulletFire();
		if (getChildByTag(KCommonBagTag))
		{
			getChildByTag(KCommonBagTag)->removeFromParent();
			mKickOutLabel = nullptr;
		}

	}
	mBlongPressed = true;
	return isVisible();
}


void GameScene::onBulletFire()
{
	
	GameShooter* pShooter = me->getShooter();
	int bulletMulriple = pShooter->getBulletMulriple();
	if (pShooter->getScore() - bulletMulriple <0)
	{
		return;
	}
	mPlayerManager->fire(SessionManager::shareInstance()->getChairId());
	BulletKind kind = pShooter->getKind();
	int view_chair_id = SwitchViewChairID(SessionManager::shareInstance()->getChairId());
	//生成子弹
	Vec2 pos ;
	Vec2 dis = Vec2(kMuzzleOffset.x * std::sin(CC_DEGREES_TO_RADIANS(pShooter->getAngle())),kMuzzleOffset.y * std::cos(CC_DEGREES_TO_RADIANS(pShooter->getAngle())));
	Vec2 bullet_pos;
	Vec2::add(pShooter->getPosition(),dis,&bullet_pos);
	GameBullet* pBullet = GameBulletManager::shareInstance()->Fire(bullet_pos,CC_DEGREES_TO_RADIANS(pShooter->getAngle()),kind,
		mGameConfig.bullet_bounding_radius[kind],view_chair_id,mGameConfig.bullet_speed[kind]);

	//批处理修改
	//myLayer->addChild(pBullet,2);
	mBulletBatchNode->addChild(pBullet);
	
	pShooter->addScore(-bulletMulriple);
	mBulletQueue.push_back(pBullet);
	m_serviceClient->sendUserFire(CC_DEGREES_TO_RADIANS(pShooter->getAngle()),bulletMulriple);
	//

	if (kind >= BULLET_2_DOUBLE) {
	//	SoundManager::playMusicEffect(CommonFunction::getEffectName("sound_LZP_fire.mp3").c_str());
		SoundManager::playFishEffect("sound_LZP_fire.mp3",0.5f);
	} else {
	//	SoundManager::playMusicEffect(CommonFunction::getEffectName("fire.mp3").c_str());
		SoundManager::playFishEffect("fire.mp3",0.5f);
	}

	mScoreIsChange = true;
	
}

void GameScene::onTouchMoved(Touch* pTouch, Event* pEvent)
{
	if(!mCanShooter)
		return ;

	long  tick = SessionManager::shareInstance()->getCurrentTime();
	Vec2 curPt = pTouch->getLocation();
	mCursorSp->setPosition(curPt);
	WORD me_chair_id = SessionManager::shareInstance()->getChairId();
	WORD chairId = SwitchViewChairID(me_chair_id);
	Vec2 turret_pos = GetTurretPosition(chairId);
	mAngle = CommonFunction::CalcAngle(curPt.x,curPt.y,turret_pos.x,turret_pos.y);
	me->getShooter()->setAngle(mAngle);
	if(!is_special_scene_ && next_scene_kind == SCENE_COUNT && tick - mLastFireTime >= kFireInterval)
	{
		mLastFireTime = tick;
		onBulletFire();	
	}
}

void GameScene::onTouchEnded(Touch* pTouch, Event* pEvent)
{
	mBlongPressed = false;
	mCursorSp->setVisible(false);
}

WORD GameScene::SwitchViewChairID(WORD chair_id) {
	WORD me_chair_id = SessionManager::shareInstance()->getChairId();
	if (me_chair_id >= GAME_PLAYER)
		return chair_id;
	if (me_chair_id < 3) {
		return (chair_id + 3) % GAME_PLAYER;
	}
	return chair_id;
}

Vec2 GameScene::GetTurretPosition(WORD view_chair_id) {
	assert(view_chair_id < GAME_PLAYER);
	if (view_chair_id >= GAME_PLAYER) {
		return Vec2(0,0);
	}
	return Vec2(kGunPos[view_chair_id].x, kGunPos[view_chair_id].y);
}

void GameScene::createShooter()
{
	WORD chair_id = SessionManager::shareInstance()->getChairId();
	if(chair_id > GAME_PLAYER)
		return;


	if(!me)
		return ;
	WORD me_view_chairId = SwitchViewChairID(chair_id);

	BulletKind kind = calBulletKind();
	int bulletMulriple = mGameConfig.min_bullet_multiple;
	/*if (bulletMulriple <= 100) {
	kind = BULLET_2_NORMAL;
	} else if (bulletMulriple > 100 && bulletMulriple <= 1000) {
	kind = BULLET_3_NORMAL;
	} else if (bulletMulriple > 1000) {
	kind = BULLET_4_NORMAL;
	}*/

	bool isMe = false;
	if (!mIsMatchFlag)//比赛除外
	{
		isMe = true;
	}
	GameShooter* pShooter = GameShooter::create(me_view_chairId,kind,0,bulletMulriple,isMe);
	pShooter->setPosition(GetTurretPosition(me_view_chairId));
	pShooter->setShootGrade(mRoomLevel);
	pShooter->setMulripleArea(mGameConfig.min_bullet_multiple,mGameConfig.max_bullet_multiple);
	me->setShooter(pShooter);
	//me->setBulletMulriple(bulletMulriple);
	myLayer->addChild(pShooter,500);
	//addChild(pShooter);

	//发送上分消息 for mobie
	DWORD uid = SessionManager::shareInstance()->getUserId();
	UserModel* pUser = SessionManager::shareInstance()->getUserById(uid);
	if(pUser && isMe)
	{
		mPlayerManager->exchangeScore(pUser->getChairId(),pUser->getUserScore());
		m_serviceClient->exchangeScore(pUser->getUserScore());
	}
}

//初始化玩家界面元素
void GameScene::initPlayer(GamePlayer* player)
{
	int chairId= player->getChairID();
	if(chairId > GAME_PLAYER)
		return;
	float angle = 0;
	if (!IsSameSide(chairId,SessionManager::shareInstance()->getChairId()))
	{
		angle +=M_PI;
	}


	if(chairId == SessionManager::shareInstance()->getChairId() )
	{
		me = player;
		CC_SAFE_RETAIN(me);
		createShooter();
		mPlayerManager->isAllPalyersIsOnSit();
	}
	else
	{
		WORD me_view_chairId = SwitchViewChairID(chairId);

		BulletKind kind = calBulletKind();
		/*int bulletMulriple = mGameConfig.min_bullet_multiple;

		if (bulletMulriple <= 100) {
		kind = BULLET_2_NORMAL;
		} else if (bulletMulriple > 100 && bulletMulriple <= 1000) {
		kind = BULLET_3_NORMAL;
		} else if (bulletMulriple > 1000) {
		kind = BULLET_4_NORMAL;
		}*/

		GameShooter* pShooter = GameShooter::create(me_view_chairId,kind,angle,mGameConfig.min_bullet_multiple);
		pShooter->setPosition(GetTurretPosition(me_view_chairId));
		pShooter->setShootGrade(mRoomLevel);
		pShooter->setMulripleArea(mGameConfig.min_bullet_multiple,mGameConfig.max_bullet_multiple);
		player->setShooter(pShooter);
		player->setBulletMulriple(mGameConfig.min_bullet_multiple);
		myLayer->addChild(pShooter,500);
	}
}

//称除玩家界面元素
void GameScene::removePlayer(GamePlayer* player)
{
	GameShooter* pShooter = player->getShooter();
	pShooter->removeFromParent();
}

//是否在同一边
bool GameScene::IsSameSide(WORD chair_id1, WORD chair_id2) {
	return (chair_id1 / 3) == (chair_id2 / 3);
}

void GameScene::receiveSubSceneFishMsg(EventCustom* event)
{
	__Array* fishArray = (__Array*)event->getUserData();
	int fishCnt = fishArray->count();
	ActionFishMove* action = nullptr;
	if (fishCnt > 0)
	{
		for (int i = 0; i < fishCnt; i++)
		{
			__Dictionary* fishInfoDic = (__Dictionary*)fishArray->getObjectAtIndex(i);
			FishKind kid = (FishKind)((__Integer*)fishInfoDic->objectForKey("fishKind"))->getValue();
			int fishId = ((__Integer*)fishInfoDic->objectForKey("fishId"))->getValue();
			int fishTag = ((__Integer*)fishInfoDic->objectForKey("tag"))->getValue();

			//佛手特别处理
			PointArray* ptArray = (PointArray*)fishInfoDic->objectForKey("ptArray");
			switch (ptArray->count())
			{
			case 2:
				{
					action = ActionFishMoveLinear::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1));
				}
				break;
			case 3:
				{
					action = ActionFishMoveBezier::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1),ptArray->getControlPointAtIndex(2));
				}
				break;
			case 4:
				{
					action = ActionFishMoveBezier::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1),ptArray->getControlPointAtIndex(2),ptArray->getControlPointAtIndex(3));
				}
				break;
			}

			//大闹天空，是佛手，在LKPY 是鱼王特别处理
			if(SessionManager::shareInstance()->getGameKind() == GameKindDntg)
			{
				//佛手特别处理
				if (kid == FISH_FOSHOU)
				{
					action = ActionFishMoveFoshou::create(mGameConfig.fish_speed[kid],ptArray->getControlPointAtIndex(0),ptArray->getControlPointAtIndex(1));
				}
			}

			DWORD  tick = ((__Integer*)fishInfoDic->objectForKey("tick"))->getValue();
			DWORD elasped = TimerManager::shareInstance()->GetDelayTick(tick);
			DWORD fish_elasped = ((__Integer*)fishInfoDic->objectForKey("elapsed"))->getValue();
			// TODO:服务器和本地时间同步
			action->step(fish_elasped + (float)elasped / 1000.f);

			{

				GameFish* fish = GameFishManager::sharedInstance()->creatFish(kid,fishId,fishTag,mGameConfig.fish_bounding_radius[kid],mGameConfig.fish_bounding_count[kid],action);
				if (fish)
					myLayer->addChild(fish);
			}
		}
	}
}

void GameScene::receiveForceTimerSyncMsg(EventCustom* event)
{
	m_serviceClient->SendTimerSync();
}

void GameScene::receiveBulletDoubleTimeoutMsg(EventCustom* event)
{
	CMD_S_BulletDoubleTimeout* time_out = static_cast<CMD_S_BulletDoubleTimeout*>(event->getUserData());
	GamePlayer* player = mPlayerManager->getPlayerByChairId(time_out->chair_id);
	int bullet_mulriple = player->getBulletMulriple();
	BulletKind bullet_kind;
	if (bullet_mulriple <= 100) {
		bullet_kind = BULLET_2_NORMAL;
	} else if (bullet_mulriple > 100 && bullet_mulriple <= 1000) {
		bullet_kind = BULLET_3_NORMAL;
	} else if (bullet_mulriple > 1000) {
		bullet_kind = BULLET_4_NORMAL;
	}
	player->switchGun(bullet_kind);
	// 播放换抢音效
	//SoundManager::playMusicEffect(CommonFunction::getEffectName("sound_change_gun.mp3").c_str());
	SoundManager::playFishEffect("sound_change_gun.mp3");
}

void GameScene::receiveCatchChainMsg(EventCustom* event)
{
	CMD_S_CatchChain* catch_chain = static_cast<CMD_S_CatchChain*>(event->getUserData());

	if (next_scene_kind != SCENE_COUNT) {
		GameChainManager::shareInstance()->chainFinish(SwitchViewChairID(catch_chain->chair_id));
	} else {
		GameChainManager::shareInstance()->chainFish(this,SwitchViewChairID(catch_chain->chair_id), catch_chain->catch_fish, catch_chain->fish_count);
	}

	GameBulletManager::shareInstance()->Explode(catch_chain->bullet_id);
}

//触发闪电效果处理
void GameScene::startChain(WORD view_chair_id)
{
	Node* chainNode = Node::create();
	addChild(chainNode,0,kChainFlagTag + view_chair_id);

	float angle = 0;
	Vec2 pos = GetTurretPosition(view_chair_id);
	if (view_chair_id < 3)
	{
		angle -=M_PI_2;
	}
	else
	{
		angle +=M_PI_2;
	}

	Sprite* flagSp = Sprite::create();
	chainNode->addChild(flagSp);

	Vector<SpriteFrame*> frames;
	bool isReload = false;
	Texture2D* pTexture =  Director::sharedDirector()->getTextureCache()->getTextureForKey("dntg2/images/fish/LChainFlag.png");
	if(pTexture)
		isReload = true;

	for(int i=0;i<3;i++)
	{
		for(int j = 0 ; j <3  ; j ++)
		{
			SpriteFrame* frame = nullptr;
			if(isReload)
				frame= SpriteFrame::createWithTexture(pTexture,CCRectMake(j*384,i*256,384,256));
			else
				frame = SpriteFrame::create("dntg2/images/fish/LChainFlag.png",CCRectMake(j*384,i*256,384,256));
			if(i==0)
			{
				flagSp->setSpriteFrame(frame);
			}
			if (NULL != frame) {
				frames.pushBack(frame);
			}
		}

	}
	Animation* anima = Animation::createWithSpriteFrames(frames,0.14f);
	Animate* anit = Animate::create(anima);

	if (view_chair_id < 3)
	{
		flagSp->setRotation(CC_RADIANS_TO_DEGREES(M_PI));
	}
	flagSp->setPosition(pos);
	flagSp->setAnchorPoint(Vec2(0.5,0));
	RotateBy* rb = RotateBy::create(1.5f,360);
	flagSp->runAction(RepeatForever::create(anit));
	flagSp->runAction(rb);
	
}

void GameScene::onChainpro(WORD view_chair_id,const CatchFish* fishArr,int cnt)
{
	startChain(view_chair_id);

	Vec2 pos = GetTurretPosition(view_chair_id);

	Vec2 move_to;
	Vec2 last_pos;
	float last_radius = 0;
	float radius[kMaxChainFishCount];
	memset(radius, 0, sizeof(radius));
	Vec2 end_pos[kMaxChainFishCount];
	float scale =0.0f;
	int link_fish_count = 0;

	//闪电圈的半径
	GameAnimation* greenLight  =  GameAnimation::create(ResourcesManager::getInstance()->getAnimationInfoByKind(FISH_CHAIN));

	for (int i = 0 ; i < cnt;i++)
	{
		GameFish* fish = GameFishManager::sharedInstance()->getFish(fishArr[i].fish_id);
		if (!fish)
			continue;
		Vec2 fish_pos = fish->position();
		float angle  = 0.f;
		fish->set_pause(true);

		//闪电连锁中的鱼，过滤子弹触碰添加
		fish->ChainLock();
		
		scale = fish->Scale();
		if (scale > 1)
			scale = 1;

		if (i == 0)
		{
			move_to.x = kGunPos[view_chair_id].x;
			if (view_chair_id < 3) {
				move_to.y = kGunPos[view_chair_id].y - 226 -  greenLight->getFrameWidth()/2 * scale;
			} else {
				move_to.y = kGunPos[view_chair_id].y + 226 +  greenLight->getFrameWidth()/2 * scale;
			}
			last_pos = move_to;
			last_radius = greenLight->getFrameWidth() * scale / 2;

			fish->onChainPro(this,pos,last_pos,view_chair_id);
		}
		else
		{
			Vec2 delta = fish_pos - last_pos;
			float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
			if (link_fish_count > 2)
			{
				for (int j = 0; j < link_fish_count - 1; ++j) 
				{
					Vec2 temp = fish_pos - end_pos[j];
					float len = std::sqrt(temp.x * temp.x + temp.y * temp.y);
					if (len < length) {
						length = len;
						delta = temp;
						last_pos = end_pos[j];
						last_radius = radius[j];
					}
				}		
			}

			float rote = 0.f;
			if (length >= 0) {
				if (delta.y > 0) {
					rote = -std::acos(delta.x / length);
				} else {
					rote = std::acos(delta.x / length);
				}
				angle =  CommonFunction::CalcAngle(fish_pos.x,fish_pos.y,last_pos.x,last_pos.y);
			}
		
			move_to.x = last_pos.x + (last_radius + greenLight->getFrameWidth() * scale / 2) * std::sin(angle);
			move_to.y = last_pos.y + (last_radius + greenLight->getFrameWidth() * scale / 2) * std::cos(angle);

			fish->onChainPro(this,last_pos,move_to,view_chair_id,i);

			last_pos = move_to;
			last_radius = greenLight->getFrameWidth() * scale / 2;
	
		}
		end_pos[i] = move_to;
		radius[i]  = last_radius;
		link_fish_count++;
	}




}

void GameScene::addChain(GameAnimation* chain,WORD view_chair_id)
{
	addChild(chain);
}

void GameScene::addCoin(Node* coinNode)
{
	mCoinBatchNode->addChild(coinNode);
}

void GameScene::receiveFishArriveMsg(EventCustom* event)
{
	__Dictionary* dic = (__Dictionary*)event->getUserData();
	int chair_id = ((__Integer*)dic->objectForKey("chairId"))->getValue();
	GameChainManager::shareInstance()->chainFishArrive(chair_id);
}

void GameScene::hideChainPro(WORD view_chair_id)
{
	Node* pChainNode = getChildByTag(kChainFlagTag + view_chair_id);
	if (pChainNode && pChainNode->isVisible())
	{
		pChainNode->removeFromParent();
	}
}

void GameScene::addScore(WORD view_chair_id,SCORE score,bool coin_effect,bool bingo_effect)
{
	WORD chair_id = SwitchViewChairID(view_chair_id);
	if (mIsMatchFlag&&mGameMatchInfo->getMatchType() == 1)//定时赛
	{

	}
	else
		mPlayerManager->AddScore(chair_id,score,coin_effect,bingo_effect);
}

void GameScene::DealBaoJiEffect(WORD chair_id)
{
	if (getChildByTag(KBaojiTag+chair_id))
	{
		Sprite* sp = (Sprite*)getChildByTag(KBaojiTag+chair_id);
		sp->stopAllActions();
		sp->removeFromParent();
	}
	Sprite* baoji = Sprite::create();
	Vector<SpriteFrame*> frames;
	for (int i= 0;i<2;i++)
	{
		SpriteFrame* frame = SpriteFrame::create("common/players/baoji.png",Rect(i*204,0,204,102));
		if(frame && i==0)
			baoji->setDisplayFrame(frame);
		if(frame)
			frames.pushBack(frame);
	}
	Animation* pAcion = Animation::createWithSpriteFrames(frames);
	Animate* pAnima = Animate::create(pAcion);
	baoji->runAction(RepeatForever::create(pAnima));
	Blink* pBlink = Blink::create(10.0f,20);
	RemoveSelf* rs = RemoveSelf::create();
	baoji->runAction(Sequence::createWithTwoActions(pBlink,rs));

	Vec2 pos = GetTurretPosition(chair_id);

	int dis =0;
	if (chair_id < 3 )
	{
		dis = -200;
	}
	else
	{
		dis = 200;
	}

	Vec2 _pos;
	Vec2::add(pos,Vec2(0,dis),&_pos);
	baoji->setPosition(_pos);
	baoji->setAnchorPoint(Vec2(0.5,0.5));
	if (chair_id<3)
	{
		baoji->setRotation(180);
	}
	addChild(baoji,1,KBaojiTag+chair_id);
}

void GameScene::receiveDealBingoEffectMsg(EventCustom* event)
{
	__Dictionary* infoDic = (__Dictionary*)event->getUserData();
	WORD chair_id = ((__Integer*)infoDic->objectForKey("chair_id"))->getValue();
	String* score_str = (__String*)infoDic->objectForKey("score");


	Vec2 pos = GetTurretPosition(chair_id);
	Node* exitNode  = getChildByTag(KBinGoFalgTag+chair_id);
	if(exitNode)
	{
		exitNode->removeFromParent();
	}
	Node* bingoNode = Node::create();

	Sprite* bingoSp = Sprite::create("common/players/bingo.png");
	RotateBy* bingoRb = RotateBy::create(0.2f,60);

	LabelAtlas* LabelScore = LabelAtlas::create("0","common/players/bingo_num.png",24,31,'0');

	LabelScore->setString(score_str->getCString());
	LabelScore->setAnchorPoint(Vec2(0.5f,0.5f));


	Vector<FiniteTimeAction*> labelactions;
	RotateTo* rb = RotateTo::create(0.3f,30);

	RotateTo* rb2 = RotateTo::create(0.6f, - 60);

	labelactions.pushBack(DelayTime::create(5.0f));
	labelactions.pushBack(FadeOut::create(1.5f));
	labelactions.pushBack(RemoveSelf::create(true));

	LabelScore->runAction(RepeatForever::create(Sequence::createWithTwoActions(rb,rb2)));
	LabelScore->runAction(Sequence::create(labelactions));

	bingoSp->runAction(RepeatForever::create(bingoRb));

	Vector<FiniteTimeAction*> actions;
	DelayTime* delay = DelayTime::create(5.0f);
	actions.pushBack(delay);
	FadeOut*  fo     = FadeOut::create(1.5f);
	actions.pushBack((fo));
	CallFuncN* call  = CallFuncN::create(this,callfuncN_selector(GameScene::removeBingo));
	actions.pushBack(call);
	bingoSp->runAction(Sequence::create(actions));

	int dis =0;
	if (chair_id < 3 )
	{
		dis = -200;
	}
	else
	{
		dis = 200;
	}

	bingoNode->addChild(bingoSp);
	bingoNode->addChild(LabelScore);

	if (chair_id < 3)
		bingoNode->setRotation(CC_RADIANS_TO_DEGREES(M_PI));
	Vec2 bingo_pos;
	Vec2::add(pos,Vec2(0,dis),&bingo_pos);
	bingoNode->setAnchorPoint(Vec2(0.5,0.5));
	bingoNode->setPosition(bingo_pos);

	addChild(bingoNode,1,KBinGoFalgTag+chair_id);

}

void GameScene::receiveDealJettonEffectMsg(EventCustom* event)
{
	GameJettonWidget* pNode = (GameJettonWidget*)event->getUserData();
	addChild(pNode,1);
}


void GameScene::removeBingo(Node* pSender)
{
	Node* parent = pSender->getParent();
	parent->removeFromParent();

}

void GameScene::receiveDealBgluEffectMsg(EventCustom* event)
{
	__Dictionary* dic = (__Dictionary*)event->getUserData();
	int fishId = ((__Integer*)(dic->objectForKey("fishId")))->getValue();
	GameFish* fish = GameFishManager::sharedInstance()->getFish(fishId);

	//屏幕震动
	shakeScreen();

	return;

	Node* exitNode  = getChildByTag(KBgluTag);
	if(exitNode)
	{
		exitNode->removeFromParent();
	}

	int tag = fish->tag();
	if (tag > 3)
		tag = 0;

	char fontName[16] ={0};
	char dyName[16] ={0};
	switch (tag)
	{
	case Bglu_Type_Ta:
		{
			strcpy(fontName,"jingangta");
			strcpy(dyName,"bglu_ta");
		}
		break;
	case Bglu_Type_Lua:
		{
			strcpy(fontName,"wdfhl");
			strcpy(dyName,"bglu_lun");
		}
		break;
	case Bglu_Type_Bang:
		{
			strcpy(fontName,"ryjgb");
			strcpy(dyName,"bglu_gun");
		}
		break;
	}
	
	Node* bgluNode = Node::create();
	addChild(bgluNode,0,KBgluTag);

	__String* pathStr =nullptr;
	pathStr = __String::createWithFormat("dntg2/images/fish/%s.png",dyName);
	
	Sprite* effectSp = Sprite::create();
	effectSp->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
	bgluNode->addChild(effectSp);

	AnimationInfo* info = ResourcesManager::getInstance()->getAnimationInfoByName(dyName);
	Sprite* fishImg = Sprite::create(pathStr->getCString());
	int row = fishImg->getContentSize().width/info->getFrameWidth();
	int col = fishImg->getContentSize().height/info->getFrameHeight();
	Animate* anim = CommonFunction::createFrameAnimate(pathStr->getCString(),0.14f,col,row,info->getFrameWidth(),info->getFrameHeight());
	
	CallFuncN* callBack = CallFuncN::create(this,callfuncN_selector(GameScene::bgLuDealOver));
	effectSp->runAction(Sequence::createWithTwoActions(anim,callBack));


	pathStr = __String::createWithFormat("dntg2/images/fish/%s.png",fontName);

	Sprite* fontSp = Sprite::create();
	fontSp->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2+100));
	bgluNode->addChild(fontSp);

	info = ResourcesManager::getInstance()->getAnimationInfoByName(fontName);
	fishImg = Sprite::create(pathStr->getCString());
	row = fishImg->getContentSize().width/info->getFrameWidth();
	col = fishImg->getContentSize().height/info->getFrameHeight();
	anim = CommonFunction::createFrameAnimate(pathStr->getCString(),0.18f,col,row,info->getFrameWidth(),info->getFrameHeight());

	callBack = CallFuncN::create(this,callfuncN_selector(GameScene::bgLuDealOver));
	fontSp->runAction(Sequence::createWithTwoActions(anim,callBack));

}

void GameScene::bgLuDealOver(Node* pSender)
{
	mDealCnt++;
	if(mDealCnt == 2)
	{
		mDealCnt = 0 ;
		Node* exitNode  = getChildByTag(KBgluTag);
		if(exitNode)
		{
			exitNode->removeFromParent();
		}
	}
}

void GameScene::preLoadSoundRes()
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	int i  =0;
	if(SessionManager::shareInstance()->getGameKind() == GameKindDntg)
	{
		for ( i = 0 ; i <MusicCount;i++)
		{
			SoundManager::preloadEffect(__String::createWithFormat("audios/%s.mp3",GameMusic[i])->getCString());
		}

	}
	else if(SessionManager::shareInstance()->getGameKind() == GameKindLkpy  )
	{
		for ( i = 0 ; i <MusicLkCount;i++)
		{
			SoundManager::preloadEffect(__String::createWithFormat("GameResources/audios_lk/%s.mp3",GameMusicForLk[i])->getCString());
		}
	}else if (SessionManager::shareInstance()->getGameKind() == GameKindJcpy)
	{
		for ( i = 0 ; i <MusicLkCount;i++)
		{
			SoundManager::preloadEffect(__String::createWithFormat("GameResources/audios_jc/%s.mp3",GameMusicForLk[i])->getCString());
		}
	}
#endif
}

void GameScene::dealbombEffect(Vec2 pos)
{

	ValueMap plist = FileUtils::getInstance()->getValueMapFromFile("particles/baozha.plist");  
	CommonParticle*p = CommonParticle::create(plist,"particles/baozha.png");
	p->setPosition(pos);
	addChild(p);


	plist = FileUtils::getInstance()->getValueMapFromFile("particles/zhendong_baodian.plist");  
	p = CommonParticle::create(plist,"particles/zhendong_baodian.png");
	p->setPosition(pos);
	addChild(p);

	plist = FileUtils::getInstance()->getValueMapFromFile("particles/zhendong_ci.plist");  
	p = CommonParticle::create(plist,"particles/zhendong_ci.png");
	p->setPosition(pos);
	addChild(p);

	plist = FileUtils::getInstance()->getValueMapFromFile("particles/yuanbao_guangqiu.plist");  
	p = CommonParticle::create(plist,"particles/yuanbao_guangqiu.png");
	p->setPosition(pos);
	addChild(p);


	plist = FileUtils::getInstance()->getValueMapFromFile("particles/yuanbao.plist");  
	p = CommonParticle::create(plist,"particles/yuanbao.png");
	p->setPosition(pos);
	addChild(p);


	plist = FileUtils::getInstance()->getValueMapFromFile("particles/paopao.plist");  
	p = CommonParticle::create(plist,"particles/paopao.png");
	p->setPosition(pos);
	addChild(p);

	/*plist = FileUtils::getInstance()->getValueMapFromFile("particles/zhendong_ciquan.plist");  
	p = CommonParticle::create(plist,"particles/zhendong_ciquan.png");
	p->setPosition(pos);
	addChild(p);*/


	/*plist = FileUtils::getInstance()->getValueMapFromFile("particles/zhendong_ciquan2.plist");  
	p = CommonParticle::create(plist,"particles/zhendong_ciquan2.png");
	p->setPosition(pos);
	addChild(p);*/

	plist = FileUtils::getInstance()->getValueMapFromFile("particles/zhendong_quan.plist");  
	p = CommonParticle::create(plist,"particles/zhendong_quan.png");
	p->setPosition(pos);
	addChild(p);

	//plist = FileUtils::getInstance()->getValueMapFromFile("particles/zhendong_xiquan.plist");  
	//p = CommonParticle::create(plist,"particles/zhendong_xiquan.png");
	//p->setPosition(pos);
	//addChild(p);
}

void GameScene::dealPauseStartEffect()
{
	if(!mIceImage)
	{
		mIceImage = ImageView::create();
		mIceImage->loadTexture("ice.png",ui::Widget::TextureResType::LOCAL);
		mIceImage->setScaleX(kScreenWidth/mIceImage->getContentSize().width);
		mIceImage->setScaleY(kScreenHeight/mIceImage->getContentSize().height);
		mIceImage->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
		addChild(mIceImage);
	}
	else
	{
		mIceImage->setVisible(true);
		//停止尚未结束的暂停效果，用新的替换
		if (getChildByTag(KFengyinTag))
		{
			getChildByTag(KFengyinTag)->stopAllActions();
			getChildByTag(KFengyinTag)->removeFromParent();
		}
	}
	

	

	Sprite* fengyinSp = Sprite::create();
	fengyinSp->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
	addChild(fengyinSp);
	fengyinSp->setTag(KFengyinTag);
	std::vector<std::string> imageNames;
	std::string name ="";
	for (int i = 0 ; i < 10;i++)
	{
		name = __String::createWithFormat("fengyin_kaishi/fengyin_0000%d.png",i)->getCString();
		imageNames.push_back(name);
	}

	CallFuncN* call = CallFuncN::create(this,callfuncN_selector(GameScene::fengyinStay));
	fengyinSp->runAction(Sequence::createWithTwoActions(CommonFunction::createFrameAnimate(imageNames,0.14f),call));
}


void GameScene::fengyinStay(Node* pSend)
{
	Sprite* sp = (Sprite*)pSend;
	std::vector<std::string> imageNames;
	std::string name ="";
	for (int i = 0 ; i < 12;i++)
	{
		name = __String::createWithFormat("fengyin_chixu/fengyin_chixu_000%02d.png",i)->getCString();
		imageNames.push_back(name);
	}
	sp->runAction(RepeatForever::create(CommonFunction::createFrameAnimate(imageNames,0.14f)));;
}



void GameScene::dealPauseEndEffect()
{
	if(mIceImage)
		mIceImage->setVisible(false);
	if (!getChildByTag(KFengyinTag))
	{
		return;
	}
	Sprite* sp = (Sprite*)getChildByTag(KFengyinTag);
	sp->stopAllActions();
	std::vector<std::string> imageNames;
	std::string name ="";
	for (int i = 0 ; i < 15;i++)
	{
		name = __String::createWithFormat("fengyin_jieshu/fengyin_jieshu_000%02d.png",i)->getCString();
		imageNames.push_back(name);
	}
	RemoveSelf* rs = RemoveSelf::create();
	sp->runAction(Sequence::createWithTwoActions(CommonFunction::createFrameAnimate(imageNames,0.14f),rs));
}

void GameScene::RenderKickoutTips()
{
	if (mIsMatchFlag)
	{
		return;
	}
	if (mLastFireTime == 0 || bSend)
		return;
	DWORD now = SessionManager::shareInstance()->getCurrentTime();
	if (now - mLastFireTime < 60000)
		return;
	if (!getChildByTag(KCommonBagTag))
	{
		Node* kickOutNode = Node::create();
		addChild(kickOutNode,0,KCommonBagTag);
		
		ImageView* bg = ImageView::create();
		bg->loadTexture("KickOutTipBack.png");
		bg->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
		kickOutNode->addChild(bg);

		mKickOutLabel = LabelAtlas::create("0","catch_num.png",44,64,'0');
		mKickOutLabel->setPosition(Vec2(kScreenWidth/2+120,kScreenHeight/2));
		mKickOutLabel->setAnchorPoint(Vec2(0.5,0.5));
		kickOutNode->addChild(mKickOutLabel);
	}

	int num = (60000 - (now - mLastFireTime - 60000)) / 1000;
	if (num < 0)
		num = 0;
	mKickOutLabel->setString(numberToString(num));
	if (num == 0 )
	{
		getChildByTag(KCommonBagTag)->removeFromParent();
	}

}

void GameScene::receiveSystemMsg(EventCustom* event)
{
	CMD_CM_SystemMessage* system_msg = (CMD_CM_SystemMessage*)event->getUserData();
	__String* msg = __String::create(system_msg->szString);
	popCommonTip(msg);
}

void GameScene::popCommonTip(__String* content)
{
	if (mMsgArray->count() > 0 )
	{
		mMsgArray->addObject(content);
	}
	else
	{
		Node* tipNode = Node::create();
		addChild(tipNode,999,KCommonTipTag);

		ImageView* bg = ImageView::create();
		bg->loadTexture("MessageTipBack.png");
		bg->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
		tipNode->addChild(bg);

		Text* tipText = Text::create();
		//__String* combin = __String::createWithFormat("%s%s",CommonFunction::GBKToUTF8("系统消息:"),CommonFunction::GBKToUTF8(content->getCString()));
		tipText->setString(CommonFunction::GBKToUTF8(content->getCString()));
		tipText->setFontSize(40);
		tipText->setColor(ccc3(255,255,255));
		tipText->setPosition(Vec2(bg->getContentSize().width/2 ,bg->getContentSize().height/2));
		bg->addChild(tipText);

		DelayTime* delay = DelayTime::create(1.0f);
		FadeOut*   fo    = FadeOut::create(0.5);
		CallFuncN* callback =  CallFuncN::create(this,callfuncN_selector(GameScene::showNextTip));
		tipText->runAction(Sequence::create(delay,fo,callback,NULL));
		mMsgArray->addObject(content);
	}

	
}

void GameScene::showNextTip(Node* pSender)
{
	//移除第一个元素
	mMsgArray->removeObjectAtIndex(0);
	if (mMsgArray->count() > 0 )
	{
		__String* tip = (__String*)mMsgArray->getObjectAtIndex(0);
		Text* tipText = (Text*)pSender;
		tipText->setText(CommonFunction::GBKToUTF8(tip->getCString()));
		tipText->setOpacity(255);
		DelayTime* delay = DelayTime::create(1.0f);
		FadeOut*   fo    = FadeOut::create(0.5);
		CallFuncN* callback =  CallFuncN::create(this,callfuncN_selector(GameScene::showNextTip));
		tipText->runAction(Sequence::create(delay,fo,callback,NULL));
	}
	else
	{
		if (getChildByTag(KCommonTipTag))
		{
			getChildByTag(KCommonTipTag)->removeFromParent();
		}
	}
}

void GameScene::standupOverTime(Node* node)
{
	log("stand up over time");
	stopAllActions();
	exitGameRoom();
}

void GameScene::receiveGameSorketCloseMsg(EventCustom* event)
{
	log("receive close msg");
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8("与服务器断开连接,请查看您的网络!")));
}

void GameScene::okCallback()
{
	if (mbEnterMatch)
	{
		GameMatchScene* pScene = GameMatchScene::create();
		pScene->setShowWait(true);
		Director::sharedDirector()->replaceScene(pScene);
	}
	else
	{
		exitGameRoom();
	}
}

void GameScene::exitGameRoom()
{
	stopAllActions();
	SessionManager::shareInstance()->setChairId(-1);
	SessionManager::shareInstance()->setGameKind(0);
	SessionManager::shareInstance()->setInGame(false);
	SessionManager::shareInstance()->setHasFinish(false);


	if(!mIsMatchFlag)
		Director::sharedDirector()->replaceScene(GameSmallScene::create(mRoomLevel));
	else
		Director::sharedDirector()->replaceScene(GameMatchScene::create());
}

void GameScene::receiveKeyEscapeMsg(EventCustom* evt)
{
	//取消长按状态
	if(mBlongPressed)
		mBlongPressed = false;
}

//比赛相关消息处理
void GameScene::receiveUpdateMatchStatusMsg(EventCustom* evt)
{
	CMD_FishMatch_Screen *pMatchScreen=(CMD_FishMatch_Screen*)evt->getUserData();
	mGameMatchInfo->setMatchType(pMatchScreen->IsTimeMatch);
	mGameMatchInfo->setMatchTime(pMatchScreen->lTimerSpan);
	mGameMatchInfo->setCurMatchStatus(pMatchScreen->dwMatchStatus);
}

void GameScene::receiveUpdateMatchInfoMsg(EventCustom* evt)
{
	CMD_FishMatch_Info *pMatchInfo=(CMD_FishMatch_Info*)evt->getUserData();

	mGameMatchInfo->setSelfRank(pMatchInfo->dwMeRank);
	mGameMatchInfo->setHaveAllUser(pMatchInfo->dwHaveuser);
	mGameMatchInfo->setPlayCount(pMatchInfo->lPlaycount);
	mGameMatchInfo->setAllPlayCount(pMatchInfo->lAllPlayCount);
	mGameMatchInfo->setMyCurScore(pMatchInfo->lmeCurScore);
	mGameMatchInfo->setMyMaxScore(pMatchInfo->lmeMaxScore);
	mGameMatchInfo->setTotalMaxScore(pMatchInfo->lMaxScore);
	mGameMatchInfo->setMatchTitle(pMatchInfo->szTitle);

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateMatchRankMsg,mGameMatchInfo);

}

//奖励消息处理
void GameScene::receiveMatchAwardMsg(EventCustom* evt)
{
	CMD_GR_MatchResult *pResult=(CMD_GR_MatchResult*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(GameMatchAwardWidget::create(pResult));
	bShowAward = true;
}

void GameScene::toSendTryAgainMatch(EventCustom* evt)
{
	m_serviceClient->sendTryAaginRequest();
}

void GameScene::showMatchTryAgainMsg(EventCustom* evt)
{
	__Dictionary* dic = (__Dictionary*)evt->getUserData();
	int bContinue = ((__Integer*)dic->objectForKey("bCotinue"))->getValue();
	mGameMatchInfo->setBContinue(bContinue);
	ModalViewManager::sharedInstance()->showWidget(GameMatchNextTryWidget::create(mGameMatchInfo));
}

void GameScene::receiveMatchIsBeingStartMsg(EventCustom* evt)
{
	MatchInfoModel* pMatchInfo = (MatchInfoModel*)evt->getUserData();
	mbEnterMatch = true;
	__String* tip_str = __String::createWithFormat(" 您报名的%s即将开始，是否进入比赛？ ",pMatchInfo->getMatchRemark().c_str());
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8(tip_str->getCString()),true));
}