/**
 * @file GuideItemModel
 * @brief  鱼百科数据结构
 * @author tjl
 * @date 15-8-27
 */
#ifndef __GuideItemModel_H_
#define __GuideItemModel_H_

#include "cocos2d.h"

USING_NS_CC;

class GuideItemModel : public Ref {
public:
	GuideItemModel();
	~GuideItemModel();

	CREATE_FUNC(GuideItemModel);
	bool init(){return true;};

	//标题
	CC_SYNTHESIZE(std::string, m_title, Title);
	//图片名称
	CC_SYNTHESIZE(std::string, m_imageName, ImageName);
	//倍率
	CC_SYNTHESIZE(int, m_power, Power);
	//描述
	CC_SYNTHESIZE(std::string, m_itemDesc, ItemDesc);
};



#endif //GuideItemModel
