#include "GameHallPaimaiWidget.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "SessionManager.h"
#include "Common/CommonLoadingWidget.h"
#include "Common/TNVisibleRect.h"
#include "GameServiceClientManager.h"
#include "Message.h"
#include "CommonFunction.h"
#include "Common/ModalViewManager.h"
#include "CommonFunction.h"
#include "Widgets/GameHallBagWidget.h"
#include "ui/UIScrollView.h"
#include "Widgets/GameBindPhoneWidget.h"

#define  per_page_size 5
#define  Tab1Tag 570
#define  Tab1TextTag 572
#define  Tab2Tag 571
#define  Tab2TextTag 573
#define  Tab3Tag 732
#define  Tab3TextTag 733
#define  ImagePopbgTag 18
#define  BtnCloseTag  42
#define  CommonPanleTag 738
#define  LabelAuctionTreasureTag 747  //拍卖所得

#define  BtnGetOutTag  748
#define  ImageSearchTextTag 751
#define  BtnSeartchTag 753
#define  ImageSearchBgTag 751
#define  LableSearchTextTag 752
#define  PanelTitleInfoTag  740
#define  PanelTitleMyTag    773
#define  PanelTitleHistoryTag 785

#define  ImageInsureScoreBg  795
#define  LabelInsureTag      797
#define  imageIngotBg  801
#define  LabelIngotTag 803

#define  PanelPaimaiListTag   574
#define  ListViewPaimaiInFoTag          737

#define  AuctionItemBigBg   886
#define  AuctionItmeSmallBg  900
#define  ListViewAuctionItemTag 901
#define  BtnPaimaiTag  890
#define  BtnMyBagTag   898

#define  LabelSellTextTag 903
#define  ImageSellCountTag 907
#define  ImageSellCostTag  909
#define  LabelSellCountTag 1114
#define  LabelSellCostTag  1113



GameHallPaimaiWidget::GameHallPaimaiWidget()
{
	mAuctionHistoryIndex = 1;
	mAuctionPageIndex  =1;
	mMyAuctionPageIndex = 1;
	mPageIndex = 1;
	mCurSelectItem = nullptr;
	mSearchPageIndex = 1;
	mSearchTotal =  0;
	mBSearch = false;
}


GameHallPaimaiWidget::~GameHallPaimaiWidget()
{

}

bool GameHallPaimaiWidget::init()
{
	if (!TNWidget::init())
		return false;




	return true;
}

std::string GameHallPaimaiWidget::getWidgetJsonName()
{
	return "paimaiWidget";
}

