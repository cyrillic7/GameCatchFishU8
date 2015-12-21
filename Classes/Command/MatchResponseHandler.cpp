#include"MatchResponseHandler.h"
#include "MsgDefine/CMD_LogonServer.h"
#include "SessionManager.h"
#include "Model/MatchModel.h"
#include "Common/pystring.h"
#include "CommonFunction.h"

bool MatchResponseHandler::init()
{
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_GAME),responsehandler_selector(MatchResponseHandler::OnSocketSubMatchGame));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_TYPE),responsehandler_selector(MatchResponseHandler::OnSocketSubMatchType));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_INFO),responsehandler_selector(MatchResponseHandler::OnSocketSubMatchInfo));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_DELETE),responsehandler_selector(MatchResponseHandler::OnSocketSubMatchDelete));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_NUM),responsehandler_selector(MatchResponseHandler::OnSocketSubMatchNum));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_START),responsehandler_selector(MatchResponseHandler::OnSocketSubMatchStart));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_RANK),responsehandler_selector(MatchResponseHandler::OnSocketSubAwardInfo));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_WITHDRAW_SUCCESS),responsehandler_selector(MatchResponseHandler::OnSocketSubWithDrawSuccess));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_WITHDRAW_FAILURE),responsehandler_selector(MatchResponseHandler::OnSocketSubWithDrawFailure));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_C_MATCH_COUNTDOWN),responsehandler_selector(MatchResponseHandler::OnSocketSubCountDown));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_SIGNUP_SUCCESS),responsehandler_selector(MatchResponseHandler::OnSocketSubSignUpSuccess));
	registerResponseHandler((int)MAKELONG(MDM_GL_C_DATA,SUB_GL_SIGNUP_FAILURE),responsehandler_selector(MatchResponseHandler::OnSocketSubSignUpFailure));
	
	return true;
}

//比赛的游戏
void MatchResponseHandler::OnSocketSubMatchGame(void* data, WORD data_size)
{
	if (data_size%sizeof(CMD_GL_MatchGame)!=0) return ;

	//变量定义
	WORD wItemCount=data_size/sizeof(CMD_GL_MatchGame);
	CMD_GL_MatchGame * pMatchKind=(CMD_GL_MatchGame *)(data);

	for (int i = 0 ; i < wItemCount;i++)
	{
		if (strcmp("捕鱼比赛",pMatchKind->szGameName) == 0 ||strcmp("大闹天宫",pMatchKind->szGameName) == 0)
		{
			SessionManager::shareInstance()->setGameMatchKind(pMatchKind->dwKindID);
			break;
		}
		pMatchKind++;
	}
	
}
//比赛类型
void MatchResponseHandler::OnSocketSubMatchType(void* data, WORD data_size)
{
	if (data_size%sizeof(CMD_GL_MatchType)!=0) return ;

	//变量定义
	WORD wItemCount=data_size/sizeof(CMD_GL_MatchType);
	CMD_GL_MatchType * pGameType=(CMD_GL_MatchType *)(data);

	for (int i = 0 ; i < wItemCount;i++)
	{
		
		pGameType++;
	}

}

