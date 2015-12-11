/**
 * @file BaseResponseHandler
 * @brief 
 * @author Tjl
 * @date 15-4-10
 */
#ifndef __BaseResponseHandler_H_
#define __BaseResponseHandler_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
#include "Message.h"
#include "MsgDefine/CMD_GameServer.h"

USING_NS_CC;

typedef void (Ref::*RESPONSE_HANDLER)(void* pData ,WORD DataSize);
#define responsehandler_selector(_SELECTOR) (RESPONSE_HANDLER)(&_SELECTOR)

class BaseResponseHandler : public Ref {
public:
    BaseResponseHandler();
    virtual ~BaseResponseHandler();

    virtual void handleResponse(int command, void* pData ,WORD DataSize);

protected:
    //注册命令
    void registerResponseHandler(int command, RESPONSE_HANDLER handler);

    //统一处理只有成功或失败消息的接口回调
    //成功返回null, 失败返回错误消息
    __String *handleErrorMessage();

private:
    std::map<int, RESPONSE_HANDLER > m_handlerMap;
};


#endif //__BaseResponseHandler_H_
