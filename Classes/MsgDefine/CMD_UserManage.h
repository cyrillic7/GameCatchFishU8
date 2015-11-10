#ifndef CMD_USERMANAGE_HEAD_FILE
#define CMD_USERMANAGE_HEAD_FILE

#include "GameGlobalSetting.h"
#include "GolbalDefine/Define.h"

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#define  MAX_SERVERID		    500										//最大服务器id
//用户管理服务器
//I D 登录
#define  MDM_UM_LOGIN			10										//房间服务器登录到UM
#define  MDM_UM_USER			11										//用户的一些信息
#define  MDM_UM_S_USER			12		
//机器人
#define  MDM_UM_ANDROID			13										//机器人

#define  MDM_UM_MB_DATA			14										//手机数据

#define  MAX_LOG				256										//日志长度
//服务间传递
								//服务器传递
#define  SUB_UM_S_SERVERLOGIN    		100								//服务器登录
#define	 SUB_UM_S_USER_ENTER	   		101								//用户进入房间
#define  SUB_UM_S_USER_LEAVE	   		102								//用户退出房间
#define	 SUB_UM_S_USER_SCORE	   		103								//用户得分 
#define  SUB_UM_S_USER_GAME_SET			104								//设置用户分数
#define  SUB_UM_S_SYS_SET				105								//设置游戏分数
#define  SUB_UM_S_TRANSFER_SCORE		106								//转账
#define	 SUB_UM_S_SYNC_USER				107								//断线时，同步有用户（房间->UM）
#define  SUB_UM_S_GET_STORAGE			108								//获取库存
#define  SUB_UM_S_TRANSFER_GAME_SCORE	109								//游戏转账
#define  SUB_UM_S_USER_LOGIN			110								//大厅登录
#define  SUB_UM_S_UPDATE_SCORE			111								//更新币
#define  SUB_UM_S_KICKUSER				112								//踢出冻结的用户
#define	 SUB_UM_S_USER_TEMP_SCORE	   	113								//临时变化分数
#define	 SUB_UM_S_MAXANDROIDCOUNT	   	114								//最大机器人个数
#define	 SUB_UM_S_STOPANDROIDIN	   		115								//机器人进入停止

//机器人
#define  SUB_UM_S_ATTRIB_OPTION			200								//服务属性,配置
#define  SUB_UM_S_ANDROIDLOGIN			201								//进入
#define  SUB_UM_S_ANDROIDLOGOUT			202								//退出
#define  SUB_UM_S_SYNC_ANDROID			203								//断线时，同步机器人
#define  SUB_UM_S_USERCOUNT				204								//玩家人数



	
//客户端
#define	 SUB_UM_USER_ENTER	   			200								//用户进入房间
#define  SUB_UM_USER_LEAVE	   			201								//用户退出房间
#define	 SUB_UM_USER_SCORE	   			202								//用户得分 
#define  SUB_UM_USER_INFO				203								//用户信息
#define  SUB_UM_GET_USER_GAME_SET		204								//获取用户分数
#define  SUB_UM_GET_SYS_SET				205								//获取游戏分数
#define  SUB_UM_SET_USER_GAME_SET		206								//设置用户分数
#define  SUB_UM_SET_SYS_SET				207								//设置游戏分数
#define  SUB_UM_SET_USER_SCORE			208								//用户输赢
#define  SUB_UM_GET_USER_SCORE			209								//用户输赢
#define  SUB_UM_SET_USER_VIP			210								//vip用户
#define  SUB_UM_DELETE_USER_VIP			211								//删除vip用户
#define  SUB_UM_GET_STORAGE				212								//获取库存
#define  SUB_UM_HEART_BEAT				213                             //socket心跳
#define  SUB_UM_SET_DEFAULT				214                             //恢复默认
#define  SUB_UM_GET_ALL_STORAGE			215								//获取所有库存
#define  SUB_UM_SET_ALL_STORAGE			216								//设置所有库存
#define  SUB_UM_ADMIN					217								//管理员列表
#define  SUB_UM_LOG						218								//操作日记
#define  SUB_UM_LTANKING				219								//放水
#define  SUB_UM_SET_SYS_SET_LTANKING	220								//设置放水
#define  SUB_UM_GET_USER				221								//获取用户
#define  SUB_UM_GET_OFFLINE				222								//获取离线用户设置过的用户
#define  SUB_UM_GET_USER_EX				223								//获取用户
#define  SUB_UM_TRANSFERSCORE			224								//转帐
#define  SUB_UM_ANDROIDCOUNT			225								//机器人个数
#define  SUB_UM_GAMESCORE				226								//查询分数
#define  SUB_UM_FORZENUSER				227								//冻结用户

