#ifndef __GameMatchEnterWidget_H_H_
#define __GameMatchEnterWidget_H_H_

/**
 * @file GameMatchEnterWidget
 * @brief  游戏大厅分享界面
 * @author tjl
 * @date 15-10-25
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "Common/AlertWidget.h"
#include "Model/MatchModel.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameMatchEnterWidget : public TNWidget
{
public:
	GameMatchEnterWidget();
	~GameMatchEnterWidget();
	virtual bool init(MatchInfoModel* info);
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	static GameMatchEnterWidget* create(MatchInfoModel* info);
private:
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onClickSignUp(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	MatchInfoModel* mData;
	Text* mLeftTime;
	Text* mSignUpPlays;
	int mSpanTime;
	void updateLeftTime(float dt);
	void receiveMatchCountDownMsg(EventCustom* evt);
};


#endif//__GameMatchEnterWidget_H_H_