#include "GameHallChargeWidget.h"
#include "Message.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameServiceClientManager.h"
#include "SessionManager.h"
#include "GameHallMsgItem.h"
#include "CommonFunction.h"
#include "GameHallChargeItem.h"
#include "GameHallVipPower.h"
#include "Common/ModalViewManager.h"
#include "Model/ChargeModel.h"
#include "U8sdkFunction.h"

#define  TabGoldTag 570
#define  ImageGoldTag 572
#define  ImageIngotTag  573
#define  TabIngotTag 571

#define  ImagePopbgTag 18
#define  BtnCloseTag   42 
#define  ListViewTag      574
#define  BtnVipPowerTag   511

#define NoMsgNodeTag    1018

#define TextUserScoreTag 3160
#define TextUserIngotTag 3166

const int kChargeItem[7] = {10,30,50,100,200,500,1000};
const int kChargeSend[7] ={1,5,2,5,12,35,80};

GameHallChargeWidget::GameHallChargeWidget()
{
	mChargeList = nullptr;
	mTextGold = nullptr;
	mTextIngot = nullptr;
	mTabGold  = nullptr;
	mTabIngot  = nullptr;
	mUserScore = nullptr;
	mUserIngot = nullptr;

	mPageIndex = 0;//默认为礼包页面

	mDataArray = __Array::createWithCapacity(6);
	mDataArray->retain();

	mCurModel = nullptr;
	mbIngotExchange = false;
}

GameHallChargeWidget::~GameHallChargeWidget()
{
	CC_SAFE_RELEASE_NULL(mDataArray);
}

GameHallChargeWidget* GameHallChargeWidget::create(tabType type)
{
	GameHallChargeWidget* pWidget = new GameHallChargeWidget();
	if (pWidget && pWidget->init(type))
	{
		pWidget->autorelease();
		return pWidget;
	}
	delete pWidget;
	pWidget = nullptr;
	return nullptr;
}

bool GameHallChargeWidget::init(tabType type)
{
	if (!TNWidget::init())
		return false;

	mTabType = type;

	return true;
}

std::string GameHallChargeWidget::getWidgetJsonName()
{
	return "chargeWidget";
}

void GameHallChargeWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	//绑定相关事件

	mTabGold = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabGoldTag));
	mTabGold->addTouchEventListener(CC_CALLBACK_2(GameHallChargeWidget::onClickGold, this));
	mTextGold = static_cast<ImageView*>(mTabGold->getChildByTag(ImageGoldTag));


	mTabIngot = static_cast<ImageView*>(m_mainWidget->getChildByTag(TabIngotTag));
	mTabIngot->addTouchEventListener(CC_CALLBACK_2(GameHallChargeWidget::onClickIngot, this));
	mTextIngot = static_cast<ImageView*>(mTabIngot->getChildByTag(ImageIngotTag));

	//
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btn_close = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btn_close->addTouchEventListener(CC_CALLBACK_2(GameHallChargeWidget::onBack, this));

	Button* toVipPower = static_cast<Button*>(popBg->getChildByTag(BtnVipPowerTag));
	toVipPower->addTouchEventListener(CC_CALLBACK_2(GameHallChargeWidget::onClickVipPower, this));

	mChargeList = static_cast<ListView*>(popBg->getChildByTag(ListViewTag));
	mChargeList->removeAllItems();

	mUserScore = static_cast<Text*>(popBg->getChildByTag(TextUserScoreTag));
	mUserIngot = static_cast<Text*>(popBg->getChildByTag(TextUserIngotTag));

}



