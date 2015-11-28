#ifndef __GameGiftCovertWidget_H_H_
#define __GameGiftCovertWidget_H_H_

/**
 * @file GameGiftCovertWidget
 * @brief  游戏大厅礼包兑换界面
 * @author tjl
 * @date 15-11-6
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/AlertWidget.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameGiftCovertWidget : public TNWidget, public AlertDelegate
{
public:
	GameGiftCovertWidget();
	~GameGiftCovertWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameGiftCovertWidget);
	void showLoading();
	void removeLoading();
	virtual void okCallback();
private:
	void receiveOperateSuccessMsg(EventCustom* evt);
	void receiveOperateFailureMsg(EventCustom* evt);
	void receivegiftConvertRspMsg(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onOk(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	TextField* mPwdText;
	ui::EditBox* mPwdEdit;
};


#endif//__GameEnterBank_H_H_