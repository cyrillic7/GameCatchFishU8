/**
 * @file GiftModel
 * @brief  礼物
 * @author tjl
 * @date 15-5-29
 */
#ifndef __GiftModel_H_
#define __GiftModel_H_

#include "cocos2d.h"

USING_NS_CC;

class GiftModel : public Ref {
public:
	GiftModel();
	~GiftModel();

	CREATE_FUNC(GiftModel);
	bool init(){return true;};

	//id
	CC_SYNTHESIZE(long, m_giftId, GiftId);

	//物品名字
	CC_SYNTHESIZE(std::string, m_giftName, GiftName);

	//图片路径
	CC_SYNTHESIZE(std::string, m_iconUrl, IconUrl);

	//价格1
	CC_SYNTHESIZE(long, m_price, Price);

	//价格描述
	CC_SYNTHESIZE(std::string , m_priceDesc, PriceDesc);

	//折扣
	CC_SYNTHESIZE(int , m_discount, Discount);

	//包含的物品描述
	CC_SYNTHESIZE(std::string, m_openDesc, OpenDesc);

	//描述
	CC_SYNTHESIZE(std::string, m_desc, Desc);

	//
	CC_SYNTHESIZE(std::string, m_soureDesc, SoureDesc);


	//礼物类型
	CC_SYNTHESIZE(int, m_giftType, GiftType);

	//礼物个数
	CC_SYNTHESIZE(int, m_giftCount, GiftCount);

	//礼物兑换类型
	CC_SYNTHESIZE(int, m_giftExchageType, GiftExchageType);

	//礼物使用类型
	CC_SYNTHESIZE(int, m_giftUseType, GiftUseType);
};



#endif //__GiftModel_H_
