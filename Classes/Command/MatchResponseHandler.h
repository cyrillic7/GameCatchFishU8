/**
 * @file MatchResponseHandler
 * @brief 
 * @author tjl
 * @date 15-10-15
 */
#ifndef __MatchResponseHandler_H_
#define __MatchResponseHandler_H_

#include "Command/BaseResponseHandler.h"
#include "cocos2d.h"


class MatchResponseHandler : public BaseResponseHandler
{
public:
	CREATE_FUNC(MatchResponseHandler);
	bool init();

	//大厅比赛消息相关处理
	//比赛的游戏
	void OnSocketSubMatchGame(void* data, WORD data_size);
	//比赛类型
	void OnSocketSubMatchType(void* data, WORD data_size);
	//比赛列表（添加，更新，是否报名）
	void OnSocketSubMatchInfo(void* data, WORD data_size);
	//冠军榜
	void OnSocketSubAwardInfo(void* data, WORD data_size);
	//删除比赛
	void OnSocketSubMatchDelete(void* data, WORD data_size);
	//比赛人数
	void OnSocketSubMatchNum(void* data, WORD data_size);
	//比赛报名
	void OnSocketSubMatchSignUp(void * pData, WORD wDataSize);
	//比赛拉人
	void OnSocketSubMatchStart(void* data, WORD data_size);
	//报名成功
	void OnSocketSubSignUpSuccess(void* data, WORD data_size);
	//报名失败
	void OnSocketSubSignUpFailure(void* data, WORD data_size);
	//退赛成功
	void OnSocketSubWithDrawSuccess(void * pData, WORD data_size);
	//退赛失败
	void OnSocketSubWithDrawFailure(void * pData, WORD data_size);
	//倒计时
	void OnSocketSubCountDown(void * pData, WORD data_size);
};



#endif // !MatchResponseHandler


