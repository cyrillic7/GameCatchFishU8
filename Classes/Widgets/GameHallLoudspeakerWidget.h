#ifndef _GAME_HALL_LOUDSPEAKERWIDGEHT_H_H_
#define _GAME_HALL_LOUDSPEAKERWIDGEHT_H_H_

#include "Common/TNWidget.h"
#include "Common/AlertWidget.h"
#include "CommonFunction.h"
#include "SessionManager.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace ui;

class GameHallLoudSpeakerWidget : public TNWidget,public AlertDelegate,public ui::EditBoxDelegate
{
public:
	GameHallLoudSpeakerWidget();
	~GameHallLoudSpeakerWidget();
	CREATE_FUNC(GameHallLoudSpeakerWidget);
	bool init();
	void onEnter();
	void onExit();
	void okCallback();
	void loadUI();
	std::string getWidgetJsonName();
	void onClickSend(Ref* pSender, ui::Widget::TouchEventType touchType);
	void receiveUpdateHornMsg(EventCustom* evt);

	//---------------------------------Ìí¼Ó±à¼­¿ò  
	virtual void editBoxEditingDidBegin(ui::EditBox* editBox);
	virtual void editBoxEditingDidEnd(ui::EditBox* editBox);
	virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string &text);
	virtual void editBoxReturn(ui::EditBox *editBox);
	void showHornContent(Node* pSender);
protected:
	void playScaleAnimation(bool less, Ref* pSender);
	void setScrollViewData();
	cocos2d::Label* createMsgContent(std::string content);
	Text* createMsgText(std::string content);
	void onPanelTouch(Ref* pSender,ui::Widget::TouchEventType touchType);
	void removSelf(Node* pSender);
	void hideHornContent();
	int  callUpHight();
private:
	ui::ScrollView* mHistoryPanel;
	ui::EditBox* mContentEdit;
	ImageView* bg;
};



#endif//_GAME_HALL_LOUDSPEAKERWIDGEHT_H_H_