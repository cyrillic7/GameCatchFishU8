/**
 * @file GameServiceClient
 * @brief 联网游戏的客户端封装
 * @author tjl
 * @date
 */

#ifndef __GAMESERVICECLIENT_H_
#define __GAMESERVICECLIENT_H_

#include "cocos2d.h"
//#include "core/TCPSocket.h"
#include "core/GameSocket.h"
#include "GolbalDefine/Macro.h"
#include "MsgDefine/CMD_UserManage.h"
#include "Model/MatchModel.h"
#include "Model/RoomLevelInfo.h"
#include "Message.h"
#include "Common/ShopManager.h"

#define MAX_TCP_LEN 1024*5
struct SendData
{
	char sSendData[MAX_TCP_LEN];
	DWORD dwDataLen;
};


#define VERSION_FRAME				16777217
#define VERSION_CLIENT				17170433
#define VERSION_PLAZA 				17235969

#define FRAME_SORKET_INDEX 1 //大厅SORKET 索引
#define GAMEROOM_SORKET_INDEX 2//游戏SORKET  索引
#define TASK_SORKET_INDEX     3 //任务SORKET 索引

#define MAX_HORN_MSG_COUNT 10
#define  taskClient "taskClient" 

USING_NS_CC;

//错误消息key
#define ERROR_MESSAGE_KEY       "ErrorMessage"

//公共消息命令
#define PublicMessageCommand    "PublicMessage"

//网络连接失败了
#define ConnectionLostMessage   "ConnectionLostMessage"

//smartfox开始自动retry
#define ConnectionStartRetryMessage "ConnectionStartRetryMessage"

//网络恢复了
#define ConnectionResumeMessage "ConnectionResumeMessage"

//房间消息
//加入房间成功, 附带参数房间名称
#define JoinRoomSucceedMessage         "JoinRoomSucceedMessage"
//加入房间失败
#define JoinRoomFailedMessage         "JoinRoomFailedMessage"

//专门给礼物列表用的加入房间消息，只有加入lobby以外的房间时，才会触发这个。别的地方不要用！！！！！！！
#define JoinGameRoomMessage "JoinGameRoomMessage"

//其他用户进入房间
#define UserEnterRoomMessage         "UserEnterRoomMessage"

//其他用户退出房间, 附带参数房间名称
#define UserExitRoomMessage         "UserExitRoomMessage"

//房间更新, 附带参数房间名称
#define UpdateRoomMessage       "UpdateRoomMessage"

//用户信息更新消息
#define UpdateUserInfoVariableMessage   "UpdateUserInfoVariableMessage"

//房间角色转换
#define PlayerToSpectatorMessage        "PlayerToSpectatorMessage"
#define PlayerToSpectatorErrorMessage   "PlayerToSpectatorErrorMessage"
#define SpectatorToPlayerMessage        "SpectatorToPlayerMessage"
#define SpectatorToPlayerErrorMessage   "SpectatorToPlayerErrorMessage"

//收到服务器推送过来的主持人消息, 进行跨游戏跳转
#define ModeratorUpdateMessage          "ModeratorUpdateMessage"

//游戏登录成功
#define LoginGameSucceedMessage "LoginGameSucceedMessage"

//接收到游戏登录成功的消息
#define ReceiveLoginGameSucceedMessage "ReceiveLoginGameSucceedMessage"

//游戏登录失败
#define LoginGameFailedMessage "LoginGameFailedMessage"

//登出游戏
#define LogoutGameMessage "LogoutGameMessage"

//网络较差的消息
#define BadNetworkingMessage "BadNetworkingMessage"

//网络恢复
#define NetworkingTurnGoodMessage "NetworkingTurnGoodMessage"

/************************************************************************/
/* GameServiceClient Class                                              */
/************************************************************************/
// 一个游戏对应这么一个客户端
// 负责读取配置，联网，登录游戏，进入房间

class GameServiceClient : public Ref,public IGameSocketSink {
public:
    static GameServiceClient *create(int sorketIndex);
    bool init(int sorketIndex);
    GameServiceClient();
    virtual ~GameServiceClient();

	// 连接事件
	virtual bool OnEventTCPSocketLink(bool bConnectSucc);
	//关闭事件
	virtual bool OnEventTCPSocketShut(unsigned char cbShutReason);

    void update(float dt);
    void cleanup();
	//virtual bool initSocket(int index);
	bool Connect(const char* ip, unsigned short port);
    //重试, 房间名称为NULL时, 连接到小厅, 不为NULL时, 连接到指定房间
    CC_SYNTHESIZE(bool, m_isRetrying, IsRetrying);

    //游戏名称
    CC_SYNTHESIZE(std::string, m_gameZone, GameZone);

    //当前房间
    CC_SYNTHESIZE(std::string, m_roomName, RoomName);

	//defautl is yes. 如果scene退出，但是client需要保存，就把这个变量设置成false
	CC_SYNTHESIZE(bool, m_needPassToResponseHandler, NeedPassToResponseHandler);
public:
	//登录大厅
	void onLogin(const char* accout,const char* pwd);
	//U8登录
	void onLoginU8(const char* umid, const char* username, const char* token, const char* userid);
	//微信登录
	void onWxLogin(const char* token);
	//登录真正房间
	void loginRealRoom(RoomLevelInfo* info);
	//登录比赛房间
	void loginMatchRoom();
	//发送获取游戏配置
	void sendGetGameConfig();
	//
	void sendGetGameMatchConfig();
	//登出
	void logout();
	//注册
	void onRegister(char* account,char* nick,char* pwd);
	//重连
	void retry();

