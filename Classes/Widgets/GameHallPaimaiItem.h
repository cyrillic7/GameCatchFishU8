/**
 * @file GameHallPaimaiItem
 * @brief  游戏大厅拍卖界面
 * @author tjl
 * @date 15-6-10
 */
#ifndef __GameHallPaimaiItem_H_
#define __GameHallPaimaiItem_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
#include "MsgDefine/CMD_LogonServer.h"

USING_NS_CC;
using namespace cocos2d::ui;

//拍买ITEM 项
class GameHallPaimaiInfoItem : public Layout
{

public:
	GameHallPaimaiInfoItem();
	~GameHallPaimaiInfoItem();
	static GameHallPaimaiInfoItem* create(__Dictionary* infodic,int width);
	bool init(__Dictionary* infodic,int width);
	__Dictionary* getData() const {return mData;}
protected:
	__Dictionary* mData;
	void onClickBuy(cocos2d::Ref* pSender,ui::Widget::TouchEventType eventtype);
	void onClickCancelBuy(cocos2d::Ref* pSender,ui::Widget::TouchEventType eventtype);
};

//我的拍卖ITEM
class GameHallMyPaiamiItem : public Layout
{

public:
	GameHallMyPaiamiItem();
	~GameHallMyPaiamiItem();
	static GameHallMyPaiamiItem* create(__Dictionary* infodic,int width);
	bool init(__Dictionary* infodic,int width);
	__Dictionary* getData() const {return mData;}
protected:
	__Dictionary* mData;
	void onClickCancel(cocos2d::Ref* pSender,ui::Widget::TouchEventType eventtype);
};

//历史ITEM
class GameHallPaimaiHistoryItem : public Layout
{

public:
	GameHallPaimaiHistoryItem();
	~GameHallPaimaiHistoryItem();
	static GameHallPaimaiHistoryItem* create(__Dictionary* infodic,int width);
	bool init(__Dictionary* infodic,int width);
};


//////////////////////////////////////////////////////////////////////////
//上架商品ITEM
class GameHallPaimaiSellItem : public Layout
{

public:
	GameHallPaimaiSellItem();
	~GameHallPaimaiSellItem();
	static GameHallPaimaiSellItem* create(__Dictionary* infodic,int width);
	bool init(__Dictionary* infodic,int width);
	void setSelect();
	bool isSelect() const {return mbSelect;}
	void unSelect();
	__Dictionary* getData() const {return mData;}
	void refreshData(int cnt);
protected:
	__Dictionary* mData;
	bool mbSelect;
	LayerColor*  mSelectLayer;
	Text* mCountLable;
	Text* mNameLable;
};
#endif //__GameHallPaimaiItem_H_