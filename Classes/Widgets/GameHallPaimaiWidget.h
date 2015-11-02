/**
 * @file GameHallPaimaiWidget
 * @brief  游戏大厅消息界面
 * @author tjl
 * @date 15-5-31
 */

#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Widgets/GameHallPaimaiItem.h"
#include "Common/AlertWidget.h"

using namespace cocos2d::ui;
USING_NS_CC;

enum BankDataType
{
	typePaiMaiInfo = 0,//所有拍卖信息
	typeMyPaiMai,//我的拍卖信息
	typePaiMaiRecord,//拍卖记录
	typeAuctionList,//上架物品列表
	TypeMaxCount
};

class  GameHallPaimaiWidget : public TNWidget,public ui::EditBoxDelegate,public AlertDelegate
{
public:
	GameHallPaimaiWidget();
	~GameHallPaimaiWidget();

	virtual bool init();
	CREATE_FUNC(GameHallPaimaiWidget);

	std::string getWidgetJsonName();
	void onEnter();
	void onExit();

	virtual void loadUI();
	void refreshData(BankDataType dataType,bool isMore = true);
	void showLoading();
	void removeLoading();

	//---------------------------------添加编辑框  
	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);  
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);  
	virtual void editBoxTextChanged(ui::EditBox* editBox,const std::string &text);  
	virtual void editBoxReturn(ui::EditBox *editBox);  

	//提示框回调
	virtual void okCallback();
protected:
	void refreshAuctionList(EventCustom* evt);//刷新所有拍卖信息列表
	void refreshMyAuctionList(EventCustom* evt);//刷新我的拍卖列表
	void refreshAuctionHistoryList(EventCustom* evt);//刷新拍卖记录列表
	void refreshAuctionItemList(EventCustom* evt);//刷新上架物品列表

	void receiveSellAuctionResult(EventCustom* evt);
	void receiveBuyAuctionResult(EventCustom* evt);
	void receiveCancelAuctionResult(EventCustom* evt);
	//获取财富返回处理
	void receiveTreasureInfoMsg(EventCustom* evt);
	//提取拍卖所得返回处理
	void receiveCoverAuctionScoreMsg(EventCustom* evt);
	//发送购买拍卖品请求
	void sendBuyAuctionRequset(EventCustom* evt);

	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onPaimai(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onMybag(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onTakeOut(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onSearch(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickPaimaiRecord(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickMyRecord(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickHistory(Ref* pSender,ui::Widget::TouchEventType eventtype);
	//点击上架商品项
	void onClickSellItem(Ref* pSender,ui::Widget::TouchEventType eventtype);

	void refreshSellData(__Dictionary*data);

	void onListViewScroll(Ref* pSender,ScrollviewEventType type);
protected:
	int mAuctionPageIndex;//记录当前拍卖信息界面的页数
	int mAuctionTotal;
	int mMyAuctionTotal;
	int mHistoryTotal;
	int mMyAuctionPageIndex;
	int mAuctionHistoryIndex;
	ListView* mPaimaiList;
	ListView* mAuctionItemList;
	Text*   mSellItemName;
	TextField*   mSellItemCount;
	ui::EditBox* mAccountEdit;//拍卖数量
	TextField*   mSellItemCost;
	ui::EditBox* mCoseEdit;//拍卖单价
	ImageView*   mSearchTextBg;
	TextField*   mSearchText;
	ui::EditBox* mSearchEdit;//搜索内容
	Button*      mSearchBtn;
	Text*   mLablePaiGet;//拍卖所得
	Text*   mLableBank;//银行元宝数
	Text*   mLableGold;//身上元宝

	Layout*  mTitlePanleInfo;
	Layout*  mTitlePanleMy;
	Layout*  mTitlePanleHistory;

	ImageView*  mTabInfo;
	ImageView*  mTabMy;
	ImageView*  mTabHistory;

	ImageView*  mTextInfo;
	ImageView*  mTextMy;
	ImageView*  mTextHistory;

	int   mPageIndex;
	int   mSearchPageIndex;
	int   mSearchTotal;
	bool  mBSearch;
	GameHallPaimaiSellItem* mCurSelectItem;

};