//比赛列表（添加，更新，是否报名）
void MatchResponseHandler::OnSocketSubMatchInfo(void* data, WORD data_size)
{
	if (data_size!=sizeof(CMD_GL_MatchInfo)) return ;
	CMD_GL_MatchInfo * MatchInfo=(CMD_GL_MatchInfo *)(data);
	//过滤不是捕鱼比赛的信息
	if (MatchInfo->MatchSerial.dwKindID == SessionManager::shareInstance()->getGameMatchKind())
	{
		MatchInfoModel* info = nullptr;
		bool isNewMatch = false;
		//log("MatchInfo->MatchSerial.dwMatchInfoID = %lld",MatchInfo->MatchSerial.dwMatchInfoID);
		info = SessionManager::shareInstance()->getMatchInfoByID(MatchInfo->MatchSerial.dwMatchInfoID);
		if (!info)
		{
			info = MatchInfoModel::create();
			SessionManager::shareInstance()->getMatchRooms()->addObject(info);
			isNewMatch = true;
		}
		MatchSerialModel* pSerialModel = info->getSerialModel();
		pSerialModel->setKindID(MatchInfo->MatchSerial.dwKindID);
		pSerialModel->setMatchInfoID(MatchInfo->MatchSerial.dwMatchInfoID);
		pSerialModel->setMatchType(MatchInfo->MatchSerial.dwMatchType);
		pSerialModel->setFullPlayerNum(MatchInfo->MatchSerial.dwFullPlayerNum);
		std::string sort_str = __String::createWithFormat("%lld",MatchInfo->MatchSerial.dwMatchInfoID)->getCString();
		std::string id_str = pystring::slice(sort_str,0,2);
		pSerialModel->setSortId(atoi(id_str.c_str()));

		__Array * pAwardArray = info->getAwardArray();
		pAwardArray->removeAllObjects();
		for ( int i = 0 ; i < 10 ; i++)
		{
			CMD_GL_MatchAward matchAward = MatchInfo->MatchAward[i];
			if (matchAward.dwBeginRank > 0)
			{
				MatchAwardModel* pAwardModel = MatchAwardModel::create();
				pAwardModel->setBeginRank(matchAward.dwBeginRank);
				pAwardModel->setEndRank(matchAward.dwEndRank);
				pAwardModel->setMatchInfoID(matchAward.dwMatchInfoID);
				pAwardModel->setAward1(matchAward.dwAward1);
				pAwardModel->setAward2(matchAward.dwAward2);
				pAwardModel->setAward3(matchAward.dwAward3);
				pAwardModel->setAwardDesc1(CommonFunction::GBKToUTF8(matchAward.szAward1));
				pAwardModel->setAwardDesc2(CommonFunction::GBKToUTF8(matchAward.szAward2));
				pAwardModel->setAwardDesc3(CommonFunction::GBKToUTF8(matchAward.szAward3));
				pAwardArray->addObject(pAwardModel);
			}

		}

		info->setCurGroupCount(MatchInfo->dwCurGroupCount);
		info->setMatchRemark(MatchInfo->szRemark);
		info->setMatchTime(MatchInfo->tMatchTime);
		info->setSignUpPlayerNum(MatchInfo->dwSignUpPlayerNum);
		info->setsignUpStatus(MatchInfo->dwSignUp);
		info->setSignUpScore(MatchInfo->dwSignUpScore);
		info->setSignUpTicket(MatchInfo->dwSignUpTicket);
		info->setSpaceTime(MatchInfo->dwSpaceTime);
		info->setServerID(MatchInfo->dwServerID);
		info->setMatchMemberOrder(MatchInfo->dwMemberOrder);

		__Dictionary* dic = __Dictionary::create();
		dic->setObject(__Bool::create(isNewMatch),"isNewMatch");
		dic->setObject(info,"matchInfo");

		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(refreshMatchListMsg,dic);
	}

}