void GameHallPaimaiWidget::loadUI()
{
	LayerColor*  shade = LayerColor::create();
	shade->setColor(ccc3(0,0,0));
	shade->setOpacity(180);
	shade->setPosition(Vec2(0,0));
	addChild(shade);

	TNWidget::loadUI();
	//绑定相关事件
	
	mTabInfo =static_cast<ImageView*>(m_mainWidget->getChildByTag(Tab1Tag));
	mTabInfo->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onClickPaimaiRecord, this));
	mTextInfo = static_cast<ImageView*>(mTabInfo->getChildByTag(Tab1TextTag));

	mTabMy =static_cast<ImageView*>(m_mainWidget->getChildByTag(Tab2Tag));
	mTabMy->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onClickMyRecord, this));

	mTextMy = static_cast<ImageView*>(mTabMy->getChildByTag(Tab2TextTag));

	mTabHistory =static_cast<ImageView*>(m_mainWidget->getChildByTag(Tab3Tag));
	mTabHistory->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onClickHistory, this));

	mTextHistory = static_cast<ImageView*>(mTabHistory->getChildByTag(Tab3TextTag));

	ImageView* popBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImagePopbgTag));
	Button* btnClose = static_cast<Button*>(popBg->getChildByTag(BtnCloseTag));
	btnClose->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onBack, this));

	Layout* pCommonPanel = static_cast<Layout*>(popBg->getChildByTag(CommonPanleTag));
	Button* btnTakeOut = static_cast<Button*>(pCommonPanel->getChildByTag(BtnGetOutTag));
	btnTakeOut->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onTakeOut, this));

	mLablePaiGet = static_cast<Text*>(pCommonPanel->getChildByTag(LabelAuctionTreasureTag));

	ImageView* insureBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(ImageInsureScoreBg));
	mLableBank =  static_cast<Text*>(insureBg->getChildByTag(LabelInsureTag));

	ImageView* ybBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(imageIngotBg));
	mLableGold =  static_cast<Text*>(ybBg->getChildByTag(LabelIngotTag));

	mSearchTextBg =  static_cast<ImageView*>(pCommonPanel->getChildByTag(ImageSearchTextTag));
	mSearchText = static_cast<TextField*>(mSearchTextBg->getChildByTag(LableSearchTextTag));

	mSearchEdit= ui::EditBox::create(mSearchText->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mSearchEdit->setPosition(Vec2(mSearchText->getContentSize().width/2,mSearchText->getContentSize().height/2));
	mSearchEdit->setFontSize(mSearchText->getFontSize());
	mSearchEdit->setFontColor(mSearchText->getColor());//设置文字颜色

	mSearchEdit->setPlaceHolder(mSearchText->getPlaceHolder().c_str());//点位符
	mSearchEdit->setPlaceholderFontSize(mSearchText->getFontSize());
	mSearchEdit->setText(mSearchText->getStringValue().c_str());

	mSearchText->addChild(mSearchEdit,10);

	mSearchBtn = static_cast<Button*>(pCommonPanel->getChildByTag(BtnSeartchTag));
	mSearchBtn->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onSearch, this));

	mTitlePanleInfo = static_cast<Layout*>(popBg->getChildByTag(PanelTitleInfoTag));
	mTitlePanleMy = static_cast<Layout*>(popBg->getChildByTag(PanelTitleMyTag));
	mTitlePanleHistory = static_cast<Layout*>(popBg->getChildByTag(PanelTitleHistoryTag));

	mPaimaiList = static_cast<ListView*>(popBg->getChildByTag(ListViewPaimaiInFoTag));
	mPaimaiList->addEventListenerScrollView(this ,SEL_ScrollViewEvent(&GameHallPaimaiWidget::onListViewScroll));

	ImageView* itemBg = static_cast<ImageView*>(m_mainWidget->getChildByTag(AuctionItemBigBg));
	ImageView* frameBg = static_cast<ImageView*>(itemBg->getChildByTag(AuctionItmeSmallBg));
	mAuctionItemList = static_cast<ListView*>(frameBg->getChildByTag(ListViewAuctionItemTag));
	mAuctionItemList->removeAllChildren();

	mSellItemName = static_cast<Text*>(itemBg->getChildByTag(LabelSellTextTag));

	ImageView* countBg = static_cast<ImageView*>(itemBg->getChildByTag(ImageSellCountTag));
	mSellItemCount =  static_cast<TextField*>(countBg->getChildByTag(LabelSellCountTag));
	mSellItemCount->setString("");

	mAccountEdit= ui::EditBox::create(mSellItemCount->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mAccountEdit->setPosition(Vec2(mSellItemCount->getContentSize().width/2,mSellItemCount->getContentSize().height/2));
	mAccountEdit->setFontSize(mSellItemCount->getFontSize());
	mAccountEdit->setFontColor(mSellItemCount->getColor());//设置文字颜色

	mAccountEdit->setPlaceHolder(mSellItemCount->getPlaceHolder().c_str());//点位符
	mAccountEdit->setPlaceholderFontSize(mSellItemCount->getFontSize());
	mAccountEdit->setText("1");
	mAccountEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	mAccountEdit->setMaxLength(4);
	mAccountEdit->setDelegate(this);

	mSellItemCount->addChild(mAccountEdit, 10);


	ImageView* costBg = static_cast<ImageView*>(itemBg->getChildByTag(ImageSellCostTag));
	mSellItemCost =  static_cast<TextField*>(costBg->getChildByTag(LabelSellCostTag));
	mSellItemCost->setString("");

	mCoseEdit= ui::EditBox::create(mSellItemCost->getContentSize(), cocos2d::ui::Scale9Sprite::create("edit_box.png"));
	mCoseEdit->setPosition(Vec2(mSellItemCost->getContentSize().width/2,mSellItemCost->getContentSize().height/2));
	mCoseEdit->setFontSize(mSellItemCount->getFontSize());
	mCoseEdit->setFontColor(mSellItemCount->getColor());//设置文字颜色

	mCoseEdit->setPlaceHolder(mSellItemCost->getPlaceHolder().c_str());//点位符
	mCoseEdit->setPlaceholderFontSize(mSellItemCost->getFontSize());
	mCoseEdit->setText("0");
	mCoseEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	mCoseEdit->setMaxLength(9);
	mCoseEdit->setDelegate(this);

	mSellItemCost->addChild(mCoseEdit,10);


	Button* btnPaimai = static_cast<Button*>(m_mainWidget->getChildByTag(BtnPaimaiTag));
	btnPaimai->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onPaimai, this));

	Button* btnMybag = static_cast<Button*>(m_mainWidget->getChildByTag(BtnMyBagTag));
	btnMybag->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onMybag, this));

}

