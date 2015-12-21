#ifndef _GAME_HALL_LUCKSPINWIDGET_H_H_
#define _GAME_HALL_LUCKSPINWIDGET_H_H_

#include "Common/TNWidget.h"
#include "Common/AlertWidget.h"
#include "CommonFunction.h"
#include "SessionManager.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CCArmatureDataManager.h"
#include "cocostudio/CCArmatureAnimation.h"
#include "cocostudio/CocoStudio.h"
using namespace ui;
using namespace cocostudio;


class GameHallLuckSpinWidget : public TNWidget,public AlertDelegate
{
public:
	GameHallLuckSpinWidget();
	~GameHallLuckSpinWidget();
	CREATE_FUNC(GameHallLuckSpinWidget);
	bool init();
	void onEnter();
	void onExit();
	void loadUI();
	void okCallback();
	std::string getWidgetJsonName();
	void onClickSpin(Ref* pSender, ui::Widget::TouchEventType touchType);
	void onClose(Ref* pSender, ui::Widget::TouchEventType touchType);
	void receiveSpinRspMsg(EventCustom* evt);
protected:
	void spinAnimationFinsh(Node* pSender);
	void showReward(float dt);
	void onAnimationEventOver(Armature *armature, MovementEventType movementType, const std::string& movementID);
	void onAnimationEventFrame(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
private:
	ImageView* mSpinArrow;//转盘指针
	Button*    mBtnGo;//开始按键
	int dialIndex;								//指针奖励索引
	enum DialState
	{
		DIAL_FREE = 0,			//转盘空闲
		DIAL_ING,				//旋转中
		DIAL_END,				//旋转结束
	};
	CC_SYNTHESIZE(DialState, eDialState, DialState);
	Armature *pAStartButton;//
	Armature *pAEnd;//
	ImageView* bg;
};



#endif//_GAME_HALL_LUCKSPINWIDGET_H_H_