//手机监控
#define  SUB_UM_MD_COUNT				100								//人数





#define  UM_MAX_ANDROID					MAX_ANDROID*20					//机器人数

enum SetType
{
	MAXWIN = 0,						//赢
	MAXLOSE,						//输
	SYSMAXWIN,						//系统赢
	SYSMAXLOSE,						//系统输
	MAXNOTIFYWIN,					//预警赢
	MAXNOTIFYLOSE,					//预警输
	MAXDEFAULT,						//系统默认
	OPFINISH						//操作完成
};

enum LogType
{
	OPLOG = 1,						//操作
	ERRORLOG						//错误
};
//房间基本信息
struct tabUMServerRoomInfo
{
	tabUMServerRoomInfo()
	{
		memset(this, 0, sizeof(tabUMServerRoomInfo));
	}
	DWORD							dwServerID;							//server I D
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	CHAR							szServerName[LEN_SERVER];			//房间名称
	DWORD							dwMinEnterScore;					//入场分数
	WORD							wKindID;							//类型索引
};

struct tagUMServerRoomLogin
{
	tagUMServerRoomLogin()
	{
		memset(this, 0, sizeof(tagUMServerRoomLogin));
	}
	//登录信息
	DWORD							dwServerID;							//server I D
	TCHAR							szPassword[LEN_MD5];				//登录密码
	TCHAR							szMachineID[LEN_MACHINE_ID];		//机器序列
	CHAR							szServerName[LEN_SERVER];			//房间名称
	DWORD							dwMinEnterScore;					//入场分数
	WORD							wKindID;							//类型索引
};

//用户基本信息
struct tagUMUserInfo
{
	tagUMUserInfo()
	{
		memset(this, 0, sizeof(tagUMUserInfo));
	}
	DWORD						dwUserID;
	DWORD						dwServerID;
	TCHAR						szNickName[LEN_ACCOUNTS];			//用户昵称	
	DWORD						dwBeginTime;						//游戏开始时间
	DWORD						dwEndTime;												
	//积分信息
	SCORE						lScore;								//用户分数
	SCORE						lInsure;							//用户银行
};

//用户分数信息
struct tagUMUserScoreSet
{
	tagUMUserScoreSet()
	{
		memset(this, 0, sizeof(tagUMUserScoreSet));
		dwType = MAXDEFAULT; 
	}
	DWORD						dwUserID;
	DWORD						dwGameID;		
	DWORD						dwKindID;							//游戏种类
	DWORD						dwServerID;							//房间id
	DWORD						dwTableID;							//桌子
	DWORD						dwIsSet;							//是否可以控制
	WORD						wOnline;							//0：离线，1：在线
	TCHAR						szNickName[LEN_ACCOUNTS];			//用户昵称	

	DWORD						dwBeginTime;						//游戏开始时间

	DWORD						dwWinCount;							//赢局
	DWORD						dwLoseCount;						//输局

	//积分信息
	SCORE						lScore;								//用户分数
	SCORE						lInsure;							//用户银行

	//转帐
	SCORE						lOutScore;							//转出
	SCORE						lInScore;							//转入
	TCHAR						szLastOut[64];						//最后一次转出详细
	TCHAR						szLastIn[64];						//最后一次转入详细

	SCORE						lAllOutScore;						//总转出
	SCORE						lAllInScore;						//总转入
	SCORE						lDifference;						//差值		

	SCORE						lWinScore;							//输赢分数

	SCORE						lTempScore;							//设置后输赢分数

	SCORE						lMaxWin;							//赢分数
	SCORE						lMaxLose;							//输分数

	SCORE						lTobayWin;							//当天输赢金币
	SCORE						lMoney;								//充值

	SCORE						lSetWinOrLose;						//设置输赢分数

	SCORE						lNotifyMaxWin;						//预警赢分数
	SCORE						lNotifyMaxLost	;					//预警输分数

	SetType						dwType;								//类型
	DWORD						dwWinOrLostRate;					//换牌比率（百分比）
	DWORD						dwChannel;							//渠道：1：试玩站；0：正常
	int							nVipType;							//用户类型 （如vip）
	DWORD						dwClientAddr;						//IP
	TCHAR						szMachineID[LEN_MACHINE_ID];		//硬件号
	DWORD						dwSetTime;							//设置的时间
	TCHAR						szRemarks[256];						//备注			
};

