#ifndef __GameMatchWidget_H_H_
#define __GameMatchWidget_H_H_

/**
 * @file GameMatchWidget
 * @brief  游戏比赛界面 
 * @author tjl
 * @date 15-10-16
 */


#include "cocos2d.h"
#include "Common/TNWidget.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Model/MatchModel.h"
using namespace cocos2d::ui;
USING_NS_CC;

class GameMatchItem : public Layout
{
public:
	GameMatchItem();
	~GameMatchItem();
	static GameMatchItem* create(bool isRoot ,MatchInfoModel* info = nullptr ,int matchtype =0);
	virtual bool init(bool isRoot ,MatchInfoModel* info = nullptr,int matchtype =0);
	bool isRoot() {return mBRoot;}
	MatchInfoModel* getMatchInfo() {return mData;}
	void refreshData(MatchInfoModel* info);
	void refreshSignUpplayer();
	int    getMatchType () {return m_matchtype;}
	void onClickSignUp(Ref* pSender,ui::Widget::TouchEventType type);
protected:
	bool mBRoot; //是根结节点
	MatchInfoModel* mData;
	int   m_matchtype;
	Text* m_signUpPlayers;//报名人数
	Button* m_btnSignUp;
};

/*class GameMatchWidget : public TNWidget
{
public:
	GameMatchWidget();
	~GameMatchWidget();
	virtual bool init();
	void onEnter();
	void onExit();
	std::string getWidgetJsonName();
	void loadUI();
	CREATE_FUNC(GameMatchWidget);
	void onBack(Ref* pSender,ui::Widget::TouchEventType type);
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
	//比赛开始返回处理
	void receiveMatchStartMsg(EventCustom* evt);
	//倒计时返回处理
	void receiveMatchCountDownMsg(EventCustom* evt);
	//登录比赛房间成功
	void loginMatchRoomSuccessRsp(EventCustom* evt);
	//登录比赛房间失败
	void loginMatchRoomFailureRsp(EventCustom* evt);
	//发送报名请求
	void toSendSignUpRequest(EventCustom* evt);
	//发送退赛请求
	void toSendunSignUpRequest(EventCustom* evt);

	void updateSignUpPlayers(float dt);
	int getMatchRoomCountByType(int type);
protected:
	void createGameSorket();
	void connectGameServer(const char* ip ,int port);//连接比赛服务器
	void showLoading();
	void removeLoading();
	ListView* mMatchList;
};*/


#endif//__GameMatchWidget_H_H_