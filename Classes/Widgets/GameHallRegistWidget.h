/**
 * @file  GameHallRegisterWidget
 * @brief  游戏注册界面
 * @author tjl
 * @date 14-09-17
 */
#ifndef __GameHallRegisterWidget_H_
#define __GameHallRegisterWidget_H_


#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace ui;


class GameHallRegisterWidget:public TNWidget
{
public:
	
	GameHallRegisterWidget();
	virtual ~GameHallRegisterWidget();
	CREATE_FUNC(GameHallRegisterWidget);
	void onEnter();
	void onExit();

	std::string getWidgetJsonName();
	virtual void loadUI();
	virtual bool init();

private:
	void onRegister(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onBack(Ref *pSender, ui::Widget::TouchEventType eventType);

	bool checkData();//检查数据

	void showLoading();
	void removeLoading();
	void LoginSuccessRsp(EventCustom* evt);
	void LoginFaildRsp(EventCustom* evt);
protected:
	TextField* mFieldAccount;
	TextField* mFieldNick;
	TextField* mFieldPwd;
	ui::EditBox* mAccountEdit;
	ui::EditBox* mPwdEdit;
	ui::EditBox* mNickEdit;
	
};

#endif