/**
 * @file GameForgetWidget
 * @brief  游戏大厅忘记密码界面
 * @author tjl
 * @date 15-7-3
 */
#ifndef __GameForgetWidget_H_H_
#define __GameForgetWidget_H_H_



#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Common/AlertWidget.h"

using namespace ui;
USING_NS_CC;

class GameForgetWidget :public TNWidget,public AlertDelegate
{
public:
	GameForgetWidget();
	~GameForgetWidget();
	virtual bool init(int pwdType,char* phone);
	static GameForgetWidget* create(int pwdType,char* phone = nullptr); // 0 为登录密码，1为银行密码
	virtual std::string getWidgetJsonName();
	void onEnter();
	void onExit();
	virtual void loadUI();
	void showLoading();
	void removeLoading();
	void onTimeOut(float dt);

	virtual void okCallback();

	//专门为登录使用
	void setAccount(const char* account);
protected:	
	void onClickModify(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickGetSecurityCode(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void receiveModifyPwdRsp(EventCustom* evt);
protected:
	int   mPwdType;
	TextField* mPwdText;
	TextField* mCodeText;
	std::string m_phone;
	ImageView* mTimeOutImage;
	int mTimeOut ;
	Text* mTimeOutLabel;
	Button* btn_get ;
	std::string m_account;

	//
	ui::EditBox* mPwdEdit;
	ui::EditBox* mCodeEdit;

};

#endif // !__GameForgetWidget_H_H_