void GameHallPaimaiWidget::onEnter()
{
	TNWidget::onEnter();

	SessionManager::shareInstance()->setNewAuctionTag(false);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRedPointMsg);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getAuctoinRecord, CC_CALLBACK_1(GameHallPaimaiWidget::refreshAuctionList, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getMyAuctionRecord, CC_CALLBACK_1(GameHallPaimaiWidget::refreshMyAuctionList, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getAuctionHistoryRecord, CC_CALLBACK_1(GameHallPaimaiWidget::refreshAuctionHistoryList, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(getAuctionList, CC_CALLBACK_1(GameHallPaimaiWidget::refreshAuctionItemList, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(sellAuctionResultMsg, CC_CALLBACK_1(GameHallPaimaiWidget::receiveSellAuctionResult, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(buyAuctionResultMsg, CC_CALLBACK_1(GameHallPaimaiWidget::receiveBuyAuctionResult, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(cancelAuctionResultMsg, CC_CALLBACK_1(GameHallPaimaiWidget::receiveCancelAuctionResult, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(treasureInfoMsg, CC_CALLBACK_1(GameHallPaimaiWidget::receiveTreasureInfoMsg, this)), this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(converAuctionScoreMsg, CC_CALLBACK_1(GameHallPaimaiWidget::receiveCoverAuctionScoreMsg, this)), this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(toBuyAuctionMsg, CC_CALLBACK_1(GameHallPaimaiWidget::sendBuyAuctionRequset, this)), this);
	
	
	refreshData(typePaiMaiInfo,false);

	//请求上架物品列表
	refreshData(typeAuctionList,false);

	//获取用户财富信息
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendTreasureRequest();
}

void GameHallPaimaiWidget::onExit()
{
	_eventDispatcher->removeEventListenersForTarget(this);
	TNWidget::onExit();
}

void GameHallPaimaiWidget::refreshData(BankDataType dataType,bool isMore)
{
	int subCmd = -1;
	int userId = SessionManager::shareInstance()->getUserId();
	int pageIndex =1;
	switch (dataType)
	{
	case typePaiMaiInfo://拍卖信息
		{
			 subCmd = SUB_GP_AUCTION_RECORD;
			 userId = 0;
			 if(isMore)
				pageIndex= mAuctionPageIndex;
			 else
				mAuctionPageIndex = 1;//切换界面置回初始值
		}
		break;
	case typeMyPaiMai://我的拍卖
		{
			subCmd = SUB_GP_MYAUCTION_RECORD;
			if(isMore)
				pageIndex = mMyAuctionPageIndex;
			else
				mMyAuctionPageIndex = 1;//切换界面置回初始值
		}
		break;
	case typePaiMaiRecord://历史记录
		{
			subCmd = SUB_GP_AUCTION_HISTORY_RECORD;
			if(isMore)
				pageIndex =  mAuctionHistoryIndex;
			else
				mAuctionHistoryIndex = 1;//切换界面置回初始值
		}
		break;
	case typeAuctionList://上架物品列表
		{
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->SendAuctionListRequset();
			return;
		}
		break;
	default:
		break;
	}
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->SendAuctionRequset(subCmd,pageIndex,per_page_size,userId);

	showLoading();


}


void GameHallPaimaiWidget::showLoading()
{
	if (!getChildByTag(COMMON_LOADING_TAG))
	{
		CommonLoadingWidget* loading = CommonLoadingWidget::create(Size(TNVisibleRect::getVisibleRect().size.width, TNVisibleRect::getVisibleRect().size.height));
		loading->setAnchorPoint(Vec2(0.5,0.5));
		loading->setPosition(TNVisibleRect::center());
		addChild(loading, 5);
	}
}

void GameHallPaimaiWidget::removeLoading()
{
	if (getChildByTag(COMMON_LOADING_TAG))
	{
		removeChildByTag(COMMON_LOADING_TAG, true);
	}
}

void GameHallPaimaiWidget::refreshAuctionHistoryList(EventCustom* evt)
{
	removeLoading();

	__Dictionary* dic = (__Dictionary*)evt->getUserData();
	int curPageIndex = ((__Integer*)dic->objectForKey("curPage"))->getValue();
	mHistoryTotal = ((__Integer*)dic->objectForKey("maxPage"))->getValue();
	if (mPageIndex != 3 || curPageIndex == 1)
	{
		mPaimaiList->removeAllChildren();
	}
	__Array* pArray = (__Array*)dic->objectForKey("items");

	if (pArray->count() > 0)
	{
		for (int i = 0; i < pArray->count(); i++)
		{
			__Dictionary* infoDic = (__Dictionary*)pArray->getObjectAtIndex(i);
			GameHallPaimaiHistoryItem* pItem = GameHallPaimaiHistoryItem::create(infoDic,mPaimaiList->getContentSize().width);
			mPaimaiList->pushBackCustomItem(pItem);
		}
	}
	mPageIndex = 3;
}

void GameHallPaimaiWidget::refreshAuctionList(EventCustom* evt)
{
	removeLoading();

	__Dictionary* dic = (__Dictionary*)evt->getUserData();
	int curPageIndex = ((__Integer*)dic->objectForKey("curPage"))->getValue();
	if(!mBSearch)
		mAuctionTotal = ((__Integer*)dic->objectForKey("maxPage"))->getValue();
	else if(mBSearch && curPageIndex == 1)//搜索第一页的时候，记录总的搜索页
		mSearchTotal = ((__Integer*)dic->objectForKey("maxPage"))->getValue();
	if (mPageIndex != 1 || curPageIndex == 1 || (mBSearch && curPageIndex == 1))//处于搜索第一页就得清
	{
		mPaimaiList->removeAllChildren();
	}
	__Array* pArray = (__Array*)dic->objectForKey("items");

	if (pArray->count() > 0)
	{
		for (int i = 0; i < pArray->count(); i++)
		{
			__Dictionary* infoDic = (__Dictionary*)pArray->getObjectAtIndex(i);
			GameHallPaimaiInfoItem* pItem = GameHallPaimaiInfoItem::create(infoDic,mPaimaiList->getContentSize().width);
			mPaimaiList->pushBackCustomItem(pItem);
		}
	}
	mPageIndex = 1;

}

void GameHallPaimaiWidget::refreshMyAuctionList(EventCustom* evt)
{
	removeLoading();

	__Dictionary* dic = (__Dictionary*)evt->getUserData();
	int curPageIndex = ((__Integer*)dic->objectForKey("curPage"))->getValue();
	mMyAuctionTotal = ((__Integer*)dic->objectForKey("maxPage"))->getValue();
	if (mPageIndex != 2 || curPageIndex == 1)
	{
		mPaimaiList->removeAllChildren();
	}
	__Array* pArray = (__Array*)dic->objectForKey("items");

	if (pArray->count() > 0)
	{
		for (int i = 0; i < pArray->count(); i++)
		{
			__Dictionary* infoDic = (__Dictionary*)pArray->getObjectAtIndex(i);
			GameHallMyPaiamiItem* pItem = GameHallMyPaiamiItem::create(infoDic,mPaimaiList->getContentSize().width);
			mPaimaiList->pushBackCustomItem(pItem);
		}
	}
	mPageIndex = 2;
}


//刷新上架物品列表
void GameHallPaimaiWidget::refreshAuctionItemList(EventCustom* evt)
{
	mAuctionItemList->removeAllChildren();
	__Array* infoArray  = (__Array*)evt->getUserData();
	for (int i = 0 ; i < infoArray->count();i++)
	{
		__Dictionary* infoDic = (__Dictionary*)infoArray->getObjectAtIndex(i);
		GameHallPaimaiSellItem* pSell = GameHallPaimaiSellItem::create(infoDic,mAuctionItemList->getContentSize().width);
		pSell->addTouchEventListener(CC_CALLBACK_2(GameHallPaimaiWidget::onClickSellItem,this));
		if(i == 0)//默认选中第一个
		{
			pSell->setSelect();
			mCurSelectItem = pSell;
			refreshSellData(pSell->getData());
		}
		mAuctionItemList->pushBackCustomItem(pSell);
	}

}

void GameHallPaimaiWidget::refreshSellData(__Dictionary* data)
{
	__String* nameStr = (__String*)data->objectForKey("propName");
	mSellItemName->setText(CommonFunction::GBKToUTF8(nameStr->getCString()));
}



void GameHallPaimaiWidget::onClickSellItem(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		GameHallPaimaiSellItem* pSell = (GameHallPaimaiSellItem*)pSender;
		if (pSell->isSelect())
			return;
		else
		{
			Vector<Widget*> items = mAuctionItemList->getItems();
			for (int i = 0 ; i < items.size();i++)
			{
				GameHallPaimaiSellItem* pItem = (GameHallPaimaiSellItem*)items.at(i);
				if(pItem->isSelect())
					pItem->unSelect();
			}

			pSell->setSelect();
			mCurSelectItem = pSell;
			refreshSellData(pSell->getData());
		}

		mSearchPageIndex = 1;
	}
}


void GameHallPaimaiWidget::onBack(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		removeFromParent();
	}
}

void GameHallPaimaiWidget::onPaimai(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		//开始上架
		__Dictionary* dic = __Dictionary::create();
		if (!mCurSelectItem)
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("请先选择上架物品!")));
			return;
		}

		OnlineUserModel* user = SessionManager::shareInstance()->getUser();
		if (  user->getPhone().length()  == 0 || (user->getPhone().length() > 0 && strcmp(user->getPhone().c_str()," ") ==0)  )
		{
			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(this,"",CommonFunction::GBKToUTF8("请绑定手机后才能拍卖道具哦~")));
			return;
		}
		__Dictionary* sellData = mCurSelectItem->getData();
		
		int propId = ((__Integer*)sellData->objectForKey("propId"))->getValue();
		dic->setObject(__Integer::create(propId),"index");

		
		dic->setObject(__Integer::create(atoi(mAccountEdit->getText())),"propNum");

		dic->setObject(__Integer::create(atoi(mCoseEdit->getText())),"cost");

		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendSellAuctionRequest(dic);

		
	}
}

void GameHallPaimaiWidget::okCallback()
{
	ModalViewManager::sharedInstance()->showWidget(GameBindPhoneWidget::create());
}

void GameHallPaimaiWidget::onMybag(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		ModalViewManager::sharedInstance()->showWidget(GameHallBagWidget::create());
	}
}

void GameHallPaimaiWidget::onTakeOut(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendConvertAutionScoreRequest();
		showLoading();
	}
}

