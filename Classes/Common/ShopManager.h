/**
 * @file ShopManager.h
 * @brief  礼包，道具管理类
 * @author tjl
 * @date 15/5/29
 */
#ifndef __ShopManager_H_
#define __ShopManager_H_

#include "cocos2d.h"

USING_NS_CC;

//物品类型
typedef enum {
    ShopTypeGift = 1,    //礼包
    ShopTypeTool,   //道具
	ShopTypeVip,
	ShopMaxCount
}ShopItemType;

class ShopManager : public Ref {
    ShopManager ();
    ~ ShopManager ();

public:
    static ShopManager* getInstance();
    static void purge();

    //获取列表
    __Array* getDataList(ShopItemType type);

    //设置数据列表
    void setDataList(ShopItemType type, __Array *dataList);



private:
    std::string getSaveDataKey(ShopItemType type);
};

#endif //__ShopManager_H_