void GameHallChargeWidget::onEnter()
{


	TNWidget::onEnter();
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(GetShopListFromServerMessage, CC_CALLBACK_1(GameHallMsgWidget::refreshMsgList, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(exchangeIngotRspMsg, CC_CALLBACK_1(GameHallChargeWidget::receiveExchangeIngoRspMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(operateFailureMsg, CC_CALLBACK_1(GameHallChargeWidget::receiveOperateFailureMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(treasureInfoMsg, CC_CALLBACK_1(GameHallChargeWidget::refreshTreasure, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(IngotCovertScoreMsg, CC_CALLBACK_1(GameHallChargeWidget::receiveIngotCoverScoreMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(RechargeOrderRspMsg, CC_CALLBACK_1(GameHallChargeWidget::receiveRechargeOrderRspMsg, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(toGetRechargeOrderMsg, CC_CALLBACK_1(GameHallChargeWidget::receiveToGetRechargeOrderMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(RechargeSuccessMsg, CC_CALLBACK_1(GameHallChargeWidget::receiveRechargeSuccessMsg, this)), this);
	
	refreshData(1);
	//获取用户财富信息
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendTreasureRequest();
	if (mTabType == tab_gold)
	{
		refreshChargeList(nullptr);
	}
}

void GameHallChargeWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallChargeWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}



void GameHallChargeWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	if (mCurModel)
	{
		CC_SAFE_RELEASE_NULL(mCurModel);
	}
	TNWidget::onExit();
}

void GameHallChargeWidget::refreshChargeList(EventCustom* event)
{
	removeLoading();
	//for test
	mChargeList->removeAllItems();
	mDataArray->removeAllObjects();
	int i;
	for (i = 0 ; i < 6;i++)
	{
		ChargeModel* model = ChargeModel::create();
		model->setPrice(kChargeItem[i]);
		model->setItemId(i+1);
		if (mTabType == tab_ingot)
		{
			model->setCovertCount(kChargeItem[i]*10000);
			model->setSendCount(kChargeSend[i]);
			model->setSendType(1);
			if (i == 0 || i==1)
			{
				model->setSendType(0);
			}
		}
		else
		{
			model->setCovertCount(kChargeItem[i]);
		
		}

		model->setChargeItemType(int(mTabType));
		mDataArray->addObject(model);
	}

	int itemCount = 0;
	Layout* lineLayout = nullptr;
	while(itemCount < mDataArray->count())
	{
		
		float interval = (mChargeList->getContentSize().width - 80*3)/3;
		
		ChargeModel* model = (ChargeModel*)(mDataArray->getObjectAtIndex(itemCount));
		GameHallChargeItem* pItem = GameHallChargeItem::create(model,mChargeList->getContentSize().width);

		Sprite* temp = Sprite::createWithSpriteFrameName("shop_item_bg.png");

		Size itemSize = Size(temp->getContentSize().width+15 ,temp->getContentSize().height);

		pItem->ignoreContentAdaptWithSize(false);
		pItem->setContentSize(itemSize);
		//pItem->setAnchorPoint(Vec2(0.5,0.5));
		

		mChargeList->pushBackCustomItem(pItem);

		itemCount++;
		//当前行，列数不够，退出
		if (itemCount >= mDataArray->count())
			break;;

		
	}
}

void GameHallChargeWidget::switchPanel(tabType type)
{
	if (type == tab_gold)
	{
		Vec2 tab1pos = mTabGold->getPosition();
		Vec2 tab2pos = mTabIngot->getPosition();
		mTabIngot->setPosition(Vec2(mTabIngot->getPositionX(),tab1pos.y));
		mTabIngot->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
		mTextIngot->loadTexture("font_yuanbao_an.png",Widget::TextureResType::PLIST);
		mTabGold->setPosition(Vec2(mTabGold->getPositionX(),tab2pos.y));
		mTabGold->loadTexture("tab.png",Widget::TextureResType::PLIST);
		mTextGold->loadTexture("font_gold.png",Widget::TextureResType::PLIST);

		refreshData(mPageIndex);
	}
	else
	{
		Vec2 tab1pos = mTabGold->getPosition();
		Vec2 tab2pos = mTabIngot->getPosition();
		log("mTaginot began y = %f",tab2pos.y);
		mTabIngot->setPosition(Vec2(mTabIngot->getPositionX(),100));
		log("mTaginot end y = %f",mTabIngot->getPosition().y);
		mTabIngot->loadTexture("tab.png",Widget::TextureResType::PLIST);
		mTextIngot->loadTexture("font_yuanbao.png",Widget::TextureResType::PLIST);
		mTabGold->setPosition(Vec2(mTabGold->getPositionX(),200));
		log("mTagGold end y1 = %f",mTabGold->getPosition().y);
		mTabGold->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
		mTextGold->loadTexture("font_gold_an.png",Widget::TextureResType::PLIST);
		
		refreshChargeList(nullptr);
	}
}

void GameHallChargeWidget::onClickIngot(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex != 2)
		{
			mPageIndex = 2 ;
			mTabType = tab_ingot;
			Vec2 tab1pos = mTabGold->getPosition();
			Vec2 tab2pos = mTabIngot->getPosition();
			mTabIngot->setPosition(Vec2(mTabIngot->getPositionX(),tab1pos.y));
			mTabIngot->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextIngot->loadTexture("font_yuanbao.png",Widget::TextureResType::PLIST);
			mTabGold->setPosition(Vec2(mTabGold->getPositionX(),tab2pos.y));
			mTabGold->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			mTextGold->loadTexture("font_gold_an.png",Widget::TextureResType::PLIST);

			refreshChargeList(nullptr);
		}
	}

}

void GameHallChargeWidget::onClickGold(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex != 1)
		{
			mPageIndex = 1 ; 
			mTabType = tab_gold;
			Vec2 tab1pos = mTabGold->getPosition();
			Vec2 tab2pos = mTabIngot->getPosition();
			mTabIngot->setPosition(Vec2(mTabIngot->getPositionX(),tab1pos.y));
			mTabIngot->loadTexture("tab_on.png",Widget::TextureResType::PLIST);
			mTextIngot->loadTexture("font_yuanbao_an.png",Widget::TextureResType::PLIST);
			mTabGold->setPosition(Vec2(mTabGold->getPositionX(),tab2pos.y));
			mTabGold->loadTexture("tab.png",Widget::TextureResType::PLIST);
			mTextGold->loadTexture("font_gold.png",Widget::TextureResType::PLIST);

			//refreshData(mPageIndex);
			refreshChargeList(nullptr);
		}
	}
}


void GameHallChargeWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
	}
}

void GameHallChargeWidget::onClickVipPower(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
		ModalViewManager::sharedInstance()->showWidget(GameHallVipPowerWidget::create());
	}
}



void GameHallChargeWidget::refreshData(int type)
{

}

void GameHallChargeWidget::receiveExchangeIngoRspMsg(EventCustom* evt)
{
	removeLoading();
	mbIngotExchange = false;
	CMD_GP_ExchangeIngotSuccess* ret = (CMD_GP_ExchangeIngotSuccess*)evt->getUserData();
	// 更新元宝值，银行金币值
	mUserScore->setString(numberToString(ret->lInsure));
	mUserIngot->setString(numberToString(ret->lIngot));

	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));

}


void GameHallChargeWidget::receiveOperateFailureMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_OperateFailure* ret = (CMD_GP_OperateFailure*)evt->getUserData();
	if (mbIngotExchange)
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,CommonFunction::GBKToUTF8("前往冲值"), CommonFunction::GBKToUTF8(ret->szDescribeString)));
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr, "", CommonFunction::GBKToUTF8(ret->szDescribeString)));
	}
		
}

