/**
 * @file ChargeModel
 * @brief  充值数据结构
 * @author tjl
 * @date 15-8-27
 */
#ifndef __ChargeModel_H_
#define __ChargeModel_H_

#include "cocos2d.h"

USING_NS_CC;

class ChargeModel : public Ref {
public:
	ChargeModel();
	~ChargeModel();

	CREATE_FUNC(ChargeModel);
	bool init(){return true;};

	//id
	CC_SYNTHESIZE(int, m_ItemId, ItemId);
	//价格1
	CC_SYNTHESIZE(long, m_price, Price);

	//礼物类型
	CC_SYNTHESIZE(int, m_ChargeItemType, ChargeItemType);//0为元宝兑换金币，1为充值

	//加赠数量
	CC_SYNTHESIZE(int, m_SendCount, SendCount);

	//加赠类型
	CC_SYNTHESIZE(int, m_SendType, SendType); //0 为碎片， 1为红包

	//获得
	CC_SYNTHESIZE(int, m_CovertCount, CovertCount); //0 为碎片， 1为红包
};



#endif //ChargeModel
