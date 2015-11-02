/**
 * @file UserModel.h
 * @brief  用户数剧模型
 * @author TJL
 * @date 15-4-27
 */

#ifndef __UserModel_H_H_
#define __UserModel_H_H_

#include "cocos2d.h"
#include "GameGlobalSetting.h"
USING_NS_CC;

class UserModel : public Ref
{
public:
	UserModel();
	~UserModel();
	bool init();
	CREATE_FUNC(UserModel);
	void setUserId(int usrId){mUserId = usrId;}
	int  getUserId() const {return mUserId;}
	void setChairId(int id){mChairID = id;}
	int  getChairId() const {return mChairID;}
	void setTableId(int id){mTableID = id;}
	int  getTableId() const {return mTableID;}
	void setUserStatus(unsigned char isValue ) {mUserStatus = isValue;}
	unsigned char getUserStatus(){return mUserStatus;}
	void setUserScore(SCORE score) { mUserScore = score;}
	SCORE getUserScore() const {return mUserScore;}
private:
	SCORE mUserScore;							//用户金币数
	int  mTableID;								//桌子索引
	int  mChairID;								//椅子索引
	unsigned char mUserStatus;						//用户状态
	bool mUserSex;							//用户性别
	unsigned long	mUserId;					//用户ID

};





#endif //__UserModel_H_H_
