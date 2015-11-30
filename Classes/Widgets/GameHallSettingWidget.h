/**
 * @file GameHallSettingWidget
 * @brief  主界面的设置弹出小菜单
 * @author tjl
 * @date 14-9-5
 */
#ifndef __GameHallSettingWidget_H_
#define __GameHallSettingWidget_H_

#include "Common/TNWidget.h"
#include "U8sdkFunction.h"

class GameHallSettingWidget:public TNWidget
{
public:
	GameHallSettingWidget();
	virtual ~GameHallSettingWidget();
	
	static GameHallSettingWidget* create(bool isShowBackBtn = false);
	virtual bool init();

	void onEnter();
	void onExit();

	virtual void loadUI();

	virtual std::string getWidgetJsonName();

private:
	//点击空白处
	void onTouchBlank(Ref* pSender,ui::Widget::TouchEventType eventtype);

	//账户
	void onClickHelp(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onModifyPwd(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickPeopleBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onAbout(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onBack(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onSwitchMusic(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onSwitchEffect(Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onChangeAccount(Ref* pSender,ui::Widget::TouchEventType eventtype);

	void playScaleAnimation(bool less, Ref* pSender);
private:
	bool m_bIsShowBackBtn;
	ImageView* m_MusicSwitch;//背景音乐开关
	ImageView* m_EffectSwith;//音效开关
	CU8sdkFunction m_U8sdk;
};

#endif