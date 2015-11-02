#include "GameUpdateWidget.h"
#include "Common/TNVisibleRect.h"
#include "CommonFunction.h"
#include "Message.h"

#define WIDGET_ZORDER 9999 //层级最高 不可随意改小


#define  ImageBgTag		  1203
#define  ImageTitleBagTag  1205
#define  LabelTitleTag     1206
#define  LabelContentTag   1207
#define  BtnOKTag		1211
#define  BtnCancelTag     12475

UpdateWidget::UpdateWidget()
{
}

UpdateWidget::~UpdateWidget()
{
}

bool UpdateWidget::init(std::string textContent)
{
	if(!TNWidget::init())
	{
		return false;
	}
	else
	{
		mContentText = CommonFunction::GBKToUTF8(textContent.c_str());

		setZOrder(WIDGET_ZORDER);
		return true;
	}
}

UpdateWidget* UpdateWidget::create(std::string textContent)
{
	UpdateWidget *w = new UpdateWidget;
	if (NULL != w && w->init(textContent)) {
		w->autorelease();
		return w;
	}
	CC_SAFE_DELETE(w);
	return NULL;
}

void UpdateWidget::onEnter()
{
	TNWidget::onEnter();
}

void UpdateWidget::onExit()
{
	TNWidget::onExit();
}

void UpdateWidget::onCancel(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(CancelUpdateMsg);
		//取消升级
		removeFromParent();
	}
}

std::string UpdateWidget::getWidgetJsonName()
{
	return "updateWidget";
}

void UpdateWidget::loadUI()
{
	TNWidget::loadUI();

	ImageView* popbg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageBgTag));

	ImageView* titleBg = static_cast<ImageView*>(popbg->getChildByTag(ImageTitleBagTag));

	m_contentLabel = static_cast<Text*>(popbg->getChildByTag(LabelContentTag));
	if (mContentText !="")
	{
		m_contentLabel->setFontSize(25);
		m_contentLabel->setText(mContentText);
	}

	if (m_contentLabel->getContentSize().width >= popbg->getContentSize().width  + 35)
	{
		m_contentLabel->setVisible(false);

		LabelTTF* labelContent = LabelTTF::create(mContentText,"Arail Regular",25,Size(popbg->getContentSize().width -100,200),TextHAlignment::LEFT);
		labelContent->setColor(m_contentLabel->getColor());
		labelContent->setPosition(Vec2(m_contentLabel->getPosition().x+30, labelContent->getContentSize().height/2 + 30));
		popbg->addChild(labelContent);
	}
	else
	{
		m_contentLabel->setPosition(Vec2(m_contentLabel->getPosition().x+35,m_contentLabel->getPosition().y));
	}

	Button* btnOK = static_cast<Button*>(popbg->getChildByTag(BtnOKTag));
	btnOK->addTouchEventListener(CC_CALLBACK_2(UpdateWidget::onOk, this));

	Button* btnCancel = static_cast<Button*>(popbg->getChildByTag(BtnCancelTag));
	btnCancel->addTouchEventListener(CC_CALLBACK_2(UpdateWidget::onCancel, this));
}


void UpdateWidget::onOk(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		//确定下载
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(UpdateMsg);
	}
}