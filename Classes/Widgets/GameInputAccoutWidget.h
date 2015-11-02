#ifndef __GameInputAccoutWidget_H_H_
#define __GameInputAccoutWidget_H_H_

/**
 * @file GameInputAccoutWidget
 * @brief  游戏大厅找回登录密码界面
 * @author tjl
 * @date 15-6-24
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameInputAccoutWidget : public TNWidget
{
public:
	GameInputAccoutWidget();
	~GameInputAccoutWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameInputAccoutWidget);
	void showLoading();
	void removeLoading();
private:
	void receiveOperateSuccessMsg(EventCustom* evt);
	void receiveOperateFailureMsg(EventCustom* evt);
	void receiveCheckAccountRspMsg(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onOk(Ref* pSender,ui::Widget::TouchEventType type);

protected:
	TextField* mAccountText;
	ui::EditBox* mAccountEdit;
};


#endif//__GameInputAccoutWidget_H_H_