	void registerHandler();
	//发送开火消息
	void sendUserFire(float angle,int bullet_mulriple);
	//发送上分消息
	void exchangeScore(int score);
	//发送同步时间消息
	void SendTimerSync();
	//发送起立消息
	void sendStandUp();
	//发送获取礼包/首具请求
	void  SendRequestShop(int type);
	void  sendBuyGiftRequest(int proType, int proNum,int proId);//购买 兑换道具
	void  sendUserKnapsackRequset(int id,int num,const char* szNote = "");//使用背包
	//发送拍卖行请求相关
	void  SendAuctionRequset(int subCmd,int pageIndex,int pageCount,WORD userId = 0,int lastday = 12);
	//获取上架物品
	void SendAuctionListRequset();
	//发送上架物品请求
	void sendSellAuctionRequest(__Dictionary*info);
	//发送购买物品请求
	void sendBuyAuctionRequest(__Dictionary*info);
	//发送撤销拍卖请求
	void sendCancelAuctionRequest(__Dictionary* info);
	//查询拍卖
	void sendQueryAuctionRequest(int pageIndex,int pageCount,char* text,int lastday = 12);
	//获取财富详细请求
	void sendTreasureRequest();
	//提取拍卖所得请求
	void sendConvertAutionScoreRequest();
	//发送礼包兑换请求
	void sendGiftConvertRequest(const char* covertCode);
	//bag 相关
	void sendKnapSackRequest();
	//vippower 相关
	void sendVipPowerRequest();
	//发送领取VIP 奖劢请求
	void sendVipAwardRequest(int awardType);
	//登录任务服务器
	void loginTaskServer();
	//发送获取任务列表请求
	void sendGetTaskList();
	//发送领取任务奖励请求
	void sendTaskAwardRequest(int taskId);
	//捕鱼比赛相关
	void sendRefrshMatchPlayers();
	//比赛报名
	void sendSignUpRequest(MatchInfoModel* info);
	//取消报名
	void sendUnSignUpRequest(MatchInfoModel* info);
	//获取比赛开始时间
	void sendCountDownRequest(MatchInfoModel* info);
	//发送比赛进入消息
	void sendMatchUserComeRequest();
	//离开比赛
	void sendLeaveMatchRequest();
	//再次挑战
	void sendTryAaginRequest();

	void closeSoket();

	//获取银行信息
	void sendQueryBankRequest();
	//存入金币
	void sendUserSaveScoreRequest(SCORE score);
	//提取金币
	void sendUserTakeScoreRequest(SCORE score);
	//修改银行密码
	void sendModifyInsurePass(const char* pwd);
	//验正银行密码
	void sendVerifyInsurePass(const char* pwd);
	//修改登录密码/银行密码
	void sendModifyPassword(const char* oldPwd,const char *newPwd ,int type); //type = 0 表示修改登录密码，1表示修改银行密码
	//发送获取签到列表
	void sendGetAssignList();
	//发送签到消息
	void sendAssignMsg();

	//绑定手机相关
	//发送获取验证码请求
	void sendGetVerifyCode(const char* phone);
	//发送绑定手机请求
	void sendBindPhoneRequest( int code );

	//找回登录密码相关
	void sendEnsureAccoutRequest(const char* accout);
	void sendGetVerifyCodeByUid();
	void sendSetPassRequest(int verifyCode ,const char* newPwd);
	//找回银密码相关
	void sendInsureVerifyRequest();
	void sendSetInsuerPass(int verifyCode,const char* newInsurePwd);
	//元宝兑换
	void sendExchangeIngotRequest(int ingot);
	//修改性别
	void sendModifyIndividual(bool isMan = false);//0为女，1为男
	//修改头像
	void sendModifyFaceId(int id);
	//发送反馈消息
	void sendFeedBackRequest(const char* content,const char* pQQ);
	//发送更多游戏
	void sendMoreGameRequest();
	//发送获取下载资源相关信息
	void sendDownLoadUrlRequest(int gameKind);
	//是否连接
	bool isConnect() { return m_GameSocket.IsConnected();}

	//发送快速登录
	void sendQucikLogin(const char* uuid);
	//充值下定单
	void sendRechargeOrder(int ingot,const char* orderInfo, bool isFirstRecharge = false);
	//解析登录附加数据
	void parseLoginExpandData(void * pDataBuffer, unsigned short wDataSize);
	//发送喇叭请求
	void sendhornRequset(const char* content);
	//发送转盘请求
	void sendLuckSpinRequest();
protected:
	bool OnTCPSocketRead(WORD wSocketID, TCP_Command Command, VOID * pData, WORD wDataSize);
	bool OnTCPNetworkMainMBLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	bool OnTCPNetworkMainMBCount(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	void parseGift(void* pData,WORD wDataSize,ShopItemType type);
	std::list<SendData> m_RecvData; 
	__Array* m_responseHandlers;
	BYTE mGameStatus;//游戏状态
	bool m_isLookOn; //是否旁观
	bool m_isLogin;
	std::mutex    m_mutex;
	CGameSocket   m_GameSocket;
	int           m_clientType;
	bool          m_bConnectFail;

};

#endif //__GAMESERVICECLIENT_H_
