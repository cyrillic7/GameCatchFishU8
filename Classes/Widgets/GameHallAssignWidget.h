#ifndef __GameHallAssginWidget_H_H_
#define __GameHallAssginWidget_H_H_

/**
 * @file GameHallAssginWidget
 * @brief  游戏大厅签到界面
 * @author tjl
 * @date 15-6-19
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/AlertWidget.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameHallAssginWidget : public TNWidget,public AlertDelegate
{
public:
	GameHallAssginWidget();
	~GameHallAssginWidget();
	virtual bool init();
	virtual void okCallback();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameHallAssginWidget);
	void showLoading();
	void removeLoading();
private:
	void refreshData(EventCustom* evt);
	void receiveAssinRsp(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onAssign(Ref* pSender,ui::Widget::TouchEventType type);
	bool isAwardBag(int tag);
	std::string getImageNameByTag(int tag);
	void dealScoreEffect(SCORE value);
	void removeScoreEffect(Node* node);
	void returnBack(Node* pSender);
protected:
	ImageView* mAssignPath;
	ImageView* mPathBag;
	int        mAssignDay;//当前所签到天数
	CMD_GP_GetSignInTaskInfo mSignInfo;
	__Array*  mItemArray;
};


#endif//__GameHallAssginWidget_H_H_