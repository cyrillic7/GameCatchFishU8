#include "GameMainScene.h"
#include "GameScene.h"
#include "Common/CommonLoadingWidget.h"
#include "CommonFunction.h"
#include "GameGlobalSetting.h"
#include "Common/ModalViewManager.h"
#include "Widgets/GameHallSettingWidget.h"
#include "Widgets/GameCreateBankPwd.h"
#include "Widgets/GameEnterBank.h"
#include "Widgets/GameHallShopWidget.h"
#include "Widgets/GameHallMsgWidget.h"
#include "Widgets/GameHallPlayerInfoWidget.h"
#include "Widgets/BackgroundGoUpParticle.h"
#include "Widgets/GameHallPaimaiWidget.h"
#include "Widgets/GameHallVipPower.h"
#include "Widgets/GameHallTaskWidget.h"
#include "Widgets/GameHallAssignWidget.h"
#include "Widgets/GameShareWidget.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"
#include "Widgets/GameHallChargeWidget.h"
#include "Widgets/GameHallFirstCharge.h"
#include "Widgets/GameHallBagWidget.h"
#include "widgets/GameHallMoreGameWidget.h"
#include "GameSmallScene.h"
#include "GameMatchScene.h"
#include "SoundManager.h"
#include "Common/pystring.h"

#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;
#define  IMAGE_BG_TAG 4
#define  IMAGE_HEADICON_TAG 5

#define  BTN_MESSAGE_TAG 7
#define  BTN_SETTING_TAG 8
#define  BTN_MOREGAME_TAG 9
#define  BTN_FIRSTCHARGE_TAG 10
#define  BTN_BAG_TAG		 4187

#define  BTN_LYD_TAG 11
#define  BTN_NLD_TAG 12
#define  BTN_SMD_TAG 13


#define  ID_SCENE_SHOP		1
#define  ID_SCENE_ACTIVITY  2
#define  ID_SCENE_TASK		3
#define  ID_SCENE_BANK		4
#define  ID_SCENE_ASSIGN	5
#define  ID_SCENE_PAIMAI    6
#define  ID_SCENE_SHARE     7

#define  jumpFishInterval 10.0f
#define  dealVipEffectInterval 5.0f

#define imageFaceBg  2435
#define LabelNickTag 2437
#define LabelScoreTag 2438
#define BtnChargeTag  2439

#define  ImageVipTag 10446
#define  LabelAtlsTag  10448
#define  ImageVipFishTag 10449

#define BtnFishMatchTag 13803

const Position jumpFishPos[10] = {
	{ 430.f,kScreenHeight -203.5f }, { 555, kScreenHeight - 234.5f }, { 923.5f, kScreenHeight - 231.5f},
	{ kScreenWidth - 280, 243.5f },{kScreenWidth-187.5f,kScreenHeight-555.f},{1268.f,kScreenHeight - 624.5f},
	{kScreenWidth-204.5f,139.f},{kScreenWidth-474.f,83.f},{315.5f,kScreenHeight - 613.5f},
	{653.5f,kScreenHeight - 414.f}
};

MainScene::MainScene()
{
	mCurInterval = 0.0f;
	mIsLogin = false;
	mScore = nullptr;
	mFishVip =nullptr;

}

MainScene::~MainScene()
{
}

