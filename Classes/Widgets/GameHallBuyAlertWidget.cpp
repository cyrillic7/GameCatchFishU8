#include "GameHallBuyAlertWidget.h"

#include "GameServiceClientManager.h"
#include "CommonFunction.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameHallBagItem.h"
#include "Common/ModalViewManager.h"
#include "Common/WebImageView.h"
#include "Common/AlertWidget.h"
#include "GameHallVipPower.h"
#include "SessionManager.h"
#include "Model/OnlineUserModel.h"

#define ImageBgTag 1203
#define TextProNameTag 1207
#define ImageIconBg 1321

#define PanelShopInputBgTag 7302
#define PanelShopInputTag	7303
#define PanelShopCostTextTag 1328

#define PanelShopTag 4188
#define PanelBagTag  4194
#define PanelAutionTag 4204
#define PanelPhoneTag 7760
#define PanelQQTag 7773

//背包转换相关TAG
#define PanelBagDescTag 4195
#define PanelBagImageNumBg 7744
#define PanelBagTextNumTag   7745
#define PanelBagBtnMax  7746
//QQ
#define PanelQQImageNumBg 7775
#define PanelQQTextNumTag   7776
//phone
#define PanelPhoneImageNumBg 7764
#define PanelPhoneTextNumTag   7765

//AUCTION
#define PanelAuctionImageNumBg 7751
#define PanelAuctionTextNumTag   7752
#define PanelAuctionTextCostTag  4207

#define  BtnCancelTag 6877
#define  BtnOkTag	  6878
#define  IamgeOkTag   7297

#define  BtnGotoVipTag 7733




GameHallBuyAlertWidget::GameHallBuyAlertWidget()
{
	mShopPanel = nullptr;
	mBagPanel= nullptr;
	mPhonePanel= nullptr;
	mQQPanle = nullptr;
	mData = nullptr;
	mInfoDic = nullptr;
	mOkImage = nullptr;

	mProTitle = nullptr;
	mShopProCost = nullptr;

	mProType = proType_shop;
}

GameHallBuyAlertWidget::~GameHallBuyAlertWidget()
{
	if(mData)
		CC_SAFE_RELEASE_NULL(mData);
	if (mInfoDic)
		CC_SAFE_RELEASE_NULL(mInfoDic);
}

void GameHallBuyAlertWidget::onEnter()
{
	TNWidget::onEnter();
}

void GameHallBuyAlertWidget::onExit()
{
	TNWidget::onExit();
}

std::string GameHallBuyAlertWidget::getWidgetJsonName()
{
	return "buyAertWidget";
}

GameHallBuyAlertWidget* GameHallBuyAlertWidget::create(proType type,GiftModel* info)
{
	GameHallBuyAlertWidget* pWidget = new GameHallBuyAlertWidget();
	if (pWidget && pWidget->init(type,info))
	{
		pWidget->autorelease();
		return pWidget;
	}
	else
	{
		delete pWidget;
		pWidget = nullptr;
		return nullptr;
	}
}

GameHallBuyAlertWidget* GameHallBuyAlertWidget::createWithDic(proType type,__Dictionary* info)
{
	GameHallBuyAlertWidget* pWidget = new GameHallBuyAlertWidget();
	if (pWidget && pWidget->initWithDic(type,info))
	{
		pWidget->autorelease();
		return pWidget;
	}
	else
	{
		delete pWidget;
		pWidget = nullptr;
		return nullptr;
	}
}

bool GameHallBuyAlertWidget::init(proType type,GiftModel* info)
{
	if (!TNWidget::init())
	{
		return false;
	}
	mProType = type;
	mData = info;
	CC_SAFE_RETAIN(mData);

	return true;
}

bool GameHallBuyAlertWidget::initWithDic(proType type,__Dictionary* info)
{
	if (!TNWidget::init())
	{
		return false;
	}
	mProType = type;
	mInfoDic = info;
	CC_SAFE_RETAIN(mInfoDic);

	return true;
}



