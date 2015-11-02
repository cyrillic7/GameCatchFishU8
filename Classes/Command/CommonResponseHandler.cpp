#include"CommonResponseHandler.h"
#include "MsgDefine/CMD_LogonServer.h"

bool CommonResponseHandler::init()
{
	//框架类的消息
	registerResponseHandler((int)MAKELONG(MDM_GF_FRAME,SUB_GF_GAME_SCENE),responsehandler_selector(CommonResponseHandler::OnReceiveGameSceneMsg));
	registerResponseHandler((int)MAKELONG(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE),responsehandler_selector(CommonResponseHandler::OnReceiveSystemMsg));


	//比赛相关消息处理
	registerResponseHandler((int)MAKELONG(MDM_GF_FRAME,SUB_GR_MATCH_SCREEN_FISH),responsehandler_selector(CommonResponseHandler::OnReceiveMatchStatusMsg));
	registerResponseHandler((int)MAKELONG(MDM_GF_FRAME,SUB_GR_MATCHFISH_INFO),responsehandler_selector(CommonResponseHandler::OnReceiveMatchInfoMsg));
	registerResponseHandler((int)MAKELONG(MDM_GF_FRAME,SUB_GR_MATCHFISH_NEXT_LEVEL),responsehandler_selector(CommonResponseHandler::OnReceiveMatchNextLevelMsg));
	registerResponseHandler((int)MAKELONG(MDM_GF_FRAME,SUB_GR_MATCH_RESULT),responsehandler_selector(CommonResponseHandler::OnReceiveMatchAwardResultMsg));
	return true;
}

void CommonResponseHandler::OnReceiveGameSceneMsg(void* data, WORD data_size)//游戏场景
{
#if 0
	CMD_S_GameStatus* gamestatus = static_cast<CMD_S_GameStatus*>(data);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GameSceneMsg,data);
#else
	// 计算鱼种类数量
	int nSubLen = data_size-sizeof(bool)-sizeof(float)-sizeof(unsigned int)-sizeof(SCORE)*GAME_PLAYER*2-sizeof(int)*5-sizeof(float)*BULLET_KIND_COUNT*2;
	if (nSubLen%(3*sizeof(float)) != 0 || nSubLen/(3*sizeof(float)) < 1)
	{
		CCLOG("GameScene data is error");
		return;
	}
	int nFishCount = nSubLen/(3*sizeof(float));
	CMD_S_GameStatus* pOrgGamestatus = static_cast<CMD_S_GameStatus*>(data);
	CMD_S_GameStatus gamestatus;
	memset(&gamestatus, 0, sizeof(CMD_S_GameStatus));
	memcpy(&gamestatus, data, data_size);
	memcpy(gamestatus.game_config.fish_bounding_radius, pOrgGamestatus->game_config.fish_speed+nFishCount, nSubLen/3);
	memcpy(gamestatus.game_config.fish_bounding_count, pOrgGamestatus->game_config.fish_speed+nFishCount*2, nSubLen/3);
	memcpy(gamestatus.game_config.bullet_speed, pOrgGamestatus->game_config.fish_speed+nFishCount*3, sizeof(float)*BULLET_KIND_COUNT*2);
	memcpy(gamestatus.fish_score, pOrgGamestatus->game_config.fish_speed+nFishCount*3+BULLET_KIND_COUNT*2, sizeof(SCORE)*GAME_PLAYER*2);
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(GameSceneMsg,&gamestatus);
#endif
	return;

}


void CommonResponseHandler::OnReceiveSystemMsg(void* data, WORD data_size)//收到系统消息
{
	CMD_CM_SystemMessage* system_msg = (CMD_CM_SystemMessage*)data;
	WORD head_size = sizeof(CMD_CM_SystemMessage) - sizeof(system_msg->szString);
	if (data_size < head_size || data_size != head_size + system_msg->wLength * sizeof(TCHAR))
	{
		log("system data is error");
		return ;
	}

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(SystemMessage,data);	
}


void CommonResponseHandler::OnReceiveMatchStatusMsg(void* data,WORD data_size)
{
	if(data_size!=sizeof(CMD_FishMatch_Screen)) return ;

	CMD_FishMatch_Screen *pMatchInfo=(CMD_FishMatch_Screen*)data;
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateMatchStatusMsg,pMatchInfo);	
}

void CommonResponseHandler::OnReceiveMatchInfoMsg(void* data,WORD data_size)
{
	if(data_size!=sizeof(CMD_FishMatch_Info)) return ;
	//变量定义
	CMD_FishMatch_Info *pMatchInfo=(CMD_FishMatch_Info*)data;
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateMatchInfoMsg,pMatchInfo);	
}

void CommonResponseHandler::OnReceiveMatchNextLevelMsg(void* data,WORD data_size)
{
	int bContinue = *((BYTE*)data);
	__Dictionary* dic = __Dictionary::create();
	dic->setObject(__Integer::create(bContinue),"bCotinue");
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(showMatchTryMsg,dic);	
}

void CommonResponseHandler::OnReceiveMatchAwardResultMsg(void* data,WORD data_size)
{
	if(data_size!=sizeof(CMD_GR_MatchResult)) return ;
	CMD_GR_MatchResult *pResult=(CMD_GR_MatchResult*)data;

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(matchAwardMsg,pResult);
}