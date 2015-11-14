#include "GameHallShopWidget.h"
#include "Message.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "Model/GiftModel.h"
#include "GameServiceClientManager.h"
#include "Common/ShopManager.h"
#include "GameHallShopItem.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "CommonFunction.h"
#include "Widgets/GameHallBagWidget.h"
#include "Common/ModalViewManager.h"
#include "Common/AlertWidget.h"

#include "cocostudio/CCSGUIReader.h"

#define  TabGiftTag 570
#define  ImageGiftTag 572
#define  ImageToolTag  573
#define  TabToolTag 571

#define  ImageVipTag  12345
#define  TabVipTag 12344


#define  ImagePopbgTag 18
#define  BtnCloseTag   42 
#define  PanelTag      574
#define  ListViewTag   1286

#define  ImageJianjuanBg 1290
#define  TextJianjuanTag 1299
#define  ImageIngotBg	 1294
#define  TextIngotTag	 1300
#define  ImageInsureBg	 1298
#define  TextInsureTag   1301

#define  ImageBagTag     4186

GameHallShopWidget::GameHallShopWidget()
{
	mTextTool = nullptr;
	mTextGift = nullptr;
	mTabGift  = nullptr;
	mTabTool  = nullptr;

	mTabVip = nullptr;
	mTextVip = nullptr;

	mPageIndex = 1;//默认为礼包页面
}

GameHallShopWidget::~GameHallShopWidget()
{

}

bool GameHallShopWidget::init()
{
	if (!TNWidget::init())
		return false;
	



	return true;
}

std::string GameHallShopWidget::getWidgetJsonName()
{
	return "shopWidget";
}

void GameHallShopWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();

	mTabGift = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabGiftTag));
	mTabGift->addTouchEventListener(CC_CALLBACK_2(GameHallShopWidget::onClickGift, this));
	mTextGift = static_cast<ImageView*>(mTabGift->getChildByTag(ImageGiftTag));


	mTabTool = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabToolTag));
	mTabTool->addTouchEventListener(CC_CALLBACK_2(GameHallShopWidget::onClickTool, this));
	mTextTool = static_cast<ImageView*>(mTabTool->getChildByTag(ImageToolTag));

	mTabVip = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabVipTag));
	mTabVip->addTouchEventListener(CC_CALLBACK_2(GameHallShopWidget::onClickVip, this));
	mTextVip = static_cast<ImageView*>(mTabVip->getChildByTag(ImageVipTag));

	//
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallShopWidget::onBack, this));

	mToolList = static_cast<ListView*>(popBg->getChildByTag(ListViewTag));
	
	ImageView* LotteryBg =  static_cast<ImageView*>(popBg->getChildByTag(ImageJianjuanBg));
	mLotteryText = static_cast<Text*>(LotteryBg->getChildByTag(TextJianjuanTag));

	ImageView* ingotBg =  static_cast<ImageView*>(popBg->getChildByTag(ImageIngotBg));
	mIngotText = static_cast<Text*>(ingotBg->getChildByTag(TextIngotTag));


	ImageView* insureBg =  static_cast<ImageView*>(popBg->getChildByTag(ImageInsureBg));
	mInsureText = static_cast<Text*>(insureBg->getChildByTag(TextInsureTag));


	ImageView* bag =  static_cast<ImageView*>(popBg->getChildByTag(ImageBagTag));
	bag->addTouchEventListener(CC_CALLBACK_2(GameHallShopWidget::onClickBag, this));
}



void GameHallShopWidget::onEnter()
{


	TNWidget::onEnter();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GetShopListFromServerMessage, CC_CALLBACK_1(GameHallShopWidget::refreshGiftList, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(treasureInfoMsg, CC_CALLBACK_1(GameHallShopWidget::refreshTreasure, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(buyGifRspMsg, CC_CALLBACK_1(GameHallShopWidget::receiveBuyGiftRsp, this)), this);	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(toUserGiftMsg,CC_CALLBACK_1(GameHallShopWidget::sendUserGiftRequset, this)), this);
	//请求礼包列表
	refreshData(1);
}

void GameHallShopWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallShopWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}



void GameHallShopWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallShopWidget::refreshGiftList(EventCustom* event)
{
	removeLoading();
	__Dictionary* infoDic = (__Dictionary*)event->getUserData();
	__Array* dataArray = (__Array*)infoDic->objectForKey("goods");
	int type = ((__Integer*)infoDic->objectForKey("type"))->getValue();

	if (dataArray->count() <=0)
		return;
	
	//刷新列表
	refreashList(dataArray,type);	

	//获取用户财富信息
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendTreasureRequest();
}

void GameHallShopWidget::refreshTreasure(EventCustom* evt)
{
	removeLoading();
	CMD_GP_UserTreasure* treasureInfo = (CMD_GP_UserTreasure*)evt->getUserData();
	mInsureText->setString(numberToString(treasureInfo->lInsureScore));
	mLotteryText->setString(numberToString(treasureInfo->lLottery));
	mIngotText->setString(numberToString(treasureInfo->lIngotScore));
}


void GameHallShopWidget::onClickTool(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex != 2)
		{
			ImageView* pTabSelect = nullptr;
			ImageView* pFontSelect = nullptr;
			std::string font_name;
			if (mPageIndex == 1)
			{
				pTabSelect = mTabGift;
				font_name = "btn_gifts_on.png";
				pFontSelect = mTextGift;
			}
			else
			{
				pTabSelect = mTabVip;
				font_name = "vipPanel.png";
				pFontSelect = mTextVip;
			}

			
			Vec2 tab1pos = pTabSelect->getPosition();
			Vec2 tab2pos = mTabTool->getPosition();
			mTabTool->setPosition(Vec2(mTabTool->getPositionX(),tab1pos.y));
			mTabTool->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextTool->loadTexture("font_daoju.png",Widget::TextureResType::PLIST);
			pTabSelect->setPosition(Vec2(pTabSelect->getPositionX(),tab2pos.y));
			pTabSelect->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			pFontSelect->loadTexture(font_name,Widget::TextureResType::PLIST);

			mPageIndex = 2 ;
			refreshData(mPageIndex);
		}
	}

}

