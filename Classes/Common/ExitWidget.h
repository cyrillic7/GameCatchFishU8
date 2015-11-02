/**
 * @file  AlertWidget
 * @brief  游戏大厅公共弹出界面
 * @author tjl
 * @date 15-07-31
 */
#ifndef __GameHallExitWidget_H_
#define __GameHallExitWidget_H_
#include "TNWidget.h"

class ExitWidget:public TNWidget
{
public:
	ExitWidget();
	virtual ~ExitWidget();
	CREATE_FUNC(ExitWidget);
	void onEnter();
	void onExit();

	std::string getWidgetJsonName();
	virtual void loadUI();
	virtual bool init();
private:
	void onCancel(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onOk(Ref *pSender, ui::Widget::TouchEventType eventType);

private:
	
};

#endif