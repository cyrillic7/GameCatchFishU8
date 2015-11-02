#include "GameHallMsgItem.h"
#include "CommonFunction.h"
#define  ITEM_HEIGHT 80

#define  ImagePopBg 18
#define  BtnCloseTag 42

GameHallMsgItem::GameHallMsgItem()
{
	mData = nullptr;
}

GameHallMsgItem::~GameHallMsgItem()
{
	CC_SAFE_RELEASE_NULL(mData);
}

GameHallMsgItem* GameHallMsgItem::create(__Dictionary* infodic,int width)
{
	GameHallMsgItem* pItem = new GameHallMsgItem();
	if (pItem && pItem->init(infodic,width))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		delete pItem;
		pItem = nullptr;
		return nullptr;
	}
}

bool GameHallMsgItem::init(__Dictionary* infodic,int width)
{
	if (!Layout::init())
	{
		return false;
	}
	mData = infodic;
	mData->retain();

	setContentSize(Size(width,ITEM_HEIGHT));
	setTouchEnabled(true);

	SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("message_bg.png");
	ui::Scale9Sprite* itembg = ui::Scale9Sprite::createWithSpriteFrame(frame);
	itembg->setContentSize(Size(width,ITEM_HEIGHT - 10));
	itembg->setPosition(Vec2(width/2,ITEM_HEIGHT/2));
	addChild(itembg);

	//__String* content = __String::create("字花样百出甘划框框一基本原理 基本原理 基本原理巧夺天工玩世不恭大型要基本原理基本原理 在一革夺爱爱爱君子坦荡荡么多");
	__String* content = (__String*)infodic->objectForKey("msgContent");
	LabelTTF* temp = LabelTTF::create();
	temp->setFontSize(20);
	temp->setString(CommonFunction::GBKToUTF8(content->getCString()));
	if(temp->getContentSize().width <= width - 50)
	{
		Text* content_text = Text::create();
		content_text->setFontSize(20);
		content_text->setColor(ccc3(96,33,0));
		content_text->setText(CommonFunction::GBKToUTF8(content->getCString()));
		content_text->setPosition(Vec2(content_text->getContentSize().width/2+20,itembg->getContentSize().height/2));
		itembg->addChild(content_text);
	}
	else
	{
		LabelTTF* labelContent = LabelTTF::create(CommonFunction::GBKToUTF8(content->getCString()),"Arail Regular",20,Size(width - 50,ITEM_HEIGHT - 10),TextHAlignment::LEFT);
		labelContent->setColor(ccc3(96,33,0));
		labelContent->setAnchorPoint(Vec2(0.5,0.5));
		labelContent->setPosition(Vec2(width/2,itembg->getContentSize().height/2 - 5));
		itembg->addChild(labelContent);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
GameHallMsgDescWidget::GameHallMsgDescWidget()
{
	m_content = "";
}

GameHallMsgDescWidget::~GameHallMsgDescWidget()
{

}

GameHallMsgDescWidget* GameHallMsgDescWidget::create(std::string textContent)
{
	GameHallMsgDescWidget* pWidget = new GameHallMsgDescWidget;
	if (pWidget && pWidget->init(textContent))
	{
		pWidget->autorelease();
		return pWidget;
	}
	delete pWidget;
	pWidget = nullptr;
	return nullptr;
}

bool GameHallMsgDescWidget::init(std::string textContent)
{
	if (!TNWidget::init())
		return false;

	m_content=textContent;

	return true;
}

std::string GameHallMsgDescWidget::getWidgetJsonName()
{
	return "descripWidget";
}

void GameHallMsgDescWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBg));


	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallMsgDescWidget::onClickClose,this));


	LabelTTF* labelContent = LabelTTF::create(CommonFunction::GBKToUTF8(m_content.c_str()),"Arail Regular",20,Size(600,popBg->getContentSize().height -80),TextHAlignment::LEFT);
	labelContent->setColor(ccc3(96,33,0));
	labelContent->setAnchorPoint(Vec2(0.5,0.5));
	labelContent->setPosition(Vec2(popBg->getContentSize().width/2,popBg->getContentSize().height/2 -40));
	popBg->addChild(labelContent);

}


void GameHallMsgDescWidget::onClickClose(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}
