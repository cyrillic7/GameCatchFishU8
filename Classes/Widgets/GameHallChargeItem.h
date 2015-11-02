/**
 * @file GameHallChargeItem
 * @brief  游戏大厅消息界面
 * @author tjl
 * @date 15-6-29
 */
#ifndef __GameHallChargeItem_H_
#define __GameHallChargeItem_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Model/ChargeModel.h"

USING_NS_CC;
using namespace cocos2d::ui;

//充值ITEM 项
class GameHallChargeItem : public Layout
{

public:
	GameHallChargeItem();
	~GameHallChargeItem();
	static GameHallChargeItem* create(ChargeModel* model,int width);
	bool init(ChargeModel* model,int width);
	ChargeModel* getData() const {return mData;}
protected:
	void onClickBuy(Ref* pSender,ui::Widget::TouchEventType type);
	void playScaleAnimation(bool less, Ref* pSender);
	
protected:
	ChargeModel* mData;
};

#endif // __GameHallChargeItem_H_