//转账
struct tagUMTransferScore
{
	tagUMTransferScore()
	{
		memset(this, 0, sizeof(tagUMTransferScore));
	}
	DWORD						dwUserID;
	DWORD						dwTargetUserID;
	//转帐
	SCORE						lOutScore;							//转出
};
//用户分数
struct tagUMGameScore
{
	tagUMGameScore()
	{
		memset(this, 0, sizeof(tagUMGameScore));
	}
	DWORD						dwServerID;							//server I D
	DWORD						dwTableID;
	DWORD						dwUserID;
	SetType						dwType;								//类型
	DWORD						dwWinOrLostRate;					//换牌比率（百分比）
	SCORE						lGameScore;							//用户得分
	SCORE						lScore;								//分数
	SCORE						lInsure;							//用户银行
};
//退出房间
struct tagUMUserLeave
{
	tagUMUserLeave()
	{
		memset(this, 0, sizeof(tagUMUserLeave));
	}
	DWORD						dwServerID;							//server I D
	DWORD						dwTableID;
	DWORD						dwAndroid;							//是否是机器人
	DWORD						dwUserID;
};
struct tabServerID
{
	tabServerID()
	{
		dwServerID=0;
	}
	DWORD dwServerID;
};
//系统分数信息
struct tagUMUserGameSet
{
	tagUMUserGameSet()
	{
		memset(this, 0, sizeof(tagUMUserGameSet));
		bNullity = true;
	}
	DWORD						dwServerID;	
	DWORD						dwSetTime;							//设置时间
	DWORD						dwIsSet;							//是否可以控制

	SCORE						lMaxWin;							//赢分数
	SCORE						lMaxLose;							//输分数

	SCORE						lMaxWin1;							//赢分数(保留)
	SCORE						lMaxLose1;							//输分数(保留)

	SCORE						lNotifyMaxWin;						//预警赢分数
	SCORE						lNotifyMaxLost	;					//预警输分数

	DWORD						dwWinOrLostRate;					//换牌比率（百分比）
	DWORD						dwWinOrLostRate1;					//换牌比率（百分比）（保留)
	TCHAR						szPhone[48];						//预警使用的手机
	TCHAR						szEmail[48];						//预警使用的邮箱

	DWORD						dwAndroidMinScore;					//机器人最小分
	DWORD						dwAndroidMaxScore;					//机器人最大分

	bool						bNullity;							//是否有效
};
//整个房间设置
struct tagUMSysGameSet
{
	tagUMSysGameSet()
	{
		memset(this, 0, sizeof(tagUMSysGameSet));
		//strcpy(szItem0, TEXT("[ %s ]上庄了，让我们给他点颜色看看"));
		//strcpy(szItem1, "[ %s ]上庄了，掌声欢迎");
		//strcpy(szItem2, "[ %s ]上庄了，大家尽情游戏吧");
	}
	DWORD  dwServerID;									//serverid
	DWORD  bValid;										//以下是否有效

	bool   bStorag;										//库存是否刷新
	LONGLONG  dwStoragStart;							//(库存起始值）	
	DWORD  dwStorageDeduct;								//（衰减值）

	//如客户端没有处理，则不支持
	//DWORD  dwFreeTime;									//空闲时间
	//DWORD  dwCallBanker;								//叫庄时间	
	//DWORD  dwBetTime;									//下注时间	
	//DWORD  dwEndTime;									//开牌时间	
	//int  dwTimeParameters1;								//参数1
	//int  dwTimeParameters2;								//参数2
	//int  dwTimeParameters3;								//参数3
	//int  dwTimeParameters4;								//参数4

	////目前通比牛牛使用
	//DWORD dwUsuallyChangePercent;						//通常情况下，换牌百分比
	//DWORD dwRobotWinScorePercent;						// ( > RobotDefineTanking) 扣除机器人赢的分数的百分比 

	//int	  dwRealBankerRobotLoseMax;						//输: 一局（*人数），最大分数有概率换牌(牛牛出来概率 *人数)
	//int	  dwRealBankerRobotLoseChangePercent;			//输: 一局（*人数），换牌百分比（*RealBankerRobotLoseMax ）
	//int	  dwRealBankerRobotLoseMaxChange;				//输: 一局（*人数），起始分数,百分百换牌（2500001 以上：没使用）

	//SCORE dwRobotLoseMax;								//输:最大分数有概率换牌
	//DWORD dwRobotLoseChangePercent; 					//输:换牌百分比(*RobotLoseMax)
	//SCORE dwRobotLoseMaxChange;							//输:的起始分数,百分百换牌

