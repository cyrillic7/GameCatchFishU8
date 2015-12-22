#include "SessionManager.h"
#include "CommonFunction.h"
#include <time.h>
#include "Common/pystring.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "MsgDefine/CMD_LogonServer.h"


using namespace rapidjson;
USING_NS_CC_EXT;
USING_NS_CC;

static SessionManager* __instance = nullptr;


// 背景音乐是否开启
#define BACKGROUND_MUSIC_ON_SAVING_KEY "BACKGROUND_MUSIC_ON_SAVING_KEY"

// 音效是否开启
#define MUSIC_EFFECT_ON_SAVING_KEY "MUSIC_EFFECT_ON_SAVING_KEYN"

// 游戏启动的次数
#define GAME_LAUNCH_COUNT_SAVING_KEY "GAME_LAUNCH_COUNT_SAVING_KEY"


SessionManager::SessionManager()
{
	mRoomLevels = nullptr;
	mAllUsers  = __Array::create();
	CC_SAFE_RETAIN(mAllUsers);
	mMsgArray = __Array::create();
	CC_SAFE_RETAIN(mMsgArray);
	mGamePlayers = __Array::createWithCapacity(6);
	mGamePlayers->retain();
	mUserId = 0;
	mChairId = -1;
	m_IsEffectOn = 1;
	m_IsMusicOn = 1;
	m_gameLaunchCount = 0;

	m_userLoginModel = LoginUserModel::create();
	m_userLoginModel->retain();

	m_user  = OnlineUserModel::create();
	m_user->retain();

	mMatchSerialModel = MatchSerialModel::create();
	mMatchSerialModel->retain();

	m_responseDataCache = std::shared_ptr<Map<std::string, Ref*> >(new Map<std::string, Ref*>());

	mPwd = "";
	m_taskCnt = 0;
	m_taskPort = 0;
	m_taskServerAddr = "";

	mBankPwdFlag = false;
	mInsurePass = "";
	mInGame = false;
	mGameKindIndex = 0;

	mAccountArray = __Array::create();
	mAccountArray->retain();

	mMatchRooms = __Array::create();
	mMatchRooms->retain();

	//登录点数组
	mLoginUrls = __Array::create();
	mLoginUrls->retain();

	mRedStatus = 0;
	mLoginScoreStatus = 0;
	mLastDealBombTick = 0;
	mGameInitFinish =false;

	mBNewAuction =false;
	mBNewTask = false;
	mBNewMsg =false;
	mCurBgMusicName = "";
	mMatchId = 0;
	mMatchGameStatus = 0;

	match_server = "";
	match_port = 0;
	bMatch_Start = 0;

	login_server = "";

}

SessionManager::~SessionManager()
{
	CC_SAFE_RELEASE_NULL(mRoomLevels);
	CC_SAFE_RELEASE_NULL(mAllUsers);
	CC_SAFE_RELEASE_NULL(mLoginUrls);
	CC_SAFE_RELEASE_NULL(m_userLoginModel);
	CC_SAFE_RELEASE_NULL(m_user);
	CC_SAFE_RELEASE_NULL(mMsgArray);
	CC_SAFE_RELEASE_NULL(mGamePlayers);
	CC_SAFE_RELEASE_NULL(mAccountArray);
	CC_SAFE_RELEASE_NULL(mMatchRooms);
	CC_SAFE_RELEASE_NULL(mMatchSerialModel);
}


SessionManager* SessionManager::shareInstance()
{
	if (__instance==nullptr)
	{
		__instance = new SessionManager();
		if (!__instance->init()) {
			CC_SAFE_RELEASE_NULL(__instance);
		};
	}
	return __instance;
}

bool SessionManager::init() {
	// 必须放在最前面
	restoreUserData();

	readAccountFile();
	// 必须放在最后
	increaseGameLaunchCount();

	return true;
}

void SessionManager::purgeInstance()
{
	CC_SAFE_RELEASE_NULL(__instance);
}

void SessionManager::setAllRoomLevels(__Array* pArr)
{
	if (pArr != mRoomLevels) {
		CC_SAFE_RELEASE_NULL(mRoomLevels);
		mRoomLevels = pArr;
		if (NULL != mRoomLevels) {
			mRoomLevels->retain();
		}
	}
}
__Array* SessionManager::getAllRoomLevels()
{
	return mRoomLevels;
}

