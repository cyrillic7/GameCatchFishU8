/**
 * @file GameServiceClientManager
 * @brief  service client管理器
 * @author wudb
 * @date 14-9-12
 */
#ifndef __GameServiceClientManager_H_
#define __GameServiceClientManager_H_

#include "cocos2d.h"
#include "GameServiceClient.h"


USING_NS_CC;

class GameServiceClientManager : public Ref {
    GameServiceClientManager();
    ~GameServiceClientManager();
public:
    static GameServiceClientManager* sharedInstance();
    static void purgeInstance();

    //添加client
    void registerServiceClient(const char* name,GameServiceClient *client, bool needScriptResponseHandler = true);

	void registerServiceClient(const char* name, bool needScriptResponseHandler,int sorketIndex);

    //获取当前service client的名称
    void setCurrentServiceClientName(const char* name);
    std::string getCurrentServiceClientName();

    // 获取当前service client
    GameServiceClient* getCurrentServiceClient();

    //删除
    void removeServiceClient(const char* name);
    void removeAllServiceClients();

    //获取
    GameServiceClient *serviceClientForName(const char* name);

    //设置和获取当前进入的房间
    void setCurrentRoom(const char* room);
    std::string getCurrentRoom();

private:
    __Dictionary    *m_clientMap;

	//response handler map
	__Dictionary	*m_responseHandlerMap;

    //当前进入的房间名称
    std::string     m_currentRoom;

    //当前service client的名称
    std::string     m_currentClientName;
};


#endif //__GameServiceClientManager_H_
