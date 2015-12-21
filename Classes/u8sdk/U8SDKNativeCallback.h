#ifndef __U8SDK_NATIVE_CALLBACK_H__
#define __U8SDK_NATIVE_CALLBACK_H__

#include "cocos2d.h"
#include "U8SDKCallback.h"

class U8SDKNativeCallback
{

private:
	U8SDKCallback* gameCallback;

public:
	
	U8SDKNativeCallback(U8SDKCallback* callback);
	virtual ~U8SDKNativeCallback();

	virtual void OnU8InitSuc(cocos2d::Node *sender, cocos2d::Value data);

	virtual void OnU8LoginSuc(cocos2d::Node *sender, cocos2d::Value data);

	virtual void OnU8SwitchLogin(cocos2d::Node *sender, cocos2d::Value data);

	virtual void OnU8Logout(cocos2d::Node *sender, cocos2d::Value data);

	virtual void OnU8PaySuc(cocos2d::Node *sender, cocos2d::Value data);

};

#endif