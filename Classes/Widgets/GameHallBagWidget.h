#ifndef __GameHallBagWidget_H_H_
#define __GameHallBagWidget_H_H_

/**
 * @file GameHallBagWidget
 * @brief  游戏大厅背包界面
 * @author tjl
 * @date 15-6-12
 */

#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Common/WebImageView.h"


class GameHallBagItem;

using namespace cocos2d::ui;
USING_NS_CC;


class GameHallBagWidget :public TNWidget
{
public:
	GameHallBagWidget();
	~GameHallBagWidget();
	virtual bool init();
	CREATE_FUNC(GameHallBagWidget);
	virtual std::string getWidgetJsonName();

	void onEnter();
	void onExit();

	virtual void loadUI();
protected:
	void showLoading();
	void removeLoading();
	void refreshBagList(EventCustom* event);
	void receiveKnapsackLog(EventCustom* event);
	void useBagItem(EventCustom* event);

	void onClose(Ref* pSender,ui::Widget::TouchEventType type);
	void onClickUse(Ref* pSender,ui::Widget::TouchEventType type);
	void switchBagItem(EventCustom* evt);
	void refreshDescNode();

	void sendUserKnascapRequset(EventCustom* evt);
	void receiveUserBagRsp(EventCustom* evt);


	void refreshDataList();
	void isNullBag();
protected:
	ListView* mBagList;
	Text*    mItemTitle;
	Text*   mItemDesc;
	Text*   mItemOpenDesc;
	WebImageView* mItemIcon;
	Layout*   mBagItemLayer;
	GameHallBagItem* mCurSelectItem;
	ImageView*  mUseFont;
	Button*     mUserButton;

	Text*   mProNumber;
	bool    mNeedRefresh;

	__Array*  mBagInfoList;//背包列表数据
	int       mSelectIndex;

};












#endif //__GameHallBagWidget_H_H_