void MainScene::onEnter()
{
	GameBaseScene::onEnter();
	SoundManager::playMusicBackground("hall_bgmusic.m4a",true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GetRoomListMsg, CC_CALLBACK_1(MainScene::GetRoomListRsp, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateUserInfoMsg, CC_CALLBACK_1(MainScene::refreshUserData, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateUserInfoFromAuciontMsg, CC_CALLBACK_1(MainScene::refreshUserDataForAuction, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateRedPointMsg, CC_CALLBACK_1(MainScene::updateRedPoint, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateHeadMsg, CC_CALLBACK_1(MainScene::updateHeadInfo, this)), this);
	

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(hasFirstRechargeMsg, CC_CALLBACK_1(MainScene::hideFirstChargeMsg, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(updateVipLevelMsg, CC_CALLBACK_1(MainScene::updateVipLevel, this)), this);
	//起定时器
	Director::getInstance()->getScheduler()->schedule(schedule_selector(MainScene::update), this,1.0f/60, false);
}


void MainScene::onExit()
{
	// 清除消息接听
	_eventDispatcher->removeEventListenersForTarget(this);
	GameBaseScene::onExit();
}

char* MainScene::getWidgetJsonName()
{
	return "mainScene";
}

MainScene* MainScene::create()
{
    // 'scene' is an autorelease object
    MainScene* myScene = new MainScene();
    
	if(myScene && myScene->init())
	{
		myScene->autorelease();
		return myScene;
	}
    // return the scene
	CC_SAFE_DELETE(myScene);
    return nullptr;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !GameBaseScene::init() )
    {
        return false;
    }
	
	loadUI();

	/*Sprite* test_fish = Sprite::create();
	test_fish->setPosition(Vec2(kScreenWidth/2,kScreenHeight/2));
	addChild(test_fish);
	__String* pathStr = String::createWithFormat("GameResources/jcpy/fish/%s.png",__String::createWithFormat("%s_walking","fish20")->getCString());

	Sprite* fishImg = Sprite::create(pathStr->getCString());
	int row = fishImg->getContentSize().width/256;
	int col = fishImg->getContentSize().height/256;

	Vector<SpriteFrame*> frames;
	SpriteFrame* frame = nullptr;
	for(int i=0;i<col;i++)
	{
	for(int j=0;j<row;j++)
	{
	if(i*row+j < 3 )
	{

	frame = SpriteFrame::create(pathStr->getCString(),CCRectMake(j*256,i*256,256,256));
	if(i==0 && j==0)
	{
	test_fish->setSpriteFrame(frame);
	}
	if (NULL != frame) {
	frames.pushBack(frame);
	}
	}
	}
	}
	Animation* pAction = Animation::createWithSpriteFrames(frames,0.2f);
	Animate* pAmate = Animate::create(pAction);


	test_fish->runAction(RepeatForever::create(pAmate));*/
    return true;
}

void MainScene::loadUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.
	// add a "close" icon to exit the progress. it's an autorelease object
	mMainWidget = CommonFunction::getWidgetFromCocostudioFile(getWidgetJsonName());
	mMainWidget->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	mMainWidget->setAnchorPoint(Vec2(0.5,0.5));
	addChild(mMainWidget);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("Splash/AnimationTitleLogon.ExportJson");  
	Armature* armature = Armature::create("AnimationTitleLogon");
	armature->setAnchorPoint(ccp(0.5, 0.5));
	armature->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 60));
	armature->getAnimation()->play("Animation1",1.5f,1);
	this->addChild(armature);

	ImageView* bg = static_cast<ImageView*>(mMainWidget->getChildByTag(IMAGE_BG_TAG));

	OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
	//face
	face = static_cast<ImageView*>(bg->getChildByTag(imageFaceBg));
	face->loadTexture(__String::createWithFormat("head/head%d.png",pUser->getFaceId())->getCString(),ui::Widget::TextureResType::LOCAL);
	face->setScale(0.45f);
	//头像
	ImageView* headIcon = static_cast<ImageView*>(bg->getChildByTag(IMAGE_HEADICON_TAG));
	face->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickHead, this));

	Text* nick = static_cast<Text*>(headIcon->getChildByTag(LabelNickTag));
	std::string trun_str = CommonFunction::truncationString(CommonFunction::GBKToUTF8(pUser->getNickname().c_str()),15);
	//nick->setString(CommonFunction::GBKToUTF8(pUser->getNickname().c_str()));
	nick->setString(trun_str);
	nick->setTextColor(ccc4(255,255,0,255));
	nick->setFontSize(25);

	mScore = static_cast<Text*>(headIcon->getChildByTag(LabelScoreTag));
	//mScore->setString(numberToString(pUser->getUserScore()));
	mScore->setString(__String::createWithFormat("%lld",pUser->getUserScore())->getCString());
	//切换到充值
	ImageView* toCharge = static_cast<ImageView*>(headIcon->getChildByTag(BtnChargeTag));
	toCharge->setTouchSize(Size(100,100));
	toCharge->addTouchEventListener(CC_CALLBACK_2(MainScene::gotoCharge, this));

	
	ImageView* btn_lyd = static_cast<ImageView*>(bg->getChildByTag(BTN_LYD_TAG));
	btn_lyd->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickFishRoom1, this));
	btn_lyd->setRotation(-0.3);

	RotateTo* rb = RotateTo::create(0.4f,0.6f);
	RotateTo* rb2 = RotateTo::create(0.4f,-0.6f);
	btn_lyd->runAction(RepeatForever::create(Sequence::createWithTwoActions(rb,rb2)));

	ImageView* btn_nyd = static_cast<ImageView*>(bg->getChildByTag(BTN_NLD_TAG));
	btn_nyd->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickFishRoom2, this));
	btn_nyd->setRotation(-0.3);
	RotateTo* rb3 = RotateTo::create(0.4f,0.6f);
	RotateTo* rb4 = RotateTo::create(0.4f,-0.6f);
	btn_nyd->runAction(RepeatForever::create(Sequence::createWithTwoActions(rb3,rb4)));


	ImageView* btn_smd = static_cast<ImageView*>(bg->getChildByTag(BTN_SMD_TAG));
	btn_smd->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickFishRoom3, this));
	btn_smd->setRotation(-0.3);
	RotateTo* rb5 = RotateTo::create(0.4f,0.6f);
	RotateTo* rb6 = RotateTo::create(0.4f,-0.6f);
	btn_smd->runAction(RepeatForever::create(Sequence::createWithTwoActions(rb5,rb6)));
	//消息按钮
	mBtnMsg = static_cast<Button*>(bg->getChildByTag(BTN_MESSAGE_TAG));
	mBtnMsg->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickMsg, this));

	//比赛岛屿
	ImageView* btn_match = static_cast<ImageView*>(bg->getChildByTag(BtnFishMatchTag));
	btn_match->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickMatch, this));

	mMsgRedPoint = ImageView::create();
	mMsgRedPoint->loadTexture("image_redpt.png",ui::Widget::TextureResType::LOCAL);
	mMsgRedPoint->setPosition(Vec2(70,85-(mBtnMsg->getContentSize().height -30)/2));
	mMsgRedPoint->setVisible(SessionManager::shareInstance()->getNewMsgTag());
	mBtnMsg->addChild(mMsgRedPoint,1);

	//设置按钮
	Button* btnSetting = static_cast<Button*>(bg->getChildByTag(BTN_SETTING_TAG));
	btnSetting->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickSetting, this));
	//更多游戏按钮
	Button* btnMoregame = static_cast<Button*>(bg->getChildByTag(BTN_MOREGAME_TAG));
	btnMoregame->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickMoreGame, this));
	//背包
	Button * btnBag = static_cast<Button*>(bg->getChildByTag(BTN_BAG_TAG));
	btnBag->addTouchEventListener(CC_CALLBACK_2(MainScene::onclickBag, this));
	//首次充值
	Button* btn_firstCharge = static_cast<Button*>(bg->getChildByTag(BTN_FIRSTCHARGE_TAG));
	if (pUser->isFirstCharge())//已经进行过首冲
	{
		btn_firstCharge->setVisible(false);
	}
	btn_firstCharge->addTouchEventListener(CC_CALLBACK_2(MainScene::onClickFirstCharge, this));
	btn_firstCharge->loadTextureNormal("default.png");
	btn_firstCharge->loadTexturePressed("default.png");
	btn_firstCharge->setOpacity(255);
	std::vector<std::string> imageNames;
	std::string path  = "";
	int i = 0;
	for ( i = 0 ; i < 12;i++ )
	{
		if(i < 6)
			path= "firstcharge" + numberToString(i+1) + ".png";
		else
			path= "firstcharge" + numberToString(7-i%6) + ".png";
		imageNames.push_back(path);
	}


	Animate* pAction = CommonFunction::createFrameAnimate(imageNames,0.2f);

	Sprite* firstSp = Sprite::create();
	firstSp->setPosition(Vec2( btn_firstCharge->getContentSize().width/2,btn_firstCharge->getContentSize().height/2));
	btn_firstCharge->addChild(firstSp);
	firstSp->runAction(RepeatForever::create(pAction));
	
	//new vip rukou
	ImageView* image_vip = static_cast<ImageView*>(bg->getChildByTag(ImageVipTag));
	image_vip->addTouchEventListener(CC_CALLBACK_2(MainScene::onGotoVip, this));

	label_vip =  static_cast<ui::TextAtlas*>(image_vip->getChildByTag(LabelAtlsTag));
	OnlineUserModel* model = SessionManager::shareInstance()->getUser();
	label_vip->setString(__String::createWithFormat("%d",model->getVipLevel())->getCString());

	mFishVip = static_cast<ImageView*>(image_vip->getChildByTag(ImageVipFishTag));

	mVipEffectSp = Sprite::create();
	mVipEffectSp->setPosition(Vec2( -55,mFishVip->getContentSize().height));

	mFishVip->addChild(mVipEffectSp);

	mFishVip->setRotation(-0.3f);
	RotateTo* fish_rb = RotateTo::create(0.2f,0.9f);
	RotateTo* fish_rb2 = RotateTo::create(0.2f,-0.9f);
	mFishVip->runAction(RepeatForever::create(Sequence::createWithTwoActions(fish_rb,fish_rb2)));

	dealVipEffect();
	//初始化菜单栏
	initMenuBar();

	createFish();

	
	scheduleUpdate();
}

