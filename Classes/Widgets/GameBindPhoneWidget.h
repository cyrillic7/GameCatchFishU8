/**
 * @file GameBindPhoneWidget
 * @brief  游戏大厅绑定手机界面
 * @author tjl
 * @date 15-5-31
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;
USING_NS_CC;

class GameBindPhoneWidget :public TNWidget
{
public:
	GameBindPhoneWidget();
	~GameBindPhoneWidget();
	virtual bool init();
	CREATE_FUNC(GameBindPhoneWidget);
	virtual std::string getWidgetJsonName();
	void onEnter();
	void onExit();
	virtual void loadUI();
protected:	
	void onClickBind(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickGetSecurityCode(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void receiveBindPhoneRsp(EventCustom* evt);
	void receiveGetCaptchaRsp(EventCustom* evt);
	void onTimeOut(float dt);
	void initCode();
protected:
	TextField* mPhoneText;
	TextField* mPwdText;
	TextField* mCodeText;
	int        mTimeOut;
	ImageView* mTimeOutImage;
	Text*      mTimeOutLabel;
	Button*	   btn_get;
	ui::EditBox* mEdit1;
	//ui::EditBox* mEdit22;
};
