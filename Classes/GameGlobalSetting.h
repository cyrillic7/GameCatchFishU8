#ifndef GAME_GLOBAL_SETTING_H_
#define GAME_GLOBAL_SETTING_H_


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef char TCHAR, *PTCHAR;

#define MAKELONG(a, b)      ((DWORD)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))


#ifndef VOID
	#define VOID void
	typedef char CHAR;
	typedef short SHORT;
	typedef long LONG;
	typedef int INT;
#endif

#endif

#ifdef WIN32
typedef __int64 LONGLONG;
typedef unsigned __int64 ULONGLONG;
#define MAXLONGLONG                      (0x7fffffffffffffff)
#else
	typedef long long  LONGLONG;
	typedef long long  ULONGLONG;

	typedef struct _SYSTEMTIME {
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
	} SYSTEMTIME;

	typedef struct _GUID {
		unsigned long  Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[ 8 ];
	} GUID;
#endif //_MAC and WIN32


#pragma pack(1)

//------------------------------------------------------------------------------
// 服务定义

#define KIND_ID               260
#define GAME_NAME             TEXT("大闹天宫")
#define GAME_PLAYER           6
#define VERSION_SERVER        PROCESS_VERSION(1,0,0)
#define VERSION_CLIENT        PROCESS_VERSION(1,0,0)

//------------------------------------------------------------------------------
// 游戏定义

/*
// 座位号
------------------
    0   1   2 

    5   4   3
------------------
*/

#ifndef SCORE
#define SCORE LONGLONG
#endif

#ifdef WIN32
#include <winsock2.h>
typedef int				socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
//
typedef int				SOCKET;

//#pragma region define win32 const variable in linux
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
//#pragma endregion
#endif
struct Position {
  float x;
  float y;
};

const int kFPS = 30;              // 帧率
const float kSpeed = 1.f / kFPS;  // 速度

const int kGameLoopElasped = 30;

const int kScreenWidth = 1366;    // 屏宽
const int kScreenHeight = 768;    // 屏高

const unsigned int  kMaxFishInManager = 4096;
const unsigned int kMaxBulletsInManager = 4096;

const int kMaxCatchFishCount = 3;
const int kMaxChainFishCount = 6;

const float kBulletDoubleTimeout = 15;

const float kTideActionDuration = 6.0f;

const float kFishPauseTime = 10;

#define Bglu_Type_Ta 0 //镇妖塔
#define Bglu_Type_Lua 1 //风火轮
#define Bglu_Type_Bang 2 


#define  ImageURLFormat "http://121.41.116.223:8090/image/PropIcon/%s.png"

#define  TaskURLFormat "http://121.41.116.223:8090/image/TaskIcon/%s.png"

//游戏资源下载后存储的路径
#define GameResourcePackStoragePath        "GameResources/"

const int KvipDiscount[8] = {100,98,95,92,90,88,85,80};
//游戏里音效资源
static const char* GameMusic[] = {
    "addgun",
	"bingo",
	"bomb_baozha",
	"bullet_baozha",
	"change_bg",
	"fhlzi",
	"fire",
	"fishdiescore",
	"highscore01",
	"highscore02",
	"highscore03",
	"highscore04",
	"highscore05",
	"hitbagualu",
	"inscore",
	"jgtzi",
	"qpzd_yuyin",
	"ryjgbzi",
	"sound_change_gun",
	"sound_LZP_fire",
	"sunwukong",
	"yuhuangdadi",
};

static const int MusicCount = sizeof(GameMusic) / sizeof(char*);


//LK游戏里音效资源
static const char* GameMusicForLk[] = {
	"addgun",
	"bingo",
	"bomb_baozha",
	"bullet_baozha",
	"change_bg",
	"fire",
	"fishdiescore",
	"fish10_1",
	"fish10_2",
	"fish11_1",
	"fish11_2",
	"fish12_1",
	"fish12_2",
	"fish13_1",
	"fish13_2",
	"fish14_1",
	"fish14_1",
	"fish15_2",
	"fish15_2",
	"fish16_1",
	"fish16_2",
	"fish17_1",
	"fish17_2",
	"fish17_3",
	"inscore",
	"sound_change_gun",
	"sound_LZP_fire",
};

