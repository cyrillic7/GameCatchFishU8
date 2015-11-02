#include "UserModel.h"

UserModel::UserModel()
{
	mTableID = 0;
	mUserId = 0;
	mChairID = -1;
	mUserScore = 0;
	mUserSex = 0;
	mUserStatus = 0;
}

UserModel::~UserModel()
{

}

bool UserModel::init()
{
	return true;
}