void GameHallPaimaiWidget::onSearch(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (strlen(mSearchEdit->getText()) ==0)
		{

			ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8("搜索内容不能为空!")));
			return ;
		}
		mBSearch = true;
		GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendQueryAuctionRequest(1,per_page_size,(char*)CommonFunction::UTF8TOGBK((char*)mSearchEdit->getText()).c_str());
	}
}

void GameHallPaimaiWidget::onClickPaimaiRecord(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex !=1)
		{
			Vec2 pos = mTabInfo->getPosition();
			mTabInfo->setPosition(Vec2(pos.x,pos.y+4));
			mTabInfo->loadTexture("tab.png",ui::Widget::TextureResType::PLIST);
			mTextInfo->loadTexture("paimai_info.png",ui::Widget::TextureResType::PLIST);

			mTabMy->loadTexture("tab_on.png",ui::Widget::TextureResType::PLIST);
			mTabMy->setPosition(Vec2(mTabMy->getPositionX(),pos.y));
			mTextMy->loadTexture("font_mypaimai_an.png",ui::Widget::TextureResType::PLIST);

			mTabHistory->loadTexture("tab_on.png",ui::Widget::TextureResType::PLIST);
			mTabHistory->setPosition(Vec2(mTabHistory->getPositionX(),pos.y));
			mTextHistory->loadTexture("font_record_an.png",ui::Widget::TextureResType::PLIST);

			mTitlePanleInfo->setVisible(true);
			mTitlePanleHistory->setVisible(false);
			mTitlePanleMy->setVisible(false);

			mSearchTextBg->setVisible(true);
			mSearchBtn->setVisible(true);

			refreshData(typePaiMaiInfo,false);

			mBSearch = false;
			mSearchPageIndex = 1;
		}
	}
}