static const int MusicLkCount = sizeof(GameMusicForLk) / sizeof(char*);


//// 炮管位置
//const Position kGunPos[GAME_PLAYER] = {
//    { 326.5f,665.f }, { 757.5f, 665.f }, { 1188.5f, 665.f},
//    { 1039.5f, 90.f }, { 608.5f, 90.f }, { 177.5f, 90.f }
//};
//
////玩家闪电鱼的起始坐标
//const Position kChainStart[GAME_PLAYER] = {
//	{ 326.5f, 306.f }, { 757.5f, 306.f }, { 1188.5f, 306.f },
//	{ 1039.5f, 448.f }, { 608.5f, 448.f }, { 177.5f, 448.f }
//};

// 炮管位置
const Position kGunPos[GAME_PLAYER] = {
    { 326.5f,715.f }, { 757.5f, 715.f }, { 1188.5f, 715.f},
    { 1039.5f, 40.f }, { 608.5f, 40.f }, { 177.5f, 40.f }
};

//玩家闪电鱼的起始坐标
const Position kChainStart[GAME_PLAYER] = {
	{ 326.5f, 256.f }, { 757.5f, 256.f }, { 1188.5f, 256.f },
	{ 1039.5f, 498.f }, { 608.5f, 498.f }, { 177.5f, 498.f }
};

//const Position kMuzzleOffset = { 41.f, 50.f };
const Position kMuzzleOffset = {50.f,50.f };


enum FishKind {
  FISH_WONIUYU = 0,         // 蜗牛鱼
  FISH_LVCAOYU,             // 绿草鱼
  FISH_HUANGCAOYU,          // 黄草鱼
  FISH_DAYANYU,             // 大眼鱼
  FISH_HUANGBIANYU,         // 黄边鱼
  FISH_XIAOCHOUYU,          // 小丑鱼 6
  FISH_XIAOCIYU,            // 小刺鱼
  FISH_LANYU,               // 蓝鱼
  FISH_DENGLONGYU,          // 灯笼鱼
  FISH_HAIGUI,              // 海龟
  FISH_HUABANYU,            // 花斑鱼
  FISH_HUDIEYU,             // 蝴蝶鱼
  FISH_KONGQUEYU,           // 孔雀鱼
  FISH_JIANYU,              // 剑鱼
  FISH_BIANFUYU,            // 蝙蝠鱼
  FISH_YINSHA,              // 银鲨
  FISH_JINSHA,              // 金鲨
  FISH_BAWANGJING,          // 霸王鲸
  FISH_JINCHAN,             // 金蝉
  FISH_SHENXIANCHUAN,       // 神仙船
  FISH_MEIRENYU,            // 美人鱼
  FISH_XIAOQINGLONG,        // 小青龙
  FISH_XIAOYINLONG,         // 小银龙
  FISH_XIAOJINLONG,         // 小金龙   23
  FISH_SWK,                 // 孙悟空
  FISH_YUWANGDADI,          // 玉皇大帝
  FISH_FOSHOU,              // 佛手
  FISH_BGLU,                // 炼丹炉   27
  FISH_DNTG,                // 大闹天宫 (FISH_WONIUYU-FISH_HAIGUI)
  FISH_YJSD,                // 一箭双雕
  FISH_YSSN,                // 一石三鸟
  FISH_QJF,                 // 全家福
  FISH_YUQUN,               // 鱼群 (FISH_WONIUYU-FISH_HAIGUI)
  FISH_CHAIN,               // 闪电鱼 (FISH_WONIUYU-FISH_LANYU) 连 (FISH_WONIUYU-FISH_DENGLONGYU)

  FISH_LKPY_BEGAN,
  FISH_KIND_1,
  FISH_KIND_2,
  FISH_KIND_3,
  FISH_KIND_4,
  FISH_KIND_5,
  FISH_KIND_6,
  FISH_KIND_7,
  FISH_KIND_8,
  FISH_KIND_9,
  FISH_KIND_10,
  FISH_KIND_11,
  FISH_KIND_12,
  FISH_KIND_13,
  FISH_KIND_14,
  FISH_KIND_15,
  FISH_KIND_16,
  FISH_KIND_17,
  FISH_KIND_18,
  FISH_KIND_19,

