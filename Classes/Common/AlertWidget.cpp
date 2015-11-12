#include "AlertWidget.h"
#include "TNVisibleRect.h"
#include "CommonFunction.h"

#define WIDGET_ZORDER 9999 //层级最高 不可随意改小


#define  ImageBgTag		  1203
#define  ImageTitleBagTag  1205
#define  LabelTitleTag     1206
#define  LabelContentTag   1207
#define  BtnColseTag		1211

#define  BtnLableTag       4901

AlertWidget::AlertWidget()
{

	m_alertDelegate = NULL;
	mTitleText = "";
	mBTwoButtons = false;
}

AlertWidget::~AlertWidget()
{
}

bool AlertWidget::init(AlertDelegate* pDelegate,std::string textTitle,std::string textContent,bool isTwoButtons )
{
	if(!TNWidget::init())
	{
		return false;
	}
	else
	{
		m_alertDelegate = pDelegate;
		mTitleText = textTitle;
		mContentText = textContent;
		mBTwoButtons = isTwoButtons;
		setZOrder(WIDGET_ZORDER);
		return true;
	}
}

AlertWidget* AlertWidget::create(AlertDelegate* pDelegate,std::string textTitle,std::string textContent,bool isTwoButtons )
{
	AlertWidget *w = new AlertWidget;
	if (NULL != w && w->init(pDelegate, textTitle, textContent,isTwoButtons)) {
		w->autorelease();
		return w;
	}
	CC_SAFE_DELETE(w);
	return NULL;
}

void AlertWidget::onEnter()
{
	TNWidget::onEnter();
}

void AlertWidget::onExit()
{
	TNWidget::onExit();
}

void AlertWidget::onClose(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

std::string AlertWidget::getWidgetJsonName()
{
	return "tipWidget";
}

void AlertWidget::loadUI()
{
	TNWidget::loadUI();

	ImageView* popbg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageBgTag));

	ImageView* titleBg = static_cast<ImageView*>(popbg->getChildByTag(ImageTitleBagTag));

	/*m_tilteLabel = static_cast<Text*>(titleBg->getChildByTag(LabelTitleTag));
	if (mTitleText !="")
	{
	m_tilteLabel->setText(mTitleText);
	}*/

	m_contentLabel = static_cast<Text*>(popbg->getChildByTag(LabelContentTag));
	if (mContentText !="")
	{
		m_contentLabel->setText(CommonFunction::replace_all(mContentText,"<br/>",""));
	}
	if (m_contentLabel->getContentSize().width >= popbg->getContentSize().width)
	{
		m_contentLabel->setVisible(false);

		LabelTTF* labelContent = LabelTTF::create(CommonFunction::replace_all(mContentText, "<br/>", ""), "Arail Regular", 29, Size(popbg->getContentSize().width - 50, 100), TextHAlignment::LEFT);
		labelContent->setColor(m_contentLabel->getColor());
		labelContent->setPosition(m_contentLabel->getPosition());
		popbg->addChild(labelContent);
	}

	Button* btnClose = static_cast<Button*>(popbg->getChildByTag(BtnColseTag));
	btnClose->addTouchEventListener(CC_CALLBACK_2(AlertWidget::onOk, this));
	if (mTitleText != "")
	{	
		Text* labelButton = static_cast<Text*>(btnClose->getChildByTag(BtnLableTag));
		labelButton->setText(mTitleText);
	}

	if (mBTwoButtons)
	{
		btnClose->setPosition(Vec2(btnClose->getContentSize().width/2 + 50,btnClose->getPositionY()));

		Button* btnOk = Button::create();
		btnOk->loadTextureNormal("btn_3.png",Widget::TextureResType::PLIST);
		btnOk->loadTexturePressed("btn_3.png",Widget::TextureResType::PLIST);
		btnOk->setTitleText(CommonFunction::GBKToUTF8("取消"));
		btnOk->setTitleColor(btnClose->getTitleColor());
		btnOk->setTitleFontSize(btnClose->getTitleFontSize());
		btnOk->setPosition(Vec2(popbg->getContentSize().width -130,btnClose->getPositionY() ));
		btnOk->addTouchEventListener(CC_CALLBACK_2(AlertWidget::onClose, this));
		popbg->addChild(btnOk);
	}
}

void AlertWidget::onOk(Ref *pSender, ui::Widget::TouchEventType eventType)
{
	if(eventType == ui::Widget::TouchEventType::ENDED)
	{
		//add by ylx
		if (NULL != m_alertDelegate)
		{
			m_alertDelegate->okCallback();
		}

		removeFromParent();
	}
}