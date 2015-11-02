#ifndef __GameShareWidget_H_H_
#define __GameShareWidget_H_H_

/**
 * @file GameShareWidget
 * @brief  游戏大厅分享界面
 * @author tjl
 * @date 15-10-22
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/AlertWidget.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameShareWidget : public TNWidget
{
public:
	GameShareWidget();
	~GameShareWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameShareWidget);
private:
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onClickShareWx(Ref* pSender,ui::Widget::TouchEventType type);
	void onClickShareFriend(Ref* pSender,ui::Widget::TouchEventType type);
};


#endif//__GameShareWidget_H_H_