void MainScene::dealVipEffect()
{
	std::vector<std::string> vipImages;
	std::string path;
	for (int i = 0 ; i < 7;i++ )
	{

		path= "image_hall_vip" + numberToString(i+1) + ".png";

		vipImages.push_back(path);
	}

	Vector<FiniteTimeAction*> actions;
	Animate* pAction = CommonFunction::createFrameAnimate(vipImages,0.14f);
	actions.pushBack(pAction);
	CallFuncN* pCall = CallFuncN::create(this,callfuncN_selector(MainScene::vipDealOver));
	actions.pushBack(pCall);
	DelayTime* d = DelayTime::create(1.0f);
	actions.pushBack(pAction);
	CallFuncN* pCall2 = CallFuncN::create(this,callfuncN_selector(MainScene::reDealVipEffect));
	actions.pushBack(pCall2);
	mVipEffectSp->runAction(Sequence::create(actions));
}

void MainScene::vipDealOver(Node* pSender)
{
	mVipEffectSp->setVisible(false);
}

void MainScene::reDealVipEffect(Node* pSender)
{
	mVipEffectSp->setVisible(true);
	dealVipEffect();

}
//场景加载完成后，检查是否弹出签到
void MainScene::onEnterTransitionDidFinish()
{
	checkAssign();
}

