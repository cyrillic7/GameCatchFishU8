#include "GameHallBagWidget.h"
#include "GameServiceClientManager.h"
#include "CommonFunction.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameHallBagItem.h"
#include "Common/ModalViewManager.h"
#include "Model/GiftModel.h"
#include "Widgets/GameHallBuyAlertWidget.h"
#include "Common/AlertWidget.h"

#define  ImagePopBgTag  1203
#define  ListViewTag    1260
#define  LayerOutBagTag 1278
#define  BtnOkTag       4899

#define  ImageDescBgTag    1261
#define  LableItemNameTag  1262
#define  ImageItemIconTag  1263

#define  LableItemOpenDescTag 1265
#define  LabelItemDescTag  1266



#define  LabelItemNumberTag 8833
#define  BtnItmeUseTag     1267
#define  ImageUseFontTag   7778

#define  colCnt  4

GameHallBagWidget::GameHallBagWidget()
{
	mBagList = nullptr;
	mBagItemLayer = nullptr;
	mItemIcon = nullptr;
	mItemTitle = nullptr;
	mItemDesc =nullptr;
	mItemOpenDesc = nullptr;
	mCurSelectItem = nullptr;
	mUseFont = nullptr;
	mUserButton = nullptr;
	mProNumber = nullptr;

	mNeedRefresh = false;
	mBagInfoList = nullptr;
	mSelectIndex = 0;
}

GameHallBagWidget::~GameHallBagWidget()
{
	CC_SAFE_RELEASE_NULL(mBagInfoList);
}

std::string GameHallBagWidget::getWidgetJsonName()
{
	return "bagWidget";
}

bool GameHallBagWidget::init()
{
	if(!TNWidget::init())
		return false;

	return true;
}

void GameHallBagWidget::onEnter()
{
	TNWidget::onEnter();

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getBagListMsg, CC_CALLBACK_1(GameHallBagWidget::refreshBagList, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(receiveKnapsackLogMsg, CC_CALLBACK_1(GameHallBagWidget::receiveKnapsackLog, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(useBagMsg, CC_CALLBACK_1(GameHallBagWidget::useBagItem, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(switchBagItemMsg,CC_CALLBACK_1(GameHallBagWidget::switchBagItem, this)), this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(toUserKnascapMsg,CC_CALLBACK_1(GameHallBagWidget::sendUserKnascapRequset, this)), this);
	

	//绑定相关消息
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(userKnapsackMsg, CC_CALLBACK_1(GameHallBagWidget::receiveUserBagRsp, this)), this);
	
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendKnapSackRequest();
	showLoading();
}


void GameHallBagWidget::onExit()
{
	
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallBagWidget::loadUI()
{
	//阴影
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);


	TNWidget::loadUI();

	//绑定相关事件
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBgTag));

	mBagList = static_cast<ListView*>(popBg->getChildByTag(ListViewTag));
	

	mBagItemLayer =  static_cast<Layout*>(mBagList->getChildByTag(LayerOutBagTag));
	//mBagList->setItemModel(mBagItemLayer);
	mBagList->setItemModel(mBagList->getItem(0));
	mBagList->removeAllItems();
	mBagList->insertDefaultItem(0);

	Button* btnClose = static_cast<Button*>(popBg->getChildByTag(BtnOkTag));
	btnClose->addTouchEventListener(CC_CALLBACK_2(GameHallBagWidget::onClose,this));

	ImageView* descBg = static_cast<ImageView*>(popBg->getChildByTag(ImageDescBgTag));
	mItemTitle = static_cast<Text*>(descBg->getChildByTag(LableItemNameTag));

	mItemOpenDesc = static_cast<Text*>(descBg->getChildByTag(LableItemOpenDescTag));
	mItemDesc = static_cast<Text*>(descBg->getChildByTag(LabelItemDescTag));

	mProNumber = static_cast<Text*>(descBg->getChildByTag(LabelItemNumberTag));
	
	ImageView* icon =  static_cast<ImageView*>(descBg->getChildByTag(ImageItemIconTag));
	icon->setVisible(false);
	mItemIcon =  WebImageView::create();
	mItemIcon->setPosition(Vec2(40,40));
	mItemIcon->setPosition(icon->getPosition());
	descBg->addChild(mItemIcon);
	

	mUserButton =  static_cast<Button*>(descBg->getChildByTag(BtnItmeUseTag));
	mUserButton->addTouchEventListener(CC_CALLBACK_2(GameHallBagWidget::onClickUse,this));

	mUseFont =  static_cast<ImageView*>(mUserButton->getChildByTag(ImageUseFontTag));

}

void GameHallBagWidget::onClose(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		ModalViewManager::sharedInstance()->removeWidget(this);
	}
}


