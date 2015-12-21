#include "U8SDKData.h"

U8PayParams::U8PayParams()
{

}
U8PayParams::~U8PayParams()
{

}

U8PayParams* U8PayParams::create()
{
	auto ret = new U8PayParams();
	if (ret)
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret); 
	return nullptr;
}


U8ExtraGameData::U8ExtraGameData()
{

}
U8ExtraGameData::~U8ExtraGameData()
{

}

U8ExtraGameData* U8ExtraGameData::create()
{
	auto ret = new U8ExtraGameData();
	if (ret)
	{
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

U8LoginResult::U8LoginResult()
{

}

U8LoginResult::~U8LoginResult()
{

}

U8LoginResult* U8LoginResult::create()
{
	auto ret = new U8LoginResult();
	if (ret)
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}