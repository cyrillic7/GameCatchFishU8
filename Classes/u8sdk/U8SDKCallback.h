#ifndef __U8SDK_CALLBACK_H__
#define __U8SDK_CALLBACK_H__

#include "U8SDKData.h"
#include "U8SDKInterface.h"

class U8SDKCallback
{

public:

	virtual void OnInitSuc() =0;

	virtual void OnLoginSuc(U8LoginResult* result) =0;

	virtual void OnSwitchLogin() =0;

	virtual void OnLogout() =0;

};
#endif