void GameHallChargeWidget::okCallback()
{
	onClickIngot(mTabIngot, ui::Widget::TouchEventType::ENDED);
}

void GameHallChargeWidget::refreshTreasure(EventCustom* evt)
{
	removeLoading();
	CMD_GP_UserTreasure* treasureInfo = (CMD_GP_UserTreasure*)evt->getUserData();
	mUserScore->setString(numberToString(treasureInfo->lInsureScore));
	mUserIngot->setString(numberToString(treasureInfo->lIngotScore));

	if(mTabType == tab_ingot)
		onClickIngot(mTabIngot,ui::Widget::TouchEventType::ENDED);
}

void GameHallChargeWidget::receiveIngotCoverScoreMsg(EventCustom* evt)
{
	showLoading();
	mbIngotExchange = true;
	ChargeModel* model = (ChargeModel*)evt->getUserData();
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendExchangeIngotRequest(model->getPrice());
}

void GameHallChargeWidget::receiveToGetRechargeOrderMsg(EventCustom* evt)
{
	ChargeModel* data = (ChargeModel*)evt->getUserData();
	if(data != mCurModel)
	{
		if (mCurModel)
		{
			mCurModel->release();
		}
		mCurModel = data;
		mCurModel->retain();
	}

	mCurOrderNumber = CommonFunction::getCurOrderNo();
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendRechargeOrder(data->getPrice(),mCurOrderNumber.c_str());
	showLoading();
}

void GameHallChargeWidget::receiveRechargeOrderRspMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_RechargeOrderLog* orderLog = (CMD_GP_RechargeOrderLog*)evt->getUserData();
	if (orderLog->dwRet == 0)	{
		log("receiveRechargeOrderRspMsg %s",mCurOrderNumber.c_str());
		__String *sAction = __String::createWithFormat("{\"name\":\"ingot\",\"desc\":\"%d ingot\",\"price\":\"%d\",\"tradeNo\":\"%s\"}", (int)mCurModel->getPrice(),(int)mCurModel->getPrice(),mCurOrderNumber.c_str());
		CommonFunction::callPay(sAction->getCString());
		__String *desc = __String::createWithFormat("%d ingot", (int)mCurModel->getPrice());
		//__String *price = __String::createWithFormat("%d", (int)mCurModel->getPrice());
		__String *tradeNo = __String::createWithFormat("%d", mCurOrderNumber.c_str());
		CU8sdkFunction::GetInstance().OnU8sdkPay("ingot", desc->getCString(), (int)mCurModel->getPrice(), tradeNo->getCString());
	}
	else
	{
		ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(orderLog->szDescribeString)));
	}
}

void GameHallChargeWidget::receiveRechargeSuccessMsg(EventCustom* evt)
{
	__Dictionary* dic = (__Dictionary*)evt->getUserData();
	__String* ingot = (__String*)dic->objectForKey("ingot");
	mUserIngot->setString(ingot->getCString());
}