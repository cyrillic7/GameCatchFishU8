/**
 * @file GameHallFeedBackWidget
 * @brief  游戏大厅反馈界面
 * @author tjl
 * @date 15-8-3
 */

#ifndef _GameHallFeedBackWidget_H_H_
#define _GameHallFeedBackWidget_H_H_


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;


class GameHallFeedBackWidget : public TNWidget
{
public:
	GameHallFeedBackWidget();
	~GameHallFeedBackWidget();
	virtual bool init();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameHallFeedBackWidget);
	void onEnter();
	void onExit();
private:
	
	void onClose(Ref* pSender,ui::Widget::TouchEventType type);
	void onSend(Ref* pSender,ui::Widget::TouchEventType type);
	void showLoading();
	void removeLoading();

	void receiveFeedBackMsg(EventCustom* evt);
protected:
	TextField* mContentText;
	TextField* mQQText;
};



#endif //_GameHallFeedBackWidget
