/**
 * @file GameHallChargeWidget
 * @brief  游戏大厅我要充值界面
 * @author tjl
 * @date 15-6-29
 */
#ifndef _GameHallChargeWidget_H_H_
#define  _GameHallChargeWidget_H_H_

#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Model/ChargeModel.h"
#include "Common/AlertWidget.h"

using namespace cocos2d::ui;
USING_NS_CC;

enum tabType
{
	tab_gold = 0,
	tab_ingot,
	tab_max
};

class GameHallChargeWidget :public TNWidget,public AlertDelegate
{
public:
	GameHallChargeWidget();
	~GameHallChargeWidget();
	virtual bool init(tabType type);
	static GameHallChargeWidget* create(tabType type);
	virtual std::string getWidgetJsonName();

	void onEnter();
	void onExit();

	virtual void okCallback();
	virtual void loadUI();
protected:
	void showLoading();
	void removeLoading();
	void refreshChargeList(EventCustom* event);
	void onClickGold(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickIngot(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickVipPower(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void refreshData(int type);
	void receiveExchangeIngoRspMsg(EventCustom* evt);
	void receiveOperateFailureMsg(EventCustom* evt);
	void refreshTreasure(EventCustom* evt);
	void receiveIngotCoverScoreMsg(EventCustom* evt);
	void receiveRechargeOrderRspMsg(EventCustom* evt);
	void receiveToGetRechargeOrderMsg(EventCustom* evt);
	void receiveRechargeSuccessMsg(EventCustom* evt);
	void switchPanel(tabType type);
private:
	ListView* mChargeList;//充值列表
	int		  mPageIndex;//当前页面索引
	ImageView*  mTabGold;//金币
	ImageView*  mTabIngot;//元宝
	ImageView*  mTextGold;
	ImageView*  mTextIngot;
	Text*       mUserScore;
	Text*       mUserIngot;
	tabType    mTabType;
	__Array*   mDataArray;
	std::string mCurOrderNumber;
	ChargeModel* mCurModel;
	bool        mbIngotExchange;
};

#endif //
