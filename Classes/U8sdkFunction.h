#pragma once
#include "cocos2d.h"
#include "u8sdk/U8SDKInterface.h"
#include "GameGlobalSetting.h"

class CU8sdkFunction :
	public U8SDKCallback
{
public:
	CU8sdkFunction();
	~CU8sdkFunction();

public:
	static CU8sdkFunction & GetInstance()
	{
		static CU8sdkFunction instance;   //局部静态变量  
		return instance;
	}
public:
	std::string channelID;

public:

	virtual void OnInitSuc();

	virtual void OnLoginSuc(U8LoginResult* result);

	virtual void OnSwitchLogin();

	virtual void OnLogout();

	 void OnU8sdkSwitchLogin();

	 void OnU8sdkInit();

	 void OnU8sdkLogin();

	 void OnU8sdkPay(std::string strName, std::string strDesc, int price, std::string tradeNo);
};