void GameHallPaimaiWidget::onClickMyRecord(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex !=2)
		{
			Vec2 pos = mTabMy->getPosition();
			mTabInfo->loadTexture("tab_on.png",ui::Widget::TextureResType::PLIST);
			mTabInfo->setPosition(Vec2(mTabInfo->getPositionX(),pos.y));
			mTextInfo->loadTexture("font_pminfo_an.png",ui::Widget::TextureResType::PLIST);

			mTabMy->loadTexture("tab.png",ui::Widget::TextureResType::PLIST);
			mTabMy->setPosition(Vec2(mTabMy->getPositionX(),pos.y+4));
			mTextMy->loadTexture("font_mypaimai.png",ui::Widget::TextureResType::PLIST);

			mTabHistory->loadTexture("tab_on.png",ui::Widget::TextureResType::PLIST);
			mTabHistory->setPosition(Vec2(mTabHistory->getPositionX(),pos.y));
			mTextHistory->loadTexture("font_record_an.png",ui::Widget::TextureResType::PLIST);


			mTitlePanleInfo->setVisible(false);
			mTitlePanleHistory->setVisible(false);
			mTitlePanleMy->setVisible(true);

			mSearchTextBg->setVisible(false);
			mSearchBtn->setVisible(false);

			refreshData(typeMyPaiMai,false);

			mBSearch = false;
			mSearchPageIndex = 1;
		}
	}
}