//冠军榜
void MatchResponseHandler::OnSocketSubAwardInfo(void* data, WORD data_size)
{
	int i=0;
}
//删除比赛
void MatchResponseHandler::OnSocketSubMatchDelete(void* data, WORD data_size)
{
	if (data_size!=sizeof(tagMatchID)) return ;
	tagMatchID * MatchID=(tagMatchID *)(data);
	//转换数据
	MatchSerialModel* pSerialModel = MatchSerialModel::create();
	pSerialModel->setKindID(MatchID->MatchSerial.dwKindID);
	pSerialModel->setMatchInfoID(MatchID->MatchSerial.dwMatchInfoID);
	pSerialModel->setMatchType(MatchID->MatchSerial.dwMatchType);
	pSerialModel->setFullPlayerNum(MatchID->MatchSerial.dwFullPlayerNum);

	//从比赛列表里移除
	SessionManager::shareInstance()->removeMatchRoomByMatchInfoID(MatchID->MatchSerial.dwMatchInfoID);
	//通知页面刷新列表
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(deleteMatchRoomMsg,pSerialModel);

}
//比赛人数
void MatchResponseHandler::OnSocketSubMatchNum(void* data, WORD data_size)
{
	if (data_size%sizeof(CMD_GL_MatchNum)!=0) return ;
	WORD wItemCount=data_size/sizeof(CMD_GL_MatchNum);
	CMD_GL_MatchNum * MatchNum=(CMD_GL_MatchNum *)(data);

	__Array* pArray = __Array::create();
	for (int i=0 ; i < wItemCount; i++)
	{
		__Dictionary* infoDic = __Dictionary::create();
		MatchSerialModel* pSerialModel = MatchSerialModel::create();
		pSerialModel->setKindID(MatchNum->MatchSerial.dwKindID);
		pSerialModel->setMatchInfoID(MatchNum->MatchSerial.dwMatchInfoID);
		pSerialModel->setMatchType(MatchNum->MatchSerial.dwMatchType);
		pSerialModel->setFullPlayerNum(MatchNum->MatchSerial.dwFullPlayerNum);

		infoDic->setObject(pSerialModel,"serialModel");
		infoDic->setObject(__Integer::create(MatchNum->nSignUpNum),"signUpNum");
		infoDic->setObject(__Integer::create(MatchNum->dwCurGroupCount),"groupCount");
		MatchNum++;
		pArray->addObject(infoDic);
	}
	if (pArray->count() > 0)
	{
		Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(updateRoomSignUpPlayersMsg,pArray);
	}
	
	
}
//比赛报名
void MatchResponseHandler::OnSocketSubMatchSignUp(void * pData, WORD wDataSize)
{
	int i=0;
}
//比赛拉人
void MatchResponseHandler::OnSocketSubMatchStart(void* data, WORD data_size)
{
	if (data_size!=sizeof(CMD_GL_MatchServer)) return ;

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(matchStartMsg,data);
}
//报名成功
void MatchResponseHandler::OnSocketSubSignUpSuccess(void* data, WORD data_size)
{
	//效验参数
	CMD_GL_SignUpSuccess * MatchID=(CMD_GL_SignUpSuccess *)data;

	if (data_size<(sizeof(CMD_GL_SignUpSuccess)-sizeof(MatchID->szDescribeString))) return ;

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(signUpSuccessMsg,MatchID);

}
//报名失败
void MatchResponseHandler::OnSocketSubSignUpFailure(void* data, WORD data_size)
{
	//效验参数
	CMD_GL_SignUpFailure * pOperateFailure=(CMD_GL_SignUpFailure *)data;
	if (data_size<(sizeof(CMD_GL_SignUpFailure)-sizeof(pOperateFailure->szDescribeString))) return ;

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(signUpFaildMsg,pOperateFailure);
}
//退赛成功
void MatchResponseHandler::OnSocketSubWithDrawSuccess(void * pData, WORD data_size)
{
	//效验参数
	CMD_GL_SignUpSuccess * MatchID=(CMD_GL_SignUpSuccess *)pData;
	if (data_size<(sizeof(CMD_GL_SignUpSuccess)-sizeof(MatchID->szDescribeString))) return ;
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(unSignUpSuccessMsg,MatchID);
}
//退赛失败
void MatchResponseHandler::OnSocketSubWithDrawFailure(void * pData, WORD data_size)
{
	CMD_GL_SignUpFailure * pOperateFailure=(CMD_GL_SignUpFailure *)pData;
	if (data_size<(sizeof(CMD_GL_SignUpFailure)-sizeof(pOperateFailure->szDescribeString))) return ;

	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(unSignUpFaildMsg,pOperateFailure);

}
//倒计时
void MatchResponseHandler::OnSocketSubCountDown(void * pData, WORD data_size)
{
	//效验参数
	if (data_size!=sizeof(CMD_GL_MatchCountDown)) return;
	Director::sharedDirector()->getEventDispatcher()->dispatchCustomEvent(matchCountDownMsg,pData);
}