void MainScene::update(float dt)
{

	mCurInterval +=dt;
	if (mCurInterval >= jumpFishInterval  )
	{
		createFish();
		mCurInterval = 0;
	}
	
	
}

void MainScene::render(Renderer* renderer)
{
	Scene::render(renderer);
}
void MainScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainScene::GetRoomListRsp(EventCustom* event)
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}


void MainScene::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		CommonLoadingWidget* loading = CommonLoadingWidget::create(visibleSize);
		loading->setTips("正在登录,请稍候...");
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
		addChild(loading, 5);
	}
}


__Array* MainScene::getResourceNeedLoad()
{
	__Array* a  = GameBaseScene::getResourceNeedLoad();
	return a;
}

//千炮房
void MainScene::onClickFishRoom1(Ref *pSender, ui::Widget::TouchEventType eventType)
{

	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->pushScene(GameSmallScene::create(RoomLevel_1));
	}
}

//百炮房
void MainScene::onClickFishRoom2(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->pushScene(GameSmallScene::create(RoomLevel_0));
	}
}

//万炮房
void MainScene::onClickFishRoom3(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->pushScene(GameSmallScene::create(RoomLevel_2));
	}
}

void MainScene::onClickMsg(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallMsgWidget::create());
	}
}

void MainScene::gotoCharge(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		//ModalViewManager::sharedInstance()->showWidget(GameHallVipPowerWidget::create());
		ModalViewManager::sharedInstance()->showWidget(GameHallChargeWidget::create(tab_gold));
	}
}