void GameHallPaimaiWidget::onClickHistory(Ref* pSender,ui::Widget::TouchEventType eventtype)
{
	if (ui::Widget::TouchEventType::ENDED == eventtype)
	{
		if (mPageIndex !=3)
		{
			Vec2 pos = mTabHistory->getPosition();
			mTabInfo->loadTexture("tab_on.png",ui::Widget::TextureResType::PLIST);
			mTabInfo->setPosition(Vec2(mTabInfo->getPositionX(),pos.y));
			mTextInfo->loadTexture("font_pminfo_an.png",ui::Widget::TextureResType::PLIST);

			mTabMy->loadTexture("tab_on.png",ui::Widget::TextureResType::PLIST);
			mTabMy->setPosition(Vec2(mTabMy->getPositionX(),pos.y));
			mTextMy->loadTexture("font_mypaimai_an.png",ui::Widget::TextureResType::PLIST);

			mTabHistory->loadTexture("tab.png",ui::Widget::TextureResType::PLIST);
			mTabHistory->setPosition(Vec2(mTabHistory->getPositionX(),pos.y+4));
			mTextHistory->loadTexture("paimai_record.png",ui::Widget::TextureResType::PLIST);


			mTitlePanleInfo->setVisible(false);
			mTitlePanleHistory->setVisible(true);
			mTitlePanleMy->setVisible(false);

			mSearchTextBg->setVisible(false);
			mSearchBtn->setVisible(false);

			refreshData(typePaiMaiRecord,false);
			mBSearch =false;
			mSearchPageIndex = 1;
		}
	}
}


