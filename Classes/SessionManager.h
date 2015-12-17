#ifndef _SessionManager_H_H_
#define _SessionManager_H_H_

#include "cocos2d.h"
#include "Model/RoomLevelInfo.h"
#include "Model/UserModel.h"
#include <iostream>
#include <time.h>
#include "Model/LoginUserModel.h"
#include "Model/OnlineUserModel.h"
#include "Model/MatchModel.h"

#define GameKindDntg    260
#define GameKindLkpy    290
#define GameKindJcpy    250

#define RoomLevel_0     511
#define RoomLevel_1     512
#define RoomLevel_2     513

#define  FULLPLAYERMATCH    4
#define  BUYUFOREVERMATCH 6
#define  BUYUTIMEMATCH       7 
#define  BUYUBIGAWARDMATCH 8

#define GAME_IP						"login1.qicainiu.net"						//正式服务器

class SessionManager : public Ref
{
public:
	SessionManager();
	~SessionManager();
	static SessionManager* shareInstance();
	virtual void purgeInstance();
	bool init();

	void setAllRoomLevels(__Array* pArr);
	__Array* getAllRoomLevels();
	RoomLevelInfo* getLevelInfoByRoomID(int id);
	RoomLevelInfo* getLevelInfoByRoomLevel(int gameKind,int roomLevel);
	void setUserId(int id);
	int  getUserId();

	void  setPwd(char* pwd);
	std::string getPwd();

	void setChairId(int id){mChairId = id;}
	int  getChairId(){return mChairId;}

	//获取时间戳，从公元1970 年1月1日0时0分0秒算起至今的UTC时间所经过的秒数。
	static time_t getTimeStamp();
	//当前日期
	static std::string getDateTime();
	//获取比赛时间
	static std::string  getMatchTime();
	//获取当前时间毫秒级
	long getCurrentTime();
	__Array* getAllUsers() {return mAllUsers;}
	UserModel* getUserById(int usrId);

	LoginUserModel* getLoginModel() {return m_userLoginModel;}
	OnlineUserModel* getUser() {return m_user;}
	void flush();
	void restoreUserData();
	void increaseGameLaunchCount();

	// 背景音乐开启
	bool getMusicOn() const;
	void setMusicOn(bool on);

	// 音效开启 , 1:开启 0:关闭
	bool getEffectOn() const;
	void setEffectOn(bool on);

	//设置对应缓存的数据
	void setResponseData(Ref *data, const char* key);
	Ref* getResponseDataForKey(const char* key);

	//清除所有缓存数据
	void clearAllResponseCache();
	//保存当前任务条数奇淫巧技
	void setTaskCnt(int cnt) {m_taskCnt = cnt;}
	int  getTaskCnt() const {return m_taskCnt;}
	//保存任务服务器IP 和端口
	void setTaskServer(char* ip) {m_taskServerAddr = ip;}
	void setTaskPort(int port){m_taskPort = port;}
	std::string getTaskServer() const {return m_taskServerAddr;}
	int getTaskPort() const {return m_taskPort;}

	bool isResetBankPwd() const { return mBankPwdFlag;}//是否重设银行密码
	void setBankPwdFlag(bool flag) {mBankPwdFlag = flag;}

	__Array* getMsgArray() {return mMsgArray;}
	void clearMsgArray() {mMsgArray->removeAllObjects();}

	void setInsurePass(std::string insurePwd);
	std::string getInsurePass();
	bool isInPlayers(UserModel* model );
	void addPlayer(UserModel*model);
	__Array* getGamePlayers() const {return mGamePlayers;}
	void clearGamePlayers();
	bool isInGame() const {return mInGame;}//自已在游戏里的标志
	void setInGame(bool flag) { mInGame = flag;} 
	void setGameKind(int value);
	int getGameKind() const {return mGameKindIndex;}


	//登录帐号列表
	void readAccountFile();
	void writeAccountFile();
	__Array* getAccountList() {return mAccountArray;}
	bool isNotLogin(std::string accountInfo);
	int getIndexByAccount(std::string account);

	//vippower相关
	void setLoginScoreStatus(int status) {mLoginScoreStatus = status;}//每日登录
	int  getLoginScoreStatus() const {return mLoginScoreStatus;}

	void setRedStatus(int status) {mRedStatus = status;}//每日返还
	int  getRedStatus() const {return mRedStatus;}

