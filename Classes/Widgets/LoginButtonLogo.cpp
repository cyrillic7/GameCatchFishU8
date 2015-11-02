
#include "LoginButtonLogo.h"


static const char ACCOUNT_LOGIN_TEXT[] = { 0xE5, 0xB8, 0x90, 0xE5, 0x8F, 0xB7, 0xE7, 0x99, 0xBB, 0xE5, 0xBD, 0x95, 0x00 };	//ÕÊºÅµÇÂ¼
static const char FAST_LOGIN_TEXT[] = { 0xE5, 0xBF, 0xAB, 0xE9, 0x80, 0x9F, 0xE7, 0x99, 0xBB, 0xE5, 0xBD, 0x95, 0x00 };		//¿ìËÙµÇÂ¼
static const char QQ_LOGIN_TEXT[] = { 0x51, 0x51, 0xE7, 0x99, 0xBB, 0xE5, 0xBD, 0x95, 0x00 };								//QQµÇÂ¼


#define LOGO_TEXT_SIZE		30


LoginButtonLogo* LoginButtonLogo::create(LoginButtonType type)
{
	auto logo = new LoginButtonLogo();
	if (logo && logo->init(type)) {
		logo->autorelease();
		return logo;
	} else {
		delete logo;
		logo = NULL;
		return NULL;
	}
}

bool LoginButtonLogo::init(LoginButtonType type)
{
	if (!Node::init()) {
		return false;
	}

	Sprite* icon;
	Label* label;

	switch (type)
	{
	case LOGIN_BUTTON_TYPE_ACCOUNT:
		icon = Sprite::create("icon_account.png");
		label = Label::createWithSystemFont(String::create(ACCOUNT_LOGIN_TEXT)->getCString(), "", LOGO_TEXT_SIZE);
		break;

	case LOGIN_BUTTON_TYPE_FAST:
		icon = Sprite::create("icon_j.png");
		label = Label::createWithSystemFont(String::create(FAST_LOGIN_TEXT)->getCString(), "", LOGO_TEXT_SIZE);
		break;

	case LOGIN_BUTTON_TYPE_QQ:
		icon = Sprite::create("icon_qq.png");
		label = Label::createWithSystemFont(String::create(QQ_LOGIN_TEXT)->getCString(), "", LOGO_TEXT_SIZE);
		break;
	}

	float iconWidth = icon->getContentSize().width;
	float iconHeight = icon->getContentSize().height;
	float labelWidth = label->getContentSize().width;
	float labelHeight = label->getContentSize().height;

	this->setContentSize(Size(iconWidth * 3 / 2 + labelWidth, (iconHeight >= labelHeight)? iconHeight : labelHeight));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	icon->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	icon->setPosition(Vec2(0.0f, 0.0f));
	this->addChild(icon);

	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	label->setPosition(Vec2(iconWidth * 3 / 2, (iconHeight >= labelHeight) ? iconHeight / 2 : labelHeight / 2));
	this->addChild(label);

	this->setScale(1.2f);
	return true;
}