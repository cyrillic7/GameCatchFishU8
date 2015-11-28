/**
 * @file  AlertWidget
 * @brief  游戏大厅公共弹出界面
 * @author tjl
 * @date 14-09-17
 */
#ifndef __GameHallAlertWidget_H_
#define __GameHallAlertWidget_H_
#include "TNWidget.h"

//add by ylx
class AlertDelegate
{
public:
	virtual void okCallback()=0;
};
//end

//modify by ylx
class AlertWidget:public TNWidget
{
public:
	AlertWidget();
	virtual ~AlertWidget();
	static AlertWidget* create(AlertDelegate* pDelegate, std::string textTitle, std::string textContent,bool isTwoButtons = false);  //add by ylx
	void onEnter();
	void onExit();

	std::string getWidgetJsonName();
	virtual void loadUI();

private:
	virtual bool init(AlertDelegate* pDelegate, std::string textTitle, std::string textContent,bool isTwoButtons = false);
	void onClose(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onOk(Ref *pSender, ui::Widget::TouchEventType eventType);

	Text* m_tilteLabel;
	Text* m_contentLabel;
	std::string mTitleText;
	std::string mContentText;
	bool   mBTwoButtons;
private:
	AlertDelegate* m_alertDelegate;  //add by ylx
};

#endif