void GameHallShopWidget::onClickGift(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex != 1)
		{
			
			ImageView* pTabSelect = nullptr;
			ImageView* pFontSelect = nullptr;
			std::string font_name;
			if (mPageIndex == 2)
			{
				pTabSelect = mTabTool;
				font_name = "font_tools.png";
				pFontSelect = mTextTool;
			}
			else
			{
				pTabSelect = mTabVip;
				font_name = "vipPanel.png";
				pFontSelect = mTextVip;
			}

			Vec2 tab1pos = mTabGift->getPosition();
			Vec2 tab2pos = pTabSelect->getPosition();
			pTabSelect->setPosition(Vec2(pTabSelect->getPositionX(),tab1pos.y));
			pTabSelect->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			pFontSelect->loadTexture(font_name,Widget::TextureResType::PLIST);
			mTabGift->setPosition(Vec2(mTabGift->getPositionX(),tab2pos.y));
			mTabGift->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextGift->loadTexture("btn_gifts.png",Widget::TextureResType::PLIST);

			mPageIndex = 1 ; 
			refreshData(mPageIndex);
		}
	}
}

void GameHallShopWidget::onClickVip(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex != 3)
		{
			ImageView* pTabSelect = nullptr;
			ImageView* pFontSelect = nullptr;
			std::string font_name;

			if (mPageIndex == 1)
			{
				pTabSelect = mTabGift;
				font_name = "btn_gifts_on.png";
				pFontSelect = mTextGift;
			}
			else
			{
				pTabSelect = mTabTool;
				font_name = "font_tools.png";
				pFontSelect = mTextTool;
			}

			Vec2 tab1pos = pTabSelect->getPosition();
			Vec2 tab2pos = mTabVip->getPosition();
			pTabSelect->setPosition(Vec2(pTabSelect->getPositionX(),tab2pos.y));
			pTabSelect->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			pFontSelect->loadTexture(font_name,Widget::TextureResType::PLIST);
			mTabVip->setPosition(Vec2(mTabVip->getPositionX(),tab1pos.y));
			mTabVip->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextVip->loadTexture("vipPanel_on.png",Widget::TextureResType::PLIST);

			mPageIndex = 3 ; 
			refreshData(mPageIndex);
		}
	}
}

void GameHallShopWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
	}
}

void GameHallShopWidget::onClickBag(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallBagWidget::create());
	}
}



void GameHallShopWidget::refreshData(int type)
{
	__Array* dataList = ShopManager::getInstance()->getDataList((ShopItemType)type);
	if (!dataList)//发送请求
	{
		////for test
		//if (type == 3)
		//{
		//	__Array* infos = __Array::create();
		//	
		//	for(int i= 0 ; i < 4;i++)
		//	{
		//		GiftModel* pGift = GiftModel::create();
		//		pGift->setGiftId(i);
		//		pGift->setGiftName("Vip1");
		//		pGift->setIconUrl("gold.png");
		//		pGift->setDiscount(0);
		//		pGift->setPrice(i*50);
		//		pGift->setPriceDesc("奖劵");
		//		pGift->setOpenDesc("");
		//		pGift->setDesc("");
		//		pGift->setGiftType(3);
		//		infos->addObject(pGift);
		//	}
		//	refreashList(infos,3);
		//	return;
		//}

		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->SendRequestShop(type);
		showLoading();
	}
	else
	{
		//根据列表刷新列表
		refreashList(dataList,type);
	}
}

void GameHallShopWidget::refreashList(__Array* pArray,int type)
{
	mToolList->removeAllItems();

	int listNum = pArray->count();
	int count =0;
	Sprite* temp = Sprite::createWithSpriteFrameName("shop_item_bg.png");

	Size itemSize = Size(temp->getContentSize().width+5 ,temp->getContentSize().height);

	//margin
	int marginLeft = (mToolList->getContentSize().width - 4*temp->getContentSize().width)/4 ;
	for (int i = 0 ;i < listNum ; i++)
	{
		GiftModel* m = (GiftModel*)(pArray->getObjectAtIndex(i));
		GameHallShopItem* itemBtn = GameHallShopItem::create(m);
		itemBtn->setTag(i);
		itemBtn->setContentSize(itemSize);

		//LinearLayoutParameter* linearLayout = LinearLayoutParameter::create();
		//linearLayout->setMargin(Margin(10,20,0,0));
		//itemBtn->setLayoutParameter(linearLayout);


		mToolList->pushBackCustomItem(itemBtn);
	}
}

void GameHallShopWidget::sendUserGiftRequset(EventCustom* evt)
{
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	int id = ((__Integer*)info->objectForKey("giftId"))->getValue();
	int type = ((__Integer*)info->objectForKey("giftType"))->getValue();
	int count = ((__Integer*)info->objectForKey("count"))->getValue();
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendBuyGiftRequest(type,count,id);
	showLoading();
}


void GameHallShopWidget::receiveBuyGiftRsp(EventCustom* evt)
{
	removeLoading();
	CMD_GP_BuyGiftLog* ret = (CMD_GP_BuyGiftLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
	if(ret->dwRet == 0)
	{
		//获取用户财富信息
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendTreasureRequest();
		showLoading();
	}
}