/**
 * @file GameResponseHandler
 * @brief 
 * @author tjl
 * @date 15-4-10
 */
#ifndef __GameResponseHandler_H_
#define __GameResponseHandler_H_

#include "Command/BaseResponseHandler.h"
#include "cocos2d.h"

////对long long 类型特别处理
//template <typename T>
//static std::string numberToString(T number) {
//	std::ostringstream ss;
//	ss << number;
//	return ss.str();
//}

class GameResponseHandler : public BaseResponseHandler
{
public:
	CREATE_FUNC(GameResponseHandler);
	bool init();

	void OnSubExchangeFishScore(void*data,WORD data_size);
	void OnSubUserFire(void* data, WORD data_size);
	void OnSubDistributeFish(void* data, WORD data_size);
	void OnSubCatchFishGroup(void* data, WORD data_size);
	void OnSubBulletDoubleTimeout(void* data, WORD data_size);
	void OnSubSwitchScene(void* data, WORD data_size);
	void OnSubCatchChain(void* data, WORD data_size);
	void OnSubSceneFish(void* data, WORD data_size);
	void OnSubSceneBullets(void* data, WORD data_size);
	void OnSubForceTimerSync(void* data, WORD data_size);
	void OnSubTimerSync(void* data, WORD data_size);
	void OnSubStockOperateResult(void* data, WORD data_size);
	void OnSubAdminControl(void* data, WORD data_size);
	void OnSubModifyLkMulriple(void* data,WORD data_size);

	//框架消息处理
	void OnReceiveGameSceneMsg(void* data, WORD data_size);//游戏场景
	void OnReceiveGameStatusMsg(void* data, WORD data_size);//
	void OnReceiveSystemMsg(void* data, WORD data_size);//收到系统消息
	void OnReceivePlayerToSperator(void* data, WORD data_size);//转为旁观者

	//上分返回
	void OnReceiveExchangeScoreMsg(void* data,WORD data_size);
};



#endif // !__GameResponseHandler_H_


