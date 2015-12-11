/**
 * @file GameHallFirstChargeWidget
 * @brief  游戏大厅首冲好礼界面
 * @author tjl
 * @date 15-6-29
 */
#ifndef _GameHallFirstChargeWidget_H_H_
#define  _GameHallFirstChargeWidget_H_H_

#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameHallFirstChargeWidget :public TNWidget
{
public:
	GameHallFirstChargeWidget();
	~GameHallFirstChargeWidget();
	virtual bool init();
	CREATE_FUNC(GameHallFirstChargeWidget);
	virtual std::string getWidgetJsonName();

	void onEnter();
	void onExit();

	virtual void loadUI();
protected:
	void showLoading();
	void removeLoading();
	void receiveRechargeOrderRspMsg(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onClickCharge(Ref* pSender,ui::Widget::TouchEventType type);
private:
	std::string mCurOrderNumber;
};

#endif //
