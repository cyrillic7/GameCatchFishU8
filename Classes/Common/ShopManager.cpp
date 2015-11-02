#include "ShopManager.h"
#include "SessionManager.h"

//数据缓存的key
//礼物列表
#define DataCache_GiftListKey           "DataCache_GiftListKey"
//道具物表
#define DataCache_ToolListKey            "DataCache_ToolListKey"
//VIP列表
#define DataCache_VipListKey              "DataCache_VipListKey"

static ShopManager *__instance = nullptr;

ShopManager::ShopManager() {

}

ShopManager::~ShopManager() {

}

ShopManager *ShopManager::getInstance() {
    if (nullptr == __instance) {
        __instance = new ShopManager;
    }
    return __instance;
}

void ShopManager::purge() {
   
    SessionManager::shareInstance()->setResponseData(nullptr, DataCache_GiftListKey);
    SessionManager::shareInstance()->setResponseData(nullptr, DataCache_ToolListKey);
	SessionManager::shareInstance()->setResponseData(nullptr, DataCache_VipListKey);
    CC_SAFE_DELETE(__instance);
}

__Array *ShopManager::getDataList(ShopItemType type) {
    __Array *dataList = (__Array*)SessionManager::shareInstance()->getResponseDataForKey(getSaveDataKey(type).c_str());
    return dataList;
}

void ShopManager::setDataList(ShopItemType type, __Array *dataList) {
    SessionManager::shareInstance()->setResponseData(dataList, getSaveDataKey(type).c_str());
}


std::string ShopManager::getSaveDataKey(ShopItemType type) {

	switch (type)
	{
	case ShopTypeGift:
		{
			return DataCache_GiftListKey;
		}
		break;
	case  ShopTypeTool:
		{
			return DataCache_ToolListKey;
		}
		break;
	case  ShopTypeVip:
		{
			return DataCache_VipListKey;
		}
		break;
	}
   // return type == ShopTypeGift ? DataCache_GiftListKey : DataCache_ToolListKey;
}