void GameHallBagWidget::onClickUse(Ref* pSender,ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if(!mCurSelectItem)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("请先选择一件物品!")));
			return;
		}
		GiftModel* model = (GiftModel*)mCurSelectItem->getData();
		if (model->getGiftExchageType() == 2)
		{
			if(model->getGiftUseType() !=5 && model->getGiftUseType() !=6)//直接使用
			{
				//to do 
				GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendUserKnapsackRequset(model->getGiftId(),1);
				mNeedRefresh = true;
				showLoading();
				return;
			}
		}
		ModalViewManager::sharedInstance()->showWidget(GameHallBuyAlertWidget::create(proType_bag,model));
	}
}


void GameHallBagWidget::refreshBagList(EventCustom* event)
{
	removeLoading();

	__Array* info = (__Array*)event->getUserData();
	if (info != mBagInfoList) {
		CC_SAFE_RELEASE_NULL(mBagInfoList);
		mBagInfoList = info;
		if (NULL != mBagInfoList) {
			mBagInfoList->retain();
		}
	}

	refreshDataList();
}


void GameHallBagWidget::refreshDataList()
{
	int itemCount = 0;
	Layout* lineLayout = nullptr;
	mBagList->removeAllItems();
	bool isCovertNull = true;//兑换成空
	while(itemCount < mBagInfoList->count())
	{
		if (itemCount < colCnt)
		{
			//lineLayout = (Layout*)((Vector<Widget*>)mBagList->getItems()).at(0);
			mBagList->insertDefaultItem(0);
			lineLayout = (Layout*)((Vector<Widget*>)mBagList->getItems()).at(0);
		}
		else
		{
			if (itemCount % colCnt == 0 )
			{
				mBagList->insertDefaultItem(mBagList->getItems().size());
				int cnt = mBagList->getItems().size();
				lineLayout = (Layout*)((Vector<Widget*>)mBagList->getItems()).at(cnt-1);
			}
		}

		float marginLeft = (mBagList->getContentSize().width - (80*4))*1.0f/(4 + 1);
		for (int i = 0 ;i < colCnt ; i++)
		{
			//__Dictionary* dic = (__Dictionary*)(info->getObjectAtIndex(i));
			GiftModel* giftMode = (GiftModel*)mBagInfoList->getObjectAtIndex(itemCount);
			GameHallBagItem* pItem = GameHallBagItem::create(giftMode);
			pItem->setTag(itemCount);

			if (itemCount == mSelectIndex )//选中项
			{
				mCurSelectItem = pItem;
				mCurSelectItem->setSelect(true);
				refreshDescNode();
			}
			

			pItem->ignoreContentAdaptWithSize(false);
			pItem->setContentSize(Size(100,100));
			pItem->setPosition(Vec2((i+1)*pItem->getContentSize().width/2+i*40,pItem->getContentSize().height/2));

			//pItem->addTouchEventListener(CC_CALLBACK_2(GameHallBagWidget::onBagItemPressed, this));
			lineLayout->addChild(pItem);

			itemCount++;
			//当前行，列数不够，退出
			if (itemCount >= mBagInfoList->count() )
				break;;

		}
	}
}
void GameHallBagWidget::receiveKnapsackLog(EventCustom* event)
{
	// to do 你的背包空空如也
	removeLoading();
	CMD_GP_KnapsackLog* info = (CMD_GP_KnapsackLog*)event->getUserData();
	
	isNullBag();
}

void GameHallBagWidget::isNullBag()
{
	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopBgTag));

	ImageView* descBg = static_cast<ImageView*>(popBg->getChildByTag(ImageDescBgTag));
	descBg->setVisible(false);

	Node* noMsgNode = Node::create();
	noMsgNode->setPosition(Vec2(popBg->getContentSize().width/2,popBg->getContentSize().height/2 + 100 ));
	noMsgNode->setRotation(-3);
	popBg->addChild(noMsgNode);

	ImageView* title = ImageView::create();
	title->loadTexture("nulltitle.png",ui::Widget::TextureResType::LOCAL);

	noMsgNode->addChild(title);


	ImageView* nomsg = ImageView::create();
	nomsg->loadTexture("nullbg.png",ui::Widget::TextureResType::LOCAL);
	nomsg->setAnchorPoint(Vec2(0.5,1.0f));
	nomsg->setLocalZOrder(title->getLocalZOrder()-1);
	nomsg->setScale(1.5f);
	noMsgNode->addChild(nomsg);

	Vector<FiniteTimeAction*> actions;
	RotateTo* rb = RotateTo::create(1.0f,6);
	actions.pushBack(rb);
	RotateTo* rb2 = RotateTo::create(1.0f,-6);
	actions.pushBack(rb2);
	noMsgNode->runAction(RepeatForever::create(Sequence::create(actions)));

	//提示文字
	Text* tipLabel = Text::create();
	tipLabel->setAnchorPoint(ccp(0.5, 0.5));
	tipLabel->setFontSize(29);
	tipLabel->setFontName("Arail Regular");
	tipLabel->setTextColor(ccc4(96,33,0,255));
	tipLabel->setText(CommonFunction::GBKToUTF8("您的背包空空如也!"));
	tipLabel->setPosition(Vec2(0,-140));
	noMsgNode->addChild(tipLabel);
}



