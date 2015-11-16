/**
 * @file  GameHallLoginWidget
 * @brief  ÓÎÏ·µÇÂ¼½çÃæ
 * @author tjl
 * @date 14-09-17
 */
#ifndef __GameHallLoginWidget_H_
#define __GameHallLoginWidget_H_


#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace ui;

enum loginDataType
{
	Type_Accout = 0 ,
	Type_Password ,
	Type_EnsurePassword,
	Type_Maxcout
};  


//ÕÊºÅÏî
class AcountItemWiget : public Layout
{
public:
	AcountItemWiget();
	virtual ~AcountItemWiget();
	static AcountItemWiget* create(const char* acount,int width);
	bool init(const char* acount,int width);

	void onClickDelete(Ref *pSender, ui::Widget::TouchEventType eventType);
	void setSelect(bool bShow) {m_shade->setVisible(bShow);}
	std::string getAccount() {return m_acount;}
	std::string getPwd() {return m_pwd;}
private:
	std::string m_acount;
	std::string m_pwd;
	LayerColor* m_shade;

};



class GameHallLoginWidget:public TNWidget,public ui::EditBoxDelegate
{
public:
	
	GameHallLoginWidget();
	virtual ~GameHallLoginWidget();
	CREATE_FUNC(GameHallLoginWidget);
	void onEnter();
	void onExit();

	std::string getWidgetJsonName();
	virtual void loadUI();
	virtual bool init();


	//---------------------------------Ìí¼Ó±à¼­¿ò  
	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);  
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);  
	virtual void editBoxTextChanged(ui::EditBox* editBox,const std::string &text);  
	virtual void editBoxReturn(ui::EditBox *editBox);  

private:
	void onLogin(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onRegister(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onBack(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onForgetPwd(Ref *pSender, ui::Widget::TouchEventType eventType);
	void onClickPanel(Ref* pSender,ui::Widget::TouchEventType eventType);

	void onShowAccountList(Ref *pSender, ui::Widget::TouchEventType eventType);

	void LoginSuccessRsp(EventCustom* evt);
	void removeSelf(EventCustom* evt);
	void LoginFaildRsp(EventCustom* evt);
	bool checkData(loginDataType type);

	void showLoading();
	void removeLoading();

	//
	void onSelectAccout(Ref *pSender, ui::Widget::TouchEventType eventType);
protected:
	TextField* mFieldAccount;
	TextField* mFieldPwd;
	ui::EditBox* mAccountEdit;
	ui::EditBox* mPwdEdit;

	ImageView* mAccountBg;
	ListView*  mAccountList;
	AcountItemWiget* mCurAccountItem;
};

#endif