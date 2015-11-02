
/**
 * @file GameMatchScene
 * @brief 游戏比赛场景
 * @author tjl
 * @date 2015-10-23
 */


#ifndef __GameMatchScene_SCENE_H__
#define __GameMatchScene_SCENE_H__

#include "cocos2d.h"
#include "GameBaseScene.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace ui;
USING_NS_CC;


class GameMatchScene : public GameBaseScene {
public:
	GameMatchScene();
	~GameMatchScene();
	static GameMatchScene* create(bool isBcontinue = false);//是否是继续比赛
	virtual bool init(bool isBcontinue);
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();

	virtual void onExit();
	virtual void okCallback();

	void loadUI();

	void DeleteMatchRoomMsg(EventCustom* evt);
	void receiveMatchRoomListMsg(EventCustom* evt);
	void updateRoomInfoMsg(EventCustom* evt);
	//报名成功返回处理
	void receiveSignUpSuccessMsg(EventCustom* evt);
	//报名失败返回处理
	void receiveSignUpFaildMsg(EventCustom* evt);
	//退赛成功返回处理
	void receiveUnSignUpSuccessMsg(EventCustom* evt);
	//退赛失败返回处理
	void receiveUnSignUpFaildMsg(EventCustom* evt);
	//倒计时返回处理
	void receiveMatchCountDownMsg(EventCustom* evt);
	//发送报名请求
	void toSendSignUpRequest(EventCustom* evt);
	//发送退赛请求
	void toSendunSignUpRequest(EventCustom* evt);
	//比赛次数达上限消息处理
	void ReceiveMatchPlayCountIsMaxMsg(EventCustom* evt);
	void taskSorketConnetFail(EventCustom* evt);

	void updateSignUpPlayers(float dt);
	int getMatchRoomCountByType(int type);

	void showLoading();
	void removeLoading();
	void setShowWait(bool value) {mShowWait = value;} 
protected:
	void onClickBack(Ref* pSender,ui::Widget::TouchEventType type);
	void playScaleAnimation(bool less, Ref* pSender) ;
	char* getWidgetJsonName();
	ListView* mMatchList;
	cocos2d::ui::Widget* m_mainWidget;
	bool          mbContinueMatch; //继续比赛
	bool          mNeedBind;
	bool          mShowWait;
};

#endif //__GameMatch_SCENE_H__