  FISH_KIND_20, 			// 企鹅
  FISH_KIND_21, 			// 李逵
  FISH_KIND_22, 			// 定屏炸弹
  FISH_KIND_23, 			// 局部炸弹
  FISH_KIND_24, 			// 超级炸弹

  FISH_KIND_25_27,			//大三元
  FISH_KIND_28_30,			//大四喜
  FISH_KIND_31_40,			//鱼王
  FISH_LKPY_END,

  FISH_JCPY_BEGAN,
  FISH_KIND_JC1,
  FISH_KIND_JC2,
  FISH_KIND_JC3,
  FISH_KIND_JC4,
  FISH_KIND_JC5,
  FISH_KIND_JC6,
  FISH_KIND_JC7,
  FISH_KIND_JC8,
  FISH_KIND_JC9,
  FISH_KIND_JC10,
  FISH_KIND_JC11,
  FISH_KIND_JC12,
  FISH_KIND_JC13,
  FISH_KIND_JC14,
  FISH_KIND_JC15,
  FISH_KIND_JC16,
  FISH_KIND_JC17,
  FISH_KIND_JC18,			//小金龙
  FISH_KIND_JC19,			//蓝豚
  FISH_KIND_JC20, 			//局部炸弹,本身也有分
  FISH_KIND_JC21, 			//巨大黄金鱼
  FISH_KIND_JC22, 			//金蟾
  FISH_KIND_JCKING, 		//一网打尽
  FISH_JCPY_END,

							
  FISH_KIND_COUNT
};

#define FISH_NAME_MAP(XX)                                                 \
  XX(FISH_WONIUYU, "fish_woniuyu")                                        \
  XX(FISH_LVCAOYU, "fish_lvcaoyu")                                        \
  XX(FISH_HUANGCAOYU, "fish_huangcaoyu")                                  \
  XX(FISH_DAYANYU, "fish_dayanyu")                                        \
  XX(FISH_HUANGBIANYU, "fish_huangbianyu")                                \
  XX(FISH_XIAOCHOUYU, "fish_xiaochouyu")                                  \
  XX(FISH_XIAOCIYU, "fish_xiaociyu")                                      \
  XX(FISH_LANYU, "fish_lanyu")                                            \
  XX(FISH_DENGLONGYU, "fish_denglongyu")                                  \
  XX(FISH_HAIGUI, "fish_haigui")                                          \
  XX(FISH_HUABANYU, "fish_huabanyu")                                      \
  XX(FISH_HUDIEYU, "fish_hudieyu")                                        \
  XX(FISH_KONGQUEYU, "fish_kongqueyu")                                    \
  XX(FISH_JIANYU, "fish_jianyu")                                          \
  XX(FISH_BIANFUYU, "fish_bianfuyu")                                      \
  XX(FISH_YINSHA, "fish_yinsha")                                          \
  XX(FISH_JINSHA, "fish_jinsha")                                          \
  XX(FISH_BAWANGJING, "fish_bawangjing")                                  \
  XX(FISH_JINCHAN, "fish_jinchan")                                        \
  XX(FISH_SHENXIANCHUAN, "fish_shenxianchuan")                            \
  XX(FISH_MEIRENYU, "fish_meirenyu")                                      \
  XX(FISH_XIAOQINGLONG, "fish_xiaoqinglong")                              \
  XX(FISH_XIAOYINLONG, "fish_xiaoyinlong")                                \
  XX(FISH_XIAOJINLONG, "fish_xiaojinlong")                                \
  XX(FISH_SWK, "fish_swk")                                                \
  XX(FISH_YUWANGDADI, "fish_yuwangdadi")                                  \
  XX(FISH_FOSHOU, "fish_foshou")                                          \
  XX(FISH_BGLU, "fish_bglu")                                              \
  XX(FISH_KIND_1, "fish_fish1")											  \
  XX(FISH_KIND_2, "fish_fish2")											  \
  XX(FISH_KIND_3, "fish_fish3")										      \
  XX(FISH_KIND_4, "fish_fish4")                                              \
  XX(FISH_KIND_5, "fish_fish5")                                              \
  XX(FISH_KIND_6, "fish_fish6")                                              \
  XX(FISH_KIND_7, "fish_fish7")											  \
  XX(FISH_KIND_8, "fish_fish8")											  \
  XX(FISH_KIND_9, "fish_fish9")											  \
  XX(FISH_KIND_10, "fish_fish10")											  \
  XX(FISH_KIND_11, "fish_fish11")											  \
  XX(FISH_KIND_12, "fish_fish12")											  \
  XX(FISH_KIND_13, "fish_fish13")											  \
  XX(FISH_KIND_14, "fish_fish14")											  \
  XX(FISH_KIND_15, "fish_fish15")											  \
  XX(FISH_KIND_16, "fish_fish16")											  \
  XX(FISH_KIND_17, "fish_fish17")											  \
  XX(FISH_KIND_18, "fish_fish18")											  \
  XX(FISH_KIND_19, "fish_fish19")											  \
  XX(FISH_KIND_20, "fish_fish20")											  \
  XX(FISH_KIND_21, "fish_fish21")											  \
  XX(FISH_KIND_22, "fish_fish22")											  \
  XX(FISH_KIND_23, "fish_fish23")											  \
  XX(FISH_KIND_24, "fish_fish24")											  \