RoomLevelInfo* SessionManager::getLevelInfoByRoomID(int id)
{
	for (int i = 0; i < mRoomLevels->count(); i ++) {
		RoomLevelInfo *info = (RoomLevelInfo*)mRoomLevels->getObjectAtIndex(i);
		if ( id == info->getRoomId())
		{
			return info;
		}
	}
	return nullptr;
}

RoomLevelInfo* SessionManager::getLevelInfoByRoomLevel(int gameKind,int roomLevel)
{
	for (int i = 0; i < mRoomLevels->count(); i ++) {
		RoomLevelInfo *info = (RoomLevelInfo*)mRoomLevels->getObjectAtIndex(i);
		if ( gameKind == info->getKindIndex() && roomLevel == info->getSortIndex())
		{
			return info;
		}
	}
	return nullptr;
}

void SessionManager::setUserId(int id)
{
	mUserId = id;
}

int  SessionManager::getUserId()
{
	return mUserId;
}


time_t SessionManager::getTimeStamp()
{
	time_t timep;
	time(&timep);
	return timep;
}

std::string SessionManager::getDateTime()
{
	char _nowDate[20];
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	time(&timep);
#else 
	struct timeval now; 
	gettimeofday(&now, NULL); 
	timep = now.tv_sec;
#endif 
	tm = localtime(&timep); 
	strftime(_nowDate, 200, "%Y.%m.%d", tm);
	return _nowDate;
}

std::string SessionManager::getMatchTime()
{
	char _nowDate[20];
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	time(&timep);
#else 
	struct timeval now; 
	gettimeofday(&now, NULL); 
	timep = now.tv_sec;
#endif 
	tm = localtime(&timep); 

	strftime(_nowDate, 200, "%m-%d", tm);
	return _nowDate;
}


long SessionManager::getCurrentTime()    
{     
	struct timeval tv;    
	gettimeofday(&tv,NULL);    
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;  	
}

UserModel* SessionManager::getUserById(int usrId)
{
	for (int i = 0; i < mAllUsers->count(); i ++) {
		UserModel *info = (UserModel*)mAllUsers->getObjectAtIndex(i);
		if ( usrId == info->getUserId())
		{
			return info;
		}
	}
	return nullptr;
}

void SessionManager::flush() {
	CCUserDefault* user = CCUserDefault::sharedUserDefault();

	user->setIntegerForKey(GAME_LAUNCH_COUNT_SAVING_KEY, m_gameLaunchCount);

	user->setBoolForKey(BACKGROUND_MUSIC_ON_SAVING_KEY, m_IsMusicOn);


	user->setBoolForKey(MUSIC_EFFECT_ON_SAVING_KEY, m_IsEffectOn);

	// 登录数据
	m_userLoginModel->flush();

	user->flush();


}

void SessionManager::restoreUserData() {
	CCUserDefault* user = CCUserDefault::sharedUserDefault();
	m_gameLaunchCount = user->getIntegerForKey(GAME_LAUNCH_COUNT_SAVING_KEY);
	if (m_gameLaunchCount >= 1) {

		m_IsMusicOn = user->getBoolForKey(BACKGROUND_MUSIC_ON_SAVING_KEY, true);

		m_IsEffectOn = user->getBoolForKey(MUSIC_EFFECT_ON_SAVING_KEY, true);
	
	}

	m_userLoginModel->loadDataFromDisk();
}

void SessionManager::increaseGameLaunchCount() {
	m_gameLaunchCount++;
	UserDefault* user = UserDefault::getInstance();
	user->setIntegerForKey(GAME_LAUNCH_COUNT_SAVING_KEY, m_gameLaunchCount);
	user->flush();
}

bool SessionManager::getEffectOn() const {
	return m_IsEffectOn;
}

void SessionManager::setEffectOn(bool on) {
	m_IsEffectOn = on;
}

bool SessionManager::getMusicOn() const {
	return m_IsMusicOn;
}

void SessionManager::setMusicOn(bool on) {
	m_IsMusicOn = on;
}

void SessionManager::setResponseData(Ref *data, const char *key) {
	if (nullptr != key) {
		if (nullptr == data) { //如果传入为null, 则表示清空数据
			m_responseDataCache->erase(key);
		} else {
			m_responseDataCache->insert(key, data);
		}
	}
}

Ref *SessionManager::getResponseDataForKey(const char *key) {
	if (nullptr != key) {
		return m_responseDataCache->at(key);
	}
	return nullptr;
}

void SessionManager::clearAllResponseCache() {
	m_responseDataCache->clear();
}



std::string SessionManager::getPwd()
{
	return getLoginModel()->getPassword();
}

void SessionManager::setInsurePass(std::string insurePwd)
{
	mInsurePass = insurePwd;
}

