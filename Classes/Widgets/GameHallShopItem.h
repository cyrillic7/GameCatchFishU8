/**
 * @file GameHallShopItem
 * @brief  游戏大厅ShopItem控件
 * @author tjl
 * @date 15-5-31
 */

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "Model/GiftModel.h"
#include "Common/WebImageView.h"
using namespace cocos2d::ui;

class GameHallShopItem : public ui::Layout
{
public:
	GameHallShopItem();
	~GameHallShopItem();
	static GameHallShopItem* create(GiftModel* m);// 0为礼包，1为道具..
	virtual bool init(GiftModel* m);
protected:
	WebImageView* mIconImage;//图标
	Label*   mPriceLabel;
	Label*   mAwardLabel;//only in gift
	Label*   mDiscountLable;//only in  tool
	int      mItemType;
	GiftModel* m_giftModel;
	//for test
	Text* labelNormalText ;

	void onClickCovert(Ref* pSender,ui::Widget::TouchEventType type);//点击兑换
};