	//DWORD dwRobotProbabilityTanking;					//赢:分数，有概率放水
	//DWORD dwRobotTankingPercent;						//赢:百分百放水（*RobotProbabilityTanking）
	//DWORD dwRobotDefineTanking;							//赢:百分百放水
	//////////////////////////////////////////////////////////////////

	//SCORE dwRobotResultScore;							//累计分数
	//SCORE dwRobotWinChiefScore;


	////机器人存款取款
	//DWORD dwRobotScoreMin;								//机器人的分数小于该值时执行取款
	//DWORD dwRobotScoreMax;								//机器人的分数大于该值时执行存款
	//DWORD dwRobotBankGet;								//取款最小数额(取款数是该值以上的一个随机数)
	//DWORD dwRobotBankGetBanker;							//取款最大数额(此数值一定要大于RobotBankGet)
	//DWORD dwRobotBankStoMul;							//存款比例

	////百人游戏
	//DWORD dwScore;										//坐庄金币数(大于此数才可做庄)
	//DWORD dwTime;										//坐庄次数
	//DWORD dwTimeAdd;									//坐庄增加次数(当玩家Time次庄后若其金币大于其他申请玩家,再可坐次数)
	//DWORD dwBankerScore;								//额外坐庄金币数(大于此金币后无论是否有大于其的申请玩家，均可继续)
	//DWORD dwBankerAdd;									//额外次数(金币达到BankerScore后,Time次坐庄后再额外增加的坐庄次数)
	//DWORD dwEnableSysBanker;							//系统坐庄


	//DWORD dwAreaLimitScore;								//区域限制(每个注区的最高封顶数量)
	//DWORD dwUserLimitScore;								//个人下注限制

	//DWORD dwMessageCount;								//上庄信息

 //	TCHAR szItem0[64];									//[ %s ]上庄了，让我们给他点颜色看看
	//TCHAR szItem1[64];									//[ %s ]上庄了，掌声欢迎
	//TCHAR szItem2[64];									//[ %s ]上庄了，大家尽情游戏吧


	//DWORD dwORDRobotMaxBetTime;							//机器人下注范围(上限)
	//DWORD dwRobotMinBetTime;							//机器人下注范围(下限)
	//DWORD dwRobotMaxJetton;								//机器人筹码范围(上限)
	//DWORD dwRobotMinJetton;								//机器人筹码范围(下限)
	//DWORD dwRobotBanker;								//机器人是否坐庄：1时，机器人可坐庄；0时机器人不能坐庄
	//DWORD dwRobotBankerCount;							//机器人坐庄次数	
	//DWORD dwRobotWaitBanker;							//机器人空几盘之后申请上庄
	//DWORD dwRobotApplyBanker;							//机器人最多申请个数
	//DWORD dwRobotBetCount;  							//机器人最多下注个数
	//DWORD dwRobotAreaLimit;	     		    			//机器人区域下注限制
    /////////////////////////////////////////////////////////////////////////////

	SCORE AllScore;										//所有累计
	SCORE MonthScore;									//当月累计(改为昨天)
	SCORE WeekScore;									//本周累计(改为前天)
	SCORE TodayScore;									//当天累计

	SCORE TodayTankingScore;							//当天累计放水
	SCORE TankingScore;									//放水
	SCORE CurTankingScore;								//当前放水
	DWORD dwServerRate;									//放水概率
};
struct tagStoragInfo
{
	tagStoragInfo()
	{
		memset(this, 0, sizeof(tagStoragInfo));
	}
	DWORD dwServerID;
	DWORD dwAll;									//是否所有（0：只是一个，1：所有）
	DWORD dwSocket;									//转发中用的socket
	DWORD  bValid;									//以下是否有效
	LONGLONG  lStoragStart;							//(库存起始值）	
	DWORD  dwStorageDeduct;							//（衰减值）
};
struct tabUserVip
{
	tabUserVip()
	{
		memset(this, 0, sizeof(tabUserVip));
	}
	DWORD dwUserID;
	int	  nType;
};
struct tabUMAndroid
{
	tabUMAndroid()
	{
		memset(this, 0, sizeof(tabUMAndroid));
	}
	DWORD dwUserID;
};