	long getLastBombTick() const {return mLastDealBombTick;}
	void setBombTick(long tick) {mLastDealBombTick = tick;}

	//加载帐号签到信息
	void readAssignInfo();
	void saveAssignInfo();
	bool isShowAssign();

	//加载帐号已读消息
	void loadReadMsgInfo();
	void SaveReadMsgInfo();
	bool hasRead(int msgId);
	//红点提醒相关
	void setNewMsgTag(bool value) {mBNewMsg = value;}
	bool getNewMsgTag() {return mBNewMsg;}

	void setNewTaskTag(bool value) {mBNewTask = value;}
	bool getNewTaskTag()  {return mBNewTask;}

	//新的拍卖行记录
	void setNewAuctionTag(bool value) {mBNewAuction = value;}
	bool getNewAuctionTag() const{return mBNewAuction;}

	//游戏界面初始化完成标记
	void setHasFinish(bool value ) {mGameInitFinish = value;}
	bool getHasFinish() const { return mGameInitFinish;}

	//记录当前背景音乐名
	void setMusicName(const char* name) {mCurBgMusicName = name;}
	std::string getMusicName() {return mCurBgMusicName;}

	//比赛种类ID
	void setGameMatchKind(int id){ mMatchId = id;}
	int   getGameMatchKind() {  return mMatchId;}

	__Array* getMatchRooms() {return mMatchRooms;}
	void sortMatchRooms();
	bool         isInMatchRooms(int matchInfoID);
	MatchInfoModel* getMatchInfoByID(LONGLONG matchInfoID);
	bool     isHastheMatch(int matchType);
	void     removeMatchRoomByMatchInfoID(LONGLONG matchInfoID);
	int       getMatchRoomCountByType(int matchType);
	void    setMatchStatus(int status) {mMatchGameStatus = status;}
	int       getMatchStatus() {return mMatchGameStatus;}
	void     clearMatchRooms() {mMatchRooms->removeAllObjects();}
	MatchSerialModel* getRoomSerial() { return mMatchSerialModel;}

	void setMatchSever(const char* addr) {match_server = addr;}
	std::string getMatchSever() {return match_server;}

	void setMatchPort(int port) {match_port = port;}
	int getMatchPort() {return match_port;}

	void setMatchStart(int value) {bMatch_Start = value;}
	int    getMatchStart() {return bMatch_Start;}

	std::string getLoginAddr();
	void        setLoginAddrNull() { login_server = ""; }
	__Array*   getLoginUrls() { return mLoginUrls; }
	void       clearHornMsg();
	//喇叭消息
	std::list<std::string> listHornMsg;
private:
	__Array* mRoomLevels;//所有房间信息列表
	__Array* mAllUsers; //所有用户列表
	int mUserId;
	int mChairId;
	bool m_IsMusicOn;
	bool m_IsEffectOn;
	LoginUserModel* m_userLoginModel;
	OnlineUserModel* m_user;
	int m_gameLaunchCount;
	//数据缓存
	std::shared_ptr<Map<std::string, Ref*> >    m_responseDataCache;
	std::string mPwd;
	int  m_taskCnt;
	std::string m_taskServerAddr;
	int  m_taskPort;
	bool mBankPwdFlag;
	__Array* mMsgArray;//消息数组
	std::string mInsurePass;
	__Array* mGamePlayers;//本桌的玩家数组
	bool mInGame;
	int  mGameKindIndex;   //游戏种类索引

	__Array* mAccountArray;

	int   mLoginScoreStatus;//登录奖励 是否领取   0:无效 1：未领取 2：已领取
	int   mRedStatus;//红包，红包碎片 是否领取 0:无效 1：未领取 2：已领取

	long   mLastDealBombTick;

	std::map<std::string,std::string> mAssignMap;
	std::map<std::string,std::vector<int>> mMsgMap;
	bool mBNewMsg;//有新的消息
	bool mBNewTask;//有新的任务完成
	bool mBNewAuction;//有新的拍卖记录

	bool mGameInitFinish ;
	std::string mCurBgMusicName;
	int   mMatchId;

	//比赛房列表
	__Array* mMatchRooms;
	int    mMatchGameStatus;

	//当前比赛房间唯 一性 
	MatchSerialModel* mMatchSerialModel;

	std::string match_server;
	int              match_port;
	bool            bMatch_Start;
	std::string login_server;
	__Array* mLoginUrls;

};




#endif //_SessionManager_H_H_