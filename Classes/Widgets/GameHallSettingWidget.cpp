#include "GameHallSettingWidget.h"

#include "Common/ModalViewManager.h"
#include "Common/TNVisibleRect.h"
#include "SessionManager.h"
#include "GameHallHelpWidget.h"
#include "LoginScene.h"
#include "GameServiceClientManager.h"
#include "GameModifyPwdWidget.h"
#include "CommonFunction.h"
#include "GameHallAboutWidget.h"
#include "GameHallFeedBackWidget.h"
#include "SoundManager.h"
//ui tag
#define  ImagePopBgTag 18
#define  LabelMusicTag 33
#define  LabelEffectTag 35
#define  BtnModifyLoginTag 36
#define  BtnModifyBankTag  38
#define  ImageHeadIconBgTag 39
#define  TextNickTag        40
#define  BtnSwitchAccountTag 41
#define  BtnColseTag	 42
#define  BtnHelpTag		 47
#define  BtnPeopleBackTag 55
#define  BtnAboutTag      63

GameHallSettingWidget::GameHallSettingWidget()
	: m_bIsShowBackBtn(true)
{

}

GameHallSettingWidget::~GameHallSettingWidget()
{

}

GameHallSettingWidget* GameHallSettingWidget::create(bool isShowBackBtn)
{
	GameHallSettingWidget* ret = new GameHallSettingWidget();
	if (ret && ret->init())
	{
		ret->autorelease();
		ret->m_bIsShowBackBtn = isShowBackBtn;
		return ret;
	}
	else
	{
		delete ret;
		ret = NULL;
		return ret;
	}
}

bool GameHallSettingWidget::init()
{
	if ( false == TNWidget::init())
	{
		return false;
	}
	return true;
}

void GameHallSettingWidget::onEnter()
{
	//日志
	TNWidget::onEnter();
}

void GameHallSettingWidget::onExit()
{
	//日志
	TNWidget::onExit();
}

std::string GameHallSettingWidget::getWidgetJsonName()
{
	return "settingScece";
}

void GameHallSettingWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBgTag));

	OnlineUserModel* user = SessionManager::shareInstance()->getUser();
	ImageView* headBg = static_cast<ImageView*>(popBg->getChildByTag(ImageHeadIconBgTag));

	ImageView* icon =ImageView::create();
	icon->loadTexture(__String::createWithFormat("head/head%d.png",user->getFaceId())->getCString(),ui::Widget::TextureResType::LOCAL);
	
	float scale = headBg->getContentSize().width*10.0f/icon->getContentSize().width;
	icon->setScale(scale/10.f);
	icon->setPosition(Vec2(headBg->getContentSize().width/2,headBg->getContentSize().height/2));
	headBg->addChild(icon);

	Text* nick =  static_cast<Text*>(popBg->getChildByTag(TextNickTag));
	nick->setString(CommonFunction::GBKToUTF8(user->getNickname().c_str()));
	nick->setTextColor(ccc4(96,33,0,255));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnColseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onBack, this));

	m_MusicSwitch = static_cast<ImageView*>(popBg->getChildByTag(LabelMusicTag));
	m_MusicSwitch->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onSwitchMusic, this));
	if (SessionManager::shareInstance()->getMusicOn())
	{
		m_MusicSwitch->loadTexture("sound_onf.png",Widget::TextureResType::PLIST);
	}

	m_EffectSwith = static_cast<ImageView*>(popBg->getChildByTag(LabelEffectTag));
	m_EffectSwith->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onSwitchEffect, this));
	if (SessionManager::shareInstance()->getEffectOn())
	{
		m_EffectSwith->loadTexture("sound_onf.png",Widget::TextureResType::PLIST);
	}

	//游戏帮助
	Button* btn_help = static_cast<Button*>(popBg->getChildByTag(BtnHelpTag));
	btn_help->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onClickHelp, this));

	//意见反馈
	Button* btn_peopleBack = static_cast<Button*>(popBg->getChildByTag(BtnPeopleBackTag));
	btn_peopleBack->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onClickPeopleBack, this));

	//关于
	Button* btn_about = static_cast<Button*>(popBg->getChildByTag(BtnAboutTag));
	btn_about->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onAbout, this));

	//切换帐号
	Button* btn_chanageAccount = static_cast<Button*>(popBg->getChildByTag(BtnSwitchAccountTag));
	btn_chanageAccount->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onChangeAccount, this));

	Button* btn_modifyBankPwd = static_cast<Button*>(popBg->getChildByTag(BtnModifyBankTag));
	btn_modifyBankPwd->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onModifyPwd, this));

	Button* btn_modifyLoginPwd = static_cast<Button*>(popBg->getChildByTag(BtnModifyLoginTag));
	LoginUserModel* model = SessionManager::shareInstance()->getLoginModel();
	if (model->getAccountType() == accountType::accountMachineID)//游客不能修改登录密码
	{
		btn_modifyLoginPwd->setTouchEnabled(false);
		btn_modifyLoginPwd->setColor(ccc3(100,100,100));
	}
	else
	{
		btn_modifyLoginPwd->addTouchEventListener(CC_CALLBACK_2(GameHallSettingWidget::onModifyPwd, this));
	}

	//m_U8sdk.OnU8sdkInit();
	CU8sdkFunction::GetInstance().OnU8sdkInit();
}