void GameHallBuyAlertWidget::loadUI()
{

	TNWidget::loadUI();

	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageBgTag));

	mShopPanel = static_cast<Layout*>(popBg->getChildByTag(PanelShopTag));
	mShopPanel->setVisible(false);
	mBagPanel = static_cast<Layout*>(popBg->getChildByTag(PanelBagTag));
	mPhonePanel = static_cast<Layout*>(popBg->getChildByTag(PanelPhoneTag));
	mQQPanle = static_cast<Layout*>(popBg->getChildByTag(PanelQQTag));
	mAuctionPanel =  static_cast<Layout*>(popBg->getChildByTag(PanelAutionTag));
	if (mProType == proType_shop)
	{
		mShopPanel->setVisible(true);
		ImageView* inputBg = static_cast<ImageView*>(mShopPanel->getChildByTag(PanelShopInputBgTag));
		TextField* text_proNumber = static_cast<TextField*>(inputBg->getChildByTag(PanelShopInputTag));

		mShopInputEdit= ui::EditBox::create(text_proNumber->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
		mShopInputEdit->setPosition(Vec2(text_proNumber->getContentSize().width/2,text_proNumber->getContentSize().height/2));
		mShopInputEdit->setFontSize(text_proNumber->getFontSize());
		mShopInputEdit->setFontColor(text_proNumber->getColor());//设置文字颜色

		mShopInputEdit->setPlaceHolder(text_proNumber->getPlaceHolder().c_str());//点位符
		mShopInputEdit->setPlaceholderFontSize(text_proNumber->getFontSize());
		mShopInputEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
		mShopInputEdit->setText(text_proNumber->getStringValue().c_str());
		mShopInputEdit->setTag(PanelShopInputTag);
		mShopInputEdit->setDelegate(this);
		text_proNumber->addChild(mShopInputEdit, 10);

		mShopProCost = static_cast<Text*>(mShopPanel->getChildByTag(PanelShopCostTextTag));

		OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
		int discount = KvipDiscount[pUser->getVipLevel()];

		if (mData->getGiftType() != ShopItemType::ShopTypeVip)
		{
			mShopProCost->setString(numberToString(mData->getPrice()*discount/100));
			mSingleCost = mData->getPrice()*discount/100;
		}
		else
		{
			mShopProCost->setString(numberToString(mData->getPrice()));
			mSingleCost = mData->getPrice();
		}
	}
	else if(mProType == proType_bag)
	{
		if (mData->getGiftExchageType() == KnapsackTypeUse)//使用
		{
			if (mData->getGiftUseType() == knapsackTypeUserPhone) //话费
			{
				mPhonePanel->setVisible(true);
				ImageView* inputBg =  static_cast<ImageView*>(mPhonePanel->getChildByTag(PanelPhoneImageNumBg));
				TextField* numberText =  static_cast<TextField*>(inputBg->getChildByTag(PanelPhoneTextNumTag));

				mPhoneEdit = ui::EditBox::create(numberText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
				mPhoneEdit->setPosition(Vec2(numberText->getContentSize().width/2,numberText->getContentSize().height/2));
				mPhoneEdit->setFontSize(numberText->getFontSize());
				mPhoneEdit->setFontColor(numberText->getColor());//设置文字颜色

				mPhoneEdit->setPlaceHolder(numberText->getPlaceHolder().c_str());//点位符
				mPhoneEdit->setPlaceholderFontSize(numberText->getFontSize());
				mPhoneEdit->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
				mPhoneEdit->setText(numberText->getStringValue().c_str());
				mPhoneEdit->setMaxLength(11);
				//mBagCovertEdit->setDelegate(this);
				numberText->addChild(mPhoneEdit, 10);
				
			}else if(mData->getGiftUseType() == KnapsackTypeUserQQ)
			{
				mQQPanle->setVisible(true);
				ImageView* inputBg =  static_cast<ImageView*>(mQQPanle->getChildByTag(PanelQQImageNumBg));
				TextField* numberText =  static_cast<TextField*>(inputBg->getChildByTag(PanelQQTextNumTag));

				mQQEdit = ui::EditBox::create(numberText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
				mQQEdit->setPosition(Vec2(numberText->getContentSize().width/2,numberText->getContentSize().height/2));
				mQQEdit->setFontSize(numberText->getFontSize());
				mQQEdit->setFontColor(numberText->getColor());//设置文字颜色

				mQQEdit->setPlaceHolder(numberText->getPlaceHolder().c_str());//点位符
				mQQEdit->setPlaceholderFontSize(numberText->getFontSize());
				mQQEdit->setInputMode(ui::EditBox::InputMode::PHONE_NUMBER);
				mQQEdit->setText(numberText->getStringValue().c_str());
				mQQEdit->setMaxLength(11);
				//mBagCovertEdit->setDelegate(this);
				numberText->addChild(mQQEdit, 10);
			}
		}else if(mData->getGiftExchageType() == KnapsackTypeCovert)//兑换
		{
			mBagPanel->setVisible(true);
			ImageView* inputBg =  static_cast<ImageView*>(mBagPanel->getChildByTag(PanelBagImageNumBg));
			TextField* numberText =  static_cast<TextField*>(inputBg->getChildByTag(PanelBagTextNumTag));

			mBagCovertEdit= ui::EditBox::create(numberText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
			mBagCovertEdit->setPosition(Vec2(numberText->getContentSize().width/2,numberText->getContentSize().height/2));
			mBagCovertEdit->setFontSize(numberText->getFontSize());
			mBagCovertEdit->setFontColor(numberText->getColor());//设置文字颜色

			mBagCovertEdit->setPlaceHolder(numberText->getPlaceHolder().c_str());//点位符
			mBagCovertEdit->setPlaceholderFontSize(numberText->getFontSize());
			mBagCovertEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
			mBagCovertEdit->setText(numberToString(mData->getGiftCount()).c_str());
			mBagCovertEdit->setTag(PanelBagTextNumTag);
			//mBagCovertEdit->setDelegate(this);
			numberText->addChild(mBagCovertEdit, 10);


			Text* desc =  static_cast<Text*>(mBagPanel->getChildByTag(PanelBagDescTag));
			desc->setText(CommonFunction::GBKToUTF8(mData->getDesc().c_str()));

			Button* btn_max =  static_cast<Button*>(mBagPanel->getChildByTag(PanelBagBtnMax));
			btn_max->addTouchEventListener(CC_CALLBACK_2(GameHallBuyAlertWidget::onClickMax,this));
		}
		
	}else if (mProType == proType_Auction)
	{
		mAuctionPanel->setVisible(true);

		ImageView* inputBg =  static_cast<ImageView*>(mAuctionPanel->getChildByTag(PanelAuctionImageNumBg));
		TextField* numberText =  static_cast<TextField*>(inputBg->getChildByTag(PanelAuctionTextNumTag));

		mAuctionEdit= ui::EditBox::create(numberText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
		mAuctionEdit->setPosition(Vec2(numberText->getContentSize().width/2,numberText->getContentSize().height/2));
		mAuctionEdit->setFontSize(numberText->getFontSize());
		mAuctionEdit->setFontColor(numberText->getColor());//设置文字颜色

		mAuctionEdit->setPlaceHolder(numberText->getPlaceHolder().c_str());//点位符
		mAuctionEdit->setPlaceholderFontSize(numberText->getFontSize());
		mAuctionEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
		mAuctionEdit->setText(numberToString(1).c_str());
		mAuctionEdit->setTag(PanelAuctionTextNumTag);
		mAuctionEdit->setDelegate(this);
		numberText->addChild(mAuctionEdit, 10);

		mAuctionCost = static_cast<Text*>(mAuctionPanel->getChildByTag(PanelAuctionTextCostTag));
		__Integer* cost = (__Integer*)mInfoDic->objectForKey("cost");
		mAuctionCost->setText(numberToString(cost->getValue()).c_str());
	}

	mProTitle =  static_cast<Text*>(popBg->getChildByTag(TextProNameTag));
	if(mProType != proType_Auction)
		mProTitle->setString(CommonFunction::GBKToUTF8(mData->getGiftName().c_str()));
	else
	{
		__String* name = (__String*)mInfoDic->objectForKey("auctionName");
		mProTitle->setString(CommonFunction::GBKToUTF8(name->getCString()));
	}

	ImageView* iconBg = static_cast<ImageView*>(popBg->getChildByTag(ImageIconBg));
	WebImageView* icon = WebImageView::create();
	if (mProType != proType_Auction)
	{
		if (strlen(mData->getIconUrl().c_str()) > 0 ) 
		{
			icon->imageWithUrl(__String::createWithFormat(ImageURLFormat,mData->getIconUrl().c_str())->getCString());
			icon->setPosition(Vec2(iconBg->getContentSize().width/2,iconBg->getContentSize().height/2));
			iconBg->addChild(icon);
		}
	}
	else
	{
		__String* path = (__String*)mInfoDic->objectForKey("imageUrl");
		icon->imageWithUrl(__String::createWithFormat(ImageURLFormat,path->getCString())->getCString());
		icon->setPosition(Vec2(iconBg->getContentSize().width/2,iconBg->getContentSize().height/2));
		iconBg->addChild(icon);
	}



	Button* btn_cancel =  static_cast<Button*>(popBg->getChildByTag(BtnCancelTag));
	btn_cancel->addTouchEventListener(CC_CALLBACK_2(GameHallBuyAlertWidget::onBack,this));

	Button* btn_ok =  static_cast<Button*>(popBg->getChildByTag(BtnOkTag));
	if (mProType == proType_Auction)
	{
		ImageView* font_ok = static_cast<ImageView*>(btn_ok->getChildByTag(IamgeOkTag));
		font_ok->loadTexture("font_buy.png",ui::Widget::TextureResType::PLIST);
	}else if (mProType == proType_bag)
	{
		ImageView* font_ok = static_cast<ImageView*>(btn_ok->getChildByTag(IamgeOkTag));
		if (mData->getGiftExchageType() == KnapsackTypeUse)
			font_ok->loadTexture("font_use.png",ui::Widget::TextureResType::PLIST);
		else
			font_ok->loadTexture("font_fasttuihuang.png",ui::Widget::TextureResType::PLIST);
	}
	btn_ok->addTouchEventListener(CC_CALLBACK_2(GameHallBuyAlertWidget::onOK,this));

	
	if (mProType == proType_shop)
	{
		Button* btn_toVip =  static_cast<Button*>(mShopPanel->getChildByTag(BtnGotoVipTag));
		btn_toVip->setVisible(false);

		if (mData->getGiftType() != ShopItemType::ShopTypeVip)
		{
			//btn_toVip->addTouchEventListener(CC_CALLBACK_2(GameHallBuyAlertWidget::onGotoVip,this));
			Text* text_discount = Text::create();
			OnlineUserModel* pUser = SessionManager::shareInstance()->getUser();
			int discount = KvipDiscount[pUser->getVipLevel()+1];
			__String* content = nullptr;
			if ( discount  % 10 == 0)
			{
				content = __String::createWithFormat("Vip%d%s%.0f%s",pUser->getVipLevel()+1,CommonFunction::GBKToUTF8("享").c_str(),discount/10.0f,CommonFunction::GBKToUTF8("折").c_str());
			}
			else
			{
				content = __String::createWithFormat("Vip%d%s%.1f%s",pUser->getVipLevel()+1,CommonFunction::GBKToUTF8("享").c_str(),discount/10.0f,CommonFunction::GBKToUTF8("折").c_str());
			}
			text_discount->setTouchEnabled(true);
			text_discount->setText(content->getCString());
			text_discount->setTextColor(ccc4(184,43,34,255));
			text_discount->setFontSize(25);
			text_discount->setPosition(btn_toVip->getPosition());
			text_discount->addTouchEventListener(CC_CALLBACK_2(GameHallBuyAlertWidget::onGotoVip,this));
			mShopPanel->addChild(text_discount);
		}
	}

}

void GameHallBuyAlertWidget::editBoxEditingDidBegin(ui::EditBox *editBox)    
{    
	CCLOG("start edit"); 
	
}

void GameHallBuyAlertWidget::editBoxEditingDidEnd(ui::EditBox *editBox)    
{    
	CCLOG("end edit");    
	auto editbox = (ui::EditBox*)editBox;  
	if (mProType == proType_shop)
	{
		if (editBox->getTag() == PanelShopInputTag)// 兑换数理的改变，总价也得跟着变
		{
			int count = atoi(editBox->getText());
			mShopProCost->setString(numberToString(mSingleCost*count));
		}
	}else if (mProType == proType_Auction)
	{
		if (editBox->getTag() == PanelAuctionTextNumTag)
		{
			int count = atoi(editBox->getText());
			__Integer* cost = (__Integer*)mInfoDic->objectForKey("cost");
			mAuctionCost->setText(numberToString(cost->getValue()*count).c_str());
		}
	}
	
} 

void GameHallBuyAlertWidget::editBoxReturn(ui::EditBox *editBox)    
{    
	CCLOG("editbox return");    
}    
void GameHallBuyAlertWidget::editBoxTextChanged(ui::EditBox *editBox,const std::string &text)    
{     

} 


void GameHallBuyAlertWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallBuyAlertWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallBuyAlertWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if(eventtype == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
	}
}

void GameHallBuyAlertWidget::onGotoVip(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if(eventtype == ui::Widget::TouchEventType::ENDED)
	{
		removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallVipPowerWidget::create());
	}
}

void GameHallBuyAlertWidget::onClickMax(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if(eventtype == ui::Widget::TouchEventType::ENDED)
	{
		mBagCovertEdit->setText(numberToString(mData->getGiftCount()).c_str());
	}
}


void GameHallBuyAlertWidget::onOK(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if(eventtype == ui::Widget::TouchEventType::ENDED)
	{
		int count =0;
		switch (mProType)
		{
		case proType_bag:
			{
				__Dictionary * dic = __Dictionary::create();
				if (mData->getGiftExchageType() == KnapsackTypeUse)//使用
				{
					if (mData->getGiftUseType() == KnapsackTypeUserQQ)
					{
						if (strlen(mQQEdit->getText()) <=0)
						{
							ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("QQ号码不能为空!")));
							return ;
						}
						dic->setObject(__String::create(mQQEdit->getText()),"szNote");
						count = 1;
						
					}else if (mData->getGiftUseType() == knapsackTypeUserPhone)
					{
						if (strlen(mPhoneEdit->getText()) <=0)
						{
							ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("手机号码不能为空!")));
							return ;
						}
						dic->setObject(__String::create(mPhoneEdit->getText()),"szNote");
						count = 1;
					}
				}
				else if (mData->getGiftExchageType() == KnapsackTypeCovert)
				{
					dic->setObject(__String::create(""),"szNote");
					count = atoi(mBagCovertEdit->getText());
				}
				dic->setObject(__Integer::create(mData->getGiftId()),"giftId");
				dic->setObject(__Integer::create(count),"count");
				dic->setObject(__Integer::create(mData->getGiftExchageType()),"exchageType");
				Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(toUserKnascapMsg,dic);
				//GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendUserKnapsackRequset(mData->getGiftId(),count);
			}
			break;
		case proType_shop:
			{
				count = atoi(mShopInputEdit->getText());
				__Dictionary * dic = __Dictionary::create();
				dic->setObject(__Integer::create(mData->getGiftId()),"giftId");
				dic->setObject(__Integer::create(count),"count");
				dic->setObject(__Integer::create(mData->getGiftType()),"giftType");
				Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(toUserGiftMsg,dic);
				//GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendBuyGiftRequest(mData->getGiftType(),count,mData->getGiftId());
			}
			break;
		case proType_Auction:
			{
				count = atoi(mAuctionEdit->getText());
				mInfoDic->setObject(__Integer::create(count),"buyCount");
				Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(toBuyAuctionMsg,mInfoDic);
			}
			break;
		}
		
		removeFromParent();
	}
}


