#include "GameHallMoreGameWidget.h"
#include "CommonFunction.h"
#include "GameServiceClientManager.h"
#include "Common/AlertWidget.h"
#include "Common/ModalViewManager.h"
#include "CommonFunction.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Message.h"
#include "Common/WebImageView.h"

#define  ImagePopBg 18
#define  BtnColseTag 42
#define  ListViewTag 43


moreItem::moreItem()
{
	mData = nullptr;
}

moreItem::~moreItem()
{
	CC_SAFE_RELEASE_NULL(mData);
}

moreItem* moreItem::create(__Dictionary* info)
{
	moreItem* pItem = new moreItem();
	if (pItem && pItem->init(info))
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


bool moreItem::init(__Dictionary* info)
{
	if (!TNWidget::init())
	{
		return false;
	}

	mData = info;
	mData->retain();

	__String* icon_name = (__String*)info->objectForKey("icon");
	WebImageView* icon = WebImageView::create();
	icon->setPosition(Vec2(65,50));
	__String* path = __String::createWithFormat(ImageURLFormat,icon_name->getCString());
	icon->imageWithUrl(path->getCString());
	//icon->setScale(1.5f);
	addChild(icon);

	//游戏名称
	__String* game_name = (__String*)info->objectForKey("appName");
	Text* tipLabel = Text::create();
	tipLabel->setAnchorPoint(ccp(0, 0.5));
	tipLabel->setFontSize(25);
	tipLabel->setTextColor(ccc4(115,50,10,255));
	tipLabel->setText(CommonFunction::GBKToUTF8(game_name->getCString()));
	tipLabel->setPosition(Vec2(icon->getPositionX() +  tipLabel->getContentSize().width/2 ,icon->getPositionY() + tipLabel->getContentSize().height/2 + 17));
	addChild(tipLabel);

	__String* game_desc = (__String*)info->objectForKey("desc");
	//Text* tipLabel2 = Text::create();
	//tipLabel2->setAnchorPoint(ccp(0, 0.5));
	//tipLabel2->setFontSize(25);
	//tipLabel2->setFontName("Arail Regular");
	//tipLabel2->setTextColor(ccc4(96,33,0,255));
	//tipLabel2->setText(CommonFunction::GBKToUTF8(game_desc->getCString()));
	//tipLabel2->setPosition(Vec2(icon->getPositionX() +  tipLabel->getContentSize().width/2 ,icon->getPositionY() - tipLabel->getContentSize().height/2 -10));
	//addChild(tipLabel2);

	LabelTTF* labelContent = LabelTTF::create(CommonFunction::GBKToUTF8(game_desc->getCString()),"黑体",22,Size(260,120),TextHAlignment::LEFT);
	labelContent->setColor(ccc3(115,50,51));
	labelContent->setAnchorPoint(Vec2(0,1));
	labelContent->setPosition(Vec2(icon->getPositionX() +  tipLabel->getContentSize().width/2 ,tipLabel->getPositionY() - tipLabel->getContentSize().height/2 - 10));

	addChild(labelContent);

	Button* btn_startDownload = Button::create();
	btn_startDownload->loadTextureNormal("btn_3.png",Widget::TextureResType::PLIST);
	btn_startDownload->loadTexturePressed("btn_3.png",Widget::TextureResType::PLIST);
	btn_startDownload->setPosition(Vec2(550 - btn_startDownload->getContentSize().width/2 - 10,icon->getPositionY() ));
	btn_startDownload->addTouchEventListener(CC_CALLBACK_2(moreItem::onClickOk, this));
	btn_startDownload->setTitleText(CommonFunction::GBKToUTF8("开始游戏"));
	btn_startDownload->setTitleFontSize(25);
	addChild(btn_startDownload);

	return true;
}


void moreItem::onClickOk(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		__String* package = (__String*)mData->objectForKey("packageName");
		__String* startName = (__String*)mData->objectForKey("activityName");
		__String* url = (__String*)mData->objectForKey("url");

		log("moreItem onClickOk %s",url->getCString());
		CommonFunction::moreGamePro(package->getCString(),startName->getCString(),url->getCString());	
	}
}








//////////////////////////////////////////////////////////////////////////
GameHallMoreGameWidget::GameHallMoreGameWidget()
{
}

GameHallMoreGameWidget::~GameHallMoreGameWidget()
{

}


bool GameHallMoreGameWidget::init()
{
	if(!TNWidget::init())
		return false;

	return true;
}


void GameHallMoreGameWidget::onEnter()
{
	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(moreGameMsg, CC_CALLBACK_1(GameHallMoreGameWidget::receiveMoreGameMsg, this)), this);

	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendMoreGameRequest();
}

void GameHallMoreGameWidget::onExit()
{
	_eventDispatcher->resumeEventListenersForTarget(this);
	TNWidget::onExit();
}


std::string GameHallMoreGameWidget::getWidgetJsonName()
{
	return "moreGameWidget";
}

void GameHallMoreGameWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBg));

	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnColseTag));

	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallMoreGameWidget::onClose,this));


	mItemList = static_cast<ListView*>(popBg->getChildByTag(ListViewTag));
	mItemList->removeAllItems();
}

void GameHallMoreGameWidget::onClose(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallMoreGameWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallMoreGameWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallMoreGameWidget::receiveMoreGameMsg(EventCustom* evt)
{
	removeLoading();
	mItemList->removeAllItems();
	__Array* pArray = (__Array*)evt->getUserData();
	for (int i = 0 ; i < pArray->count(); i++)
	{
		__Dictionary* info = (__Dictionary*)pArray->getObjectAtIndex(i);
		moreItem* pItem = moreItem::create(info);
		pItem->setContentSize(Size(mItemList->getContentSize().width,100));
		mItemList->pushBackCustomItem(pItem);
	}
}