struct tabUMAndroidArray
{
	tabUMAndroidArray()
	{
		memset(this, 0, sizeof(tabUMAndroidArray));
	}
	DWORD dwServerID;
	DWORD dwCount;
	DWORD dwUserID[1];
};
struct tabOnLineCount
{
	tabOnLineCount()
	{
		memset(this, 0, sizeof(tabOnLineCount));
	}
	DWORD dwServerID;
	DWORD dwAllUserCount;
	DWORD dwUserCount;
};
struct tabUserID
{
	tabUserID()
	{
		memset(this, 0, sizeof(tabUserID));
	}
	DWORD dwUserID;
};
struct tabStoragStart
{
	tabStoragStart()
	{
		memset(this, 0, sizeof(tabStoragStart));
	}
	DWORD dwServerID;
	DWORD bValid;
	SCORE lStoragStart;
	DWORD dwStorageDeduct;
};
struct tabStoragStartArray
{
	tabStoragStartArray()
	{
		memset(this, 0, sizeof(tabStoragStartArray));
	}
	DWORD dwCount;
	tabStoragStart StoragStart[100];
};
//在线管理员
struct tagAdmin
{
	tagAdmin()
	{
		memset(this, 0, sizeof(tagAdmin));
	}
	DWORD						dwUserID;
	DWORD						dwGameID;		
	DWORD						dwType;								//0：在线， 1：离线
	TCHAR						szNickName[LEN_ACCOUNTS];			//用户昵称	
	DWORD						dwLastLoginTime;					//最后登录时间				
};
struct tagLog
{
	tagLog()
	{
		memset(this, 0, sizeof(tagLog));
	}
	DWORD						dwType;					
	TCHAR						szLog[MAX_LOG];
};
struct tagUserLogin
{
	tagUserLogin()
	{
		memset(this, 0, sizeof(tagUserLogin));
	}
	DWORD						dwUserID;
	DWORD						dwGameID;		
	TCHAR						szNickName[LEN_ACCOUNTS];			//用户昵称	

	//积分信息
	SCORE						lScore;								//用户分数
	SCORE						lInsure;							//用户银行

	DWORD						dwClientAddr;						//IP
	TCHAR						szMachineID[LEN_MACHINE_ID];		//硬件号
};
struct tagUpdateScore
{
	tagUpdateScore()
	{
		memset(this, 0, sizeof(tagUpdateScore));
	}
	DWORD						dwUserID;
	//积分信息
	SCORE						lScore;								//用户分数
	SCORE						lInsure;							//用户银行
};
struct tagFindUser
{
	TCHAR						szText[LEN_ACCOUNTS];			//找查的内容 
};
//用户分数信息
struct tagTransferScore
{
	tagTransferScore()
	{
		memset(this, 0, sizeof(tagTransferScore));
	}
	DWORD						dwUserID;
	DWORD						dwGameID;					
	DWORD						dwTargetUserID;						
	DWORD						dwTargetGameID;		
	TCHAR						szNikeName[LEN_ACCOUNTS];
	TCHAR						szTargetNikeName[LEN_ACCOUNTS];	
	SCORE						lScore;
};

struct tagAndroidCount
{
	tagAndroidCount()
	{
		memset(this, 0, sizeof(tagAndroidCount));
	}
	DWORD   dwServerID;
	DWORD	dwBeginTime;
	DWORD	dwEndTime;
	int		nAndroidCount;
};

struct tagQueryGame
{
	tagQueryGame()
	{
		memset(this, 0, sizeof(tagQueryGame));
	}
	DWORD dwServerID;
	DWORD dwDay;
};
struct tagQueryGameScore
{
	tagQueryGameScore()
	{
		memset(this, 0, sizeof(tagQueryGameScore));
	}
	TCHAR						szNickName[LEN_ACCOUNTS];			//用户昵称	
	DWORD						dwUserID;
	DWORD						dwGameID;		
	SCORE						lScore;
};
//多个用户，逗号分开
struct tagUserID
{
	tagUserID()
	{
		memset(this, 0, sizeof(tagUserID));
	}
	TCHAR							szUserID[256];						//用户ID 如 （102，103，105）
	TCHAR							szForzen[64];						//原因
};
//最大机器人
struct tagMaxAndroidCount
{
	tagMaxAndroidCount()
	{
		memset(this, 0, sizeof(tagMaxAndroidCount));
	}
	DWORD						dwServerID;
	DWORD						dwMaxAndroidCount;
};
struct tagMDCount
{
	tagMDCount()
	{
		memset(this, 0, sizeof(tagMDCount));
	}
	DWORD						dwCount;
};
#pragma pack()

#endif //CMD_USERMANAGE_HEAD_FILE