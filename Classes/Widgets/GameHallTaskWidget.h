/**
 * @file GameHallTaskWidget
 * @brief  游戏大厅任务界面
 * @author tjl
 * @date 15-6-19
 */

#ifndef _GameHallTaskWidget_H_H_
#define _GameHallTaskWidget_H_H_


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"

using namespace cocos2d::ui;
USING_NS_CC;


class GameHallTaskItem : public ui::Layout
{
public:
	GameHallTaskItem();
	~GameHallTaskItem();
	static GameHallTaskItem* create(__Dictionary* info);
    bool init(__Dictionary* info);
	void showLoading();
	void removeLoading();
protected:
	void onGetAward(Ref* pSender,ui::Widget::TouchEventType type);
	int  calItemCount(__Dictionary* info);
	__Dictionary* mdata;
};



class GameHallTaskWidget : public TNWidget
{
public:
	GameHallTaskWidget();
	~GameHallTaskWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameHallTaskWidget);
	void showLoading();
	void removeLoading();
private:
	void playScaleAnimation(bool less, Ref* pSender) ;
	void refreshData(EventCustom* evt);
	void receiveAwardResultMsg(EventCustom* evt);
	void sendAwardRequest(EventCustom* evt);
	void taskSorketConnetFail(EventCustom* evt);
	void receiveLoginSuccessMsg(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onGet(Ref* pSender,ui::Widget::TouchEventType type);//领取
protected:
	std::string mClientName;
	ListView* mGoldTaskList;
	int       mRequestType;//0 请求任务列表，1表示奖励
	int       mTaskId;
	bool    mBGetFail;
	
};

#endif // _GameHallTaskWidget_H_H_
