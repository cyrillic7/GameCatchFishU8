/**
 * @file GameHallFeedBackWidget
 * @brief  游戏大厅更多游戏界面
 * @author tjl
 * @date 15-8-4
 */

#ifndef _GameHallMoreGameWidget_H_H_
#define _GameHallMoreGameWidget_H_H_


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;


class moreItem : public TNWidget
{
public:
	moreItem();
	~moreItem();
	static moreItem* create(__Dictionary* info);
	bool init(__Dictionary* info);
private:
	void onClickOk(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	__Dictionary* mData;
};

//////////////////////////////////////////////////////////////////////////
class GameHallMoreGameWidget : public TNWidget
{
public:
	GameHallMoreGameWidget();
	~GameHallMoreGameWidget();
	virtual bool init();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameHallMoreGameWidget);
	void onEnter();
	void onExit();
	void showLoading();
	void removeLoading();
private:
	void onClose(Ref* pSender,ui::Widget::TouchEventType type);
	void receiveMoreGameMsg(EventCustom* evt);
	
protected:
	ListView* mItemList;
};



#endif //_GameHallMoreGameWidget_H_H_