std::string SessionManager::getInsurePass()
{
	return mInsurePass;
}

void SessionManager::addPlayer(UserModel*model)
{
	if (!isInPlayers(model))
	{
		mGamePlayers->addObject(model);
	}
}
bool SessionManager::isInPlayers(UserModel* model )
{
	for (int i = 0 ; i < mGamePlayers->count();i++)
	{
		UserModel* pModel = (UserModel*)mGamePlayers->getObjectAtIndex(i);
		if (pModel->getUserId() == model->getUserId())
		{
			return true;
		}
	}
	return false;
}

void SessionManager::clearGamePlayers()
{
	mGamePlayers->removeAllObjects();
}

void SessionManager::readAccountFile()
{
	std::string path = CommonFunction::getWritablePath()+"account.xml";
	ValueMap accounts = FileUtils::getInstance()->getValueMapFromFile(path);
	
	for (int i=0;i<accounts.size();i++)
	{
		mAccountArray->addObject(__String::create(accounts[numberToString(i)].asString()));
	}
}


void SessionManager::writeAccountFile()
{
	std::string path = CommonFunction::getWritablePath()+"account.xml";
	ValueMap accounts;
	for (int i = 0 ; i < mAccountArray->count();i++)
	{
		std::string accountInfo = ((__String*)mAccountArray->getObjectAtIndex(i))->getCString();
		accounts.insert(std::make_pair(numberToString(i),cocos2d::Value(accountInfo)));
	}

	FileUtils::sharedFileUtils()->writeToFile(accounts,path);
}

bool SessionManager::isNotLogin(std::string account)
{
	for (int i = 0 ; i < mAccountArray->count();i++)
	{
		std::string accountInfo = ((__String*)mAccountArray->getObjectAtIndex(i))->getCString();
		int pos = accountInfo.find('&');
		if (pos > 0 )
		{
			std::string _account = pystring::slice(accountInfo,0,pos);
			//std::string  _pwd  =  pystring::slice(accountInfo,pos+1,accountInfo.length());
			if (strcmp(_account.c_str(),account.c_str()) == 0 )//帐房已保存
			{
				return  false;
			}
		}
		
	}
	return true;
}

int SessionManager::getIndexByAccount(std::string account)
{
	for (int i = 0 ; i < mAccountArray->count();i++)
	{
		std::string accountInfo = ((__String*)mAccountArray->getObjectAtIndex(i))->getCString();
		int pos = accountInfo.find('&');
		if (pos > 0 )
		{
			std::string _account = pystring::slice(accountInfo,0,pos);
			//std::string  _pwd  =  pystring::slice(accountInfo,pos+1,accountInfo.length());
			if (strcmp(_account.c_str(),account.c_str()) == 0 )//帐房已保存
			{
				return  i;
			}
		}

	}
	return -1;
}

void SessionManager::readAssignInfo()
{
	rapidjson::Document readdoc;
	std::string assignData = m_userLoginModel->getAssignDate();
	if (assignData.length() > 0 )
	{
		readdoc.Parse<0>(assignData.c_str());	
		if(readdoc.HasParseError())
		{
			log("GetParseError%s\n", readdoc.GetParseError());
		}
		rapidjson::Value& _array = readdoc["array"];
		if (_array.IsArray())
		{
			for(int i=0; i<_array.Capacity(); i++)
			{
				std::string account = "";
				std::string assignDate = "";
				rapidjson::Value& jsonItem = _array[i];
				if (jsonItem.HasMember("account"))
				{
					account = jsonItem["account"].GetString();
				}
				if (jsonItem.HasMember("date"))
				{
					assignDate = jsonItem["date"].GetString();
				}
				mAssignMap.insert(std::make_pair(account,assignDate));
			}
		}
	}
}

void SessionManager::saveAssignInfo()
{
	std::map<std::string, std::string >::iterator it;
	if (m_userLoginModel->getAccountType() == accountType::accountMachineID)
		it = mAssignMap.find(m_userLoginModel->getQucikAccount().c_str());
	else
		it =mAssignMap.find(m_userLoginModel->getAccount().c_str());
	//it =mAssignMap.find(m_userLoginModel->getAccount().c_str());
	if (it !=mAssignMap.end())
	{
		it->second = SessionManager::shareInstance()->getDateTime();
	}
	else
	{
		if (m_userLoginModel->getAccountType() == accountType::accountMachineID)
			mAssignMap.insert(std::make_pair(m_userLoginModel->getQucikAccount(),SessionManager::shareInstance()->getDateTime()));
		else
			mAssignMap.insert(std::make_pair(m_userLoginModel->getAccount(),SessionManager::shareInstance()->getDateTime()));
	}

	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);
	

	for (it = mAssignMap.begin(); it != mAssignMap.end(); it++)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("account", it->first.c_str(), allocator);
		object.AddMember("date", it->second.c_str(), allocator);
		array.PushBack(object, allocator);
	}
	writedoc.AddMember("array", array, allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);

	std::string test = buffer.GetString();
	m_userLoginModel->setAssignDate(buffer.GetString());
	m_userLoginModel->flush();
}

