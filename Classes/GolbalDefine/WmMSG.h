#ifndef WM_MSG_HEAD_FILE
#define WM_MSG_HEAD_FILE
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////////////
//socket消息
#define WM_SOCKET_NOTIFY			    WM_USER+3000						//SOCKET 消息
#define WM_WEB_SOCKET_NOTIFY			WM_USER+3001						//消息标识
//进程控制
#define WM_ASYNCHRONISM_DATA		   (WM_USER+3002)						//异步数据
//通知消息
#define WM_PLATFORM_EVENT			   (WM_USER+3003)						//平台事件

//下载组件(1-19)
#define WM_ITEM_STATUS_UPDATE		   (WM_USER+1)					       //状态更新
//游戏引擎(20-39)
#define WM_GRAPHNOTIFY                 (WM_USER+20)

//游戏框架(100-699)
//渲染消息
#define WM_D3D_RENDER				(WM_USER+100)							//渲染消息
#define WM_CREADED					(WM_USER+101) 
#define WM_ADD_FRIEND				(WM_USER+102)						//加为好友
#define WM_SET_DETEST				(WM_USER+103)						//设为厌恶

//界面控件（700-1000）
#define WM_BN_LIST_CLICK			(WM_USER+700)						//listctrl控件按钮消息
#define WM_BN_HALL_MATCH_CLICK		(WM_USER+701)						//listctrl控件按钮消息
#define WM_BN_HALL_LAST_CLICK		(WM_USER+702)						//listctrl控件按钮消息



//大厅(1000-1680)
#define WM_UPDATE_RESOUCE           (WM_USER+700)                      //更新资源
#define WM_LOGIN_QQ                 (WM_USER+701)                      //QQ登录
#define WM_SAFA_BIND                (WM_USER+702)                      //绑定

//游戏控件(1680-2000)
#define WM_INSUREPLAZA_UPDATA       (WM_USER+1680)                      //银行更新消息

//界面控件(2000+)
//菜单命令
#define IDM_CLEAR_ALL				(WM_USER+2001)						//删除信息
#define IDM_SELECT_ALL				(WM_USER+2002)						//全部选择
#define IDM_COPY_STRING				(WM_USER+2003)						//拷贝字符
#define IDM_DELETE_STRING			(WM_USER+2004)						//删除字符
#define IDM_SAVE_STRING				(WM_USER+2005)						//保存信息

#define WM_SELECT_ROOM				(WM_USER+2006)						//选择房间


//////////////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif