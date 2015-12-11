/**
 * @file RoomLevelInfo.h
 * @brief  
 * @author TJL
 * @date 15-4-10
 */
#ifndef __RoomLevelInfo_H_
#define __RoomLevelInfo_H_

#include "cocos2d.h"

USING_NS_CC;

enum RoomType
{
	RoomTypeNormal = 0,
	RoomTypeNew,
	RoomTypeHot,
	RoomType_MAXCOUNT
};

class RoomLevelInfo : public Ref {
public:
    RoomLevelInfo ();
    virtual ~RoomLevelInfo ();

    static RoomLevelInfo *create();
    virtual bool init();

    //名称索引
    void setKindIndex(int index);
    int getKindIndex()const;

    //节点索引
    void setNodeIndex(int index);
    int getNodeIndex()const;

    //排序索引
    void setSortIndex(int index);
    int getSortIndex()const;

    //房间名称
    void setRoomName(std::string name);
    std::string getRoomName()const;

	//房间ID
	void setRoomId(int id);
	int getRoomId()const;

	//房间SERVER PORT
	void setServerPort(int port);
	int  getServerPort() const;

	//在线人数
	void setOnlineCount(int cnt);
	int  getOnlineCount() const;

	//人数限制
	void setRoomMax(int max);
	int  getRoomMax() const;

	//房间服务器地址
	void setRoomServerAddr(std::string addr);
	std::string getRoomServerAddr() const;

	//场次
	void setRoomLevel(std::string name);
	std::string getRoomLevel() const;

	//房间描述
	void setRoomDesc(std::string desc);
	std::string getRoomDesc() const;

	//房间类型
	void setRoomType(RoomType type);
	RoomType getRoomType() const;
private:
	int mKindIndex;
	int mNodeIndex;
	int mSortIndex;
	int mServerId;
	int mServerPort;
	int mOnlineCount;
	RoomType mRoomType;
	int mRoomLimit;

	std::string mServerAddr;
	std::string mServerName;
	std::string mLevelName;
	std::string mRoomDesc;

  
};

#endif //__RoomLevelInfo_H_
