/**
 * @file GameHallMsgItem
 * @brief  游戏大厅消息界面
 * @author tjl
 * @date 15-6-10
 */
#ifndef __GameHallMsgItem_H_
#define __GameHallMsgItem_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/TNWidget.h"

USING_NS_CC;
using namespace cocos2d::ui;

//拍买ITEM 项
class GameHallMsgItem : public Layout
{

public:
	GameHallMsgItem();
	~GameHallMsgItem();
	static GameHallMsgItem* create(__Dictionary* infodic,int width);
	bool init(__Dictionary* infodic,int width);
	__Dictionary* getData() const{return mData;}
protected:
	__Dictionary* mData;
};


class GameHallMsgDescWidget:public TNWidget
{
public:
	GameHallMsgDescWidget();
	virtual ~GameHallMsgDescWidget();
	static GameHallMsgDescWidget* create(std::string textContent); 

	bool init(std::string textContent);
	std::string getWidgetJsonName();
	virtual void loadUI();
	void onClickClose(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	std::string m_content;
};

#endif // GameHallMsgItem