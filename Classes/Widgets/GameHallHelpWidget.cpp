#include "GameHallHelpWidget.h"

#define  ImagePopbgTag  18
#define  BtnCloseTag 42
#define  BtnLastPageTag  538
#define  BtnNextPageTag  540
#define  ImageHelpTextTag 541


GameHallHelpWidget::GameHallHelpWidget()
{
	mPageIndex = 1;
	mHelpImage = nullptr;
}

GameHallHelpWidget::~GameHallHelpWidget()
{

}

bool GameHallHelpWidget::init()
{
	if (!TNWidget::init())
		return false;

	return true;
}

std::string GameHallHelpWidget::getWidgetJsonName()
{
	return "helpWidget";
}

void GameHallHelpWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	//给相关按钮绑定事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallHelpWidget::onBack, this));

	/*Button* btn_lastpage = static_cast<Button*>(popBg->getChildByTag(BtnLastPageTag));
	btn_lastpage->addTouchEventListener(CC_CALLBACK_2(GameHallHelpWidget::onClickLastPage, this));

	Button* btn_nextpage= static_cast<Button*>(popBg->getChildByTag(BtnNextPageTag));
	btn_nextpage->addTouchEventListener(CC_CALLBACK_2(GameHallHelpWidget::onClickNextPage, this));


	mHelpImage =  static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageHelpTextTag));*/
}

void GameHallHelpWidget::onEnter()
{
	TNWidget::onEnter();
}

void GameHallHelpWidget::onExit()
{
	TNWidget::onExit();
}

void GameHallHelpWidget::onClickLastPage(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{

		removeFromParent();
	}
}

void GameHallHelpWidget::onClickNextPage(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
	}

}

void GameHallHelpWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
	}
}