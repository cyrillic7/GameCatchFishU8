#include "GameHallChargeItem.h"
#include "CommonFunction.h"
#include "Common/WebImageView.h"
#include "GameServiceClientManager.h"

#define  ItemWidth  160
#define  ItemHeight 160
GameHallChargeItem::GameHallChargeItem()
{
	mData = nullptr;
}


GameHallChargeItem::~GameHallChargeItem()
{
	CC_SAFE_RELEASE_NULL(mData);
}

GameHallChargeItem* GameHallChargeItem::create(ChargeModel* model,int width)
{
	GameHallChargeItem* pItem = new GameHallChargeItem();
	if (pItem && pItem->init(model,width))
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

bool GameHallChargeItem::init(ChargeModel* model,int width)
{
	if (!Layout::init())
	{
		return false ;
	}
	mData = model;
	CC_SAFE_RETAIN(mData);

	ImageView* itmebg = ImageView::create("shop_item_bg.png",Widget::TextureResType::PLIST);
	itmebg->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height/2));
	addChild(itmebg);


	ImageView* shine = ImageView::create("shop_item_shine.png",Widget::TextureResType::PLIST);
	shine->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height/2 + 60));
	itmebg->addChild(shine);

	RotateBy* rb = RotateBy::create(0.4f,30);
	shine->runAction(RepeatForever::create(rb));

	//shade
	ImageView* shade = ImageView::create("iamge_charge_shade.png",Widget::TextureResType::PLIST);
	int dis = 20;
	if (mData->getItemId() == 1)
	{
		dis = 45;
	}
	else if(mData->getItemId() == 2)
	{
		dis = 30;
	}
	shade->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height/2 +dis));
	itmebg->addChild(shade);

	int type = mData->getChargeItemType();
	std::string imageName ="";
	//物品图片
	ImageView* icon = ImageView::create();
	if (type == 0)
	{
		imageName = __String::createWithFormat("image_gold_%d.png",mData->getItemId())->getCString();
		
	}
	else
	{
		imageName = __String::createWithFormat("image_ingot_%d.png",mData->getItemId())->getCString();
		
	}
	icon->loadTexture(imageName,ui::Widget::TextureResType::PLIST);

	icon->setPosition(Vec2(itmebg->getContentSize().width/2,itmebg->getContentSize().height/2 + 60));
	itmebg->addChild(icon);



	Button* buyBtn = Button::create();
	buyBtn->loadTextureNormal("btn_4.png",Widget::TextureResType::PLIST);
	buyBtn->loadTexturePressed("btn_4.png",Widget::TextureResType::PLIST);
	buyBtn->setPosition(Vec2(itmebg->getContentSize().width/2,buyBtn->getContentSize().height/2 + 20 ));
	buyBtn->addTouchEventListener(CC_CALLBACK_2(GameHallChargeItem::onClickBuy, this));
	buyBtn->setScale(0.9f);
	itmebg->addChild(buyBtn);

	ImageView* imageIngot = ImageView::create();
	if(type == 0)
		imageIngot->loadTexture("icon_yuanbao.png",ui::Widget::TextureResType::PLIST);
	else
		imageIngot->loadTexture("image_rmb.png",ui::Widget::TextureResType::PLIST);
	imageIngot->setPosition(Vec2(buyBtn->getContentSize().width/2 - 20,buyBtn->getContentSize().height/2));
	buyBtn->addChild(imageIngot);

	LabelAtlas* costLabel = LabelAtlas::create("0","costnumber.png",14,21,'0');
	costLabel->setPosition(Vec2(imageIngot->getPositionX() + imageIngot->getContentSize().width/2  + 5,buyBtn->getContentSize().height/2));
	costLabel->setAnchorPoint(Vec2(0,0.5));
	costLabel->setString(__String::createWithFormat("%d", mData->getPrice())->getCString());
	buyBtn->addChild(costLabel);


	SpriteFrame* frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("shop_award_bg.png");
	ui::Scale9Sprite* priceBg = ui::Scale9Sprite::createWithSpriteFrame(frame);
	if (type == 1)
	{
		priceBg->setContentSize(Size(itmebg->getContentSize().width - 10,62));
	}
	else
	{
		priceBg->setContentSize(Size(itmebg->getContentSize().width - 10,50));
	}
	
	priceBg->setPosition(Vec2(itmebg->getContentSize().width/2,priceBg->getContentSize().height/2 + 100));
	itmebg->addChild(priceBg);

	LabelAtlas* desc = LabelAtlas::create("0:","charge_number2.png",17,22,'0');
	int bit = 0;
	int price = mData->getPrice();
	while (price)
	{
		price = price/10;
		bit++;
	}
	desc->setString(numberToString(mData->getPrice()));
	
	desc->setAnchorPoint(Vec2(0.5,0.5));
	if(type == 1)
		desc->setPosition(Vec2(priceBg->getContentSize().width/2 -72/2 ,priceBg->getContentSize().height - desc->getContentSize().height/2 - 5));
	else
		desc->setPosition(Vec2(priceBg->getContentSize().width/2 -48/2 ,priceBg->getContentSize().height/2));

	priceBg->addChild(desc);
	if (type == 1)
	{
		ImageView* imageBit = ImageView::create();
		imageBit->loadTexture("font_charge_ingot.png",ui::Widget::TextureResType::PLIST);
		imageBit->setPosition(Vec2(desc->getPositionX()+imageBit->getContentSize().width/2 + bit*17/2 ,desc->getPositionY()));
		priceBg->addChild(imageBit);
		//send
		ImageView* imageSend = ImageView::create();
		imageSend->loadTexture("font_charge_send.png",ui::Widget::TextureResType::PLIST);
		int sendbit = 0;
		price = mData->getSendCount();
		while (price)
		{
			sendbit++;
			price /=10;
		}
		if (mData->getSendType() == 0 )
		{
			imageSend->setPosition(Vec2(priceBg->getContentSize().width/2  - 45 - sendbit*6 ,imageBit->getPositionY() - imageBit->getContentSize().height-5));
			priceBg->addChild(imageSend);
			ImageView* redpiece = ImageView::create();
			redpiece->loadTexture("font_redpieces.png",ui::Widget::TextureResType::PLIST);
			redpiece->setPosition(Vec2(imageSend->getPositionX()+imageSend->getContentSize().width/2+redpiece->getContentSize().width/2,imageSend->getPositionY()));
			priceBg->addChild(redpiece);

			LabelAtlas* count = LabelAtlas::create("0:","charge_number1.png",12,16,'0');
			count->setString(__String::createWithFormat(":%d",mData->getSendCount())->getCString());
			count->setAnchorPoint(Vec2(0.5,0.5));
			count->setPosition(Vec2(redpiece->getPositionX()+redpiece->getContentSize().width/2+sendbit*6+5,redpiece->getPositionY()-2));
			priceBg->addChild(count);
		}
		else
		{
			imageSend->setPosition(Vec2(priceBg->getContentSize().width/2 - 25 - sendbit*6,imageBit->getPositionY() - imageBit->getContentSize().height-5));
			priceBg->addChild(imageSend);

			ImageView* red= ImageView::create();
			red->loadTexture("font_red.png",ui::Widget::TextureResType::PLIST);
			red->setPosition(Vec2(imageSend->getPositionX()+imageSend->getContentSize().width/2+red->getContentSize().width/2,imageSend->getPositionY()));
			priceBg->addChild(red);

			LabelAtlas* count = LabelAtlas::create("0:","charge_number1.png",12,16,'0');
			count->setString(__String::createWithFormat(":%d",mData->getSendCount())->getCString());
			count->setPosition(Vec2(red->getPositionX()+red->getContentSize().width/2+sendbit*6+5,red->getPositionY()-2));
			count->setAnchorPoint(Vec2(0.5,0.5));
			priceBg->addChild(count);
		}
	}
	else
	{

		ImageView* imageBit = ImageView::create();
		imageBit->loadTexture("font_charge_w.png",ui::Widget::TextureResType::PLIST);
		imageBit->setPosition(Vec2(desc->getPositionX()+imageBit->getContentSize().width/2 + bit*17/2 ,desc->getPositionY()));
		priceBg->addChild(imageBit);

		ImageView* imageType = ImageView::create();
		imageType->loadTexture("font_charge_gold.png",ui::Widget::TextureResType::PLIST);
		imageType->setPosition(Vec2(imageBit->getPositionX()+imageType->getContentSize().width/2 + imageBit->getContentSize().width/2,desc->getPositionY()));
		priceBg->addChild(imageType);

		
	}
	return true;

}
void GameHallChargeItem::onClickBuy(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		playScaleAnimation(true,pSender);
	}
	else
	{
		playScaleAnimation(false,pSender);
	}

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		//
		if(mData->getChargeItemType() == 0)//元宝兑换积分
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(IngotCovertScoreMsg,mData);
			//GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendExchangeIngotRequest(mData->getPrice());
		else
			Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(toGetRechargeOrderMsg,mData);
			//CommonFunction::callPay();
	}
}

void GameHallChargeItem::playScaleAnimation(bool less, Ref* pSender) {
	float scale = less ? 0.8f : 0.9f;
	CCScaleTo *scaleTo = CCScaleTo::create(0.2f, scale);
	Node* pNode = (Node*)pSender;
	pNode->runAction(scaleTo);
}