void MainScene::onGotoVip(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation2(true,pSender);
	}
	else
	{
		playScaleAnimation2(false,pSender);
	}

	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		//ModalViewManager::sharedInstance()->showWidget(GameHallVipPowerWidget::create());
		ModalViewManager::sharedInstance()->showWidget(GameHallVipPowerWidget::create());
	}
}

void MainScene::onClickSetting(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallSettingWidget::create());
	}
}

void MainScene::onClickMoreGame(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallMoreGameWidget::create());
	}
}

void MainScene::onClickHead(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallPlayerInfoWidget::create());
	}
}


void MainScene::onclickBag(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallBagWidget::create());
	}
}



void MainScene::initMenuBar()
{
	menuLayer = CCMenuLayer::create();
	menuLayer->setPosition(Vec2(30,30));
	menuLayer->AddToggleItem("menu.png","menu_on.png");
	mMainWidget->addChild(menuLayer);
	
	
	menuLayer->RegisterMenuHandler(this,CC_CALLBACK_2(MainScene::menuCallback, this));
	

	menuLayer->AppendItem("icon_assign.png","font_assign.png",CCMenuLayer::vertype, ID_SCENE_ASSIGN);
	menuLayer->AppendItem("icon_tast.png","font_task.png",CCMenuLayer::vertype, ID_SCENE_TASK);
	menuLayer->AppendItem("icon_activity.png","font_activity.png",CCMenuLayer::vertype, ID_SCENE_ACTIVITY);
	
	menuLayer->AppendItem("icon_shop.png","font_shop.png",CCMenuLayer::hortype, ID_SCENE_SHOP);
	menuLayer->AppendItem("icon_paimai.png","font_paimai.png",CCMenuLayer::hortype, ID_SCENE_PAIMAI);
	menuLayer->AppendItem("icon_bank.png","font_bank2.png",CCMenuLayer::hortype, ID_SCENE_BANK);
	menuLayer->AppendItem("icon_share.png","font_share.png",CCMenuLayer::hortype, ID_SCENE_SHARE);
	
}

void MainScene::onClickMatch(Ref*pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->replaceScene(GameMatchScene::create());
	}
}

