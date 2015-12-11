/**
 * @file GameHallBuyAlertWidget
 * @brief  游戏大厅购买，兑换二次确认框
 * @author tjl
 * @date 15-7-3
 */

#ifndef _GameHallBuyAlertWidget_H_H_
#define _GameHallBuyAlertWidget_H_H_

#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Model/GiftModel.h"


#define KnapsackTypeUse 2//使用
#define KnapsackTypeCovert 3//兑换
#define KnapsackTypeUserQQ 6 //Q币
#define knapsackTypeUserPhone 5//话费

using namespace cocos2d::ui;
USING_NS_CC;

enum proType //物品类型
{
	proType_shop = 0,//从商城点进来的
	proType_bag,//从背包点进来的
	proType_Auction,//从拍卖点进来的
	proType_MaxCount

};

class GameHallBuyAlertWidget : public TNWidget,public ui::EditBoxDelegate
{
public:
	GameHallBuyAlertWidget();
	~GameHallBuyAlertWidget();
	virtual bool init(proType type,GiftModel* info);
	bool initWithDic(proType type,__Dictionary* info);
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	static GameHallBuyAlertWidget* create(proType type,GiftModel* info);
	static GameHallBuyAlertWidget* createWithDic(proType type,__Dictionary* info);
	void showLoading();
	void removeLoading();

	//---------------------------------添加编辑框  
	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);  
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);  
	virtual void editBoxTextChanged(ui::EditBox* editBox,const std::string &text);  
	virtual void editBoxReturn(ui::EditBox *editBox);  

protected:
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype); 
	void onOK(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onGotoVip(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickMax(Ref* pSender,ui::Widget::TouchEventType eventtype);
private:
	Layout* mShopPanel;
	Layout* mBagPanel;
	Layout* mPhonePanel;
	Layout* mQQPanle;
	Layout* mAuctionPanel;
	Text*   mProTitle;//物品名称
	Text*   mShopProCost;//商城里的物品价格
	GiftModel* mData;
	ImageView*  mOkImage;
	ui::EditBox* mShopInputEdit;//商城里的数量购买
	proType mProType;
	ui::EditBox* mQQEdit;//qq号码
	ui::EditBox* mPhoneEdit;//手机号码
	ui::EditBox* mBagCovertEdit;//背包转换数字
	Text* mBagDesc;//背包兑换描述

	//拍买相关
	__Dictionary* mInfoDic;
	ui::EditBox*  mAuctionEdit;
	Text*         mAuctionCost;

	int           mSingleCost;
protected:

};




#endif //_GameHallBuyAlertWidget_H_H_