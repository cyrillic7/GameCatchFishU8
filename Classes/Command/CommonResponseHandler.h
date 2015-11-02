/**
 * @file CommonResponseHandler
 * @brief 
 * @author tjl
 * @date 15-4-10
 */
#ifndef __CommonResponseHandler_H_
#define __CommonResponseHandler_H_

#include "Command/BaseResponseHandler.h"
#include "cocos2d.h"


class CommonResponseHandler : public BaseResponseHandler
{
public:
	CREATE_FUNC(CommonResponseHandler);
	bool init();

	//框架消息处理
	void OnReceiveGameSceneMsg(void* data, WORD data_size);//游戏场景
	void OnReceiveSystemMsg(void* data, WORD data_size);//收到系统消息

	//比赛房间的消息处理
	void OnReceiveMatchStatusMsg(void* data,WORD data_size);
	void OnReceiveMatchInfoMsg(void* data,WORD data_size);
	void OnReceiveMatchNextLevelMsg(void* data,WORD data_size);
	void OnReceiveMatchAwardResultMsg(void* data,WORD data_size);
};



#endif // !CommonResponseHandler


