#include "GameHallShopItem.h"
#include "Common/WebImageView.h"
#include "GameGlobalSetting.h"
#include "CommonFunction.h"
#include "GameHallBuyAlertWidget.h"
#include "Common/ModalViewManager.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"

#define MSG_COLOR	ccc3(255,0,0)


GameHallShopItem::GameHallShopItem()
{
	m_giftModel = nullptr;
}

GameHallShopItem::~GameHallShopItem()
{
	CC_SAFE_RELEASE_NULL(m_giftModel);
}

GameHallShopItem* GameHallShopItem::create(GiftModel* m)
{
	GameHallShopItem* pItem = new GameHallShopItem();
	if (pItem && pItem->init(m))
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

bool GameHallShopItem::init(GiftModel* m)
{
	if (!Layout::init())
		return false;
	m_giftModel = m;
	m_giftModel->retain();

	ImageView* itmebg = ImageView::create("shop_item_bg.png",Widget::TextureResType::PLIST);
	itmebg->setPosition(Vec2(itmebg->getContentSize().width/2+5,itmebg->getContentSize().height/2));
	addChild(itmebg);

	Text* title = Text::create();
	title->setFontSize(25);
	title->setAnchorPoint(Vec2(0.5,0.5));
	title->setText(CommonFunction::GBKToUTF8(m_giftModel->getGiftName().c_str()));
	title->setColor(ccc3(96,34,0));
	title->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height - title->getContentSize().height/2 - 15));
	itmebg->addChild(title);


	ImageView* shine = ImageView::create("shop_item_shine.png",Widget::TextureResType::PLIST);
	shine->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height/2 + 60));
	if (m_giftModel->getGiftType() == 2)
	{
		shine->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height/2 + 50));
	}
	
	itmebg->addChild(shine);


	WebImageView* icon = WebImageView::create();
	icon->setPosition(Vec2(shine->getContentSize().width/2,shine->getContentSize().height/2));
	__String* url = __String::createWithFormat(ImageURLFormat,m_giftModel->getIconUrl().c_str());
	icon->setLoadingAnimation(true);
	icon->setCustomSize(Size(70,70));
	icon->imageWithUrl(url->getCString());
	//icon->setScale(1.5f);
	shine->addChild(icon);

	if(m_giftModel->getGiftType() != 3)
	{
		ImageView* vipBg = ImageView::create("image_vipPricebg.png",Widget::TextureResType::PLIST);
		vipBg->setPosition(Vec2(vipBg->getContentSize().width/2 -5,itmebg->getContentSize().height/2  - vipBg->getContentSize().height/2 -5));
		itmebg->addChild(vipBg);

		Text* vipPrice = Text::create();
		vipPrice->setFontSize(21);
		__String* price_str = __String::create("»áÔ±¼Û");
		vipPrice->setText(CommonFunction::GBKToUTF8(price_str->getCString()));
		vipPrice->setColor(ccc3(255,255,255));
		vipPrice->setPosition(Vec2(vipBg->getContentSize().width/2 - 55,vipBg->getContentSize().height/2+5));
		vipBg->addChild(vipPrice);


		Text* Price = Text::create();
		Price->setFontSize(30);
		int proPrice = 0;

	
		OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
		int discount = KvipDiscount[pUser->getVipLevel()];
		if (pUser->getVipLevel() == 0)
		{
			discount =  KvipDiscount[1];
		}
		proPrice = m_giftModel->getPrice()*discount/100 ;
	
		price_str = __String::createWithFormat("%d",proPrice);
		Price->setText(price_str->getCString());
		Price->setColor(ccc3(255,255,255));
		Price->setPosition(Vec2(vipPrice->getPositionX()+Price->getContentSize().width/2 +vipPrice->getContentSize().width/2 + 5,vipBg->getContentSize().height/2+5));
		vipBg->addChild(Price);

		Text* price = Text::create();
		price->setFontSize(21);
		price->setAnchorPoint(Vec2(0.5,0.5));
		price_str = __String::createWithFormat("%s%d",CommonFunction::GBKToUTF8("½±„»:").c_str(),m_giftModel->getPrice());
		price->setText(price_str->getCString());
		price->setColor(ccc3(96,34,0));
		price->setPosition(Vec2(itmebg->getContentSize().width/2,vipBg->getPositionY() - price->getContentSize().height - 15));
		itmebg->addChild(price);
	}
	else
	{
		SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shop_award_bg.png");
		ui::Scale9Sprite* priceBg = ui::Scale9Sprite::createWithSpriteFrame(frame);
		priceBg->setContentSize(Size(itmebg->getContentSize().width - 10,50));
		priceBg->setPosition(Vec2(itmebg->getContentSize().width/2,priceBg->getContentSize().height/2 + 100));
		itmebg->addChild(priceBg);

		Text* price = Text::create();
		price->setFontSize(25);
		price->setAnchorPoint(Vec2(0.5,0.5));
		__String* price_str = __String::createWithFormat("%s %d",CommonFunction::GBKToUTF8("½±„»:").c_str(),m_giftModel->getPrice());
		price->setText(price_str->getCString());
		price->setColor(ccc3(184,48,34));
		price->setPosition(Vec2(priceBg->getContentSize().width/2,priceBg->getContentSize().height/2));
		priceBg->addChild(price);
	}

	Button* buyBtn = Button::create();
	buyBtn->loadTextureNormal("btn_4.png",Widget::TextureResType::PLIST);
	buyBtn->setPosition(Vec2(itmebg->getContentSize().width/2,buyBtn->getContentSize().height/2 + 15));
	buyBtn->setScale(0.8f);
	buyBtn->setTitleText(CommonFunction::GBKToUTF8("¹º Âò"));
	buyBtn->setTitleColor(ccc3(255,255,255));
	buyBtn->setTitleFontSize(30);
	buyBtn->addTouchEventListener(CC_CALLBACK_2(GameHallShopItem::onClickCovert,this));
	itmebg->addChild(buyBtn);


	return true;
}

void GameHallShopItem::onClickCovert(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallBuyAlertWidget::create(proType_shop,m_giftModel));
	}
}