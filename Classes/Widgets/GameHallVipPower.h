/**
 * @file GameHallVipPowerWidget
 * @brief  游戏大厅VIP特权界面
 * @author tjl
 * @date 15-6-18
 */

#ifndef _GameHallVipPowerWidget_H_H_
#define _GameHallVipPowerWidget_H_H_


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class VipItemWidget : public TNWidget
{
public:
	enum  ItemType
	{
		type_login = 0,
		type_dayAward,
		type_shopDiscount,
	};
	VipItemWidget();
	~VipItemWidget();
	static VipItemWidget* create(int width,__Dictionary* info,ItemType type);
	bool init(int width,__Dictionary* info,ItemType type);
	void onGetAward(Ref* pSender, ui::Widget::TouchEventType type);
	void onGotoShop(Ref* pSender,ui::Widget::TouchEventType type);
	void refreshData(__Dictionary* info);
	void addItem();
	void playScaleAnimation(bool less, Ref* pSender);
	ItemType getItemType() const{return mType;}
	void setBtnValid();//设置按钮不可用
	void onShopTips(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	void showTips(Ref* pSender);
	void hideTips();
private:
	__Dictionary* mData;
	Text*  mTextLable;//描述文字
	Button* mBtnGet;//领取按钮/前往商城按钮
	ItemType mType;
	ImageView* mImageTips;
};



//////////////////////////////////////////////////////////////////////////

class GameHallVipPowerWidget : public TNWidget
{
public:
	GameHallVipPowerWidget();
	~GameHallVipPowerWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameHallVipPowerWidget);
	void showLoading();
	void removeLoading();
	void recevieGetAwardResultMsg(EventCustom* evt);
	void recevieToSendGetAwardMsg(EventCustom* evt);
	void recevieGotoShopMsg(EventCustom* evt);
	void receiveGotoChargeMsg(EventCustom* evt);
	void receiveDealTipsMsg(EventCustom* evt);
private:
	void playScaleAnimation(bool less, Ref* pSender) ;
	void refreshData(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onCharge(Ref* pSender,ui::Widget::TouchEventType type);
	void onGetGold(Ref* pSender,ui::Widget::TouchEventType type);
	void onGetRed(Ref* pSender,ui::Widget::TouchEventType type);
	void onGotoShop(Ref* pSender,ui::Widget::TouchEventType type);
	void switchVipGrade(Ref* pSender,ui::Widget::TouchEventType type);
	void hideTips(Node* pSender);
	LabelAtlas* mCurVipLabel;
	LabelAtlas* mNexVipLable;
	Text*      mVipDesc;
	ImageView*  mCurVip;
	ImageView*  mCurFontVip;
	ImageView*  mNextFontVip;
	ProgressTimer* mVipProgress;

	__Array*    mVipPowerInfos;
	ListView*   mItemListView;
	Layout*     mTabPanel;

	ImageView*  mSelectTab;
	int         mGetType;//领取类型
	ImageView* mTipsWindow;
	Text*           mTipContent;
};

#endif // _GameHallVipPowerWidget_H_H_
