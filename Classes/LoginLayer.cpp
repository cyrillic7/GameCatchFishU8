
#include "LoginLayer.h"
#include "Widgets/LoginButtonLogo.h"
#include "CommonFunction.h"
#include "Widgets/GameHallLoginWidget.h"
#include "Common/ModalViewManager.h"
#include "Message.h"
#include "SessionManager.h"

#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CocoStudio.h"

using namespace cocostudio;

bool LoginLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto size = Director::getInstance()->getVisibleSize();

	//±³¾°
	auto bgSprite = Sprite::create("login_bg.jpg");
	bgSprite->setPosition(Vec2(size.width / 2, size.height / 2));
	bgSprite->setScale(1.2f);
	this->addChild(bgSprite);
	
	//title 
	//auto logoSp = Sprite::create("logo.png");
	//logoSp->setPosition(Vec2(size.width / 2, size.height - logoSp->getContentSize().height*1.2f/2));
//	logoSp->setScale(1.2f);
	//this->addChild(logoSp);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("Splash/AnimationTitleLogon.ExportJson");  
	Armature* armature = Armature::create("AnimationTitleLogon");
	armature->setAnchorPoint(ccp(0.5, 0.5));
	armature->setPosition(Vec2(size.width / 2, size.height - 60));
	armature->getAnimation()->play("Animation1",1.5f,1);
	this->addChild(armature);


	//Èý¸ö²Ëµ¥°´Å¥
	//ÕË»§µÇÂ¼

	Button* btn_AccountLogin = Button::create();
	btn_AccountLogin->loadTextureNormal("button_bg_yellow.png",ui::Widget::TextureResType::LOCAL);
	btn_AccountLogin->loadTexturePressed("button_bg_yellow.png",ui::Widget::TextureResType::LOCAL);
	btn_AccountLogin->setPosition(Vec2(size.width / 6 + 25, btn_AccountLogin->getContentSize().height / 3 * 2));
	addChild(btn_AccountLogin);
	btn_AccountLogin->addTouchEventListener(CC_CALLBACK_2(LoginLayer::onClickAccountLogin, this));

	int menuItemHeight =  btn_AccountLogin->getContentSize().height;
	auto accountLoginText = LoginButtonLogo::create(LoginButtonLogo::LOGIN_BUTTON_TYPE_ACCOUNT);
	accountLoginText->setPosition(Vec2(btn_AccountLogin->getContentSize().width/2, menuItemHeight / 2));

	//¿ìËÙµÇÂ¼
	Button* btn_FastLogin = Button::create();
	btn_FastLogin->loadTextureNormal("button_bg_blue.png",ui::Widget::TextureResType::LOCAL);
	btn_FastLogin->loadTexturePressed("button_bg_blue.png",ui::Widget::TextureResType::LOCAL);
	btn_FastLogin->setPosition(Vec2(size.width / 2, menuItemHeight / 3 * 2));
	addChild(btn_FastLogin);
	btn_FastLogin->addTouchEventListener(CC_CALLBACK_2(LoginLayer::onClickFastLogin, this));

	auto fastLoginText = LoginButtonLogo::create(LoginButtonLogo::LOGIN_BUTTON_TYPE_FAST);
	fastLoginText->setPosition(Vec2(btn_FastLogin->getContentSize().width / 2, menuItemHeight / 2));

	//QQµÇÂ¼
	Button* btn_QQLogin = Button::create();
	btn_QQLogin->loadTextureNormal("button_bg_yellow.png",ui::Widget::TextureResType::LOCAL);
	btn_QQLogin->loadTexturePressed("button_bg_yellow.png",ui::Widget::TextureResType::LOCAL);
	btn_QQLogin->setPosition(Vec2(size.width / 6 * 5 - 25, menuItemHeight / 3 * 2));
	addChild(btn_QQLogin);
	btn_QQLogin->addTouchEventListener(CC_CALLBACK_2(LoginLayer::onClickQQLogin, this));

	auto QQLoginText = LoginButtonLogo::create(LoginButtonLogo::LOGIN_BUTTON_TYPE_QQ);
	QQLoginText->setPosition(Vec2(btn_QQLogin->getContentSize().width/2, menuItemHeight / 2));

	btn_AccountLogin->addChild(accountLoginText);
	btn_FastLogin->addChild(fastLoginText);
	btn_QQLogin->addChild(QQLoginText);

	return true;
}

void LoginLayer::runSpriteFrameAnimation(Sprite* sprite, FishType type)
{
	int frameCount = 0;
	char str[16] = { 0 };
	auto sCache = SpriteFrameCache::getInstance();
	auto animation = Animation::create();
	
	switch (type)
	{
	case FISH_TYPE_1:
	case FISH_TYPE_2:
	case FISH_TYPE_3:
		frameCount = 4;
		animation->setDelayPerUnit(0.3f / 4);
		break;

	case FISH_TYPE_4:
		frameCount = 2;
		animation->setDelayPerUnit(0.1f / 2);
		break;
	}

	for (int i = 0; i < frameCount; i++) {
		memset(str, 0, 16);
		sprintf(str, "fish_%d_%d.png", type, i + 1);
		animation->addSpriteFrame(sCache->getSpriteFrameByName(str));
	}
	
	auto animate = Animate::create(animation);
	sprite->runAction(RepeatForever::create(Sequence::create(animate, animate->reverse(), NULL)));
}

void LoginLayer::onClickAccountLogin(Ref* pRef,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pRef);
	}
	else
	{
		playScaleAnimation(false,pRef);
	}

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallLoginWidget::create());
	}
}

void LoginLayer::onClickFastLogin(Ref* pRef,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pRef);
	}
	else
	{
		playScaleAnimation(false,pRef);
	}
	
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		LoginUserModel*  model = SessionManager::shareInstance()->getLoginModel();

		/*	if (model->getQucikAccount().length() > 0 && model->getQucikPwd().length() > 0)
		{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(qucikLoginByAccount);
		}
		else*/
		{
			 Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(RequestFastLoginNotification);
		}
	}
}

void LoginLayer::onClickQQLogin(Ref* pRef,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pRef);
	}
	else
	{
		playScaleAnimation(false,pRef);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		CommonFunction::callJni("http://www.qicainiu.net/QQLogin.aspx/",WEB_QQ_LOGIN);
	}
}

void LoginLayer::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.9f : 1.0f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}