void MainScene::menuCallback(Ref*pSender,ui::Widget::TouchEventType eventType)
{
	Node* pNode = (Node*)pSender;
	if(eventType == ui::Widget::TouchEventType::BEGAN)
	{
		CCScaleTo *scaleTo = CCScaleTo::create(0.2f, 1.1f);
		pNode->runAction(scaleTo);
	}
	else
	{
		CCScaleTo *scaleTo = CCScaleTo::create(0.2f, 1.2f);
		pNode->runAction(scaleTo);
	}

	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		MenuItemSprite* item = (MenuItemSprite*)pSender;
		int scene_id = (int)item->getUserData();
		switch (scene_id)
		{
		case ID_SCENE_ACTIVITY :
			{
				LoginUserModel* loginModel = SessionManager::shareInstance()->getLoginModel();
				__String* ulr = __String::createWithFormat("http://m.qicainiu.com/Home/Index/?account=%s&pwd=%s&plat=1",loginModel->getAccount().c_str(),loginModel->getPassword().c_str());
				CommonFunction::callJni(ulr->getCString(),WEB_ACTIVITY);
			}
			break;
		case ID_SCENE_ASSIGN:
			ModalViewManager::sharedInstance()->showWidget(GameHallAssginWidget::create());
			break;
		case ID_SCENE_BANK:
			if (SessionManager::shareInstance()->isResetBankPwd())
			{
				ModalViewManager::sharedInstance()->showWidget(GameEnterBank::create());
			}
			else
			{
				ModalViewManager::sharedInstance()->showWidget(GameBankCreatePwdWidget::create());
			}
			break;
		case ID_SCENE_SHOP:
			ModalViewManager::sharedInstance()->showWidget(GameHallShopWidget::create());
			break;
		case ID_SCENE_TASK:
			ModalViewManager::sharedInstance()->showWidget(GameHallTaskWidget::create());
			break;
		case ID_SCENE_PAIMAI:
				ModalViewManager::sharedInstance()->showWidget(GameHallPaimaiWidget::create());
			break;
		case ID_SCENE_SHARE:
			{
				//CommonFunction::callWx();
				ModalViewManager::sharedInstance()->showWidget(GameShareWidget::create());
			}
			break;
		default:
			break;
		}
	}
}

void MainScene::createFish()
{
	int pos_index = rand()%10;
	float angle = fgRangeRand(0,360);
	int fish_id = rand()%3 + 1;

	Sprite* water = Sprite::create();
	Vector<SpriteFrame*> waterFrames;
	for (int i = 0; i < 5; i++)
	{
		SpriteFrame* frame = SpriteFrame::create(__String::createWithFormat("water%d.png",i+1)->getCString(),CCRectMake(0,0,55,25));
		waterFrames.pushBack(frame);
		if (i == 0)
		{
			water->setDisplayFrame(frame);
		}
	}
	Animation* anima = Animation::createWithSpriteFrames(waterFrames,0.14f);
	Animate* animate =  Animate::create(anima);

	RemoveSelf* rs = RemoveSelf::create();
	water->runAction(Sequence::createWithTwoActions(animate,rs));

	Sprite* jumpFish = Sprite::create();
	//float scale = fgRangeRand(1.5f,3.0f);
	//jumpFish->setScale(scale);
	Vector<SpriteFrame*> frames;
	for (int i = 0; i < 7; i++)
	{
		SpriteFrame* frame = SpriteFrame::create(__String::createWithFormat("fish%d_%d.png",fish_id,i+1)->getCString(),CCRectMake(0,0,84,54));
		frames.pushBack(frame);
		if (i == 0)
		{
			jumpFish->setDisplayFrame(frame);
		}
	}
	Vector<FiniteTimeAction*> actions;
	
	anima = Animation::createWithSpriteFrames(frames,0.14f);
	
	animate =  Animate::create(anima);
	actions.pushBack(animate);

	CallFuncN* callback = CallFuncN::create(this,callfuncN_selector(MainScene::fishIntoWater));
	actions.pushBack(callback);

	jumpFish->runAction(Sequence::create(actions));

	jumpFish->setPosition(Vec2(jumpFishPos[pos_index].x,jumpFishPos[pos_index].y));
	if(rand()%2 == 0)
		jumpFish->setFlippedX(true);
	water->setScale(jumpFish->getScale());
	if(jumpFish->isFlippedX())
		water->setPosition(Vec2(jumpFish->getPositionX()-jumpFish->getScale()*jumpFish->getContentSize().width/2,jumpFish->getPositionY()-jumpFish->getScale()*jumpFish->getContentSize().height/2));
	else
		water->setPosition(Vec2(jumpFish->getPositionX()+jumpFish->getScale()*jumpFish->getContentSize().width/2,jumpFish->getPositionY()-jumpFish->getScale()*jumpFish->getContentSize().height/2));

	addChild(water);
	addChild(jumpFish);

}

