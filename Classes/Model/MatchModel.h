/**
 * @file MatchModel
 * @brief  礼物
 * @author tjl
 * @date 15-10-15
 */
#ifndef __MatchModel_H_
#define __MatchModel_H_

#include "GameGlobalSetting.h"
#include "cocos2d.h"

USING_NS_CC;

//比赛房间唯一性数据结构类
class MatchSerialModel : public Ref {
public:
	MatchSerialModel();
	~MatchSerialModel();

	CREATE_FUNC(MatchSerialModel);
	bool init(){return true;};

	CC_SYNTHESIZE(LONGLONG, m_matchInfoID, MatchInfoID);
	CC_SYNTHESIZE(int, m_kindId, KindID);
	CC_SYNTHESIZE(int, m_matchType, MatchType);
	CC_SYNTHESIZE(int, m_fullPlayerNum, FullPlayerNum);
	CC_SYNTHESIZE(int, m_sortId, SortId);
};

//比赛奖励数据结构
class MatchAwardModel : public Ref {
public:
	MatchAwardModel();
	~MatchAwardModel();

	CREATE_FUNC(MatchAwardModel);
	bool init(){return true;};

	CC_SYNTHESIZE(LONGLONG, m_matchInfoID, MatchInfoID);
	CC_SYNTHESIZE(int, m_beginRank, BeginRank);
	CC_SYNTHESIZE(int, m_endRank, EndRank);
	CC_SYNTHESIZE(int, m_award1, Award1);
	CC_SYNTHESIZE(int, m_award2, Award2);
	CC_SYNTHESIZE(int, m_award3, Award3);
	CC_SYNTHESIZE(std::string, m_awardDesc1, AwardDesc1);
	CC_SYNTHESIZE(std::string, m_awardDesc2, AwardDesc2);
	CC_SYNTHESIZE(std::string, m_awardDesc3, AwardDesc3);
};

//比赛房间信息数据结构
class MatchInfoModel : public Ref {
public:
	MatchInfoModel();
	~MatchInfoModel();

	CREATE_FUNC(MatchInfoModel);
	bool init(){return true;};

	//server id
	CC_SYNTHESIZE(DWORD, m_serverID, ServerID);
	//房间唯一数据类
	CC_SYNTHESIZE(MatchSerialModel*, m_serialModel, SerialModel);

	//比赛奖励对象
	CC_SYNTHESIZE(__Array*, m_awardArray, AwardArray);
	//系统时间
	CC_SYNTHESIZE(SYSTEMTIME, m_matchTime, MatchTime);

	//space time
	CC_SYNTHESIZE(DWORD, m_spaceTime, SpaceTime);

	//报名费
	CC_SYNTHESIZE(LONGLONG, m_signUpScore, SignUpScore);

	//sign up tick
	CC_SYNTHESIZE(DWORD, m_signUpTicket,SignUpTicket);
	//满人时，即将开赛组人数
	CC_SYNTHESIZE(DWORD, m_curGroupCount,CurGroupCount);
	//报名人数
	CC_SYNTHESIZE(DWORD, m_signUpPlayerNum,SignUpPlayerNum);
	//报名状态
	CC_SYNTHESIZE(int, m_signUpStatus,signUpStatus);
	
	CC_SYNTHESIZE(std::string, m_matchRemark,MatchRemark);
	//
	CC_SYNTHESIZE(int, m_matchMemberOrder, MatchMemberOrder);
};

//游戏里的MATCH INFO MODEL
class GameMatchModel : public Ref {
public:
	GameMatchModel();
	~GameMatchModel();

	CREATE_FUNC(GameMatchModel);
	bool init(){return true;};

	//自已排名
	CC_SYNTHESIZE(DWORD, m_selfRank,SelfRank);

	CC_SYNTHESIZE(DWORD, m_haveAllUser,HaveAllUser);

	CC_SYNTHESIZE(int, m_playCount,PlayCount);

	CC_SYNTHESIZE(int, m_AllPlayCount,AllPlayCount);

	CC_SYNTHESIZE(SCORE, m_myCurScore,MyCurScore);

	CC_SYNTHESIZE(SCORE, m_myMaxScore,MyMaxScore);
	
	CC_SYNTHESIZE(SCORE, m_totalMaxScore,TotalMaxScore);

	//比赛状态
	CC_SYNTHESIZE(int, m_matchStatus,CurMatchStatus);
	//比赛类型 
	CC_SYNTHESIZE(int, m_matchType,MatchType);
	//比赛时间
	CC_SYNTHESIZE(SCORE, m_matchTime, MatchTime);
	//房间名称
	CC_SYNTHESIZE(std::string, m_matchTitle,MatchTitle);
	//是否能继续挑战
	CC_SYNTHESIZE(int, m_bContinue,BContinue);
	
};

#endif //__MatchModel_H_
