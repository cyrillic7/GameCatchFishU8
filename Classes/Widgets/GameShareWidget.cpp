#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "GameShareWidget.h"

#define  ImagePopTag 18
#define  BtnCloseTag 42
#define  BtnBackTag  511
#define  BtnShareWxFlag 12539
#define  BtnShareFriendFlag      12545

GameShareWidget::GameShareWidget()
{
}

GameShareWidget::~GameShareWidget()
{

}

std::string GameShareWidget::getWidgetJsonName()
{
	return "wxShareWidget";
}

void GameShareWidget::onExit()
{
	TNWidget::onExit();
}

void GameShareWidget::onEnter()
{
	TNWidget::onEnter();
}

bool GameShareWidget::init()
{
	if (!TNWidget::init())
	{
		return false;
	}

	return true;
}
void GameShareWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	
	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopTag));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameShareWidget::onBack, this));

	//Button* btn_back = static_cast<Button*>(popBg->getChildByTag(BtnBackTag));
	//btn_back->addTouchEventListener(CC_CALLBACK_2(GameShareWidget::onBack, this));

	Button* btn_shareWx = static_cast<Button*>(popBg->getChildByTag(BtnShareWxFlag));
	btn_shareWx->addTouchEventListener(CC_CALLBACK_2(GameShareWidget::onClickShareWx, this));

	Button* btn_ShareFriend= static_cast<Button*>(popBg->getChildByTag(BtnShareFriendFlag));
	btn_ShareFriend->addTouchEventListener(CC_CALLBACK_2(GameShareWidget::onClickShareFriend, this));
}


void GameShareWidget::onBack(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}


void GameShareWidget::onClickShareWx(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		CommonFunction::callWx(0);
	}
}


void GameShareWidget::onClickShareFriend(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		CommonFunction::callWx(1);
	}
}