void GameHallBagWidget::switchBagItem(EventCustom* evt)
{	
	if ((GameHallBagItem*)evt->getUserData() == mCurSelectItem)
	{
		return;
	}
	else
	{
		if (nullptr != mCurSelectItem  )
		{
			mCurSelectItem->setSelect(false);
		}
		GameHallBagItem* cur = (GameHallBagItem*)evt->getUserData();
		mCurSelectItem = cur;
		//刷新每一项的描述信息
		refreshDescNode();
	}
	
}


void GameHallBagWidget::refreshDescNode()
{
	GiftModel* info = mCurSelectItem->getData();
	mItemTitle->setText(CommonFunction::GBKToUTF8(info->getGiftName().c_str()));
	mItemIcon->imageWithUrl(__String::createWithFormat(ImageURLFormat,info->getIconUrl().c_str())->getCString());

	mItemDesc->setText(CommonFunction::GBKToUTF8(info->getSoureDesc().c_str()));
	mItemOpenDesc->setText(CommonFunction::GBKToUTF8(info->getOpenDesc().c_str()));
	__String* num_str = __String::createWithFormat("%s:%d",CommonFunction::GBKToUTF8("数量").c_str(),info->getGiftCount());
	mProNumber->setText(num_str->getCString());

	if (info->getGiftExchageType() == 1)//不能使用，隐藏按钮
	{
		mUserButton->setVisible(false);
	}
	else if(info->getGiftExchageType() == 2)//使用
	{
		if(!mUserButton->isVisible())
			mUserButton->setVisible(true);
		mUseFont->loadTexture("font_use.png",ui::Widget::TextureResType::PLIST);
	}else if(info->getGiftExchageType() == 3)//兑换
	{
		if(!mUserButton->isVisible())
			mUserButton->setVisible(true);
		mUseFont->loadTexture("font_fasttuihuang.png",ui::Widget::TextureResType::PLIST);
	}

}


void GameHallBagWidget::useBagItem(EventCustom* event)
{
	GiftModel* info = mCurSelectItem->getData();
	if (info->getGiftExchageType() == 2)
	{
		if(info->getGiftUseType() !=5 && info->getGiftUseType() !=6)//直接使用
		{
			//to do 
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendUserKnapsackRequset(info->getGiftId(),1);
			return;
		}
	}
	ModalViewManager::sharedInstance()->showWidget(GameHallBuyAlertWidget::create(proType_bag,info));
}


//void GameHallBagWidget::showLoading()
//{
//	if (!getChildByTag(COMMON_LOADING_TAG))
//	{
//		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
//		loading->setAnchorPoint(Vec2(0.5,0.5));
//		loading->setPosition(TNVisibleRect::center());
//		addChild(loading, 5);
//	}
//}
//
//void GameHallBagWidget::removeLoading()
//{
//	if (getChildByTag(COMMON_LOADING_TAG))
//	{
//		removeChildByTag(COMMON_LOADING_TAG, true);
//	}
//}

void GameHallBagWidget::sendUserKnascapRequset(EventCustom* evt)
{
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	int id = ((__Integer*)info->objectForKey("giftId"))->getValue();
	__String* note = (__String*)info->objectForKey("szNote");
	int count = ((__Integer*)info->objectForKey("count"))->getValue();
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendUserKnapsackRequset(id,count,note->getCString());
	int exchangeType = ((__Integer*)info->objectForKey("exchageType"))->getValue();
	if (exchangeType == KnapsackTypeCovert || exchangeType == KnapsackTypeUse)//
	{
		mNeedRefresh = true;
	}
	showLoading();
}


void GameHallBagWidget::receiveUserBagRsp(EventCustom* evt)
{
	removeLoading();
	CMD_GP_UseKnapsackLog* ret = (CMD_GP_UseKnapsackLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(ret->szDescribeString)));
	
	mSelectIndex = mCurSelectItem->getTag();
	
	if (ret->dwRet == 0)//成功
	{
		if (mNeedRefresh)//兑换强制刷新背包
		{
			mCurSelectItem = nullptr;
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendKnapSackRequest();
			showLoading();
			mNeedRefresh  = false;
			return;
		}
		GiftModel* info = mCurSelectItem->getData();
		if (info->getGiftCount()  - ret->dwNum > 0)
		{
			info->setGiftCount(info->getGiftCount() - ret->dwNum);
			mCurSelectItem->refreshData();
			refreshDescNode();
			return;
		}
		else
		{
			mCurSelectItem->removeFromParent();
			mCurSelectItem = nullptr;
			mBagInfoList->removeObjectAtIndex(mSelectIndex);

			if (mSelectIndex ==mBagInfoList->count() && mBagInfoList->count() > 0)//最后一项用完了
			{
				mSelectIndex = mBagInfoList->count() - 1;

				refreshDataList();
			}
			if (mSelectIndex < mBagInfoList->count())//直接刷新
			{
				refreshDataList();
			}
			if( mBagInfoList->count() == 0)//整个背包为空
			{
				//您的背包空空如也
				isNullBag();
			}
		
		}
	}
}