bool SessionManager::isShowAssign()
{
	std::map<std::string, std::string >::iterator it;
	if (m_userLoginModel->getAccountType() == accountType::accountMachineID)
		it = mAssignMap.find(m_userLoginModel->getQucikAccount().c_str());
	else
		it =mAssignMap.find(m_userLoginModel->getAccount().c_str());
	if (it !=mAssignMap.end())
	{
		if (strcmp(it->second.c_str(),SessionManager::shareInstance()->getDateTime().c_str()) == 0)
		{
			return false;
		}
	}
	return true;
}

//加载已读邮件
void SessionManager::loadReadMsgInfo()
{
	rapidjson::Document readdoc;
	std::string msgData = m_userLoginModel->getMsgData();
	if (msgData.length() > 0 )
	{
		readdoc.Parse<0>(msgData.c_str());	
		if(readdoc.HasParseError())
		{
			log("GetParseError%s\n", readdoc.GetParseError());
		}
		rapidjson::Value& _array = readdoc["array"];
		if (_array.IsArray())
		{
			for(int i=0; i<_array.Capacity(); i++)
			{
				std::string account = "";
				std::string assignDate = "";
				rapidjson::Value& jsonItem = _array[i];
				if (jsonItem.HasMember("account"))
				{
					account = jsonItem["account"].GetString();
				}
				rapidjson::Value& msgIdArray = jsonItem["data"];
				if (msgIdArray.IsArray())
				{
					int msgId = 0;
					std::vector<int> vec;
					for (int j=0; j<msgIdArray.Capacity();j++)
					{
						rapidjson::Value& msgItem = msgIdArray[j];
						if (msgItem.HasMember("msgId"))
						{
							msgId = msgItem["msgId"].GetInt();
							vec.push_back(msgId);
						}
					}
					mMsgMap.insert(std::make_pair(account,vec));
				}	
			}
		}
	}
}

void SessionManager::SaveReadMsgInfo()
{
	std::map<std::string, std::vector<int> >::iterator it;
	if (m_userLoginModel->getAccountType() == accountType::accountMachineID)
		it = mMsgMap.find(m_userLoginModel->getQucikAccount().c_str());
	else
		it =mMsgMap.find(m_userLoginModel->getAccount().c_str());
	if (it !=mMsgMap.end())
	{
		it->second.clear();
		for (int i = 0 ; i < mMsgArray->count(); i++ )
		{
			__Dictionary* dic = (__Dictionary*)mMsgArray->getObjectAtIndex(i);
			int msgType = ((__Integer*)dic->objectForKey("msgType"))->getValue();
			int msgId = ((__Integer*)dic->objectForKey("msgId"))->getValue();
			if (msgType == Msg_Notice)//系统消息
			{
				it->second.push_back(msgId);
			}
		}
	}
	else
	{
		std::vector<int> vec;
		for (int i = 0 ; i < mMsgArray->count(); i++ )
		{
			__Dictionary* dic = (__Dictionary*)mMsgArray->getObjectAtIndex(i);
			int msgType = ((__Integer*)dic->objectForKey("msgType"))->getValue();
			int msgId = ((__Integer*)dic->objectForKey("msgId"))->getValue();
			if (msgType == Msg_Notice)//系统消息
			{
				vec.push_back(msgId);
			}
		}
		if (m_userLoginModel->getAccountType() == accountType::accountMachineID)
		{
			mMsgMap.insert(std::make_pair(m_userLoginModel->getQucikAccount(),vec));
		}
		else
		{
			mMsgMap.insert(std::make_pair(m_userLoginModel->getAccount(),vec));
		}
	}

	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);


	for (it = mMsgMap.begin(); it != mMsgMap.end(); it++)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("account", it->first.c_str(), allocator);

		rapidjson::Value idArray(rapidjson::kArrayType);
		for(int i = 0 ; i < it->second.size();i++)
		{
			rapidjson::Value objectId(rapidjson::kObjectType);
			objectId.AddMember("msgId", it->second[i], allocator);
			idArray.PushBack(objectId,allocator);
		}
		object.AddMember("data",idArray, allocator);
		array.PushBack(object, allocator);
	}
	writedoc.AddMember("array", array, allocator);

	StringBuffer buffer;
	rapidjson::Writer<StringBuffer> writer(buffer);
	writedoc.Accept(writer);

	std::string test = buffer.GetString();
	m_userLoginModel->setMsgData(buffer.GetString());
	m_userLoginModel->flush();
}

