#include "U8SDKInterface.h"

USING_NS_CC;

U8SDKInterface* U8SDKInterface::_instance = nullptr;

U8SDKInterface* U8SDKInterface::getInstance()
{
	if (nullptr == _instance)
	{
		_instance = new U8SDKInterface();
	}

	return _instance;
}


void U8SDKInterface::initSDK(U8SDKCallback* callback)
{
	log("initSDK2222");
	this->nativeCallback = new U8SDKNativeCallback(callback);

	NDKHelper::addSelector("U8Selectors", "OnU8InitSuc", CC_CALLBACK_2(U8SDKNativeCallback::OnU8InitSuc, nativeCallback), nullptr);
	NDKHelper::addSelector("U8Selectors", "OnU8LoginSuc", CC_CALLBACK_2(U8SDKNativeCallback::OnU8LoginSuc, nativeCallback), nullptr);
	NDKHelper::addSelector("U8Selectors", "OnU8SwitchLogin", CC_CALLBACK_2(U8SDKNativeCallback::OnU8SwitchLogin, nativeCallback), nullptr);
	NDKHelper::addSelector("U8Selectors", "OnU8Logout", CC_CALLBACK_2(U8SDKNativeCallback::OnU8Logout, nativeCallback), nullptr);
	NDKHelper::addSelector("U8Selectors", "OnU8PaySuc", CC_CALLBACK_2(U8SDKNativeCallback::OnU8PaySuc, nativeCallback), nullptr);

}

void U8SDKInterface::login()
{
	Value v;
	sendMessageWithParams("login", v);
}

std::string U8SDKInterface::getchannelId()
{
	Value v;
	std::string channel;
	channel = getMessageWithParams("getChannel", v);
	log("U8SDKInterface::getchannelId()  %s", channel.c_str());
	return channel;
}
void U8SDKInterface::loginCustom(const std::string& customData)
{
	ValueMap params;
	params["customData"] = customData;
	Value v = Value(params);
	sendMessageWithParams("loginCustom", v);
}

void U8SDKInterface::switchLogin()
{
	Value v;
	sendMessageWithParams("switchLogin", v);
}

void U8SDKInterface::logout()
{
	Value v;
	sendMessageWithParams("logout", v);
}

void U8SDKInterface::showAccoutCenter()
{
	Value v;
	sendMessageWithParams("showAccountCenter", v);
}

void U8SDKInterface::submitGameData(U8ExtraGameData* data)
{
	ValueMap params;
	params["dataType"] = data->dataType;
	params["roleID"] = data->roleId;
	params["roleName"] = data->roleName;
	params["roleLevel"] = data->roleLevel;
	params["serverID"] = data->serverId;
	params["serverName"] = data->serverName;
	params["moneyNum"] = data->moneyNum;
	Value v = Value(params);
	sendMessageWithParams("submitExtraData", v);
}

void U8SDKInterface::sdkExit()
{
	Value v;
	sendMessageWithParams("exit", v);
}

void U8SDKInterface::pay(U8PayParams* data)
{
	if (data == nullptr)
	{
		CCLOG("U8SDKInterface::pay data is null.");
		return;
	}

	ValueMap params;
	params["productId"] = data->productId;
	params["productName"] = data->productName;
	params["productDesc"] = data->productDesc;
	params["price"] = data->price;
	params["buyNum"] = data->buyNum;
	params["coinNum"] = data->coinNum;
	params["serverId"] = data->serverId;
	params["serverName"] = data->serverName;
	params["roleId"] = data->roleId;
	params["roleName"] = data->roleName;
	params["roleLevel"] = data->roleLevel;
	params["vip"] = data->vip;
	params["orderID"] = data->orderID;
	params["extension"] = data->extension;
	log("CU8sdkFunction::OnU8sdkPay productName==%s,productDesc==%s,price==%d,orderID==%s", data->productName.c_str()
		, data->productDesc.c_str()
		, data->price
		, data->orderID.c_str());
	Value v = Value(params);
	sendMessageWithParams("pay", v);
}

void U8SDKInterface::setSupports(bool exit, bool accountCenter, bool logout)
{
	this->supportExit = exit;
	this->supportAccountCenter = accountCenter;
	this->supportLogout = logout;
}

bool U8SDKInterface::isSupportExit()
{
	return this->supportExit;
}

bool U8SDKInterface::isSupportAccountCenter()
{
	return this->supportAccountCenter;
}

bool U8SDKInterface::isSupportLogout()
{
	return this->supportLogout;
}


U8SDKInterface::~U8SDKInterface()
{
	NDKHelper::removeSelectorsInGroup("U8Selectors");

	do
	{
		delete (this->nativeCallback);
		this->nativeCallback = nullptr;

	} while(0);

}
