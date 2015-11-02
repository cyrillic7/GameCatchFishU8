
#include "GameHallBagItem.h"
#include "Common/WebImageView.h"
#include "GameGlobalSetting.h"
#include "CommonFunction.h"
#include "Message.h"

GameHallBagItem::GameHallBagItem()
{
	mImageUrl = "";
	mData = nullptr;
	mBSelect = false;
}

GameHallBagItem::~GameHallBagItem()
{
	CC_SAFE_RELEASE(mSelectImage);
	CC_SAFE_RELEASE_NULL(mData);
}

GameHallBagItem* GameHallBagItem::create(GiftModel* info)
{
	GameHallBagItem* pItem = new GameHallBagItem;
	if(pItem && pItem->init(info))
	{
		pItem->autorelease();
		return pItem;
	}
	else
	{
		delete pItem;
		pItem = nullptr;
		return pItem;
	}
}

bool  GameHallBagItem::init(GiftModel* info)
{
	if (!Layout::init())
	{
		return false;
	}
	mData = info;
	mData->retain();
	//setTouchEnabled(true);
	//bg
	ImageView* bg = ImageView::create();
	bg->loadTexture("image_frame.png",ui::Widget::TextureResType::PLIST);

	bg->setTouchEnabled(true);
	bg->addTouchEventListener(CC_CALLBACK_2(GameHallBagItem::onBagItemPressed, this));

	mSelectImage = ImageView::create();
	mSelectImage->retain();
	//mSelectImage->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2));
	mSelectImage->loadTexture("image_frame_select.png",ui::Widget::TextureResType::PLIST);
	mSelectImage->setVisible(false);
	addChild(mSelectImage);

	//icon
	WebImageView* icon = WebImageView::create();
	if (strlen(mData->getIconUrl().c_str()) > 0 ) 
	{
		icon->setLoadingAnimation(true);
		icon->setCustomSize(Size(71,71));
		icon->imageWithUrl(__String::createWithFormat(ImageURLFormat,mData->getIconUrl().c_str())->getCString());
		//icon->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height/2));
		addChild(icon);
	}
	

	ImageView* numbg = ImageView::create();
	numbg->loadTexture("number_bg_2.png",ui::Widget::TextureResType::PLIST);
	numbg->setPosition(Vec2((bg->getContentSize().width/2 - numbg->getContentSize().width/2 -4 ),-bg->getContentSize().height/2+numbg->getContentSize().height/2+4));
	addChild(numbg);


	mCountText = Text::create();
	mCountText->setTextColor(ccc4(255,255,255,255));
	mCountText->setFontSize(20);
	if (mData->getGiftCount() > 99 )
	{
		mCountText->setText(__String::createWithFormat("%d+",99)->getCString());
	}
	else
	{
		mCountText->setText(__String::createWithFormat("x%d",mData->getGiftCount())->getCString());
	}
	mCountText->setPosition(Vec2(numbg->getContentSize().width/2 +4,numbg->getContentSize().height/2));
	numbg->addChild(mCountText);


	addChild(bg);


	return true;
}

void GameHallBagItem::refreshData()
{
	if (mData->getGiftCount() >=99)
	{
		return;
	}
	else
	{
		mCountText->setText(__String::createWithFormat("x%d",mData->getGiftCount())->getCString());
	}
}

void GameHallBagItem::onBagItemPressed(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (!mBSelect)
		{
			setSelect(true);
		}
	}
}

void GameHallBagItem::setSelect(bool value)
{
	mSelectImage->setVisible(value);
	mBSelect = value;

	if (mBSelect)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(switchBagItemMsg,this);
	}
	
}