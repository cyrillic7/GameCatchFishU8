/**
 * @file  UpdateWidget
 * @brief  游戏升级界面
 * @author tjl
 * @date 15-09-25
 */
#ifndef __GameUpdateWidget_H_
#define __GameUpdateWidget_H_
#include "Common/TNWidget.h"


class UpdateWidget:public TNWidget
{
public:
	UpdateWidget();
	virtual ~UpdateWidget();
	static UpdateWidget* create( std::string textContent); 
	void onEnter();
	void onExit();

	std::string getWidgetJsonName();
	virtual void loadUI();

private:
	virtual bool init(std::string textContent);
	void onCancel(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onOk(Ref *pSender, ui::Widget::TouchEventType eventType);

	Text* m_contentLabel;
	std::string mContentText;

private:
};

#endif