bool SessionManager::hasRead(int msgId)
{
	std::map<std::string, std::vector<int> >::iterator it;
	if (m_userLoginModel->getAccountType() == accountType::accountMachineID)
		it = mMsgMap.find(m_userLoginModel->getQucikAccount().c_str());
	else
		it =mMsgMap.find(m_userLoginModel->getAccount().c_str());
	if (it !=mMsgMap.end())
	{
		for(int i = 0 ; i < it->second.size();i++)
		{
			if (it->second[i] == msgId)
			{
				return true;
			}
		}
	}
	return false;
}

bool SessionManager::isInMatchRooms(int matchInfoID)
{
	bool ret =false;
	for ( int i = 0 ; i< mMatchRooms->count(); i++)
	{
		MatchInfoModel* infoModel = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(i);
		if (infoModel->getSerialModel()->getMatchInfoID() == matchInfoID)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

MatchInfoModel* SessionManager::getMatchInfoByID(LONGLONG matchInfoID)
{
	for ( int i = 0 ; i< mMatchRooms->count(); i++)
	{
		MatchInfoModel* infoModel = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(i);
		if (infoModel->getSerialModel()->getMatchInfoID() == matchInfoID)
		{
			return infoModel;
		}
	}
	return nullptr;
}

void SessionManager::removeMatchRoomByMatchInfoID(LONGLONG matchInfoID)
{
	for ( int i = 0 ; i< mMatchRooms->count(); i++)
	{
		MatchInfoModel* infoModel = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(i);
		if (infoModel->getSerialModel()->getMatchInfoID() == matchInfoID)
		{
			mMatchRooms->removeObjectAtIndex(i);
			break;
		}
	}
}

bool SessionManager::isHastheMatch(int matchType)
{
	for ( int i = 0 ; i< mMatchRooms->count(); i++)
	{
		MatchInfoModel* infoModel = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(i);
		if (infoModel->getSerialModel()->getMatchType() == matchType)
		{
			return true;
		}
	}

	return false;
}

int SessionManager::getMatchRoomCountByType(int matchType)
{
	int cnt = 0;
	for ( int i = 0 ; i< mMatchRooms->count(); i++)
	{
		MatchInfoModel* infoModel = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(i);
		if (infoModel->getSerialModel()->getMatchType() == matchType)
		{
			cnt++;
		}
	}

	return cnt;
}

void SessionManager::sortMatchRooms()
{
	if (mMatchRooms->count() > 1)
	{
		for (int i = 0 ;i < mMatchRooms->count();i++)
		{
			MatchInfoModel* minMode = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(i);
			for (int j = i ; j < mMatchRooms->count(); j++)
			{
				MatchInfoModel* curMode = (MatchInfoModel*)mMatchRooms->getObjectAtIndex(j);
				if (curMode->getSerialModel()->getSortId() < minMode->getSerialModel()->getSortId())
				{
				
					//交换两个接点的位置
					mMatchRooms->swap(i, j);
					//记录当前最少的一项
					minMode = curMode;
				}
			}
		}
	}
}

void SessionManager::setGameKind(int value)
{
	mGameKindIndex = value;
}

std::string SessionManager::getLoginAddr()
{
	//return "121.40.31.203";
	int index = m_userLoginModel->getLoginUrlIndex();
	if (mLoginUrls->count() == 0)
	{
		struct hostent* hostInfo = gethostbyname(GAME_IP);
		if (hostInfo)
		{
			login_server = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
		}
		return login_server;
	}
	
	 if (login_server == "")
	 {
		 __String* loginStr = (__String*)mLoginUrls->getObjectAtIndex(index);
		 struct hostent* hostInfo = gethostbyname(loginStr->getCString());
		 if (hostInfo)
		 {
			 login_server = inet_ntoa(*(struct in_addr *)*hostInfo->h_addr_list);
		 }
	 }
	
	
	return login_server;
}

void SessionManager::clearHornMsg()
{
	listHornMsg.clear();
}