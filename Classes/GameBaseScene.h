/**
 * @file GameBaseScene
 * @brief 所有游戏的基础类
 * @author tjl
 * @date
 */

/*
    * 大厅也认为是一个游戏。 每个子游戏，游戏场景连同小厅算一个游戏。
    * 大厅加载流程如下：
      1. 显示 [loading页面]
      2. 异步加载必须的资源
      3. 登陆1
      4. 获取 [用户基本信息]
      5. 去除 [loading页面]
      6. 加载UI
 */

#ifndef __GameBaseScene_H_
#define __GameBaseScene_H_

#include "GameServiceClient.h"
#include "Common/ResourceLoader.h"

#include "cocos2d.h"
#include "Common/AlertWidget.h"
#include "MsgDefine/CMD_LogonServer.h"

class GameBaseScene : public Scene,public ResourceLoaderDelegate,public AlertDelegate
{
public:
	GameBaseScene();
	virtual ~GameBaseScene();
    static GameBaseScene* create();
    virtual void onEnter();
    virtual void onExit();
	bool init();
    // 登陆
    // 如果已经登陆的话，返回false， 如果没有登陆，返回true
 //   void login();

	//// 退出登录
 //   // 如果已经登陆，返回true，否则返回false
	//void logout();

	//// 进入房间
	//virtual void enterRoom(const char* roomName);

	//// 退出房间
	//virtual void exitRoom(const char* roomName);

    // 子类在ServiceManager注册了client后，直接设置进来
    void setServiceClient(GameServiceClient *client);

    // 重登游戏界面, 子类继承实现了重登就返回true, 否则返回false
    virtual bool reenterRoom();
    virtual void setCurrentRoom(std::string room);
    virtual std::string getCurrentRoom();
    // 当前登陆房间的名称
    std::string currentRoom;

    // 游戏名称, 子类必须在 onEnter 之前设置好该数据。否则无法登陆。
    std::string m_gameName;

    // 加载UI
    virtual void loadUI() {m_alreadyLoadedUI = true;};

    //设置是否为在游戏内, 默认为false, 在厅里
    void setInGame(bool inGame);
    bool getInGame()const;

	// 基类实现
	virtual __Array* getResourceNeedLoad();
	//进度条更新(子类必须实现）
	virtual void updateProgress(int resourceLeftToLoad);

  /*  virtual void onBack();
    virtual void clearResources();*/
	virtual void okCallback();

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) ;

	//登录比赛房间成功
    void loginMatchRoomSuccessRsp(EventCustom* evt);
	//登录比赛房间失败
	 void loginMatchRoomFailureRsp(EventCustom* evt);
	 //
	 void ReceiveRemoveGameSoketMsg(EventCustom* evt);
protected:
	// 异步加载资源
	virtual void loadResourceNeed();
	// 一个资源被加载成功后，调用该函数
	// resourceLeftToLoad，剩余等待加载的资源数
	virtual void oneResourceLoaded(ResourceDefinition* d, int resourceLeftToLoad);

	// 加载资源成功后，需要完成的操作
	// 登陆或者重新获取用户信息
	virtual void actionAfterLoadResource();


	// 显示 资源加载 loading页面
	//virtual void showLoadingWidget();
	// 显示 retry loading页面
	//virtual void showRetryLoadingWidget();
	// 移除 [loading页面]
	//virtual void removeLoadingWidget();
	void onloginTaskServer(EventCustom* evt);
	//收到帐号在异地登录的消息
	void onloginAtOtherPlace(EventCustom* evt);
	//收到比赛马上开始的消息
	void receiveMatchStartMsg(EventCustom* evt);
	//退出
	void onExitGame(EventCustom* evt);
	void onEnterMatch(EventCustom* evt);


protected:
	void createGameSorket();
	void connectGameServer(const char* ip ,int port);//连接比赛服务器

	void showLoading();
	void removeLoading();
	bool m_alreadyLoadedUI;
	bool m_inGame;
	GameServiceClient* m_serviceClient;
	//资源是否加载完成
	bool        m_isResourceLoaded;
	ResourceLoader* m_resourceLoader;
	bool      bNeedRelogin;
};

#endif //__GameBaseScene_H_