// 一箭双雕
const FishKind kYjsdSubFish[5][2] = { {FISH_HUANGBIANYU, FISH_LANYU}, {FISH_XIAOCHOUYU, FISH_XIAOCIYU}, {FISH_HUANGBIANYU, FISH_XIAOCIYU}, {FISH_XIAOCHOUYU, FISH_LANYU}, {FISH_HUANGCAOYU, FISH_XIAOCIYU} };
const int kYjsdMulriple[5] = { 13, 13, 12, 14, 10 };
// 一石三鸟
const FishKind kYssnSubFish[6][3] = { {FISH_LVCAOYU, FISH_HUANGCAOYU, FISH_XIAOCHOUYU}, {FISH_WONIUYU, FISH_DAYANYU, FISH_XIAOCHOUYU}, {FISH_WONIUYU, FISH_XIAOCHOUYU, FISH_XIAOCIYU}, {FISH_DAYANYU, FISH_HUANGBIANYU, FISH_LANYU}, {FISH_DAYANYU, FISH_HUANGBIANYU, FISH_HAIGUI}, {FISH_DAYANYU, FISH_XIAOCHOUYU, FISH_XIAOCIYU} };
const int kYssnMulriple[6] = { 11, 12, 15, 17, 18, 17 };
// 全家福
const FishKind kQuanJiaFu[10] = { FISH_HAIGUI, FISH_WONIUYU, FISH_DENGLONGYU, FISH_LVCAOYU, FISH_XIAOCIYU, FISH_LANYU, FISH_HUANGCAOYU, FISH_DAYANYU, FISH_XIAOCHOUYU, FISH_HUANGBIANYU };
// 闪电鱼的包围盒半径
const float kChainFishRadius[9] = { 35.f, 27.f, 39.f, 49.5f, 62.f, 52.f, 60.f, 65.5f, 106.5f };



enum BulletKind {
  BULLET_2_NORMAL = 0,
  BULLET_3_NORMAL,
  BULLET_4_NORMAL,
  BULLET_2_DOUBLE,
  BULLET_3_DOUBLE,
  BULLET_4_DOUBLE,

  BULLET_KIND_COUNT
};

enum SceneKind {
  SCENE_1 = 0,
  SCENE_2,
  SCENE_3,
  SCENE_4,
  SCENE_5,

  SCENE_COUNT
};

//------------------------------------------------------------------------------
// 服务端命令

#define SUB_S_SCENE_FISH                    100
#define SUB_S_SCENE_BULLETS                 101
#define SUB_S_EXCHANGE_FISHSCORE            102
#define SUB_S_USER_FIRE                     103
#define SUB_S_BULLET_DOUBLE_TIMEOUT         104
#define SUB_S_DISTRIBUTE_FISH               105
#define SUB_S_SWITCH_SCENE                  106
#define SUB_S_CATCH_CHAIN                   107
#define SUB_S_CATCH_FISH_GROUP              108
#define SUB_S_FORCE_TIMER_SYNC              109
#define SUB_S_TIMER_SYNC                    110
#define SUB_S_STOCK_OPERATE_RESULT          111
#define SUB_S_ADMIN_CONTROL                 112
#define SUB_S_HIT_FISH_LK					113 //修改李逵的倍数

