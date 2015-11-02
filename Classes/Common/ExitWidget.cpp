#include "ExitWidget.h"
#include "TNVisibleRect.h"
#include "CommonFunction.h"
#include "Message.h"

#define WIDGET_ZORDER 9999 //层级最高 不可随意改小


#define  ImageBgTag		  1203
#define  ImageTitleBagTag  1205
#define  LabelTitleTag     1206
#define  LabelContentTag   1207
#define  BtnColseTag		9336
#define  BtnOkTag  1211

ExitWidget::ExitWidget()
{

}

ExitWidget::~ExitWidget()
{
}

bool ExitWidget::init()
{
	if(!TNWidget::init())
	{
		return false;
	}
	else
	{
		setZOrder(WIDGET_ZORDER);
		return true;
	}
}



void ExitWidget::onEnter()
{
	TNWidget::onEnter();
}

void ExitWidget::onExit()
{
	TNWidget::onExit();
}

void ExitWidget::onCancel(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

std::string ExitWidget::getWidgetJsonName()
{
	return "exitWidget";
}

void ExitWidget::loadUI()
{
	TNWidget::loadUI();

	ImageView* popbg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageBgTag));

	Button* btnCancel = static_cast<Button*>(popbg->getChildByTag(BtnColseTag));
	btnCancel->addTouchEventListener(CC_CALLBACK_2(ExitWidget::onCancel, this));

	Button* btnOk = static_cast<Button*>(popbg->getChildByTag(BtnOkTag));
	btnOk->addTouchEventListener(CC_CALLBACK_2(ExitWidget::onOk, this));
}


void ExitWidget::onOk(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(ExitGameMsg);
		removeFromParent();
	}
}