void MainScene::fishIntoWater(Node* pSender)
{
	Sprite* fish = (Sprite*)pSender;
	Vec2 pos = fish->getPosition();

	Sprite* water = Sprite::create();
	mMainWidget->addChild(water);

	Vector<SpriteFrame*> waterFrames;
	for (int i = 0; i < 5; i++)
	{
		SpriteFrame* frame = SpriteFrame::create(__String::createWithFormat("water%d.png",i+1)->getCString(),CCRectMake(0,0,55,25));
		waterFrames.pushBack(frame);
		if (i == 0)
		{
			water->setDisplayFrame(frame);
		}
	}
	Animation* anima = Animation::createWithSpriteFrames(waterFrames,0.14f);
	Animate* animate =  Animate::create(anima);

	RemoveSelf* rs = RemoveSelf::create();
	water->setScale(fish->getScale());
	if (fish->isFlippedX())
		water->setPosition(Vec2(pos.x + fish->getScale()*fish->getContentSize().width/2,pos.y-fish->getScale()*fish->getContentSize().height/2));
	else
		water->setPosition(Vec2(pos.x - fish->getScale()*fish->getContentSize().width/2,pos.y-fish->getScale()*fish->getContentSize().height/2));
	water->runAction(Sequence::createWithTwoActions(animate,rs));

	pSender->removeFromParent();
}

//取随机浮点数
float MainScene::fgRangeRand( float min, float max )  
{  
	float rnd = ((float)rand() / (float)RAND_MAX);  
	return rnd * (max - min) + min; 
}


void MainScene::onClickFirstCharge(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::BEGAN) {
		playScaleAnimation(true,pSender);
	} else {
		playScaleAnimation(false,pSender);
	}

	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallFirstChargeWidget::create());
	}

}

void MainScene::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}


void MainScene::playScaleAnimation2(bool less, Ref* pSender) {
	float scale = less ? 1.1f : 1.2f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}


void MainScene::refreshUserData(EventCustom* evt)
{
	//CMD_GP_UserInsureSuccess* info = (CMD_GP_UserInsureSuccess*)evt->getUserData();
	//SessionManager::shareInstance()->getUser()->setUserScore(info->lUserScore);
	OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
	mScore->setString(numberToString(pUser->getUserScore()));
}

void MainScene::updateHeadInfo(EventCustom* evt)
{
	OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
	face->loadTexture(__String::createWithFormat("head/head%d.png",pUser->getFaceId())->getCString(),ui::Widget::TextureResType::LOCAL);
}

void MainScene::refreshUserDataForAuction(EventCustom* evt)
{
	CMD_GP_Buy_AuctionLog* info = (CMD_GP_Buy_AuctionLog*)evt->getUserData();
	SessionManager::shareInstance()->getUser()->setUserScore(info->lGold);
	mScore->setString(numberToString(info->lGold));
}

void MainScene::checkAssign()
{
	if(SessionManager::shareInstance()->isShowAssign())
		ModalViewManager::sharedInstance()->showWidget(GameHallAssginWidget::create());
}

//刷新所有区域的红点
void MainScene::updateRedPoint(EventCustom* evt)
{
	menuLayer->showRedPoint(SessionManager::shareInstance()->getNewTaskTag(),true);
	menuLayer->showRedPoint(SessionManager::shareInstance()->getNewAuctionTag(),false);
	mMsgRedPoint->setVisible(SessionManager::shareInstance()->getNewMsgTag());
}

void MainScene::hideFirstChargeMsg(EventCustom* evt)
{
	ImageView* bg = static_cast<ImageView*>(mMainWidget->getChildByTag(IMAGE_BG_TAG));
	Button* btn_firstCharge = static_cast<Button*>(bg->getChildByTag(BTN_FIRSTCHARGE_TAG));
	btn_firstCharge->setVisible(false);
}

void MainScene::updateVipLevel(EventCustom* evt)
{
	OnlineUserModel* model = SessionManager::shareInstance()->getUser();
	label_vip->setString(__String::createWithFormat("%d",model->getVipLevel())->getCString());
}