struct ClientGameConfig {
  int exchange_ratio_userscore;
  int exchange_ratio_fishscore;
  int exchange_count;

  int min_bullet_multiple;
  int max_bullet_multiple;

  float fish_speed[FISH_KIND_COUNT];
  float fish_bounding_radius[FISH_KIND_COUNT];
  int fish_bounding_count[FISH_KIND_COUNT];

  float bullet_speed[BULLET_KIND_COUNT];
  float bullet_bounding_radius[BULLET_KIND_COUNT];
};

struct CMD_S_GameStatus {
  bool is_special_scene;
  float special_sceene_waited_time;
  unsigned int tick_count;
  ClientGameConfig game_config;
  SCORE fish_score[GAME_PLAYER];
  SCORE exchange_fish_score[GAME_PLAYER];
};

struct CMD_S_DistributeFish {
  FishKind fish_kind;
  int fish_id;
  int tag;
  unsigned int tick_count;
  int position_count;
  Position position[4];
};

struct CMD_S_ExchangeFishScore {
  WORD chair_id;
  SCORE swap_fish_score;
  SCORE exchange_fish_score;
};

struct CMD_S_UserFire {
  unsigned long tick_count;
  unsigned short chair_id;
  int bullet_id;
  float angle;
  bool bullet_double;
  int bullet_mulriple;
  int lock_fish_id;
};

struct CatchFish {
  int fish_id;
  FishKind fish_kind;
  SCORE fish_score;
  bool bullet_double;
  int link_fish_id;
};

struct CMD_S_CatchFishGroup {
  DWORD tick_count;
  WORD  chair_id;
  int fish_count;
  CatchFish catch_fish[kMaxCatchFishCount];
  int bullet_id;
};

struct CMD_S_BulletDoubleTimeout {
  WORD chair_id;
};

struct CMD_S_SwitchScene {
  SceneKind next_scene;
  unsigned int tick_count;
};

struct CMD_S_CatchChain {
  WORD chair_id;
  int fish_count;
  CatchFish catch_fish[kMaxChainFishCount];
  int bullet_id;
};

struct CMD_S_SceneFish {
  int fish_id;
  FishKind fish_kind;
  Position position[4];
  int position_count;
  int tag;
  float elapsed;
  unsigned int tick_count;
};

struct CMD_S_SceneBullet {
  unsigned int chair_id;
  int bullet_id;
  Position position;
  float angle;
  unsigned int tick_count;
  int bullet_mulriple;
  bool is_double;
  int lock_fish_id;
};

struct CMD_S_ForceTimerSync {
  WORD chair_id;
};

struct CMD_S_TimerSync {
  WORD chair_id;
  DWORD client_tick;
  DWORD server_tick;
};

struct CMD_S_StockOperateResult {
  unsigned char operate_code;
  SCORE stock_score;
};

struct CMD_S_AdminControl {
  unsigned char operate_code;
  unsigned int game_id[30];
  int id_count;
};

//------------------------------------------------------------------------------
// 客户端命令

#define SUB_C_USER_FIRE                     1
#define SUB_C_EXCHANGE_FISHSCORE            2
#define SUB_C_TIMER_SYNC                    3
#define SUB_C_STOCK_OPERATE                 4
#define SUB_C_ADMIN_CONTROL                 5

struct CMD_C_ExchangeFishScore {
  SCORE exchange_score;
};

struct CMD_C_UserFire {
  unsigned int tick_count;
  float angle;
  int lock_fish_id;
  int bullet_mulriple;
};

struct CMD_C_TimerSync {
  unsigned int client_tick;
};

struct CMD_C_StockOperate {
  unsigned char operate_code;
};

struct CMD_C_AdminControl {
  unsigned char operate_code;
  unsigned int game_id;
  SCORE limit_score;
  FishKind fish_kind;
  int catch_count;
  double catch_probability;
};

//李逵专用
struct CMD_S_HitFishLK {
	WORD chair_id;
	int fish_id;
	int fish_mulriple;
};

#pragma pack()

#endif // GAME_GLOBAL_SETTING_H_
