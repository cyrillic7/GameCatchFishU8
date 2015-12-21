
#ifndef __U8SDKDATA_H__
#define __U8SDKDATA_H__

#include "cocos2d.h"

/*支付接口需要的参数*/
class U8PayParams : public cocos2d::Ref
{
public:
	U8PayParams();
	virtual ~U8PayParams();

	static U8PayParams* create();

	//游戏中商品ID
	std::string productId;

	//游戏中商品名称，比如元宝，钻石...
	std::string productName;

	//游戏中商品描述
	std::string productDesc;

	//商品价格，单位元
	int price;

	//购买数量,一般都为1.注意下，比如游戏中“100元宝”是一条充值商品，
	//对应的价格是90元。那么上面price是90元。这里buyNum=1而不是100
	int buyNum;

	//当前玩家身上剩余的虚拟币数量
	int coinNum;

	//当前角色所在的服务器ID
	std::string serverId;

	//当前角色所在的服务器名称
	std::string serverName;

	//当前角色ID
	std::string roleId;

	//当前角色名称
	std::string roleName;

	//当前角色等级
	int roleLevel;

	//当前角色的vip等级
	std::string vip;

	//游戏服务器订单号，由服务器生成
	std::string orderID;

	//扩展字段，由游戏服务器生成，各个渠道SDK可能不一样
	std::string extension;

};

/*数据上报接口需要的参数*/
class U8ExtraGameData : public cocos2d::Ref
{
public:

	static const int TYPE_SELECT_SERVER = 1;		//选择服务器
	static const int TYPE_CREATE_ROLE = 2;			//创建角色
	static const int TYPE_ENTER_GAME = 3;			//进入游戏
	static const int TYPE_LEVEL_UP = 4;				//等级提升
	static const int TYPE_EXIT_GAME = 5;			//退出游戏


	U8ExtraGameData();
	virtual ~U8ExtraGameData();

	static U8ExtraGameData* create();

	//调用时机，设置为上面定义的类型，在各个对应的地方调用submitGameData方法
	int dataType;

	//角色ID
	std::string roleId;

	//角色名称
	std::string roleName;

	//角色等级
	int roleLevel;

	//服务器ID
	int serverId;

	//服务器名称
	std::string serverName;

	//当前角色生成拥有的虚拟币数量
	int moneyNum;
};

class U8LoginResult : public cocos2d::Ref
{
public:
	U8LoginResult();
	virtual ~U8LoginResult();

	static U8LoginResult* create();

	//是否认证成功
	bool isSuc;

	//当前是否为SDK界面中切换帐号的回调
	bool isSwitchAccount;

	//
	std::string channelID;
	//u8server 返回的userId
	std::string userId;

	//渠道SDK的userID
	std::string sdkUserId;

	//u8server返回的用户名
	std::string username;


	//渠道SDK 用户名
	std::string sdkUsername;

	//u8server返回的用于登录认证的凭据
	std::string token;

};


#endif