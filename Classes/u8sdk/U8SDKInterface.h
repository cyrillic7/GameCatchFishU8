#ifndef __U8SDK_H__
#define __U8SDK_H__

#include "cocos2d.h"
#include "U8SDKData.h"
#include "U8SDKNativeCallback.h"
#include "U8SDKCallback.h"
#include "NDKHelper/NDKHelper.h"

/*
 * U8SDK 操作接口 单例
 */
class U8SDKInterface : cocos2d::CCObject
{
private:
	static U8SDKInterface* _instance;
	bool supportExit;
	bool supportAccountCenter;
	bool supportLogout;
	U8SDKNativeCallback* nativeCallback;

public:
	static U8SDKInterface* getInstance();
	virtual ~U8SDKInterface();

	/*SDK 初始化 */
	void initSDK(U8SDKCallback* callback);
	void onU8LoginResult(cocos2d::Node *sender, cocos2d::Value data);


	/*登录*/
	void login();
	
	/*登录 应用宝如果游戏层处理，则调用这个接口 QQ或者WX*/
	void loginCustom(const std::string& customData);

	/*切换帐号*/
	void switchLogin();
	
	/*登出*/
	void logout();
	
	/*显示用户个人中心*/
	void showAccoutCenter();
	
	/*上报游戏数据*/
	void submitGameData(U8ExtraGameData * data);
	
	/*SDK 退出确认框*/
	void sdkExit();
	
	/*支付接口*/
	void pay(U8PayParams * data);

	void setSupports(bool exit, bool accountCenter, bool logout);

	/*SDK是否支持退出确认框，不支持则使用游戏自己的*/
	bool isSupportExit();

	/*SDK是否支持用户个人中心，部分SDK要求游戏中必须要有进入用户中心的按钮，这里游戏可以根据这个来判断显示还是不显示*/
	bool isSupportAccountCenter();
	
	/*SDK是否支持登出接口*/
	bool isSupportLogout();


};

#endif

