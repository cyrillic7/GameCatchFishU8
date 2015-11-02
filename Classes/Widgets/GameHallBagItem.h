#ifndef __GameHallBagItem_H_H_
#define __GameHallBagItem_H_H_

/**
 * @file GameHallBagItem
 * @brief  游戏大厅背包界面
 * @author tjl
 * @date 15-6-12
 */
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Model/GiftModel.h"

USING_NS_CC;
using namespace ui;


class GameHallBagItem : public Layout
{
public:
	GameHallBagItem();
	~GameHallBagItem();
	static GameHallBagItem* create(GiftModel* info);
	virtual bool init(GiftModel* info);
	void setSelect(bool value);
	GiftModel* getData() const {return mData;}
	bool isSelect() const {return mBSelect;}
	void onBagItemPressed(Ref* pSender, ui::Widget::TouchEventType type);
	void refreshData();
protected:
	GiftModel* mData;
	std::string  mImageUrl;
	ImageView*  mSelectImage;
	bool        mBSelect;
	Text*       mCountText;//背包项的个数
};
#endif // __GameHallBagItem_H_H_