void GameHallPaimaiWidget::receiveSellAuctionResult(EventCustom* evt)
{
	CMD_GP_Sell_AuctionLog* selllog = (CMD_GP_Sell_AuctionLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(selllog->szDescribeString)));

	if (selllog->dwRet == 0)//成功
	{
		
		Vector<Widget*> items = mAuctionItemList ->getItems();
		__Dictionary* selData  = mCurSelectItem->getData();
		int selPropId = ((__Integer*)selData->objectForKey("propId"))->getValue();

		for (int i = 0 ; i < items.size();i++)
		{
			GameHallPaimaiSellItem* pItem = (GameHallPaimaiSellItem*)items.at(i);
			__Dictionary* infoDic = pItem->getData();
			int propId = ((__Integer*)infoDic->objectForKey("propId"))->getValue();
			int propCnt = ((__Integer*)infoDic->objectForKey("propCnt"))->getValue();

			
			if (propId == selPropId)
			{
				if(int(selllog->dwNum) - propCnt >=0)
				{
					pItem->removeFromParent();
					items.erase(i);
					mCurSelectItem = nullptr;
					mSellItemName->setText(CommonFunction::GBKToUTF8("无"));
				}
				else
				{
					pItem->refreshData(propCnt - int(selllog->dwNum));
				}
			}
		}

		mBSearch = false;
		mSearchPageIndex = 1;

		if (mPageIndex == 1)
		{
			refreshData(typePaiMaiInfo,false);
		}
		else if(mPageIndex ==2)
		{
			refreshData(typeMyPaiMai,false);
		}

	}

}

void GameHallPaimaiWidget::receiveBuyAuctionResult(EventCustom* evt)
{
	removeLoading();
	CMD_GP_Buy_AuctionLog* buylog = (CMD_GP_Buy_AuctionLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(buylog->szDescribeString)));

	if (buylog->dwRet == 0 )//购买成功
	{
		
		if(mBSearch)
			GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendQueryAuctionRequest(mSearchPageIndex,per_page_size,(char*)CommonFunction::UTF8TOGBK((char*)mSearchEdit->getText()).c_str());
		else
			refreshData(typePaiMaiInfo,false);
		refreshData(typeAuctionList,false);

		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateUserInfoFromAuciontMsg,buylog);
	}
	

	
}