void GameHallSettingWidget::onTouchBlank(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
	}
}

void GameHallSettingWidget::onClickHelp(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallHelpWidget::create());
	}
}

void GameHallSettingWidget::onClickPeopleBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallFeedBackWidget::create());
	}
}

void GameHallSettingWidget::onAbout(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		this->removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallAboutWidget::create());
	}
}

void GameHallSettingWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		SessionManager::shareInstance()->flush();
        this->removeFromParent();
	}
}

void GameHallSettingWidget::onModifyPwd(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::BEGAN) {
		playScaleAnimation(true,pSender);
	} else {
		playScaleAnimation(false,pSender);
	}

	if (ui::Widget::TouchEventType::ENDED == eventtype) {
        removeFromParent();
		Button* pButton = (Button*)pSender;
		PwdType type ;
		if (pButton->getTag() == BtnModifyBankTag)
		{
			type = PwdType_Bank;
		}
		else
		{
			type = PwdType_Login;
		}
		ModalViewManager::sharedInstance()->showWidget(GameModifyPwdWidget::create(type));
	}
}

void GameHallSettingWidget::onSwitchEffect(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype) {
		if (SessionManager::shareInstance()->getEffectOn())
		{
			SessionManager::shareInstance()->setEffectOn(0);
			m_EffectSwith->loadTexture("sound_off.png",Widget::TextureResType::PLIST);
		}
		else
		{
			SessionManager::shareInstance()->setEffectOn(1);
			m_EffectSwith->loadTexture("sound_onf.png",Widget::TextureResType::PLIST);
		}
	}
}

void GameHallSettingWidget::onSwitchMusic(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype) {
		if (SessionManager::shareInstance()->getMusicOn())
		{
			SessionManager::shareInstance()->setMusicOn(0);
			m_MusicSwitch->loadTexture("sound_off.png",Widget::TextureResType::PLIST);
			SoundManager::pauseMusicBackground();
		}
		else
		{
			SessionManager::shareInstance()->setMusicOn(1);
			m_MusicSwitch->loadTexture("sound_onf.png",Widget::TextureResType::PLIST);
			SoundManager::resumeMusicBackground();
		}
	}
}

void GameHallSettingWidget::onChangeAccount(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (eventtype == ui::Widget::TouchEventType::BEGAN) {
		playScaleAnimation(true,pSender);
	} else {
		playScaleAnimation(false,pSender);
	}

	if (eventtype == ui::Widget::TouchEventType::ENDED)
	{
		std::string name = GameServiceClientManager::sharedInstance()->getCurrentServiceClientName();
		GameServiceClientManager::sharedInstance()->removeServiceClient(name.c_str());
		SessionManager::shareInstance()->clearMsgArray();

		//断开大厅SORKET
		GameServiceClient* c = GameServiceClientManager::sharedInstance()->serviceClientForName(taskClient);
		c->closeSoket();

		LoginUserModel *userModel = SessionManager::shareInstance()->getLoginModel();
		//上次是QQ 登录，切换帐号后清空缓存的帐号的信息
		if (userModel->getAccountType() == accountType::accountQQ)
		{
			userModel->setAccount("");
			userModel->setPassword("");
		}
		removeFromParent();
		Director::sharedDirector()->replaceScene(LoginScene::create());
		//m_U8sdk.OnSwitchLogin();
		CU8sdkFunction::GetInstance().OnSwitchLogin();
	}
}

void GameHallSettingWidget::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}

