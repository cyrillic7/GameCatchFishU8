#include "MatchModel.h"


MatchSerialModel::MatchSerialModel()
{
		m_matchInfoID = 0;
		m_kindId = 0;
		m_matchType = 0;
		m_fullPlayerNum = 0;
		m_sortId = 0;
}

MatchSerialModel::~MatchSerialModel()
{

}

MatchAwardModel::MatchAwardModel()
{
	m_matchInfoID = 0;
	m_beginRank = 0;
	m_endRank = 0;
	m_award1 = 0;
	m_award2 = 0;
	m_award3 = 0;
	m_awardDesc1 ="";
	m_awardDesc2 ="";
	m_awardDesc3 ="";
}

MatchAwardModel::~MatchAwardModel()
{

}

MatchInfoModel::MatchInfoModel()
{
	m_serverID = 0;
	m_serialModel = MatchSerialModel::create();
	m_serialModel->retain();
	m_awardArray = __Array::create();
	m_awardArray->retain();
	memset(&m_matchTime,0,sizeof(m_matchTime));
	m_spaceTime = 0;
	m_signUpScore = 0;
	m_signUpTicket = 0;
	m_curGroupCount = 0;
	m_signUpPlayerNum = 0;
	m_signUpStatus = 0;
	m_matchRemark ="";
	m_matchMemberOrder = 0;
}

MatchInfoModel::~MatchInfoModel()
{
	CC_SAFE_RELEASE_NULL(m_awardArray);
	CC_SAFE_RELEASE_NULL(m_serialModel);
}


//////////////////////////////////////////////////////////////////////////
GameMatchModel::GameMatchModel()
{

}

GameMatchModel::~GameMatchModel()
{
	m_selfRank = 0;
	m_haveAllUser = 0;
	m_playCount = 0;
	m_selfRank = 0;
	m_AllPlayCount = 0;
	m_myCurScore = 0;
	m_myMaxScore = 0;
	m_totalMaxScore = 0;
	m_matchStatus = 0;
	m_matchType = 0;
	m_matchTime = 0;
	m_matchTitle = "";
	m_bContinue = 0;
	
}