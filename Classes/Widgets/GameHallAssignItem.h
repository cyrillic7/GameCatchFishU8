#ifndef __GameHallAssignItem_H_H_
#define __GameHallAssignItem_H_H_

/**
 * @file GameHallAssignItem
 * @brief  游戏大厅签到子项
 * @author tjl
 * @date 15-7-13
 */
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Model/GiftModel.h"
#include "MsgDefine/CMD_LogonServer.h"
USING_NS_CC;
using namespace ui;


class GameHallAssignItem : public Layout
{
public:
	GameHallAssignItem();
	~GameHallAssignItem();
	static GameHallAssignItem* create(CMD_GP_SignInTask info);
	virtual bool init(CMD_GP_SignInTask info);
	void setSelect(bool value);
	int getAssignDay() const {return mInfo.dwDay;}
protected:
	ImageView*  mSelectImage;
	ImageView*  mHasGetImage;
	ImageView*  gold;
	CMD_GP_SignInTask mInfo;

};
#endif // __GameHallAssignItem_H_H_


