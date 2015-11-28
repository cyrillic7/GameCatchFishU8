#ifndef __GameModifyPwdWidget_H_H_
#define __GameModifyPwdWidget_H_H_

/**
 * @file GameModifyPwdWidget
 * @brief  修改用户登录密码界面
 * @author tjl
 * @date 15-6-24
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"

using namespace cocos2d::ui;
USING_NS_CC;

//密码类型
enum PwdType
{
	PwdType_Login = 0,
	PwdType_Bank,
	PwdType_MAXCOUNT

};

class GameModifyPwdWidget : public TNWidget
{
public:
	GameModifyPwdWidget();
	~GameModifyPwdWidget();
	virtual bool init(PwdType type);
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	static GameModifyPwdWidget* create(PwdType type);
	void showLoading();
	void removeLoading();
private:
	void receiveOperateSuccessMsg(EventCustom* evt);
	void receiveOperateFailureMsg(EventCustom* evt);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
	void onOk(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	TextField* mOldPwdText;//密码
	TextField* mNewPwdText;//密码
	TextField* mPwdAgainText;//确认密码
	PwdType    mType;
	ui::EditBox* mOldPwdEdit;//旧密码
	ui::EditBox* mNewPwdEdit;//新密码
	ui::EditBox* mEnsureEdit;//二次确认
};


#endif//__GameModifyPwdWidget_H_H_