void GameHallPaimaiWidget::receiveCancelAuctionResult(EventCustom* evt)
{
	CMD_GP_Cancel_AuctionLog* cancellog = (CMD_GP_Cancel_AuctionLog*)evt->getUserData();
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(cancellog->szDescribeString)));
	if (cancellog->dwRet == 0)//成功
	{
		Vector<Widget*> items = mPaimaiList->getItems();
		for (int i = 0 ; i < items.size();i++)
		{
			GameHallMyPaiamiItem* pItem = (GameHallMyPaiamiItem*)items.at(i);
			__Dictionary* infoDic = pItem->getData();
			int propId = ((__Integer*)infoDic->objectForKey("index"))->getValue();
			if (propId == cancellog->dwID)
			{
				pItem->removeFromParent();
				items.erase(i);
			}
		}
		//刷新上架物品列表
		refreshData(typeAuctionList,false);
	}

}

void GameHallPaimaiWidget::onListViewScroll(Ref* pSender,ScrollviewEventType type)
{
	if (type == ScrollviewEventType::SCROLLVIEW_EVENT_SCROLL_TO_BOTTOM)
	{
		if(!mBSearch)
		{
			switch (mPageIndex)
			{
			case 1:
				{
					if(mAuctionPageIndex < mAuctionTotal)
					{
						mAuctionPageIndex++;
						refreshData(typePaiMaiInfo);
					}
				};
				break;
			case 2:
				{
					if(mMyAuctionPageIndex < mMyAuctionTotal)
					{
						mMyAuctionPageIndex++;
						refreshData(typeMyPaiMai);
					}
			
				};
				break;
			case 3:
				{
					if(mAuctionHistoryIndex < mHistoryTotal)
					{
						mAuctionHistoryIndex++;
						refreshData(typePaiMaiRecord);
					}
				};
				break;
			default:
				break;
			}
		}
		else
		{
			if (mSearchPageIndex < mSearchTotal)
			{
				mSearchPageIndex++;
				GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendQueryAuctionRequest(mSearchPageIndex,per_page_size,(char*)CommonFunction::UTF8TOGBK((char*)mSearchEdit->getText()).c_str());
			}
		}
	}
}

//获取财富返回处理
void GameHallPaimaiWidget::receiveTreasureInfoMsg(EventCustom* evt)
{
	CMD_GP_UserTreasure* treasureInfo = (CMD_GP_UserTreasure*)evt->getUserData();
	mLablePaiGet->setString(numberToString(treasureInfo->lAuctionScore));
	mLableBank->setString(numberToString(treasureInfo->lInsureScore));
	mLableGold->setString(numberToString(treasureInfo->lIngotScore));
}

//提取拍卖所得返回处理
void GameHallPaimaiWidget::receiveCoverAuctionScoreMsg(EventCustom* evt)
{
	removeLoading();
	CMD_GP_ConversionAuctionScore* info = (CMD_GP_ConversionAuctionScore*)evt->getUserData();
	if(info->lResultCode == 0)
	{
		mLableBank->setString(numberToString(info->lInsureScore));
		//清空拍卖所得
		mLablePaiGet->setString(numberToString(0));
	}
	ModalViewManager::sharedInstance()->showWidget(AlertWidget::create(nullptr,"",CommonFunction::GBKToUTF8(info->szDescribeString)));
}

void GameHallPaimaiWidget::sendBuyAuctionRequset(EventCustom* evt)
{
	__Dictionary* info = (__Dictionary*)evt->getUserData();
	GameServiceClientManager::sharedInstance()->getCurrentServiceClient()->sendBuyAuctionRequest(info);
	showLoading();
}

void GameHallPaimaiWidget::editBoxEditingDidBegin(ui::EditBox *editBox)    
{    
	CCLOG("start edit"); 

}

void GameHallPaimaiWidget::editBoxEditingDidEnd(ui::EditBox *editBox)    
{    
	CCLOG("end edit");    
	auto editbox = (ui::EditBox*)editBox;  
	long value =strtol(editbox->getText(),NULL,10);
	editbox->setText(numberToString(value).c_str());

} 

void GameHallPaimaiWidget::editBoxReturn(ui::EditBox *editBox)    
{    
	CCLOG("editbox return");    
}    
void GameHallPaimaiWidget::editBoxTextChanged(ui::EditBox *editBox,const std::string &text)    
{     

} 

